#pragma once

#include <QMainWindow>
#include <memory>

#include "utils/SettingsManager.h"

namespace bcclone::ui {

// Top-level application window. Presentation only: owns the menu bar,
// toolbar, status bar, and window chrome. No comparison logic lives here.
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void restoreWindowState();
    void saveWindowState();

    std::unique_ptr<utils::SettingsManager> m_settings;
};

} // namespace bcclone::ui
