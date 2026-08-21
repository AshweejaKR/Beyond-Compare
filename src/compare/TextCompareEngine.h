#pragma once

#include <QString>

#include "compare/TextNormalizer.h"
#include "models/DiffModel.h"

namespace bcclone::compare {

class TextCompareEngine final {
public:
    [[nodiscard]] models::DiffModel compare(const QString& leftText,
                                            const QString& rightText,
                                            const NormalizationOptions& options = {}) const;
};

} // namespace bcclone::compare
