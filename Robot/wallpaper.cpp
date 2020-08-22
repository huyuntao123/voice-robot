#include "wallpaper.h"
#include "ui_wallpaper.h"

QString filePath_4;   //视频路径
QList<QFileInfo> *fileInfo_4;
int i_wall =2;

wallpaper::wallpaper(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::wallpaper)
{
    ui->setupUi(this);
    this->setGeometry(400,200,900,506);

    QPixmap pic;
    filePath_4 = "/home/gec/Pictures";
    QDir *dir=new QDir(filePath_4);
    QStringList filter;
    fileInfo_4=new QList<QFileInfo>(dir->entryInfoList(filter));

    pic.load(fileInfo_4->at(i_wall).filePath());

    pic = pic.scaled(ui->label->width(), ui->label->height());

    ui->label->setPixmap(pic);
}

wallpaper::~wallpaper()
{
    delete ui;
}

void wallpaper::on_pushButton_clicked()
{
    this->parentWidget()->show();
    this->hide();
}

void wallpaper::on_pushButton_4_clicked()
{
    if(i_wall == 2)
    {
        i_wall = fileInfo_4->size()-1;
    }
    else
    {
        i_wall--;
    }

    QPixmap pic;
    pic.load(fileInfo_4->at(i_wall).filePath());
    pic = pic.scaled(ui->label->width(), ui->label->height());

    ui->label->setPixmap(pic);
}

void wallpaper::on_pushButton_2_clicked()
{
    QString sheet = QString("QMainWindow{border-image: url(%1);}").arg(fileInfo_4->at(i_wall).filePath());
    parentWidget()->setStyleSheet(sheet);
}

void wallpaper::on_pushButton_3_clicked()
{
    if(i_wall == fileInfo_4->size()-1)
    {
        i_wall = 2;
    }
    else
    {
        i_wall++;
    }

    QPixmap pic;
    pic.load(fileInfo_4->at(i_wall).filePath());
    pic = pic.scaled(ui->label->width(), ui->label->height());
    ui->label->setPixmap(pic);
}
