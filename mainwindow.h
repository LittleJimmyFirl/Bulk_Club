#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QtDebug>
#include <QFile>
#include <QCoreApplication>
#include <QTextStream>
#include <QFileInfo>
#include <QKeyEvent>
#include <QDate>
#include "controller.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(Controller *controller, QWidget *parent = nullptr);
    ~MainWindow();

    int page = 0;

private slots:


    void on_pushButton_2_clicked();

    void on_pushButton_clicked();
    void changeToAdmin();
    void changeToManager();
    void changeToValidate();
    void changeToMain();

    void on_viewItems_clicked();

    void on_loadSales_clicked();

    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_loadSales_2_clicked();


    void on_comboBox_2_currentIndexChanged(const QString &arg1);

    void on_validate_clicked();

    void showTables();


    QString generateMemberID();

    QString getMonth(QString month);

    QString getFullMonth(QString month);

    void on_createMember_clicked();

    void on_adminTable_clicked(const QModelIndex &index);

    void on_purchaseButton_clicked();

    void savetoNewFile(QString date, QString ID, QString item, QString price, QString QTY);

    void on_logoutAdmin_clicked();


    void on_logOut_clicked();

protected:
    void keyPressEvent(QKeyEvent* pe);

private:
    Ui::MainWindow *ui;
    Controller *m_controller;
};
#endif // MAINWINDOW_H
