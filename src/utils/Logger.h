#pragma once

#include <filesystem>
#include <string_view>

namespace bcclone::utils {

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
};

// Lightweight, dependency-free logging facility. Writes timestamped,
// leveled messages to stderr and, optionally, to a log file.
namespace Logger {

// Optional: mirror all log output to a file in addition to stderr.
// Pass an empty path to disable file logging.
void setLogFile(const std::filesystem::path& path);

void log(LogLevel level, std::string_view message);

void debug(std::string_view message);
void info(std::string_view message);
void warning(std::string_view message);
void error(std::string_view message);

} // namespace Logger

} // namespace bcclone::utils
