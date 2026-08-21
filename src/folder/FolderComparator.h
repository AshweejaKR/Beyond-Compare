#pragma once

#include <QString>
#include <QStringList>
#include <filesystem>

#include "models/FolderDiffModel.h"

namespace bcclone::folder {

struct FolderCompareOptions {
    QStringList filters{QStringLiteral("*")};
    bool compareContent{true};
};

struct FolderCompareResult {
    bool success{false};
    models::FolderDiffModel entries;
    QString error;
};

class FolderComparator final {
public:
    [[nodiscard]] FolderCompareResult compare(const std::filesystem::path& leftRoot,
                                              const std::filesystem::path& rightRoot,
                                              const FolderCompareOptions& options = {}) const;
};

} // namespace bcclone::folder
