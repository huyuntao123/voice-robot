#include "secret.h"
#include "ui_secret.h"

secret::secret(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::secret)
{
    ui->setupUi(this);
    this->setGeometry(400,200,900,506);


}

secret::~secret()
{
    delete ui;
}

void secret::on_pushButton_clicked()    //返回
{
    this->parentWidget()->show();
    this->hide();
}

void secret::on_pushButton_2_clicked()
{
    bool ok;
    QSqlQuery sql_cmd;
    if(ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty()
                                      || ui->lineEdit_2->text().isEmpty())
    {
        QMessageBox::warning(this, tr("密保"),tr("请设置完整！"),QMessageBox::Ok);
        return;
    }
    else
    {
        QString secret = QString("update robot set q1='%1',q2='%2',q3='%3' where id='%4';")
                .arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text())
                .arg(ui->lineEdit_3->text()).arg(ui->lineEdit_4->text());
        qDebug() << secret;
        ok = sql_cmd.exec(secret);
        if(ok)
        {
            QMessageBox::warning(this, tr("密保"),tr("设置成功！"),QMessageBox::Ok);
            this->parentWidget()->show();
            this->hide();
        }
        else
        {
            QMessageBox::warning(this, tr("密保"),tr("设置失败！"),QMessageBox::Ok);
            return;
        }
    }

}

