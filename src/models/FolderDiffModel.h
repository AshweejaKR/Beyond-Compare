#pragma once

#include <QString>
#include <QVector>
#include <cstdint>

namespace bcclone::models {

enum class FolderDiffStatus {
    Equal,
    Different,
    LeftOnly,
    RightOnly
};

struct FolderDiffEntry {
    QString relativePath;
    FolderDiffStatus status{FolderDiffStatus::Equal};
    std::uintmax_t leftSize{0};
    std::uintmax_t rightSize{0};
    qint64 leftModified{0};
    qint64 rightModified{0};
};

using FolderDiffModel = QVector<FolderDiffEntry>;

} // namespace bcclone::models
