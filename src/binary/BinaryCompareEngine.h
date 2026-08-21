#pragma once

#include <QString>
#include <filesystem>

#include "models/BinaryDiffModel.h"

namespace bcclone::binary {

struct BinaryCompareResult {
    bool success{false};
    models::BinaryDiffModel model;
    QString error;
};

class BinaryCompareEngine final {
public:
    static constexpr qint64 maximumFileSize = 2 * 1024 * 1024;

    [[nodiscard]] BinaryCompareResult compare(const std::filesystem::path& leftPath,
                                              const std::filesystem::path& rightPath) const;
};

} // namespace bcclone::binary
