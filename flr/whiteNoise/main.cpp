#include <QApplication>
#include <QtCore>
#include "window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Window window;
    //window.showMaximized();
    window.resize(512, 400);
    window.show();
    return app.exec();
}
