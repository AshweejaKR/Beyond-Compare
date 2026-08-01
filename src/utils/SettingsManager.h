#pragma once

#include <QPoint>
#include <QSize>
#include <QStringList>

namespace bcclone::utils {

enum class Theme {
    System,
    Light,
    Dark,
};

// Thin, RAII wrapper around QSettings for persisting user preferences.
// Not a singleton: owned by whichever component needs it (typically MainWindow).
class SettingsManager {
public:
    SettingsManager();

    Theme theme() const;
    void setTheme(Theme theme);

    QSize windowSize() const;
    void setWindowSize(const QSize& size);

    QPoint windowPosition() const;
    void setWindowPosition(const QPoint& position);

    QStringList recentFiles() const;
    void setRecentFiles(const QStringList& files);

private:
    static constexpr QSize kDefaultWindowSize{1024, 768};
};

} // namespace bcclone::utils
