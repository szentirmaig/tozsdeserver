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
    if(Data.startsWith("REGISZTRACIO "))
    {
        regisztracioEllenorzese(Data);
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
            QSqlQuery online(db);
            online.exec("UPDATE felhasznalok set belepve=1 WHERE felhasznalonev='" + uzenetek.at(1) + "'");
            felhasznalok.insert(socketDescriptor,uzenetek.at(1));
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

void MyThread::regisztracioEllenorzese(QByteArray uzenet)
{
    uzenet.replace("REGISZTRACIO ","");
    QList<QByteArray> uzenetek=uzenet.split(',');
    QString felhasznalonev=uzenetek[0];
    QString nev = QString::fromLocal8Bit(uzenetek[1]);
    QString email=uzenetek[2];
    QString telefonszam=uzenetek[3];
    QString jelszo=uzenetek[4];
    QSqlQuery felhasznalok("SELECT * FROM felhasznalok WHERE felhasznalonev='" + uzenetek.at(0) + "'",db);
    if(!felhasznalok.next())
    {
        QSqlQuery insertaccount(db);
        insertaccount.prepare("INSERT INTO felhasznalok(felhasznalonev,nev,email,telefonszam,jelszo) VALUES(:felhasznalonev,:nev,:email,:telefonszam,:jelszo)");
        insertaccount.bindValue(":felhasznalonev",felhasznalonev);
        insertaccount.bindValue(":nev",nev);
        insertaccount.bindValue(":email",email);
        insertaccount.bindValue(":telefonszam",telefonszam);
        insertaccount.bindValue(":jelszo",jelszo);
        if(!insertaccount.exec())
        {
            socket->write("REGISZTRACIO nem tudjuk letrehozni a felhasznaloi fiokot");
            qDebug()<<insertaccount.lastError();
        }
        else
        {
            socket->write("REGISZTRACIO fiok hozzaadva");
            szamlaletrehozas();
            QSqlQuery lastaccount("SELECT top 1 * FROM szamlak order by id desc",db);
            lastaccount.next();
            QString szamlaid=lastaccount.value("id").toString();
            QSqlQuery updateaccount(db);
            updateaccount.exec("UPDATE felhasznalok SET szamlaid=" + szamlaid + " WHERE felhasznalonev='" + felhasznalonev + "'");
            emit fiokhozzaadva();
        }
    }
    else
    {
        socket->write("REGISZTRACIO a felhasznalonev mar foglalt");
    }
}

void MyThread::szamlaletrehozas()
{
    QSqlQuery insertszamla(db);
    insertszamla.prepare("INSERT INTO szamlak VALUES(?,?,?,?)");
    insertszamla.addBindValue("USD");
    insertszamla.addBindValue(10000000);
    insertszamla.addBindValue(10000000);
    insertszamla.addBindValue(0);
    if(!insertszamla.exec())
    {
        qDebug()<< "Nem jott letre a szamla (" << insertszamla.lastError() << ")";
    }
}

void MyThread::disconnected()
{
    qDebug()<< socketDescriptor << " Disconnected";
    QSqlQuery online(db);
    online.exec("UPDATE felhasznalok set belepve=0 WHERE felhasznalonev='" + felhasznalok[socketDescriptor] + "'");
    felhasznalok.remove(socketDescriptor);
    socket->deleteLater();
    exit(0);
}
