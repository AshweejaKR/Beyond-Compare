#pragma once

#include <QWidget>

class QCheckBox;
class QComboBox;
class QLineEdit;
class QPlainTextEdit;

namespace bcclone::ui {

class TextCompareWidget final : public QWidget {
    Q_OBJECT

public:
    explicit TextCompareWidget(QWidget* parent = nullptr);

public slots:
    void newComparison();
    void openLeftFile();
    void openRightFile();
    void compareFiles();
    void refreshFiles();

signals:
    void statusMessage(const QString& message);

private:
    enum class Side { Left, Right };

    void setupUi();
    void chooseFile(Side side);
    bool loadFile(const QString& path, Side side);
    void renderComparison();
    [[nodiscard]] QString displayLine(const QString& text, int lineNumber,
                                      int numberWidth) const;

    QLineEdit* m_leftPath{nullptr};
    QLineEdit* m_rightPath{nullptr};
    QPlainTextEdit* m_leftEditor{nullptr};
    QPlainTextEdit* m_rightEditor{nullptr};
    QCheckBox* m_ignoreWhitespace{nullptr};
    QCheckBox* m_ignoreCase{nullptr};
    QCheckBox* m_showLineNumbers{nullptr};
    QComboBox* m_viewMode{nullptr};
    QString m_leftText;
    QString m_rightText;
};

} // namespace bcclone::ui
