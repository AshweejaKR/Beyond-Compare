#include "binary/BinaryCompareEngine.h"

#include <QFile>
#include <QIODevice>
#include <algorithm>
#include <optional>
#include <utility>

namespace bcclone::binary {

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

std::optional<QByteArray> readFile(const std::filesystem::path& path, QString& error) {
    QFile file(toQString(path));
    if (!file.open(QIODevice::ReadOnly)) {
        error = QStringLiteral("Unable to open %1: %2").arg(file.fileName(), file.errorString());
        return std::nullopt;
    }
    if (file.size() > BinaryCompareEngine::maximumFileSize) {
        error = QStringLiteral("%1 is larger than the %2 MiB viewer limit")
                    .arg(file.fileName())
                    .arg(BinaryCompareEngine::maximumFileSize / (1024 * 1024));
        return std::nullopt;
    }
    const QByteArray bytes = file.readAll();
    if (file.error() != QFileDevice::NoError) {
        error = QStringLiteral("Unable to read %1: %2").arg(file.fileName(), file.errorString());
        return std::nullopt;
    }
    return bytes;
}

} // namespace

BinaryCompareResult BinaryCompareEngine::compare(const std::filesystem::path& leftPath,
                                                 const std::filesystem::path& rightPath) const {
    QString error;
    const std::optional<QByteArray> left = readFile(leftPath, error);
    if (!left) {
        return {false, {}, error};
    }
    const std::optional<QByteArray> right = readFile(rightPath, error);
    if (!right) {
        return {false, {}, error};
    }

    models::BinaryDiffModel model;
    model.leftBytes = *left;
    model.rightBytes = *right;
    const qsizetype length = std::max(left->size(), right->size());
    for (qsizetype offset = 0; offset < length; ++offset) {
        if (offset >= left->size() || offset >= right->size() ||
            left->at(offset) != right->at(offset)) {
            model.differentOffsets.append(offset);
        }
    }
    return {true, std::move(model), {}};
}

} // namespace bcclone::binary
