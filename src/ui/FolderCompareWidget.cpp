#include "ui/FolderCompareWidget.h"

#include <QBrush>
#include <QCheckBox>
#include <QDir>
#include <QFileDialog>
#include <QFormLayout>
#include <QHash>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <filesystem>

#include "folder/FolderComparator.h"

namespace bcclone::ui {

namespace {

std::filesystem::path toPath(const QString& path) {
#ifdef _WIN32
    return std::filesystem::path(path.toStdWString());
#else
    const QByteArray utf8 = path.toUtf8();
    const auto* begin = reinterpret_cast<const char8_t*>(utf8.constData());
    return std::filesystem::path(std::u8string(begin, begin + utf8.size()));
#endif
}

QString statusText(const models::FolderDiffStatus status) {
    switch (status) {
    case models::FolderDiffStatus::Equal: return QStringLiteral("Equal");
    case models::FolderDiffStatus::Different: return QStringLiteral("Different");
    case models::FolderDiffStatus::LeftOnly: return QStringLiteral("Left only");
    case models::FolderDiffStatus::RightOnly: return QStringLiteral("Right only");
    }
    return {};
}

QColor statusColor(const models::FolderDiffStatus status) {
    switch (status) {
    case models::FolderDiffStatus::Equal: return QColor(220, 255, 220);
    case models::FolderDiffStatus::Different: return QColor(255, 220, 170);
    case models::FolderDiffStatus::LeftOnly: return QColor(255, 205, 205);
    case models::FolderDiffStatus::RightOnly: return QColor(205, 225, 255);
    }
    return Qt::white;
}

} // namespace

FolderCompareWidget::FolderCompareWidget(QWidget* parent) : QWidget(parent) {
    setupUi();
}

void FolderCompareWidget::setupUi() {
    auto* rootLayout = new QVBoxLayout(this);
    auto* pathsLayout = new QFormLayout();
    auto createPathRow = [this](QLineEdit*& edit, const Side side) {
        auto* row = new QWidget(this);
        auto* layout = new QHBoxLayout(row);
        layout->setContentsMargins(0, 0, 0, 0);
        edit = new QLineEdit(row);
        edit->setReadOnly(true);
        edit->setPlaceholderText(tr("Select a folder"));
        auto* browse = new QPushButton(tr("Browse..."), row);
        connect(browse, &QPushButton::clicked, this, [this, side] { chooseFolder(side); });
        layout->addWidget(edit, 1);
        layout->addWidget(browse);
        return row;
    };
    pathsLayout->addRow(tr("Left folder:"), createPathRow(m_leftPath, Side::Left));
    pathsLayout->addRow(tr("Right folder:"), createPathRow(m_rightPath, Side::Right));
    rootLayout->addLayout(pathsLayout);

    auto* options = new QHBoxLayout();
    options->addWidget(new QLabel(tr("Filters:"), this));
    m_filters = new QLineEdit(QStringLiteral("*"), this);
    m_filters->setToolTip(tr("Semicolon-separated wildcards, for example *.cpp;*.h"));
    options->addWidget(m_filters, 1);
    m_compareContent = new QCheckBox(tr("Verify contents (SHA-256)"), this);
    m_compareContent->setChecked(true);
    options->addWidget(m_compareContent);
    auto* compareButton = new QPushButton(tr("Compare Folders"), this);
    connect(compareButton, &QPushButton::clicked, this, &FolderCompareWidget::compareFolders);
    options->addWidget(compareButton);
    rootLayout->addLayout(options);

    m_tree = new QTreeWidget(this);
    m_tree->setHeaderLabels({tr("Name"), tr("Status"), tr("Left Size"), tr("Right Size")});
    m_tree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_tree->setAlternatingRowColors(true);
    rootLayout->addWidget(m_tree, 1);
}

void FolderCompareWidget::chooseFolder(const Side side) {
    const QString path = QFileDialog::getExistingDirectory(
        this, side == Side::Left ? tr("Select left folder") : tr("Select right folder"));
    if (path.isEmpty()) {
        return;
    }
    (side == Side::Left ? m_leftPath : m_rightPath)->setText(QDir::toNativeSeparators(path));
    if (!m_leftPath->text().isEmpty() && !m_rightPath->text().isEmpty()) {
        compareFolders();
    }
}

void FolderCompareWidget::compareFolders() {
    if (m_leftPath->text().isEmpty() || m_rightPath->text().isEmpty()) {
        QMessageBox::information(this, tr("Select folders"),
                                 tr("Select both folders before comparing."));
        return;
    }

    const QStringList filters = m_filters->text().split(QLatin1Char(';'), Qt::SkipEmptyParts);
    folder::FolderComparator comparator;
    const folder::FolderCompareResult result = comparator.compare(
        toPath(QDir::fromNativeSeparators(m_leftPath->text())),
        toPath(QDir::fromNativeSeparators(m_rightPath->text())),
        {filters, m_compareContent->isChecked()});
    if (!result.success) {
        QMessageBox::critical(this, tr("Folder comparison failed"), result.error);
        return;
    }

    m_tree->clear();
    QHash<QString, QTreeWidgetItem*> folders;
    int differences = 0;
    for (const models::FolderDiffEntry& entry : result.entries) {
        const QStringList parts = entry.relativePath.split(QLatin1Char('/'), Qt::SkipEmptyParts);
        QTreeWidgetItem* parent = nullptr;
        QString currentPath;
        for (qsizetype index = 0; index + 1 < parts.size(); ++index) {
            currentPath += (currentPath.isEmpty() ? QString() : QStringLiteral("/")) + parts.at(index);
            if (!folders.contains(currentPath)) {
                auto* item = parent != nullptr ? new QTreeWidgetItem(parent)
                                               : new QTreeWidgetItem(m_tree);
                item->setText(0, parts.at(index));
                folders.insert(currentPath, item);
            }
            parent = folders.value(currentPath);
        }

        auto* item = parent != nullptr ? new QTreeWidgetItem(parent) : new QTreeWidgetItem(m_tree);
        item->setText(0, parts.isEmpty() ? entry.relativePath : parts.constLast());
        item->setText(1, statusText(entry.status));
        if (entry.status != models::FolderDiffStatus::RightOnly) {
            item->setText(2, QString::number(entry.leftSize));
        }
        if (entry.status != models::FolderDiffStatus::LeftOnly) {
            item->setText(3, QString::number(entry.rightSize));
        }
        const QBrush brush(statusColor(entry.status));
        for (int column = 0; column < m_tree->columnCount(); ++column) {
            item->setBackground(column, brush);
            item->setForeground(column, QBrush(QColor(32, 32, 32)));
        }
        if (entry.status != models::FolderDiffStatus::Equal) {
            ++differences;
        }
    }
    m_tree->expandAll();
    emit statusMessage(tr("Folder comparison complete: %1 files, %2 differences")
                           .arg(result.entries.size()).arg(differences));
}

} // namespace bcclone::ui
