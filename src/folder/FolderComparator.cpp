#include "folder/FolderComparator.h"

#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QIODevice>
#include <algorithm>
#include <map>
#include <optional>
#include <system_error>

namespace bcclone::folder {

namespace {

struct FileMetadata {
    std::filesystem::path absolutePath;
    std::uintmax_t size{0};
    std::filesystem::file_time_type modified;
};

using FileMap = std::map<std::filesystem::path, FileMetadata>;

QString toQString(const std::filesystem::path& path) {
#ifdef _WIN32
    return QString::fromStdWString(path.wstring());
#else
    const auto bytes = path.u8string();
    return QString::fromUtf8(reinterpret_cast<const char*>(bytes.data()),
                             static_cast<qsizetype>(bytes.size()));
#endif
}

QString toGenericQString(const std::filesystem::path& path) {
    const auto bytes = path.generic_u8string();
    return QString::fromUtf8(reinterpret_cast<const char*>(bytes.data()),
                             static_cast<qsizetype>(bytes.size()));
}

std::optional<FileMap> scan(const std::filesystem::path& root,
                            const QStringList& filters, QString& error) {
    std::error_code statusError;
    if (!std::filesystem::is_directory(root, statusError)) {
        error = QStringLiteral("Not a readable directory: %1").arg(toQString(root));
        return std::nullopt;
    }

    FileMap files;
    std::error_code iteratorError;
    std::filesystem::recursive_directory_iterator iterator(
        root, std::filesystem::directory_options::skip_permission_denied, iteratorError);
    const std::filesystem::recursive_directory_iterator end;
    if (iteratorError) {
        error = QStringLiteral("Unable to scan %1: %2")
                    .arg(toQString(root), QString::fromStdString(iteratorError.message()));
        return std::nullopt;
    }

    while (iterator != end) {
        const std::filesystem::directory_entry entry = *iterator;
        std::error_code entryError;
        if (entry.is_regular_file(entryError)) {
            const std::filesystem::path relative = entry.path().lexically_relative(root);
            if (filters.isEmpty() || QDir::match(filters, toQString(relative.filename()))) {
                const std::uintmax_t size = entry.file_size(entryError);
                if (!entryError) {
                    const auto modified = entry.last_write_time(entryError);
                    if (!entryError) {
                        files.emplace(relative, FileMetadata{entry.path(), size, modified});
                    }
                }
            }
        }
        iterator.increment(iteratorError);
        if (iteratorError) {
            error = QStringLiteral("Unable to continue scanning %1: %2")
                        .arg(toQString(root), QString::fromStdString(iteratorError.message()));
            return std::nullopt;
        }
    }
    return files;
}

std::optional<QByteArray> fileHash(const std::filesystem::path& path) {
    QFile file(toQString(path));
    if (!file.open(QIODevice::ReadOnly)) {
        return std::nullopt;
    }

    QCryptographicHash hash(QCryptographicHash::Sha256);
    while (!file.atEnd()) {
        const QByteArray block = file.read(1024 * 1024);
        if (block.isEmpty() && file.error() != QFileDevice::NoError) {
            return std::nullopt;
        }
        hash.addData(block);
    }
    return hash.result();
}

qint64 modifiedValue(const std::filesystem::file_time_type& value) {
    return static_cast<qint64>(value.time_since_epoch().count());
}

} // namespace

FolderCompareResult FolderComparator::compare(const std::filesystem::path& leftRoot,
                                              const std::filesystem::path& rightRoot,
                                              const FolderCompareOptions& options) const {
    QString error;
    const std::optional<FileMap> leftFiles = scan(leftRoot, options.filters, error);
    if (!leftFiles) {
        return {false, {}, error};
    }
    const std::optional<FileMap> rightFiles = scan(rightRoot, options.filters, error);
    if (!rightFiles) {
        return {false, {}, error};
    }

    FolderCompareResult result;
    result.success = true;
    auto left = leftFiles->cbegin();
    auto right = rightFiles->cbegin();
    while (left != leftFiles->cend() || right != rightFiles->cend()) {
        if (right == rightFiles->cend() ||
            (left != leftFiles->cend() && left->first < right->first)) {
            result.entries.append({toGenericQString(left->first), models::FolderDiffStatus::LeftOnly,
                                   left->second.size, 0,
                                   modifiedValue(left->second.modified), 0});
            ++left;
            continue;
        }
        if (left == leftFiles->cend() || right->first < left->first) {
            result.entries.append({toGenericQString(right->first), models::FolderDiffStatus::RightOnly,
                                   0, right->second.size, 0,
                                   modifiedValue(right->second.modified)});
            ++right;
            continue;
        }

        bool equal = left->second.size == right->second.size;
        if (equal && options.compareContent) {
            const std::optional<QByteArray> leftHash = fileHash(left->second.absolutePath);
            const std::optional<QByteArray> rightHash = fileHash(right->second.absolutePath);
            if (!leftHash || !rightHash) {
                return {false, {}, QStringLiteral("Unable to hash %1").arg(toQString(left->first))};
            }
            equal = *leftHash == *rightHash;
        } else if (equal) {
            equal = left->second.modified == right->second.modified;
        }

        result.entries.append({toGenericQString(left->first),
                               equal ? models::FolderDiffStatus::Equal
                                     : models::FolderDiffStatus::Different,
                               left->second.size, right->second.size,
                               modifiedValue(left->second.modified),
                               modifiedValue(right->second.modified)});
        ++left;
        ++right;
    }
    return result;
}

} // namespace bcclone::folder
