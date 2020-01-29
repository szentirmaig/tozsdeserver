#include "mythread.h"

MyThread::MyThread(int id, QObject *parent) : QThread(parent)
{
    connecttoDatabase();
    if(db.open())
    {
        qDebug()<<"Database open!";
    }
    else
    {
        qDebug()<<db.lastError();
        return;
    }
    this->socketDescriptor = id;
}

void MyThread::connecttoDatabase()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("DESKTOP-SPD4O8F\\SQLEXPRESS");
    db.setDatabaseName("mydsn64");
}

void MyThread::run()
{
    qDebug()<< socketDescriptor << "Starting thread";
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()),Qt::DirectConnection);
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()),Qt::DirectConnection);

    qDebug()<<socketDescriptor<<"Client connected";

    exec();
}

void MyThread::readyRead()
{
    QByteArray Data = socket->readAll();

    qDebug()<< socketDescriptor << " Data in: " << Data;

    if(Data.startsWith("BELEPES "))
    {
        belepesEllenorzese(Data);
    }
}

void MyThread::belepesEllenorzese(QByteArray uzenet)
{
    QList<QByteArray> uzenetek=uzenet.split(' ');
    QSqlQuery query("SELECT * FROM felhasznalok WHERE felhasznalonev='" + uzenetek.at(1) + "'",db);
    if(query.next())
    {
        if(uzenetek.at(2)==query.value("jelszo").toString() && uzenetek.at(2)!="")
        {
            socket->write("BELEPES helyes");
        }
        else
        {
            socket->write("BELEPES rossz jelszo");
        }
    }
    else
    {
        socket->write("BELEPES rossz felhasznalonev");
    }
}

void MyThread::disconnected()
{
    qDebug()<< socketDescriptor << " Disconnected";
    socket->deleteLater();
    exit(0);
}
