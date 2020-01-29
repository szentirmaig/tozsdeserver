#include "myserver.h"


MyServer::MyServer(QObject *parent) : QTcpServer(parent)
{
}

void MyServer::StartServer()
{
    if(!this->listen(QHostAddress::LocalHost,1234))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void MyServer::incomingConnection(qintptr handle)
{
    qDebug()<<handle<<"Connecting...";
    MyThread *thread=new MyThread(handle,this);
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    thread->start();
}
