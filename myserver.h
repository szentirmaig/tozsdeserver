#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include "mythread.h"


class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = nullptr);
    void StartServer();

signals:

public slots:

protected:
    void incomingConnection(qintptr handle) override;

private:

};

#endif // MYSERVER_H
