#include "utils/SettingsManager.h"

#include <QSettings>

namespace bcclone::utils {

namespace {

constexpr auto kThemeKey = "appearance/theme";
constexpr auto kWindowSizeKey = "window/size";
constexpr auto kWindowPositionKey = "window/position";
constexpr auto kRecentFilesKey = "files/recent";

} // namespace

SettingsManager::SettingsManager() = default;

Theme SettingsManager::theme() const {
    const QSettings settings;
    const int value = settings.value(kThemeKey, static_cast<int>(Theme::System)).toInt();
    return static_cast<Theme>(value);
}

void SettingsManager::setTheme(Theme theme) {
    QSettings settings;
    settings.setValue(kThemeKey, static_cast<int>(theme));
}

QSize SettingsManager::windowSize() const {
    const QSettings settings;
    return settings.value(kWindowSizeKey, kDefaultWindowSize).toSize();
}

void SettingsManager::setWindowSize(const QSize& size) {
    QSettings settings;
    settings.setValue(kWindowSizeKey, size);
}

QPoint SettingsManager::windowPosition() const {
    const QSettings settings;
    return settings.value(kWindowPositionKey, QPoint()).toPoint();
}

void SettingsManager::setWindowPosition(const QPoint& position) {
    QSettings settings;
    settings.setValue(kWindowPositionKey, position);
}

QStringList SettingsManager::recentFiles() const {
    const QSettings settings;
    return settings.value(kRecentFilesKey, QStringList()).toStringList();
}

void SettingsManager::setRecentFiles(const QStringList& files) {
    QSettings settings;
    settings.setValue(kRecentFilesKey, files);
}

} // namespace bcclone::utils
