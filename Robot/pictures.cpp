#include "pictures.h"
#include "ui_pictures.h"

QString picPath;   //图片路径
QList<QFileInfo> *picInfo;
int i_pic =2;
int push_time = 0;
int pic_time = 0;
QTimer timer_pic;
QTimer timer_i;
QTimer timer_sp;

pictures::pictures(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::pictures)
{
    ui->setupUi(this);
    this->setGeometry(400,200,900,506);

    connect(&timer_pic, SIGNAL(timeout()), this, SLOT(on_pushButton_3_clicked()));
    connect(&timer_i, SIGNAL(timeout()), this, SLOT(push()));
    connect(&timer_sp, SIGNAL(timeout()), this, SLOT(ro_speech()));

    QPixmap pic;
    picPath = "/home/gec/Pictures";
    QDir *dir=new QDir(picPath);
    QStringList filter;
    picInfo=new QList<QFileInfo>(dir->entryInfoList(filter));
    pic.load(picInfo->at(i_pic).filePath());
    pic = pic.scaled(ui->label->width(), ui->label->height());
    ui->label->setPixmap(pic);

    ui->textBrowser->hide();
}

pictures::~pictures()
{
    delete ui;
}

void pictures::on_pushButton_clicked()  //返回
{
    this->parentWidget()->show();
    this->hide();
}

void pictures::on_pushButton_2_clicked()    //上一张
{
    if(i_pic == 2)
    {
        i_pic = picInfo->size()-1;
    }
    else
    {
        i_pic--;
    }
    QPixmap pic;
    pic.load(picInfo->at(i_pic).filePath());
    pic = pic.scaled(ui->label->width(), ui->label->height());
    ui->label->setPixmap(pic);
}

void pictures::on_pushButton_3_clicked()    //下一张
{
    if(i_pic == picInfo->size()-1)
    {
        i_pic = 2;
    }
    else
    {
        i_pic++;
    }

    QPixmap pic;
    pic.load(picInfo->at(i_pic).filePath());
    pic = pic.scaled(ui->label->width(), ui->label->height());
    ui->label->setPixmap(pic);
}

void pictures::on_pushButton_4_clicked()    //幻灯片
{
    pic_time++;
    timer_i.start(10);
}

void pictures::push()   //单双击响应
{
    push_time++;
    if(push_time == 40)
    {
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_3->setDisabled(true);
        timer_pic.start(1000);
        timer_i.stop();
        push_time = 0;
        pic_time = 0;
    }
    if(pic_time == 2)
    {
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        timer_pic.stop();
        timer_i.stop();
        push_time = 0;
        pic_time = 0;
    }
}

void pictures::ro_speech()  //语音模式
{
    system("echo 'star' > /home/gec/pipeg");
    int pipe_fd = open("/home/gec/pipe",O_RDWR);
    if(pipe_fd == -1)
    {
        perror("open pipe error");
        return;
    }
    char buf[2048] = {};
    read(pipe_fd,buf,2048);

    QString str =QString(buf);
    QString sc =QString(buf);

    int ret_2 = sc.lastIndexOf("\"sc\":");
    sc.remove(0,ret_2).remove("\"sc\":");
    ret_2 = sc.indexOf("}");
    sc.remove(ret_2,sc.length()-1);
    int n = sc.toInt();
    qDebug() << "sc=" << n;
    ::close(pipe_fd);
    if(n<=10)
    {
        ui->textBrowser->setText("对不起，识别失败...");
        return;
    }
    else
    {
        int ret = str.indexOf("\"w\":");
        str.remove(0,ret).remove("\"w\":\"");
        ret = str.indexOf("\"");
        str.remove(ret,str.length()-1);
    }
    if(str == "上一张")
    {
        QTimer *pTimer = new QTimer(this);
        connect(pTimer,SIGNAL(timeout()),this,SLOT(on_pushButton_2_clicked()));
        pTimer->setSingleShot(true);
        pTimer->start(10);
    }
    else if(str == "下一张")
    {
        QTimer *pTimer = new QTimer(this);
        connect(pTimer,SIGNAL(timeout()),this,SLOT(on_pushButton_3_clicked()));
        pTimer->setSingleShot(true);
        pTimer->start(10);
    }
    else if(str == "播放幻灯片")
    {
        timer_pic.start(1000);
    }
    else if(str == "关闭幻灯片")
    {
        timer_pic.stop();
    }
    else if(str == "返回主界面")
    {
        timer_pic.stop();
        timer_sp.stop();
        this->parentWidget()->show();
        this->hide();
    }
    else if(str == "退出语音")
    {
        ui->textBrowser->hide();
        ui->textBrowser->clear();
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        timer_pic.stop();
        timer_sp.stop();
    }
    else
    {
        ui->textBrowser->setText("对不起，识别失败...");
    }
}

void pictures::on_pushButton_5_clicked()    //语音
{
    ui->textBrowser->show();
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_3->setDisabled(true);
    ui->pushButton_4->setDisabled(true);
    timer_sp.start(7000);
    timer_pic.stop();
    QTimer *pTimer = new QTimer(this);
    connect(pTimer,SIGNAL(timeout()),this,SLOT(ro_speech()));
    pTimer->setSingleShot(true);
    pTimer->start(10);

}
