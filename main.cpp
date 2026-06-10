#include "MainWindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;

    mainWindow.setWindowTitle("Simulation Platform");

    mainWindow.show();

    return app.exec();
}