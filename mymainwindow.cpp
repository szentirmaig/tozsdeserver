#include "mymainwindow.h"
#include "ui_mymainwindow.h"

MyMainWindow::MyMainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyMainWindow)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Window);
//    this->showFullScreen();

    connecttoDatabase();

    if(maindb.open())
    {
        comboboxfrissit();
    }
}

MyMainWindow::~MyMainWindow()
{
    delete ui;
}

void MyMainWindow::connecttoDatabase()
{
    maindb = QSqlDatabase::addDatabase("QODBC","maindb");
    maindb.setHostName("DESKTOP-SPD4O8F\\SQLEXPRESS");
    maindb.setDatabaseName("mydsn64");
}

void MyMainWindow::comboboxfrissit()
{
    ui->comboBox_felhasznalok->clear();
    QString comboboxitem;
    QSqlQuery query("select felhasznalonev, belepve from felhasznalok",maindb);
    while(query.next())
    {
        if(query.value("belepve").toInt()==0)
        {
            comboboxitem=query.value("felhasznalonev").toString() + " (offline)";
        }
        else
        {
            comboboxitem=query.value("felhasznalonev").toString() + " (online)";
        }

        felhasznalok.push_back(query.value("felhasznalonev").toString());
        ui->comboBox_felhasznalok->addItem(comboboxitem);
    }
}

void MyMainWindow::on_comboBox_felhasznalok_currentIndexChanged(int index)
{
    QString felhasznalonev=felhasznalok.at(index);
    QSqlQuery account("SELECT * FROM felhasznalok WHERE felhasznalonev='" + felhasznalonev + "'",maindb);
    account.next();
    QString szamlaid=account.value("szamlaid").toString();

    QSqlTableModel *accountmodel = new QSqlTableModel(this,maindb);
    accountmodel->setTable("szamlak");
    accountmodel->setFilter("Id=" + szamlaid);
    accountmodel->select();

    ui->tableView_szamla->setModel(accountmodel);
    ui->tableView_szamla->verticalHeader()->hide();
    ui->tableView_szamla->setItemDelegateForColumn(2,new NumberFormatDelegate(this));
    ui->tableView_szamla->setItemDelegateForColumn(3,new NumberFormatDelegate(this));
    ui->tableView_szamla->setItemDelegateForColumn(4,new NumberFormatDelegate(this));


    QSqlTableModel *poziciokmodel = new QSqlTableModel(this,maindb);
    poziciokmodel->setTable("nyitott_poziciok");
    poziciokmodel->setFilter("SzámlaId=" + szamlaid);
    poziciokmodel->select();

    ui->tableView_nyitott_poziciok->setModel(poziciokmodel);

    QSqlTableModel *megbizasokmodel = new QSqlTableModel(this,maindb);
    megbizasokmodel->setTable("megbizasok");
    megbizasokmodel->setFilter("szamlaid=" + szamlaid);
    megbizasokmodel->select();

    ui->tableView_megbizasok->setModel(megbizasokmodel);
    ui->tableView_megbizasok->setColumnHidden(11,true);

    QSqlTableModel *lezart_poziciokmodel = new QSqlTableModel(this,maindb);
    lezart_poziciokmodel->setTable("lezart_poziciok");
    lezart_poziciokmodel->setFilter("SzámlaId=" + szamlaid);
    lezart_poziciokmodel->select();

    ui->tableView_lezart_poziciok->setModel(lezart_poziciokmodel);

}
