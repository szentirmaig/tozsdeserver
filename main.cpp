#include "mainwindow.h"
#include "myserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyServer mserver;
    mserver.StartServer();
    MainWindow w;
    w.show();
    return a.exec();
}
