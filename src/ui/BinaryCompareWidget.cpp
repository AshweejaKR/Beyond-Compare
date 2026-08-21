#include "ui/BinaryCompareWidget.h"

#include <QFileDialog>
#include <QFontDatabase>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QSet>
#include <QSplitter>
#include <QTextCursor>
#include <QTextEdit>
#include <QVBoxLayout>
#include <algorithm>
#include <filesystem>

#include "binary/BinaryCompareEngine.h"

namespace bcclone::ui {

namespace {

constexpr qsizetype bytesPerLine = 16;

struct RenderedBytes {
    QString text;
    QVector<QPair<int, int>> differenceRanges;
};

std::filesystem::path toPath(const QString& path) {
#ifdef _WIN32
    return std::filesystem::path(path.toStdWString());
#else
    const QByteArray utf8 = path.toUtf8();
    const auto* begin = reinterpret_cast<const char8_t*>(utf8.constData());
    return std::filesystem::path(std::u8string(begin, begin + utf8.size()));
#endif
}

QChar printableByte(const unsigned char value) {
    return value >= 32U && value <= 126U
        ? QChar::fromLatin1(static_cast<char>(value)) : QLatin1Char('.');
}

RenderedBytes formatBytes(const QByteArray& bytes, const qsizetype totalLength,
                          const QSet<qsizetype>& differences) {
    RenderedBytes rendered;
    const qsizetype rows = (totalLength + bytesPerLine - 1) / bytesPerLine;
    rendered.text.reserve(static_cast<int>(rows * 78));
    for (qsizetype row = 0; row < rows; ++row) {
        const qsizetype rowOffset = row * bytesPerLine;
        rendered.text += QStringLiteral("%1  ").arg(rowOffset, 8, 16, QLatin1Char('0')).toUpper();
        QVector<int> hexPositions;
        hexPositions.reserve(bytesPerLine);
        for (qsizetype column = 0; column < bytesPerLine; ++column) {
            const qsizetype offset = rowOffset + column;
            hexPositions.append(static_cast<int>(rendered.text.size()));
            if (offset < bytes.size()) {
                const auto value = static_cast<unsigned char>(bytes.at(offset));
                rendered.text += QStringLiteral("%1").arg(value, 2, 16, QLatin1Char('0')).toUpper();
            } else {
                rendered.text += QStringLiteral("  ");
            }
            rendered.text += QLatin1Char(' ');
        }

        rendered.text += QStringLiteral(" |");
        const int asciiStart = static_cast<int>(rendered.text.size());
        for (qsizetype column = 0; column < bytesPerLine; ++column) {
            const qsizetype offset = rowOffset + column;
            rendered.text += offset < bytes.size()
                ? printableByte(static_cast<unsigned char>(bytes.at(offset)))
                : QLatin1Char(' ');
            if (offset < totalLength && differences.contains(offset)) {
                rendered.differenceRanges.append({hexPositions.at(column), 2});
                rendered.differenceRanges.append({asciiStart + static_cast<int>(column), 1});
            }
        }
        rendered.text += QStringLiteral("|\n");
    }
    return rendered;
}

void applyHighlights(QPlainTextEdit* editor, const QVector<QPair<int, int>>& ranges) {
    QList<QTextEdit::ExtraSelection> selections;
    selections.reserve(ranges.size());
    for (const auto& [position, length] : ranges) {
        QTextEdit::ExtraSelection selection;
        selection.cursor = editor->textCursor();
        selection.cursor.setPosition(position);
        selection.cursor.setPosition(position + length, QTextCursor::KeepAnchor);
        selection.format.setBackground(QColor(255, 190, 100));
        selection.format.setForeground(QColor(32, 32, 32));
        selections.append(selection);
    }
    editor->setExtraSelections(selections);
}

} // namespace

BinaryCompareWidget::BinaryCompareWidget(QWidget* parent) : QWidget(parent) {
    setupUi();
}

void BinaryCompareWidget::setupUi() {
    auto* rootLayout = new QVBoxLayout(this);
    auto* pathsLayout = new QFormLayout();
    auto createPathRow = [this](QLineEdit*& edit, const Side side) {
        auto* row = new QWidget(this);
        auto* layout = new QHBoxLayout(row);
        layout->setContentsMargins(0, 0, 0, 0);
        edit = new QLineEdit(row);
        edit->setReadOnly(true);
        edit->setPlaceholderText(tr("Select a binary file"));
        auto* browse = new QPushButton(tr("Browse..."), row);
        connect(browse, &QPushButton::clicked, this, [this, side] { chooseFile(side); });
        layout->addWidget(edit, 1);
        layout->addWidget(browse);
        return row;
    };
    pathsLayout->addRow(tr("Left file:"), createPathRow(m_leftPath, Side::Left));
    pathsLayout->addRow(tr("Right file:"), createPathRow(m_rightPath, Side::Right));
    rootLayout->addLayout(pathsLayout);

    auto* compareButton = new QPushButton(tr("Compare Binary Files"), this);
    connect(compareButton, &QPushButton::clicked, this, &BinaryCompareWidget::compareFiles);
    auto* actionLayout = new QHBoxLayout();
    actionLayout->addStretch();
    actionLayout->addWidget(compareButton);
    rootLayout->addLayout(actionLayout);

    auto* splitter = new QSplitter(Qt::Horizontal, this);
    m_leftView = new QPlainTextEdit(splitter);
    m_rightView = new QPlainTextEdit(splitter);
    for (QPlainTextEdit* view : {m_leftView, m_rightView}) {
        view->setReadOnly(true);
        view->setLineWrapMode(QPlainTextEdit::NoWrap);
        view->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    }
    splitter->setSizes({500, 500});
    rootLayout->addWidget(splitter, 1);

    connect(m_leftView->verticalScrollBar(), &QScrollBar::valueChanged,
            m_rightView->verticalScrollBar(), &QScrollBar::setValue);
    connect(m_rightView->verticalScrollBar(), &QScrollBar::valueChanged,
            m_leftView->verticalScrollBar(), &QScrollBar::setValue);
}

void BinaryCompareWidget::chooseFile(const Side side) {
    const QString path = QFileDialog::getOpenFileName(
        this, side == Side::Left ? tr("Open left binary file") : tr("Open right binary file"));
    if (path.isEmpty()) {
        return;
    }
    (side == Side::Left ? m_leftPath : m_rightPath)->setText(path);
    if (!m_leftPath->text().isEmpty() && !m_rightPath->text().isEmpty()) {
        compareFiles();
    }
}

void BinaryCompareWidget::compareFiles() {
    if (m_leftPath->text().isEmpty() || m_rightPath->text().isEmpty()) {
        QMessageBox::information(this, tr("Select files"),
                                 tr("Select both binary files before comparing."));
        return;
    }

    const binary::BinaryCompareResult result = binary::BinaryCompareEngine().compare(
        toPath(m_leftPath->text()), toPath(m_rightPath->text()));
    if (!result.success) {
        QMessageBox::critical(this, tr("Binary comparison failed"), result.error);
        return;
    }
    render(result.model);
    emit statusMessage(result.model.differentOffsets.isEmpty()
                           ? tr("Binary files are identical")
                           : tr("Binary comparison complete: %1 differing bytes")
                                 .arg(result.model.differentOffsets.size()));
}

void BinaryCompareWidget::render(const models::BinaryDiffModel& model) {
    const qsizetype totalLength = std::max(model.leftBytes.size(), model.rightBytes.size());
    QSet<qsizetype> differences;
    differences.reserve(model.differentOffsets.size());
    for (const qsizetype offset : model.differentOffsets) {
        differences.insert(offset);
    }
    const RenderedBytes left = formatBytes(model.leftBytes, totalLength, differences);
    const RenderedBytes right = formatBytes(model.rightBytes, totalLength, differences);
    m_leftView->setPlainText(left.text);
    m_rightView->setPlainText(right.text);
    applyHighlights(m_leftView, left.differenceRanges);
    applyHighlights(m_rightView, right.differenceRanges);
}

} // namespace bcclone::ui
