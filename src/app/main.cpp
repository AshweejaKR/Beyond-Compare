#include <QApplication>

#include "ui/MainWindow.h"
#include "utils/Logger.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QApplication::setOrganizationName("BCClone");
    QApplication::setApplicationName("BCClone");
    QApplication::setApplicationVersion("0.1.0");

    bcclone::utils::Logger::info("BCClone starting up");

    bcclone::ui::MainWindow window;
    window.show();

    const int result = QApplication::exec();

    bcclone::utils::Logger::info("BCClone shutting down");
    return result;
}
