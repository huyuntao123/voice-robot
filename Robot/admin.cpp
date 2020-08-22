#include "admin.h"
#include "ui_admin.h"

int sel_time = 0;
int ad_s = 0;
QTimer timer_sel;
admin::admin(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::admin)
{
    ui->setupUi(this);

    this->setGeometry(400,200,900,506);

     connect(&timer_sel, SIGNAL(timeout()), this, SLOT(ad_sel()));
}

admin::~admin()
{
    delete ui;
}

void admin::on_pushButton_5_clicked()   //返回
{
    this->parentWidget()->show();
    this->hide();
}

void admin::on_pushButton_clicked() //添加用户
{
    bool ok;
    QSqlQuery sql_cmd;

    QString insert = QString("insert into robot values('%1','%2','%3','%4','%5');")
            .arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text())
            .arg(ui->lineEdit_3->text()).arg(ui->lineEdit_4->text())
            .arg(ui->lineEdit_5->text());
    qDebug() << insert;
    ok = sql_cmd.exec(insert);
    if(ok)
    {
        ui->label_6->setText("添加成功！");
    }
    else
    {
        ui->label_6->setText("添加失败！");
    }
}

void admin::on_pushButton_2_clicked()   //删除用户
{
    bool ok;
    QSqlQuery sql_cmd;

    QString del = QString("delete from robot where id='%1';").arg(ui->lineEdit->text());

    qDebug() << del;
    ok = sql_cmd.exec(del);
    if(ok)
    {
    ui->label_6->setText("删除成功！");
    }
    else
    {
    ui->label_6->setText("删除失败！");
    }
}

void admin::on_pushButton_3_clicked()   //修改用户
{
    bool ok;
    QSqlQuery sql_cmd;
    QString upd = QString("update robot set id='%1',pwd='%2',q1='%3',q2='%4',q3='%5' where id='%1';")
            .arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text())
            .arg(ui->lineEdit_3->text()).arg(ui->lineEdit_4->text())
            .arg(ui->lineEdit_5->text());
    qDebug() << upd;
    ok = sql_cmd.exec(upd);
    if(ok)
    {
        ui->label_6->setText("修改成功");
    }
    else
    {
        ui->label_6->setText("修改失败");
    }
}

void admin::on_pushButton_4_clicked()   //查找用户
{
    timer_sel.start(10);

    ad_s++;

}

void admin::ad_sel()
{
    sel_time++;
    ui->textBrowser->clear();
    bool ok;
    QSqlQuery sql_cmd;
    if(sel_time == 40)
    {
        timer_sel.stop();
        qDebug() << "双击失败";
        QString sel = QString("select * from robot where id='%1' or pwd='%2' or q1='%3' or q2='%4' or q3='%5';")
                .arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text())
                .arg(ui->lineEdit_3->text()).arg(ui->lineEdit_4->text())
                .arg(ui->lineEdit_5->text());
        qDebug() << sel;
        ok = sql_cmd.exec(sel);
        if(ok)
        {
            ui->label_6->setText("查询成功");
        }
        else
        {
            ui->label_6->setText("查询失败");
        }
        while(sql_cmd.next())//表格中的数据有多少项就会循环多少遍
        {
            QString str =   sql_cmd.value(0).toString() + " | " +
                            sql_cmd.value(1).toString() + " | " +
                            sql_cmd.value(2).toString() + " | " +
                            sql_cmd.value(3).toString() + " | " +
                            sql_cmd.value(4).toString();
            ui->textBrowser->append(str);

        }
        sel_time = 0;
        ad_s = 0;
    }
    if(ad_s == 2)
    {
        timer_sel.stop();
        ok = sql_cmd.exec("select * from robot; ");
        if(ok)
        {
            ui->label_6->setText("查询成功");
        }
        else
        {
            ui->label_6->setText("查询失败");
        }
        while(sql_cmd.next())//表格中的数据有多少项就会循环多少遍
        {
            QString str =   sql_cmd.value(0).toString() + " | " +
                            sql_cmd.value(1).toString() + " | " +
                            sql_cmd.value(2).toString() + " | " +
                            sql_cmd.value(3).toString() + " | " +
                            sql_cmd.value(4).toString();

            ui->textBrowser->append(str);
        }
        sel_time = 0;
        ad_s = 0;
    }

}
