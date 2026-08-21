#pragma once

#include <QString>
#include <QVector>

namespace bcclone::models {

enum class DiffType {
    Equal,
    Added,
    Removed
};

struct DiffLine {
    DiffType type{DiffType::Equal};
    QString leftText;
    QString rightText;
    int leftLine{-1};
    int rightLine{-1};
};

using DiffModel = QVector<DiffLine>;

} // namespace bcclone::models
