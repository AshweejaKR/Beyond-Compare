#include "ui/MainWindow.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QColor>
#include <QFont>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QStatusBar>
#include <QToolBar>

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

    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    restoreWindowState();

    log(LogLevel::Info, "MainWindow initialized");
}

MainWindow::~MainWindow() = default;

void MainWindow::setupMenuBar() {
    auto* fileMenu = menuBar()->addMenu(tr("&File"));
    auto* exitAction = fileMenu->addAction(tr("E&xit"));
    exitAction->setShortcut(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);

    menuBar()->addMenu(tr("&Edit"));
    menuBar()->addMenu(tr("&View"));

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

    auto* placeholderAction = toolBar->addAction(tr("New Comparison"));
    placeholderAction->setEnabled(false);
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
