#pragma once

#include <QWidget>

class QLineEdit;
class QPlainTextEdit;

namespace bcclone::models {
struct BinaryDiffModel;
}

namespace bcclone::ui {

class BinaryCompareWidget final : public QWidget {
    Q_OBJECT

public:
    explicit BinaryCompareWidget(QWidget* parent = nullptr);

signals:
    void statusMessage(const QString& message);

private:
    enum class Side { Left, Right };

    void setupUi();
    void chooseFile(Side side);
    void compareFiles();
    void render(const models::BinaryDiffModel& model);

    QLineEdit* m_leftPath{nullptr};
    QLineEdit* m_rightPath{nullptr};
    QPlainTextEdit* m_leftView{nullptr};
    QPlainTextEdit* m_rightView{nullptr};
};

} // namespace bcclone::ui
