#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>
#include <QtSql/QtSql>


class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(int id, QObject *parent = nullptr);
    void run() override;
    void connecttoDatabase();
    void belepesEllenorzese(QByteArray uzenet);
    void regisztracioEllenorzese(QByteArray uzenet);
    void szamlaletrehozas();

signals:
    void error(QTcpSocket::SocketError socketerror);
    void fiokhozzaadva();

public slots:
    void readyRead();
    void disconnected();

private:
    QTcpSocket *socket;
    int socketDescriptor;
    QSqlDatabase db;
    QMap<int,QString> felhasznalok;
};

#endif // MYTHREAD_H
