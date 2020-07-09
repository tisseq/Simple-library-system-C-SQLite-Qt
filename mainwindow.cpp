#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    TestSQLite();
}

MainWindow::~MainWindow()
{
    delete ui;
    db.close();
    db2.close();
}


void MainWindow::TestSQLite()
{
    QString name;
    {
        db = QSqlDatabase::addDatabase("QSQLITE", "bazatestowa");
        db.setDatabaseName(":memory:");

        if (!db.open())
        {
             return ;
        }

        QSqlQuery query(db);
        query.exec("create table znajomi (id integer primary key AUTOINCREMENT," " imie varchar(20), nazwisko varchar(20), id_ksiazka integer)");
        query.exec("insert into znajomi (imie, nazwisko, id_ksiazka) values('Jan', 'Kowalski', 0)");
        query.exec("insert into znajomi (imie, nazwisko, id_ksiazka) values('Kuba', 'Szklanka', 0)");
        query.exec("insert into znajomi (imie, nazwisko, id_ksiazka) values('Maciek', 'Cola', 0)");
        query.exec("insert into znajomi (imie, nazwisko, id_ksiazka) values('Ania', 'Taka', 0)");

        RecordInfo2(db, "znajomi");
        name = db.connectionName();
    }

    QString name2;
    {
        db2 = QSqlDatabase::addDatabase("QSQLITE", "bazatestowa2");
        db2.setDatabaseName(":memory:");

        if (!db2.open())
        {
             return ;
        }

        QSqlQuery query(db2);
        query.exec("create table ksiazki (id integer primary key AUTOINCREMENT," " imie varchar(20), nazwisko varchar(20), id_znajomy integer)");
        query.exec("insert into ksiazki (imie, nazwisko,id_znajomy) values('''Dzuma''', 'Henryk Jagwa', 0)");
        query.exec("insert into ksiazki (imie, nazwisko, id_znajomy) values('''Pies i kot''', 'Kuba Wojewodzki', 0)");
        query.exec("insert into ksiazki (imie, nazwisko, id_znajomy) values('''Start czlowiek i morze''', 'Adam Mickiewicz', 0)");
        query.exec("insert into ksiazki (imie, nazwisko, id_znajomy) values('''Pan Tadeusz''', 'Tomasz Karolak', 0)");

        RecordInfo(db2, "ksiazki");
        name2 = db2.connectionName();
    }


}

//ksiazki
void MainWindow::RecordInfo(QSqlDatabase& _db, QString _name)
{

    QSqlQuery query(QString("select * from '") + _name + "' where id_znajomy = 0", _db);
    QString text;
    ui->listWidget->clear();
    ui->comboBox->clear();
    while(query.next())
        {
            for(int i= 0; i < query.record().count()-1; i++)
            {
                text += (((const char*)query.value(i).toString().toLatin1()));
                text += " ";
            }
            ui->comboBox->addItem(text);
            ui->listWidget->addItem(text);
            ui->comboBox_4->addItem(text);
            text="";
        }



        query.finish();
}

//znajomy
void MainWindow::RecordInfo2(QSqlDatabase& _db, QString _name)
{

    QSqlQuery query(QString("select * from '") + _name + "' where id_ksiazka = 0", _db);
    QString text;
    ui->listWidget_2->clear();
    ui->comboBox_2->clear();
    while(query.next())
        {
            for(int i= 0; i < query.record().count() -1; i++)
            {
                text += (((const char*)query.value(i).toString().toLatin1()));
                text += " ";
            }
            ui->listWidget_2->addItem(text);
            ui->comboBox_2->addItem(text);
            ui->comboBox_3->addItem(text);
            text="";
        }



        query.finish();
}

//wypozczenia
void MainWindow::RecordInfo3(QSqlDatabase& _db, QSqlDatabase& _db2, QString _name, QString _name2)
{
    QSqlQuery query(QString("select * from '") + _name + "' where id_ksiazka != 0", _db);
    QSqlQuery query2(QString("select * from '") + _name2 + "' where id_znajomy != 0", _db2);
    QString text="";
    QString text2="";

    ui->listWidget_3->clear();
    ui->comboBox_5->clear();
    while(query.next() && query2.next())
        {
            for(int i= 0; i < query.record().count() -1 ; i++)
            {
                text += (((const char*)query.value(i).toString().toLatin1()));
                text2+= (((const char*)query.value(i).toString().toLatin1()));
                text += " ";
                text2+=" ";
            }

            text +="   wypozyczyl/a   ";
            text2+= "  -  ";

            for(int i= 0; i < query2.record().count() -1; i++)
            {
                text += (((const char*)query2.value(i).toString().toLatin1()));
                text += " ";
                text2+= (((const char*)query2.value(i).toString().toLatin1()));
                text2+= " ";

            }

            ui->listWidget_3->addItem(text);
            ui->comboBox_5->addItem(text2);
            text="";
            text2="";
        }


}






void MainWindow::on_pushButton_3_clicked()
{
    if(ui->lineEdit_3->text().isEmpty() || ui->lineEdit_4->text().isEmpty()){
        return;
    }

    QSqlQuery query(db);
    QString nick, name;
    nick = (QString)ui->lineEdit_3->text();
    name = (QString)ui->lineEdit_4->text();

    query.prepare("INSERT INTO znajomi (imie, nazwisko, id_ksiazka) "
                      "VALUES (:nick, :name, :idks)");
        query.bindValue(":nick", nick);
        query.bindValue(":name", name);
        query.bindValue(":idks", 0);
        query.exec();
        ui->comboBox_3->clear();

    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    RecordInfo2(db, "znajomi");
}


void MainWindow::on_pushButton_2_released()
{
    QSqlQuery query(db2);
    QString text;

    text = ui->comboBox->currentText();

    QString firstWord = text.split(" ").at(0);
    firstWord.toInt();


    query.prepare("delete from ksiazki where id = :value");
    query.bindValue(":value", firstWord);
    query.exec();

    //QMessageBox s;
    //s.setText(firstWord);
    //s.exec();

    ui->comboBox_4->clear();
    RecordInfo(db2,"ksiazki");

}

void MainWindow::on_pushButton_4_released()
{
    QSqlQuery query(db);
    QString text;

    text = ui->comboBox_2->currentText();

    QString firstWord = text.split(" ").at(0);
    firstWord.toInt();


    query.prepare("delete from znajomi where id = :value");
    query.bindValue(":value", firstWord);
    query.exec();

    //QMessageBox s;
    //s.setText(firstWord);
    //s.exec();
    ui->comboBox_3->clear();
    RecordInfo2(db, "znajomi");

    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
}

void MainWindow::on_pushButton_released()
{
    if(ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty()){
        return;
    }

    QSqlQuery query(db2);
    QString author, name;
    author = (QString)ui->lineEdit->text();
    name = (QString)ui->lineEdit_2->text();
    query.prepare("INSERT INTO ksiazki (imie, nazwisko, id_znajomy) "
                      "VALUES (:author, :name, :idok)");
        query.bindValue(":author", author);
        query.bindValue(":name", name);
        query.bindValue(":idok", 0);
        query.exec();



        ui->comboBox_4->clear();
    RecordInfo(db2, "ksiazki");


    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
}

void MainWindow::on_pushButton_6_released()
{
    if(ui->comboBox_3->currentText().isEmpty() || ui->comboBox_4->currentText().isEmpty()){
            return;
    }

    qint32 pos, pos2;

    QString temp1 = ui->comboBox_3->currentText();
    QString firstWord = temp1.split(" ").at(0);
    pos = firstWord.toInt();

    QString temp2 = ui->comboBox_4->currentText();
    QString secondWord = temp2.split(" ").at(0);
    pos2 = secondWord.toInt();


    QSqlQuery query(db);
    QSqlQuery query2(db2);


    query2.prepare("update ksiazki set id_znajomy = :pos where id = :pos2");
    query2.bindValue(":pos", pos);
    query2.bindValue(":pos2", pos2);
    query2.exec();

    query.prepare("update znajomi set id_ksiazka = :pos where id = :pos");
    query.bindValue(":pos", pos);
    query.bindValue(":pos2", pos2);
    query.exec();




    ui->comboBox_3->clear();
    ui->comboBox_4->clear();
    RecordInfo(db2, "ksiazki");
    RecordInfo2(db, "znajomi");
    RecordInfo3(db, db2,  "znajomi", "ksiazki");



}


void MainWindow::on_pushButton_5_released()
{
    if(ui->comboBox_5->currentText().isEmpty()){
            return;
    }

    qint32 pos;
    QString text = ui->comboBox_5->currentText();
    QString temp = text.split(" ").at(0);
    pos = temp.toInt();

   QSqlQuery query(db);
   QSqlQuery query2(db2);

   query.prepare("update znajomi set id_ksiazka = :zero where id = :pos");
   query.bindValue(":pos", pos);
   query.bindValue(":zero", 0);
   query.exec();

   query2.prepare("update ksiazki set id_znajomy = :zero where id_znajomy = :pos");
   query2.bindValue(":pos", pos);
   query2.bindValue(":zero", 0);
   query2.exec();


   ui->comboBox_3->clear();
   ui->comboBox_4->clear();
   RecordInfo(db2, "ksiazki");
   RecordInfo2(db, "znajomi");
   RecordInfo3(db, db2,  "znajomi", "ksiazki");

}
