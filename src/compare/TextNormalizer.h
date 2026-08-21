#pragma once

#include <QString>
#include <QStringList>

namespace bcclone::compare {

struct NormalizationOptions {
    bool ignoreWhitespace{false};
    bool ignoreCase{false};
};

class TextNormalizer final {
public:
    [[nodiscard]] static QStringList splitLines(const QString& text);
    [[nodiscard]] static QString normalizeLine(QString line,
                                               const NormalizationOptions& options);
};

} // namespace bcclone::compare
