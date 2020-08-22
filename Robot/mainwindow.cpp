#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setGeometry(400,200,900,506);
    w2.setParent(this,Qt::Window);
    w3.setParent(this,Qt::Window);
    w4.setParent(this,Qt::Window);
    w5.setParent(this,Qt::Window);

    //１．设置数据库类型
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    //２．设置数据库名字
    db.setDatabaseName("my_sql.db");

    //３．打开数据库
    bool ok = db.open();

    if(ok)
    {
        qDebug() << "打开成功";
    }
    else
    {
        qDebug() << "打开失败";
    }

    //操作数据库
    QStringList table = db.tables();

    qDebug() << table.length();

    //创建表格
    QSqlQuery sql_cmd;
    //如果创建表格时，没有指定类型，则默认为字符串类型，如果是字符串类型，则需要加上单引号
    ok = sql_cmd.exec("create table robot(id,pwd,q1,q2,q3);");
    if(ok)
    {
        qDebug() << "创建表格成功";
    }
    else
    {
        qDebug() << "创建失败";
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()    //注册
{
    bool ok;
    //创建表格
    QSqlQuery sql_cmd;

    //查询数据
    ok = sql_cmd.exec("select id from robot;");

    while(sql_cmd.next())//表格中的数据有多少项就会循环多少遍
    {
        QString id = sql_cmd.value(0).toString();

        if( id == ui->lineEdit->text())
        {
            QMessageBox::warning(this, tr("注册"),tr("该用户名已被使用！"),QMessageBox::Ok);
            return;
        }
    }

    if( ui->lineEdit->text().length() < 5 || ui->lineEdit->text().length() > 12 )
    {
        QMessageBox::warning(this, tr("注册"),tr("用户名格式错误，请输入5-12位！"),QMessageBox::Ok);
        return;
    }
    else if( ui->lineEdit_2->text().length() < 5 || ui->lineEdit_2->text().length() > 12 )
    {
        QMessageBox::warning(this, tr("注册"),tr("密码格式错误，请输入5-12位！"),QMessageBox::Ok);
        return;
    }
    QString insert = QString("insert into robot(id ,pwd ) values('%1','%2');")
            .arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text());
    qDebug() << insert;
    ok = sql_cmd.exec(insert);
    if(ok)
    {
        QMessageBox::warning(this, tr("注册"),tr("注册成功,请添加密保！"),QMessageBox::Ok);
        w2.show();
        this->hide();
    }
    else
    {
        QMessageBox::warning(this, tr("注册"),tr("注册失败！"),QMessageBox::Ok);
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
    }
}

void MainWindow::on_pushButton_2_clicked()  //登录
{
    QSqlQuery sql_cmd;

    if( ui->lineEdit->text().length() < 5 || ui->lineEdit->text().length() > 12 )
    {
        QMessageBox::warning(this, tr("登录"),tr("用户名格式错误，请输入5-12位！"),QMessageBox::Ok);
        return;
    }
    else if( ui->lineEdit_2->text().length() < 5 || ui->lineEdit_2->text().length() > 12 )
    {
        QMessageBox::warning(this, tr("登录"),tr("密码格式错误，请输入5-12位！"),QMessageBox::Ok);
        return;
    }

    sql_cmd.exec("select id from robot;");

    while(sql_cmd.next())//表格中的数据有多少项就会循环多少遍
    {
        QString id = sql_cmd.value(0).toString();
        if( id == ui->lineEdit->text())
        {
            QString pwd = QString("select pwd from robot where id='%1';").arg(id);
            sql_cmd.exec(pwd);
            while(sql_cmd.next())
            {
                if( sql_cmd.value(0).toString() == ui->lineEdit_2->text())
                {
                    QMessageBox::warning(this, tr("登录"),tr("登录成功！"),QMessageBox::Ok);
                    ui->lineEdit->clear();
                    ui->lineEdit_2->clear();
                    w5.show();
                    this->hide();
                    return;
                }
                else
                {
                    QMessageBox::warning(this, tr("登录"),tr("密码错误！"),QMessageBox::Ok);                    
                    return;
                }
            }

        }
    }
    QMessageBox::warning(this, tr("登录"),tr("该用户名未注册！"),QMessageBox::Ok);

}

void MainWindow::on_pushButton_4_clicked()  //忘记密码
{
    w3.show();
    this->hide();
}

void MainWindow::on_pushButton_5_clicked()  //管理员登录
{
    if(ui->lineEdit->text() == "huyuntao" && ui->lineEdit_2->text() == "123456")
    {
        QMessageBox::warning(this, tr("管理员登录"),tr("管理员登陆成功！"),QMessageBox::Ok);
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        w4.show();
        this->hide();
        return;
    }
    else
    {
        QMessageBox::warning(this, tr("管理员登录"),tr("用户名或密码错误！"),QMessageBox::Ok);
        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        return;
    }

}
