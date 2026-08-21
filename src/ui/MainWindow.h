#pragma once

#include <QMainWindow>
#include <memory>

#include "utils/SettingsManager.h"

class QTabWidget;

namespace bcclone::ui {

class TextCompareWidget;
class FolderCompareWidget;
class BinaryCompareWidget;

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
    void applyDarkTheme();
    void applyLightTheme();
    void restoreWindowState();
    void saveWindowState();

    std::unique_ptr<utils::SettingsManager> m_settings;
    TextCompareWidget* m_textCompareWidget{nullptr};
    FolderCompareWidget* m_folderCompareWidget{nullptr};
    BinaryCompareWidget* m_binaryCompareWidget{nullptr};
    QTabWidget* m_tabs{nullptr};
};

} // namespace bcclone::ui
