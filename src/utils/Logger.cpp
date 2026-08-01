#include "utils/Logger.h"

#include <chrono>
#include <format>
#include <fstream>
#include <iostream>
#include <mutex>
#include <optional>

namespace bcclone::utils::Logger {

namespace {

std::mutex g_mutex;
std::optional<std::ofstream> g_logFile;

constexpr std::string_view levelLabel(LogLevel level) {
    switch (level) {
        case LogLevel::Debug:
            return "DEBUG";
        case LogLevel::Info:
            return "INFO";
        case LogLevel::Warning:
            return "WARNING";
        case LogLevel::Error:
            return "ERROR";
    }
    return "UNKNOWN";
}

std::string timestamp() {
    using namespace std::chrono;
    const auto now = system_clock::now();
    return std::format("{:%Y-%m-%d %H:%M:%S}", floor<seconds>(now));
}

} // namespace

void setLogFile(const std::filesystem::path& path) {
    std::lock_guard lock(g_mutex);
    g_logFile.reset();
    if (path.empty()) {
        return;
    }
    g_logFile.emplace(path, std::ios::out | std::ios::app);
}

void log(LogLevel level, std::string_view message) {
    std::lock_guard lock(g_mutex);
    const std::string line = std::format("[{}] [{}] {}", timestamp(), levelLabel(level), message);

    std::ostream& out = (level == LogLevel::Error) ? std::cerr : std::cout;
    out << line << std::endl;

    if (g_logFile && g_logFile->is_open()) {
        *g_logFile << line << std::endl;
    }
}

void debug(std::string_view message) {
    log(LogLevel::Debug, message);
}

void info(std::string_view message) {
    log(LogLevel::Info, message);
}

void warning(std::string_view message) {
    log(LogLevel::Warning, message);
}

void error(std::string_view message) {
    log(LogLevel::Error, message);
}

} // namespace bcclone::utils::Logger
