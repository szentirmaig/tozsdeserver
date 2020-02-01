#ifndef MYMAINWINDOW_H
#define MYMAINWINDOW_H

#include <QDialog>
#include <QDebug>
#include <QtSql/QtSql>
#include "numberformatdelegate.h"

namespace Ui {
class MyMainWindow;
}

class MyMainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MyMainWindow(QWidget *parent = nullptr);
    ~MyMainWindow();

    void connecttoDatabase();

public slots:
    void comboboxfrissit();

private slots:
    void on_comboBox_felhasznalok_currentIndexChanged(int index);

private:
    Ui::MyMainWindow *ui;
    QSqlDatabase maindb;
    QStringList felhasznalok;
};

#endif // MYMAINWINDOW_H
