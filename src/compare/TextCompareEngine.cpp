#include "compare/TextCompareEngine.h"

#include <algorithm>
#include <cstddef>
#include <vector>

namespace bcclone::compare {

models::DiffModel TextCompareEngine::compare(const QString& leftText,
                                             const QString& rightText,
                                             const NormalizationOptions& options) const {
    const QStringList leftLines = TextNormalizer::splitLines(leftText);
    const QStringList rightLines = TextNormalizer::splitLines(rightText);

    QStringList normalizedLeft;
    QStringList normalizedRight;
    normalizedLeft.reserve(leftLines.size());
    normalizedRight.reserve(rightLines.size());
    for (const QString& line : leftLines) {
        normalizedLeft.append(TextNormalizer::normalizeLine(line, options));
    }
    for (const QString& line : rightLines) {
        normalizedRight.append(TextNormalizer::normalizeLine(line, options));
    }

    const std::size_t leftCount = static_cast<std::size_t>(leftLines.size());
    const std::size_t rightCount = static_cast<std::size_t>(rightLines.size());
    const std::size_t columns = rightCount + 1U;
    std::vector<int> lcs((leftCount + 1U) * columns, 0);
    const auto cell = [columns, &lcs](const std::size_t row, const std::size_t column) -> int& {
        return lcs[(row * columns) + column];
    };

    for (std::size_t left = leftCount; left-- > 0U;) {
        for (std::size_t right = rightCount; right-- > 0U;) {
            if (normalizedLeft.at(static_cast<qsizetype>(left)) ==
                normalizedRight.at(static_cast<qsizetype>(right))) {
                cell(left, right) = cell(left + 1U, right + 1U) + 1;
            } else {
                cell(left, right) = std::max(cell(left + 1U, right), cell(left, right + 1U));
            }
        }
    }

    models::DiffModel result;
    result.reserve(leftLines.size() + rightLines.size());
    std::size_t left = 0;
    std::size_t right = 0;
    while (left < leftCount || right < rightCount) {
        if (left < leftCount && right < rightCount &&
            normalizedLeft.at(static_cast<qsizetype>(left)) ==
                normalizedRight.at(static_cast<qsizetype>(right))) {
            result.append({models::DiffType::Equal,
                           leftLines.at(static_cast<qsizetype>(left)),
                           rightLines.at(static_cast<qsizetype>(right)),
                           static_cast<int>(left + 1U), static_cast<int>(right + 1U)});
            ++left;
            ++right;
        } else if (right < rightCount &&
                   (left == leftCount || cell(left, right + 1U) >= cell(left + 1U, right))) {
            result.append({models::DiffType::Added, {},
                           rightLines.at(static_cast<qsizetype>(right)), -1,
                           static_cast<int>(right + 1U)});
            ++right;
        } else {
            result.append({models::DiffType::Removed,
                           leftLines.at(static_cast<qsizetype>(left)), {},
                           static_cast<int>(left + 1U), -1});
            ++left;
        }
    }

    return result;
}

} // namespace bcclone::compare
