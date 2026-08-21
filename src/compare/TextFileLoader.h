#pragma once

#include <QString>
#include <filesystem>

namespace bcclone::compare {

enum class TextEncoding {
    Utf8,
    Utf16LittleEndian,
    Utf16BigEndian,
    Latin1
};

struct TextLoadResult {
    bool success{false};
    QString text;
    TextEncoding encoding{TextEncoding::Utf8};
    QString error;
};

class TextFileLoader final {
public:
    [[nodiscard]] static TextLoadResult load(const std::filesystem::path& path);
};

} // namespace bcclone::compare
