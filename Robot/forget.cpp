#include "forget.h"
#include "ui_forget.h"

QString id_1;

forget::forget(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::forget)
{
    ui->setupUi(this);

    ui->lineEdit_4->hide();
    ui->pushButton_2->hide();

    this->setGeometry(400,200,900,506);
}

forget::~forget()
{
    delete ui;
}

void forget::on_pushButton_clicked()    //密保验证
{
    bool ok;
    QSqlQuery sql_cmd;

    QString q1,q2,q3;
    ok = sql_cmd.exec("select id from robot;");
    if(ok)
    {
        ui->label->setText("查询成功");
    }
    else
    {
        ui->label->setText("查询失败");
    }


    while(sql_cmd.next())//表格中的数据有多少项就会循环多少遍
    {
        QString id = sql_cmd.value(0).toString();
        if( id == ui->lineEdit_5->text())
        {
            qDebug()<< id;
            QString forget = QString("select q1,q2,q3 from robot where id='%1';").arg(id);
            qDebug() << forget;
            ok=sql_cmd.exec(forget);
            while(sql_cmd.next())//表格中的数据有多少项就会循环多少遍
            {
            q1 = sql_cmd.value(0).toString();
            q2 = sql_cmd.value(1).toString();
            q3 = sql_cmd.value(2).toString();
            }
            if(q1 == ui->lineEdit->text() || q2 == ui->lineEdit_2->text() || q3 == ui->lineEdit_3->text())
            {
                QMessageBox::warning(this, tr("验证密保"),tr("验证成功,请输入新密码！"),QMessageBox::Ok);
                id_1 = id;
                ui->lineEdit_2->clear();
                ui->lineEdit_3->clear();
                ui->lineEdit_4->show();
                ui->pushButton_2->show();
                return;
            }
            else
            {
                QMessageBox::warning(this, tr("验证密保"),tr("密保错误，请重新输入或联系管理员！"),QMessageBox::Ok);
                ui->lineEdit->clear();
                ui->lineEdit_2->clear();
                ui->lineEdit_3->clear();
                return;
            }

        }

    }
}


void forget::on_pushButton_3_clicked()  //返回
{
    this->parentWidget()->show();
    this->hide();
}

void forget::on_pushButton_2_clicked()
{
    bool ok;
    QSqlQuery sql_cmd;
    if(ui->lineEdit_4->text().length() < 5 || ui->lineEdit_4->text().length() > 12)
    {
        QMessageBox::warning(this, tr("新密码"),tr("密码格式错误，请输入5-12位密码！"),QMessageBox::Ok);
        return;
    }
    QString new_pwd = QString("update robot set pwd='%1' where id='%2';")
            .arg(ui->lineEdit_4->text()).arg(id_1);

    qDebug() << new_pwd;
    ok = sql_cmd.exec(new_pwd);
    if(ok)
    {
        QMessageBox::warning(this, tr("新密码"),tr("修改成功，返回主界面！"),QMessageBox::Ok);
        ui->lineEdit_4->clear();
        this->parentWidget()->show();
        this->hide();
        return;
    }
    else
    {
        QMessageBox::warning(this, tr("新密码"),tr("修改失败，请确认密码格式是否正确！"),QMessageBox::Ok);
        return;
    }
}
