#pragma once

#include <QWidget>

class QCheckBox;
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

signals:
    void statusMessage(const QString& message);

private:
    enum class Side { Left, Right };

    void setupUi();
    void chooseFile(Side side);
    bool loadFile(const QString& path, Side side);
    void renderComparison();

    QLineEdit* m_leftPath{nullptr};
    QLineEdit* m_rightPath{nullptr};
    QPlainTextEdit* m_leftEditor{nullptr};
    QPlainTextEdit* m_rightEditor{nullptr};
    QCheckBox* m_ignoreWhitespace{nullptr};
    QCheckBox* m_ignoreCase{nullptr};
    QString m_leftText;
    QString m_rightText;
};

} // namespace bcclone::ui
