#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCoreApplication>
#include <QtSql>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void TestSQLite();

    void DBInfo(QSqlDatabase& _db);
    void RecordInfo(QSqlDatabase& _db, QString _name);
    void RecordInfo2(QSqlDatabase& _db, QString _name);
    void RecordInfo3(QSqlDatabase& _db, QSqlDatabase& _db2, QString _name, QString _name2);
    void DisplayQuery(QSqlDatabase& _db, QString _q);
    void displayStringList(const QStringList& _list);


private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_released();

    void on_pushButton_4_released();

    void on_pushButton_released();

    void on_pushButton_6_released();
    void on_pushButton_5_released();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlDatabase db2;
};
#endif // MAINWINDOW_H
