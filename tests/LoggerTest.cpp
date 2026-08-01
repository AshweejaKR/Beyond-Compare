#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>

#include "utils/Logger.h"

using bcclone::utils::LogLevel;
namespace Logger = bcclone::utils::Logger;

namespace {

std::filesystem::path uniqueTempLogPath() {
    return std::filesystem::temp_directory_path() /
           std::filesystem::path("bcclone_logger_test.log");
}

} // namespace

TEST(LoggerTest, WritesMessagesToConfiguredLogFile) {
    const std::filesystem::path logPath = uniqueTempLogPath();
    std::filesystem::remove(logPath);

    Logger::setLogFile(logPath);
    Logger::info("sample info message");
    Logger::setLogFile({}); // flush/close before reading

    ASSERT_TRUE(std::filesystem::exists(logPath));

    std::ifstream file(logPath);
    std::ostringstream contents;
    contents << file.rdbuf();

    EXPECT_NE(contents.str().find("INFO"), std::string::npos);
    EXPECT_NE(contents.str().find("sample info message"), std::string::npos);

    std::filesystem::remove(logPath);
}

TEST(LoggerTest, AllSeverityLevelsCanBeLogged) {
    EXPECT_NO_THROW(Logger::debug("debug"));
    EXPECT_NO_THROW(Logger::info("info"));
    EXPECT_NO_THROW(Logger::warning("warning"));
    EXPECT_NO_THROW(Logger::error("error"));
}
