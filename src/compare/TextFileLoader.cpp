#include "compare/TextFileLoader.h"

#include <QByteArray>
#include <QFile>
#include <QStringDecoder>

namespace bcclone::compare {

namespace {

QString toQString(const std::filesystem::path& path) {
#ifdef _WIN32
    return QString::fromStdWString(path.wstring());
#else
    const auto bytes = path.u8string();
    return QString::fromUtf8(reinterpret_cast<const char*>(bytes.data()),
                             static_cast<qsizetype>(bytes.size()));
#endif
}

QString decodeUtf16(const QByteArray& bytes, const bool littleEndian, const qsizetype offset) {
    QString text;
    text.reserve((bytes.size() - offset) / 2);

    for (qsizetype index = offset; index + 1 < bytes.size(); index += 2) {
        const auto first = static_cast<unsigned char>(bytes.at(index));
        const auto second = static_cast<unsigned char>(bytes.at(index + 1));
        const ushort value = littleEndian
            ? static_cast<ushort>(first | (static_cast<ushort>(second) << 8U))
            : static_cast<ushort>((static_cast<ushort>(first) << 8U) | second);
        text.append(QChar(value));
    }
    return text;
}

} // namespace

TextLoadResult TextFileLoader::load(const std::filesystem::path& path) {
    QFile file(toQString(path));
    if (!file.open(QIODevice::ReadOnly)) {
        return {false, {}, TextEncoding::Utf8,
                QStringLiteral("Unable to open %1: %2").arg(file.fileName(), file.errorString())};
    }

    const QByteArray bytes = file.readAll();
    if (file.error() != QFileDevice::NoError) {
        return {false, {}, TextEncoding::Utf8,
                QStringLiteral("Unable to read %1: %2").arg(file.fileName(), file.errorString())};
    }

    if (bytes.startsWith("\xEF\xBB\xBF")) {
        return {true, QString::fromUtf8(bytes.constData() + 3, bytes.size() - 3),
                TextEncoding::Utf8, {}};
    }
    if (bytes.startsWith("\xFF\xFE")) {
        return {true, decodeUtf16(bytes, true, 2), TextEncoding::Utf16LittleEndian, {}};
    }
    if (bytes.startsWith("\xFE\xFF")) {
        return {true, decodeUtf16(bytes, false, 2), TextEncoding::Utf16BigEndian, {}};
    }

    QStringDecoder utf8Decoder(QStringDecoder::Utf8);
    const QString utf8Text = utf8Decoder.decode(bytes);
    if (!utf8Decoder.hasError()) {
        return {true, utf8Text, TextEncoding::Utf8, {}};
    }

    return {true, QString::fromLatin1(bytes), TextEncoding::Latin1, {}};
}

} // namespace bcclone::compare
