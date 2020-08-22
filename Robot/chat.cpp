#include "chat.h"
#include "ui_chat.h"

QTimer timer_chat;
QTimer timer_clock;
QTimer timer_c;
QTimer timer_cm;

QString c_time;
QString clo_Path;   //铃声路径
QList<QFileInfo> *clo_Info;

int i_cm = 0;
int i_clo;
int push_clock = 0;
int clock_time = 0;
chat::chat(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chat)
{
    ui->setupUi(this);

    this->setGeometry(400,200,900,506);

    connect(&timer_chat, SIGNAL(timeout()), this, SLOT(ro_chat()));
    connect(&timer_clock, SIGNAL(timeout()), this, SLOT(ro_clock()));
    connect(&timer_c, SIGNAL(timeout()), this, SLOT(clo_time()));
    connect(&timer_cm, SIGNAL(timeout()), this, SLOT(cm_time()));

    ui->label_2->hide();
    clo_Path = "/home/gec/Music";
    QDir *dir=new QDir(clo_Path);
    QStringList filter;
    clo_Info=new QList<QFileInfo>(dir->entryInfoList(filter));

    for(int n = 2; n < clo_Info->size(); n++)
    {
        ui->listWidget->addItem( clo_Info->at(n).filePath()
                                 .remove("/home/gec/Music/")
                                 .remove(".mp3"));
    }
    ui->listWidget->hide();

}

chat::~chat()
{
    delete ui;
}

void chat::on_pushButton_clicked()
{
    this->parentWidget()->show();
    this->hide();
}

void chat::on_pushButton_2_clicked()    //语音按钮
{
    ui->label_2->show();
    QPixmap pic;
    pic.load("/home/gec/pic2/5.gif");
    pic = pic.scaled(ui->label_2->width(), ui->label_2->height());
    ui->label_2->setPixmap(pic);
    timer_chat.start(7000);
    QTimer *pTimer = new QTimer(this);
    connect(pTimer,SIGNAL(timeout()),this,SLOT(ro_chat()));
    pTimer->setSingleShot(true);
    pTimer->start(10);
}

void chat::ro_chat()
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
    if (str == "设置闹钟")
    {
        QTimer *pTimer = new QTimer(this);
        connect(pTimer,SIGNAL(timeout()),this,SLOT(on_pushButton_4_clicked()));
        pTimer->setSingleShot(true);
        pTimer->start(10);
    }
    else if(str == "退出语音")
    {
        timer_chat.stop();
        ui->label_2->clear();
        ui->label_2->hide();
    }
    else
    {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
        QString http_str = QString("http://api.qingyunke.com/api.php?key=free&appid=0&msg=%1").arg(str);
        manager->get(QNetworkRequest(QUrl(http_str)));
    }

}

void chat::replyFinished(QNetworkReply *arg)
{
    //把反馈的内容存放到str_data
    QString str_data = arg->readAll();

    str_data.remove("{\"result\":0,\"content\":\"").remove("\"}").remove("{br}").remove("[").remove("]").remove(" ");

    qDebug() << str_data;

    //利用http请求，把反馈的内容打印到textbrowser中
    ui->textBrowser->setText(str_data);

    QString str_data_2 = QString("/home/gec/xunfei/bin/tts_offline_sample %1").arg(str_data);
    system(str_data_2.toStdString().c_str());
    system("aplay  /home/gec/xunfei/bin/tts_sample.wav");
}

void chat::on_pushButton_3_clicked()    //发送按钮
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),this, SLOT(replyFinished(QNetworkReply*)));
    QString http_str = QString("http://api.qingyunke.com/api.php?key=free&appid=0&msg=%1").arg(ui->lineEdit->text());
    manager->get(QNetworkRequest(QUrl(http_str)));
}

void chat::on_pushButton_4_clicked()    //设置闹钟
{
    push_clock++;
    timer_c.start(10);

}

void chat::ro_clock()   //闹钟计时
{
    QString str_time = QTime::currentTime().toString("hh:mm:ss");
    QString c_t_time = QString("%1/%2").arg(str_time).arg(c_time);
    ui->label->setText(c_t_time);
    if(c_time == str_time)
    {
        timer_clock.stop();
        timer_cm.start(1000);
        QMessageBox::warning(this, tr("闹钟设置"),tr("时间到！"),QMessageBox::Ok);
        QString clo_mus= QString("madplay %1 &").arg(clo_Info->at(i_clo).filePath());
        system(clo_mus.toStdString().c_str());
        ui->label->clear();
        ui->pushButton_4->setText("设置闹钟");
    }
}

void chat::clo_time()
{
    clock_time++;
    if(clock_time == 40)
    {
        c_time = ui->lineEdit->text();
        QStringList c_time_2 = c_time.split(":");
        if(c_time.length() != 8 || c_time_2.length() != 3)
        {
            QMessageBox::warning(this, tr("闹钟设置"),tr("闹钟格式错误，请重新输入。"),QMessageBox::Ok);
            ui->lineEdit->clear();
            return;
        }
        ui->pushButton_4->setText("关闭闹钟");
        timer_clock.start(1000);
        timer_c.stop();
        clock_time = 0;
        push_clock = 0;
    }
    if(push_clock == 2)
    {
        ui->pushButton_4->setText("设置闹钟");
        ui->label->clear();
        QMessageBox::warning(this, tr("闹钟设置"),tr("闹钟已提前关闭！"),QMessageBox::Ok);
        timer_clock.stop();
        timer_c.stop();
        clock_time = 0;
        push_clock = 0;
    }
}

void chat::cm_time()
{
    i_cm++;
    if(i_cm == 10)
    {
        timer_cm.stop();
        system("killall -9 madplay");
    }
}

void chat::on_pushButton_5_clicked()    //设置铃声
{
    if(ui->listWidget->isHidden())
    {
       ui->listWidget->show();
    }
    else
    {
        ui->listWidget->hide();
    }

}

void chat::on_listWidget_clicked()
{
    i_clo = ui->listWidget->currentRow() + 2;
}
