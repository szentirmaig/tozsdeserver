#include "myserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyServer mserver;
    mserver.StartServer();

    return a.exec();
}
