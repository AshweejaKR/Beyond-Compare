#include "ui/MainWindow.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QColor>
#include <QFont>
#include <QKeySequence>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QStatusBar>
#include <QTabWidget>
#include <QToolBar>

#include "ui/BinaryCompareWidget.h"
#include "ui/FolderCompareWidget.h"
#include "ui/TextCompareWidget.h"
#include "utils/Logger.h"

namespace bcclone::ui {

namespace {

using bcclone::utils::LogLevel;
using bcclone::utils::Logger::log;

QIcon placeholderAppIcon() {
    QPixmap pixmap(64, 64);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor("#2B579A"));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(pixmap.rect(), 12, 12);

    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setBold(true);
    font.setPixelSize(28);
    painter.setFont(font);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, "BC");

    return QIcon(pixmap);
}

} // namespace

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), m_settings(std::make_unique<utils::SettingsManager>()) {
    setWindowTitle(tr("BCClone"));
    setWindowIcon(placeholderAppIcon());
    m_tabs = new QTabWidget(this);
    m_textCompareWidget = new TextCompareWidget(m_tabs);
    m_folderCompareWidget = new FolderCompareWidget(m_tabs);
    m_binaryCompareWidget = new BinaryCompareWidget(m_tabs);
    m_tabs->addTab(m_textCompareWidget, tr("Text Compare"));
    m_tabs->addTab(m_folderCompareWidget, tr("Folder Compare"));
    m_tabs->addTab(m_binaryCompareWidget, tr("Binary Compare"));
    setCentralWidget(m_tabs);

    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    restoreWindowState();

    connect(m_textCompareWidget, &TextCompareWidget::statusMessage, this,
            [this](const QString& message) { statusBar()->showMessage(message); });
    connect(m_folderCompareWidget, &FolderCompareWidget::statusMessage, this,
            [this](const QString& message) { statusBar()->showMessage(message); });
    connect(m_binaryCompareWidget, &BinaryCompareWidget::statusMessage, this,
            [this](const QString& message) { statusBar()->showMessage(message); });

    log(LogLevel::Info, "MainWindow initialized");
}

MainWindow::~MainWindow() = default;

void MainWindow::setupMenuBar() {
    auto* fileMenu = menuBar()->addMenu(tr("&File"));
    auto* newAction = fileMenu->addAction(tr("&New Comparison"));
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, &QAction::triggered,
            m_textCompareWidget, &TextCompareWidget::newComparison);

    fileMenu->addSeparator();
    auto* openLeftAction = fileMenu->addAction(tr("Open &Left File..."));
    connect(openLeftAction, &QAction::triggered,
            m_textCompareWidget, &TextCompareWidget::openLeftFile);
    auto* openRightAction = fileMenu->addAction(tr("Open &Right File..."));
    connect(openRightAction, &QAction::triggered,
            m_textCompareWidget, &TextCompareWidget::openRightFile);
    auto* compareAction = fileMenu->addAction(tr("&Compare Files"));
    connect(compareAction, &QAction::triggered,
            m_textCompareWidget, &TextCompareWidget::compareFiles);
    auto* refreshAction = fileMenu->addAction(tr("&Refresh Files"));
    refreshAction->setShortcut(Qt::Key_F5);
    connect(refreshAction, &QAction::triggered,
            m_textCompareWidget, &TextCompareWidget::refreshFiles);

    fileMenu->addSeparator();
    auto* exitAction = fileMenu->addAction(tr("E&xit"));
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);

    menuBar()->addMenu(tr("&Edit"));
    auto* viewMenu = menuBar()->addMenu(tr("&View"));
    auto* lightThemeAction = viewMenu->addAction(tr("&Light Theme"));
    auto* darkThemeAction = viewMenu->addAction(tr("&Dark Theme"));
    connect(lightThemeAction, &QAction::triggered, this, &MainWindow::applyLightTheme);
    connect(darkThemeAction, &QAction::triggered, this, &MainWindow::applyDarkTheme);

    auto* helpMenu = menuBar()->addMenu(tr("&Help"));
    auto* aboutAction = helpMenu->addAction(tr("&About BCClone"));
    connect(aboutAction, &QAction::triggered, this, [this] {
        QMessageBox::about(this, tr("About BCClone"),
                            tr("BCClone\n\nA modern, native alternative to Beyond Compare."));
    });
}

void MainWindow::setupToolBar() {
    auto* toolBar = addToolBar(tr("Main Toolbar"));
    toolBar->setObjectName("MainToolBar");
    toolBar->setMovable(false);

    auto* newAction = toolBar->addAction(tr("New Comparison"));
    connect(newAction, &QAction::triggered,
            m_textCompareWidget, &TextCompareWidget::newComparison);
    auto* openLeftAction = toolBar->addAction(tr("Open Left"));
    connect(openLeftAction, &QAction::triggered,
            m_textCompareWidget, &TextCompareWidget::openLeftFile);
    auto* openRightAction = toolBar->addAction(tr("Open Right"));
    connect(openRightAction, &QAction::triggered,
            m_textCompareWidget, &TextCompareWidget::openRightFile);
    auto* compareAction = toolBar->addAction(tr("Compare"));
    connect(compareAction, &QAction::triggered,
            m_textCompareWidget, &TextCompareWidget::compareFiles);
    auto* refreshAction = toolBar->addAction(tr("Refresh"));
    connect(refreshAction, &QAction::triggered,
            m_textCompareWidget, &TextCompareWidget::refreshFiles);
}

void MainWindow::applyDarkTheme() {
    qApp->setStyleSheet(QStringLiteral(
        "QWidget { background: #252526; color: #e8e8e8; }"
        "QLineEdit, QPlainTextEdit { background: #1e1e1e; color: #e8e8e8; "
        "border: 1px solid #555; }"
        "QPushButton { background: #3b3b3b; padding: 5px 12px; border: 1px solid #666; }"
        "QPushButton:hover { background: #4b4b4b; }"
        "QMenu::item:selected { background: #2b579a; }"));
}

void MainWindow::applyLightTheme() {
    qApp->setStyleSheet({});
}

void MainWindow::setupStatusBar() {
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::restoreWindowState() {
    resize(m_settings->windowSize());

    const QPoint position = m_settings->windowPosition();
    if (!position.isNull()) {
        move(position);
    }
}

void MainWindow::saveWindowState() {
    m_settings->setWindowSize(size());
    m_settings->setWindowPosition(pos());
}

void MainWindow::closeEvent(QCloseEvent* event) {
    saveWindowState();
    log(LogLevel::Info, "MainWindow closing");
    QMainWindow::closeEvent(event);
}

} // namespace bcclone::ui
