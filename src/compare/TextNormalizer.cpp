#include "compare/TextNormalizer.h"

#include <QRegularExpression>

namespace bcclone::compare {

QStringList TextNormalizer::splitLines(const QString& text) {
    QString normalized = text;
    normalized.replace(QStringLiteral("\r\n"), QStringLiteral("\n"));
    normalized.replace(QLatin1Char('\r'), QLatin1Char('\n'));
    return normalized.split(QLatin1Char('\n'), Qt::KeepEmptyParts);
}

QString TextNormalizer::normalizeLine(QString line, const NormalizationOptions& options) {
    if (options.ignoreWhitespace) {
        static const QRegularExpression whitespace(QStringLiteral("\\s+"));
        line = line.trimmed();
        line.replace(whitespace, QStringLiteral(" "));
    }
    if (options.ignoreCase) {
        line = line.toCaseFolded();
    }
    return line;
}

} // namespace bcclone::compare
