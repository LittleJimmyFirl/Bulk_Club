#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QMessageBox>
#include <QPixmap>

MainWindow::MainWindow(Controller *controller, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      m_controller(controller)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->MAINWINDOW);
    QPixmap pix("/Users/littlejimmyfirl/Desktop/bulkClub/Logo.png");
    int w = ui ->logo->width();
    int h = ui ->logo->height();
    ui->logo->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));

    //sami changes
    QFile file(QDir::homePath() + "/storedDates.txt");
    if(!file.exists())
    {
        qDebug() << file.fileName() << " does not exist";
    }

    QString line;
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
           while (!stream.atEnd())
           {
                line = stream.readLine();
                ui-> comboBox->addItem(line);
           }

    }
            file.close();
    /* This is just simply reading in the storedDates text and adding them to the combo box. */
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_2_clicked()
{
    this->ui->username->setText("");
    this->ui->password->setText("");
}

void MainWindow::keyPressEvent(QKeyEvent* pe)
{
    if(page == 0)
    {
        if(pe->key()== Qt::Key_Return) on_pushButton_clicked();
    }
}

void MainWindow::on_pushButton_clicked()
{
    QString username = ui->username ->text();
    QString password = ui->password->text();

    QSqlQuery qry;
    // RANK 2 IS USED FOR ADMINS
    // RANK 1 IS USED FOR STORE MANAGERS
    qry.prepare("select * from admin where username ='"+username+"' and password ='"+password+"' and rank= '2'");
    if (qry.exec())
    {
        int count = 0;
        while (qry.next())
        {
            count++;
        }
           if(count == 1)
           {
            QMessageBox::information(this, "Login", "Username and password is correct");
            changeToAdmin();
           }
           else if (count!= 1)
           {
               if(qry.exec("select * from admin where username ='"+username+"' and password ='"+password+"' and rank= '1'"))
                   count  = 0;
                    while(qry.next())
                    {
                        count++;
                    }
                    if (count == 1)
                    {
                       QMessageBox::information(this,"Login", "Username and Password is correct");
                       changeToManager();
                    }
                    else
                    {
                        QMessageBox::warning(this,"Login","Username and password is not correct");
                        this->ui->username->setText("");
                        this->ui->password->setText("");
                    }
           }
    }
}
void MainWindow::changeToAdmin()
{
    ui->stackedWidget->setCurrentWidget(ui->adminLogin);
    page++;
}

void MainWindow::changeToManager()
{
    ui->stackedWidget->setCurrentWidget(ui->managerLogin);
    page++;
}

void MainWindow::changeToMain()
{
    ui->stackedWidget->setCurrentWidget(ui->MAINWINDOW);
    page = 0;
}

void MainWindow::changeToValidate()
{
    ui->stackedWidget->setCurrentWidget(ui->addMember);
}

void MainWindow::on_viewItems_clicked()
{
   showTables();
   ui->managerTable->setModel(m_controller->getCommoditiesQueryModel());
   ui->managerTable->resizeColumnsToContents();
}

void MainWindow::on_loadSales_clicked()
{
    //sami changes

    QString comboDate;
    m_controller->readRecordFile();
    m_controller->getComboDate(comboDate);
    qDebug() << comboDate;

    ui-> comboBox->addItem(comboDate);
    QFile file(QDir::homePath() + "/storedDates.txt");
    if(!file.exists())
    {
        qDebug() << file.fileName() << " does not exist";
    }

    if(file.open(QIODevice::Append| QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream txtStream (&file);

        qDebug() << "~~~~~ Writing To File ~~~~~";

        txtStream << comboDate << endl;
        file.close();
    }
    /* The addition to the original controller function here adds functinailty
     * for the date to be added to a new file that stores the dates that have
     * already been added to the application. This is helpful because, without saving this information
     * the combobox does not keep the dynamically added dates. Therefore the dates are saved, and loaded
     * into the combobox when the application is started up.
     */

}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QString date = ui->comboBox->currentText();
    QStringList str_split = date.split(',');
    int day = str_split[1].toInt();

    QString condition = "day==";
    condition += QString::number(day);

    ui->managerTable->setModel(
    m_controller->getRecordsQueryModelWithCondition(condition));
    ui->managerTable->setColumnHidden(0,true);
    ui->managerTable->setColumnHidden(2,true);
    ui->managerTable->setColumnHidden(3,true);
    ui->managerTable->setColumnHidden(4,true);
    ui->managerTable->show();
    ui->managerTable->resizeColumnsToContents();

}
void MainWindow::showTables()
{
    ui->managerTable->setColumnHidden(0,false);
    ui->managerTable->setColumnHidden(2,false);
    ui->managerTable->setColumnHidden(3,false);
    ui->managerTable->setColumnHidden(4,false);
}
void MainWindow::on_loadSales_2_clicked()
{
   m_controller->readMemberFile();

}

void MainWindow::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    QString select = ui->comboBox_2->currentText();

    if(select == "Sort By ID")
    {
      showTables();
      ui->managerTable->setModel(m_controller->getRevenueSortedById());
      ui->managerTable->resizeColumnsToContents();

    }
    if(select == "Sort By REV")
    {
       showTables();
       ui->managerTable->setModel(m_controller->getRevenueSortedByRev());
       ui->managerTable->resizeColumnsToContents();
    }
}

void MainWindow::on_validate_clicked()
{
    changeToValidate();
}


QString MainWindow::generateMemberID()
{
    int ID;
    qsrand(QDateTime::currentMSecsSinceEpoch() / 1000);
    ID = (qrand() % 100000);
    QString memberID = QString::number(ID);
    return memberID;

    //This function generaters a random 5 digit number that will be used as a unique MemberID

}
QString MainWindow ::getMonth(QString month)
{
    if (month == "Jan")
        month = "01";
    else if (month == "Feb")
        month = "02";
    else if (month == "Mar")
        month = "03";
    else if (month == "Apr")
        month = "04";
    else if (month == "May")
        month = "05";
    else if (month == "Jun")
        month = "06";
    else if (month == "Jul")
        month = "07";
    else if (month == "Aug")
        month = "08";
    else if (month == "Sep")
        month = "09";
    else if (month == "Oct")
        month = "10";
    else if (month == "Nov")
        month = "11";
    else if (month == "Dec")
        month = "12";
    else
        qDebug() << "No Valid Month" << endl;

    return month;

    // This function takes a 3 letter month value that is taken from "QDate::currentDate()" and
    // converts it into a two number string that is formatted to work with our database.
    // Ideally this would in a switch statement however that only functions properly with integer values or enum values.
}

QString MainWindow ::getFullMonth(QString month)
{
    if (month == "01")
        month = "January";
    else if (month == "02")
        month = "February";
    else if (month == "03")
        month = "March";
    else if (month == "04")
        month = "April";
    else if (month == "05")
        month = "May";
    else if (month == "06")
        month = "June";
    else if (month == "07")
        month = "July";
    else if (month == "08")
        month = "August";
    else if (month == "09")
        month = "September";
    else if (month == "10")
        month = "October";
    else if (month == "11")
        month = "November";
    else if (month == "12")
        month = "December";
    else
        qDebug() << "No Valid Month" << endl;

    return month;

    // This function takes a 3 letter month value that is taken from "QDate::currentDate()" and
    // converts it into a two number string that is formatted to work with our database.
    // Ideally this would in a switch statement however that only functions properly with integer values or enum values.
}

void MainWindow::on_createMember_clicked()
{
    QString name;
    QString type;
    QString memberId;
    QString year;
    QString month;
    QString day;
    QString spent = "0.0";
    QString rebate = "0.0";

    QDate date = QDate::currentDate();
    QString stringDate = date.toString();
    QString monthDate = stringDate.mid( 4, 3);
    QString dayDate = stringDate.mid(8,2);
    QString dayYear = stringDate.mid(11,5);

    int intYear = dayYear.toInt() + 1;
    year = QString::number(intYear);

    month = getMonth(monthDate);
    if (dayDate.toInt() < 10)
    {
        dayDate = "0" + dayDate;
    }
    /*
     * QDate date takes the current date in the format as "11 - 06 - 2019" in a QDate value.
     * It is then converted into a string wich converts it into the format "Wed Nov 6 2019
     * The new string is broken down into substring which individually hold the month, day, and year.
     *
     * The Year in converted to an integer so it can be incremented by one, this is because the memberships expires in one year.
     * The new value is converted back to a string.
     *
     * The month substring is passed into a function which converts it from a 3 letter string to a two number format.
     * Example:
     * Jan will be converted into 01
     *
     * Day is sent into an if statement which converts it to an int, a,d if that value is less than 10 a 0 is placed infront of its number
     * Example:
     * 6 will be converted into 06
     */

    name = ui->nameEdit->text();
    type = ui->typeBox->currentText();
    memberId =generateMemberID();

    /*
     * memberId is created before the execution which consistently generates a new 5 digit number to be assigned as a unique memberId
     */

    QSqlQuery qry;
    qry.prepare("INSERT INTO member (id, name, type, year, month, day, spent, rebate )"
                " values( '"+memberId+"','"+name+"','"+type+"','"+year+"','"+month+"','"+dayDate+"', '"+spent+"','"+rebate+"')");
    if(!qry.exec())
    {
        qDebug() << memberId << " " << name << " " << type << " " << year << " " << month << " " << dayDate <<
                    " " << spent << " " << rebate << endl;
        qDebug() <<"error Loading values to db" << endl;
    }

    /*
     * All values are passed into a query which updates the member table of the database that with the new values and sets their total spent
     * and their rebate to 0.0. These 0's are placeholder values which will be changed once the admin makes a purchase on the newly made account.
     */


    QMessageBox::information(this,"Account Creation", "Member Sucsessfully Created with a Unique Member ID of " + memberId);
    ui->nameEdit->setText("");

    ui->memberName->setText(name);
    ui->memberType->setText(type);
    ui->memberId->setText(memberId);


    ui->adminTable->setModel(m_controller->getCommoditiesQueryModel());
    ui->adminTable->resizeColumnsToContents();
}

void MainWindow::on_adminTable_clicked(const QModelIndex &index)
{

    QString name;

    if(index.isValid())
    {
        QSqlQuery qry;
        name = index.data().toString();
        QString price;
        QString subTotal;
        ui->nameShow->setText(name);

        qry.prepare("Select * from commodity where item = '"+name+"'");
        if (qry.exec())
        {
            while(qry.next())
            {
                price = qry.value(1).toString();
                subTotal = (QString::number(price.toDouble() * 1.0775));
                ui->priceShow->setText(QString::number(price.toDouble(),'g',6));
                ui->subShow->setText(QString::number(subTotal.toDouble(),'g',3));
            }
        }

    }
}

void MainWindow::savetoNewFile(QString date, QString ID, QString item, QString price, QString QTY)
{
    QString year;
    QString month;
    QString day;

    QDate dates = QDate::currentDate();
    QString stringDate = dates.toString();
    QString monthDate = stringDate.mid( 4, 3);
    QString dayDate = stringDate.mid(8,2);
    QString dayYear = stringDate.mid(11,5);

    int intYear = dayYear.toInt();
    year = QString::number(intYear);

    month = getMonth(monthDate);
    if (dayDate.toInt() < 10)
    {
        dayDate = "0" + dayDate;
    }

    QString finalDate = month + dayDate + year;

    QFile file(QDir::homePath() + "/" + finalDate + ".txt");

    if(!file.exists())
    {
        qDebug() << file.fileName() << " does not exist";
    }

    if(file.open(QIODevice::Append| QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream txtStream (&file);

        qDebug() << "~~~~~ Writing To File ~~~~~";

        txtStream << date << endl;
        txtStream << ID << endl;
        txtStream << item << endl;
        txtStream << price << endl;
        txtStream << QTY << endl;
        file.close();

        /* This is a basic QFile function that creates a unique daily sales report file each day,
         * this is accomplished by taking the current date and breaking it into the format of
         * mmddyyy (m = month, d = day, y = year) and saves it. However if that file already
         * exists the file is just updated.
         */
    }
    else
    {
        qDebug() << "Could not open the fie";
        return;
    }
}

void MainWindow::on_purchaseButton_clicked()
{
    QString price;
    QString rebate;
    QString name;
    QString item;
    QString memberType;
    QString memberID;
    QSqlQuery qry;

    QString year;
    QString month;
    QString day;

    price = ui->priceShow->text();
    name = ui->nameEdit->text();
    memberType = ui->memberType->text();
    memberID = ui->memberId->text();
    item = ui->nameShow->text();

    rebate = QString::number(price.toDouble() * 0.02);

    qry.exec("update member set spent = '"+price+"', rebate = '"+rebate+"' where name = '"+name+"'");

    QMessageBox::information(this,"Purchase Complete", "Purchase Has Sucsessfully Been Completed by Member " + name);

    /* QStrings are taking the values of displayed on the screen. Theses values are things such as item prie, member name,
     * member ID, member Type, and the item name. The purchase is then stored in the database. The inital database update occurs
     * when the member is created through the on_createMember_clicked() function, the second occurs during this function
     * which updates that member with their corresponding purchase, including calculating their rebate.
     */

    QDate date = QDate::currentDate();
    QString stringDate = date.toString();
    QString monthDate = stringDate.mid( 4, 3);
    QString dayDate = stringDate.mid(8,2);
    QString dayYear = stringDate.mid(11,5);

    int intYear = dayYear.toInt();
    year = QString::number(intYear);

    month = getMonth(monthDate);
    if (dayDate.toInt() < 10)
    {
        dayDate = "0" + dayDate;
    }

    QString finalDate = month + '/' + dayDate + '/' + year;

    savetoNewFile(finalDate, memberID,item, price, "1");

    /* Borrowing the date gathering code from on_createMember_clicked() function (this should be converted to its own function before
     * the final product is submitted) the date is taken and formatted properly to be used to store in our new purchase date tool.
     * A new file is created and the values are stored in the text document.*/

    QString fullMonth = getFullMonth(month);
    QString fullDate = fullMonth + ", " + dayDate +", "+ year;
    qDebug() << fullDate;


    changeToAdmin();

}

void MainWindow::on_logoutAdmin_clicked()
{
    changeToMain();
    this->ui->username->setText("");
    this->ui->password->setText("");
}

void MainWindow::on_logOut_clicked()
{
    changeToMain();
    this->ui->username->setText("");
    this->ui->password->setText("");
}
