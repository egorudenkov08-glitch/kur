#include "mainWindow.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Weather Widget");

    MainWindow window;
    window.show();

    return app.exec();
}
