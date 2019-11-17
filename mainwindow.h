#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controller.h"
#include "member.h"
#include "record.h"
#include "admin.h"
#include "commodity.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Controller *controller,QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_btn_record_clicked();

    void on_btn_member_clicked();

private:
    Ui::MainWindow *ui;
    Controller *m_controller;


};

#endif // MAINWINDOW_H