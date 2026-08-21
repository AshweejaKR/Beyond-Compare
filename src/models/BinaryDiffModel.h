#pragma once

#include <QByteArray>
#include <QVector>

namespace bcclone::models {

struct BinaryDiffModel {
    QByteArray leftBytes;
    QByteArray rightBytes;
    QVector<qsizetype> differentOffsets;
};

} // namespace bcclone::models
