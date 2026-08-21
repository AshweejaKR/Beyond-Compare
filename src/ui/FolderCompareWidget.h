#pragma once

#include <QWidget>

class QCheckBox;
class QLineEdit;
class QTreeWidget;

namespace bcclone::ui {

class FolderCompareWidget final : public QWidget {
    Q_OBJECT

public:
    explicit FolderCompareWidget(QWidget* parent = nullptr);

signals:
    void statusMessage(const QString& message);

private:
    enum class Side { Left, Right };

    void setupUi();
    void chooseFolder(Side side);
    void compareFolders();

    QLineEdit* m_leftPath{nullptr};
    QLineEdit* m_rightPath{nullptr};
    QLineEdit* m_filters{nullptr};
    QCheckBox* m_compareContent{nullptr};
    QTreeWidget* m_tree{nullptr};
};

} // namespace bcclone::ui
