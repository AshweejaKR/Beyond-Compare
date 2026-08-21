#include "ui/TextCompareWidget.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFileDialog>
#include <QFontDatabase>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QScrollBar>
#include <QSplitter>
#include <QTextBlock>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QVBoxLayout>
#include <algorithm>
#include <filesystem>

#include "compare/TextCompareEngine.h"
#include "compare/TextFileLoader.h"

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

QTextEdit::ExtraSelection lineSelection(QPlainTextEdit* editor, const int line,
                                        const QColor& color) {
    QTextEdit::ExtraSelection selection;
    const QTextBlock block = editor->document()->findBlockByNumber(line);
    selection.cursor = QTextCursor(block);
    selection.format.setBackground(color);
    selection.format.setForeground(QColor(32, 32, 32));
    selection.format.setProperty(QTextFormat::FullWidthSelection, true);
    return selection;
}

} // namespace

TextCompareWidget::TextCompareWidget(QWidget* parent) : QWidget(parent) {
    setupUi();
}

void TextCompareWidget::setupUi() {
    auto* rootLayout = new QVBoxLayout(this);
    auto* pathsLayout = new QFormLayout();

    auto createPathRow = [this](QLineEdit*& pathEdit, const Side side) {
        auto* row = new QWidget(this);
        auto* layout = new QHBoxLayout(row);
        layout->setContentsMargins(0, 0, 0, 0);
        pathEdit = new QLineEdit(row);
        pathEdit->setReadOnly(true);
        pathEdit->setPlaceholderText(tr("Select a text file"));
        auto* browse = new QPushButton(tr("Browse..."), row);
        connect(browse, &QPushButton::clicked, this, [this, side] { chooseFile(side); });
        layout->addWidget(pathEdit, 1);
        layout->addWidget(browse);
        return row;
    };

    pathsLayout->addRow(tr("Left file:"), createPathRow(m_leftPath, Side::Left));
    pathsLayout->addRow(tr("Right file:"), createPathRow(m_rightPath, Side::Right));
    rootLayout->addLayout(pathsLayout);

    auto* optionsLayout = new QHBoxLayout();
    m_ignoreWhitespace = new QCheckBox(tr("Ignore whitespace"), this);
    m_ignoreCase = new QCheckBox(tr("Ignore case"), this);
    m_showLineNumbers = new QCheckBox(tr("Show line numbers"), this);
    m_showLineNumbers->setChecked(true);
    m_viewMode = new QComboBox(this);
    m_viewMode->addItem(tr("Show all lines"));
    m_viewMode->addItem(tr("Show differences only"));
    auto* refreshButton = new QPushButton(tr("Refresh"), this);
    refreshButton->setToolTip(tr("Reload both files from disk and compare again (F5)"));
    auto* compareButton = new QPushButton(tr("Compare"), this);
    compareButton->setDefault(true);
    connect(refreshButton, &QPushButton::clicked, this, &TextCompareWidget::refreshFiles);
    connect(compareButton, &QPushButton::clicked, this, &TextCompareWidget::compareFiles);
    optionsLayout->addWidget(m_ignoreWhitespace);
    optionsLayout->addWidget(m_ignoreCase);
    optionsLayout->addWidget(m_showLineNumbers);
    optionsLayout->addSpacing(12);
    optionsLayout->addWidget(new QLabel(tr("View:"), this));
    optionsLayout->addWidget(m_viewMode);
    optionsLayout->addStretch();
    optionsLayout->addWidget(refreshButton);
    optionsLayout->addWidget(compareButton);
    rootLayout->addLayout(optionsLayout);

    auto* splitter = new QSplitter(Qt::Horizontal, this);
    m_leftEditor = new QPlainTextEdit(splitter);
    m_rightEditor = new QPlainTextEdit(splitter);
    for (QPlainTextEdit* editor : {m_leftEditor, m_rightEditor}) {
        editor->setReadOnly(true);
        editor->setLineWrapMode(QPlainTextEdit::NoWrap);
        editor->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    }
    splitter->addWidget(m_leftEditor);
    splitter->addWidget(m_rightEditor);
    splitter->setSizes({500, 500});
    rootLayout->addWidget(splitter, 1);

    connect(m_leftEditor->verticalScrollBar(), &QScrollBar::valueChanged,
            m_rightEditor->verticalScrollBar(), &QScrollBar::setValue);
    connect(m_rightEditor->verticalScrollBar(), &QScrollBar::valueChanged,
            m_leftEditor->verticalScrollBar(), &QScrollBar::setValue);
    connect(m_ignoreWhitespace, &QCheckBox::toggled, this, &TextCompareWidget::compareFiles);
    connect(m_ignoreCase, &QCheckBox::toggled, this, &TextCompareWidget::compareFiles);
    connect(m_showLineNumbers, &QCheckBox::toggled,
            this, &TextCompareWidget::compareFiles);
    connect(m_viewMode, &QComboBox::currentIndexChanged,
            this, &TextCompareWidget::compareFiles);
}

void TextCompareWidget::newComparison() {
    m_leftPath->clear();
    m_rightPath->clear();
    m_leftEditor->clear();
    m_rightEditor->clear();
    m_leftText.clear();
    m_rightText.clear();
    emit statusMessage(tr("Ready"));
}

void TextCompareWidget::openLeftFile() {
    chooseFile(Side::Left);
}

void TextCompareWidget::openRightFile() {
    chooseFile(Side::Right);
}

void TextCompareWidget::chooseFile(const Side side) {
    const QString path = QFileDialog::getOpenFileName(
        this, side == Side::Left ? tr("Open left file") : tr("Open right file"));
    if (!path.isEmpty() && loadFile(path, side) &&
        !m_leftPath->text().isEmpty() && !m_rightPath->text().isEmpty()) {
        renderComparison();
    }
}

bool TextCompareWidget::loadFile(const QString& path, const Side side) {
    const compare::TextLoadResult result = compare::TextFileLoader::load(toPath(path));
    if (!result.success) {
        QMessageBox::critical(this, tr("Unable to open file"), result.error);
        return false;
    }

    if (side == Side::Left) {
        m_leftPath->setText(path);
        m_leftText = result.text;
        m_leftEditor->setPlainText(result.text);
    } else {
        m_rightPath->setText(path);
        m_rightText = result.text;
        m_rightEditor->setPlainText(result.text);
    }
    emit statusMessage(tr("Loaded %1").arg(path));
    return true;
}

void TextCompareWidget::compareFiles() {
    if (m_leftPath->text().isEmpty() || m_rightPath->text().isEmpty()) {
        return;
    }
    renderComparison();
}

void TextCompareWidget::refreshFiles() {
    if (m_leftPath->text().isEmpty() || m_rightPath->text().isEmpty()) {
        QMessageBox::information(this, tr("Select files"),
                                 tr("Select both text files before refreshing."));
        return;
    }

    const compare::TextLoadResult left =
        compare::TextFileLoader::load(toPath(m_leftPath->text()));
    const compare::TextLoadResult right =
        compare::TextFileLoader::load(toPath(m_rightPath->text()));
    if (!left.success || !right.success) {
        const QString error = !left.success ? left.error : right.error;
        QMessageBox::critical(this, tr("Unable to refresh files"), error);
        return;
    }

    m_leftText = left.text;
    m_rightText = right.text;
    renderComparison();
}

QString TextCompareWidget::displayLine(const QString& text, const int lineNumber,
                                       const int numberWidth) const {
    if (!m_showLineNumbers->isChecked()) {
        return text;
    }
    const QString number = lineNumber > 0
        ? QString::number(lineNumber).rightJustified(numberWidth)
        : QString(numberWidth, QLatin1Char(' '));
    return QStringLiteral("%1 | %2").arg(number, text);
}

void TextCompareWidget::renderComparison() {
    compare::TextCompareEngine engine;
    const compare::NormalizationOptions options{m_ignoreWhitespace->isChecked(),
                                                 m_ignoreCase->isChecked()};
    const models::DiffModel model = engine.compare(m_leftText, m_rightText, options);

    int maximumLeftLine = 0;
    int maximumRightLine = 0;
    for (const models::DiffLine& line : model) {
        maximumLeftLine = std::max(maximumLeftLine, line.leftLine);
        maximumRightLine = std::max(maximumRightLine, line.rightLine);
    }
    const int leftNumberWidth = QString::number(maximumLeftLine).size();
    const int rightNumberWidth = QString::number(maximumRightLine).size();

    const bool differencesOnly = m_viewMode->currentIndex() == 1;
    models::DiffModel visibleModel;
    visibleModel.reserve(model.size());
    for (const models::DiffLine& line : model) {
        if (!differencesOnly || line.type != models::DiffType::Equal) {
            visibleModel.append(line);
        }
    }

    QStringList leftLines;
    QStringList rightLines;
    leftLines.reserve(visibleModel.size());
    rightLines.reserve(visibleModel.size());
    for (const models::DiffLine& line : visibleModel) {
        leftLines.append(displayLine(line.leftText, line.leftLine, leftNumberWidth));
        rightLines.append(displayLine(line.rightText, line.rightLine, rightNumberWidth));
    }
    m_leftEditor->setPlainText(leftLines.join(QLatin1Char('\n')));
    m_rightEditor->setPlainText(rightLines.join(QLatin1Char('\n')));

    QList<QTextEdit::ExtraSelection> leftSelections;
    QList<QTextEdit::ExtraSelection> rightSelections;
    int differenceCount = 0;
    for (qsizetype index = 0; index < visibleModel.size(); ++index) {
        const models::DiffType type = visibleModel.at(index).type;
        if (type == models::DiffType::Removed) {
            leftSelections.append(lineSelection(m_leftEditor, static_cast<int>(index),
                                                QColor(255, 205, 205)));
            rightSelections.append(lineSelection(m_rightEditor, static_cast<int>(index),
                                                 QColor(245, 245, 245)));
            ++differenceCount;
        } else if (type == models::DiffType::Added) {
            leftSelections.append(lineSelection(m_leftEditor, static_cast<int>(index),
                                                QColor(245, 245, 245)));
            rightSelections.append(lineSelection(m_rightEditor, static_cast<int>(index),
                                                 QColor(205, 255, 205)));
            ++differenceCount;
        }
    }
    m_leftEditor->setExtraSelections(leftSelections);
    m_rightEditor->setExtraSelections(rightSelections);
    if (differenceCount == 0) {
        emit statusMessage(tr("Files are identical"));
    } else if (differencesOnly) {
        emit statusMessage(tr("Showing %1 differing lines").arg(differenceCount));
    } else {
        emit statusMessage(tr("Comparison complete: %1 changed lines").arg(differenceCount));
    }
}

} // namespace bcclone::ui
