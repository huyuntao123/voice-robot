#include "menu.h"
#include "ui_menu.h"

QTimer timer_menu;

menu::menu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::menu)
{
    ui->setupUi(this);
    this->setGeometry(400,200,900,506);

    w6.setParent(this,Qt::Window);
    w7.setParent(this,Qt::Window);
    w8.setParent(this,Qt::Window);
    w9.setParent(this,Qt::Window);
    w10.setParent(this,Qt::Window);

    connect(&timer_menu, SIGNAL(timeout()), this, SLOT(menu_voice()));


    ui->textBrowser->hide();
    ui->listWidget->hide();
    ui->listWidget->addItem("退出登录");
    ui->listWidget->addItem("更换壁纸");
}

menu::~menu()
{
    delete ui;
}

void menu::on_pushButton_clicked()  //菜单按钮
{
    ui->listWidget->clearSelection();

    if(ui->listWidget->isHidden())
    {
        ui->listWidget->show();
    }
    else
    {
        ui->listWidget->hide();
    }
}

void menu::on_listWidget_activated()    //菜单点击事件
{
    int item;
    item = ui->listWidget->currentRow();
    if( item == 0)
    {
        this->parentWidget()->show();
        this->hide();
    }
    if( item == 1)
    {
        w10.show();
        this->hide();
    }

}

void menu::on_pushButton_6_clicked()
{
    w6.show();
    this->hide();
}

void menu::on_pushButton_4_clicked()
{
    w7.show();
    this->hide();
}

void menu::on_pushButton_2_clicked()
{
    w8.show();
    this->hide();
}

void menu::on_pushButton_3_clicked()
{
    w9.show();
    this->hide();
}

void menu::on_pushButton_5_clicked()
{
    ui->pushButton_6->setDisabled(true);
    ui->pushButton_4->setDisabled(true);
    ui->pushButton_2->setDisabled(true);
    ui->pushButton_3->setDisabled(true);
    ui->pushButton->setDisabled(true);
    ui->textBrowser->show();
    timer_menu.start(7000);
    QTimer *pTimer = new QTimer(this);
    connect(pTimer,SIGNAL(timeout()),this,SLOT(menu_voice()));
    pTimer->setSingleShot(true);
    pTimer->start(10);
}

void menu::menu_voice()
{
    system("echo 'star' > /home/gec/pipeg");
    //打开管道文件
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
        ui->textBrowser->setText("对不起，没听清。");
        return;
    }
    else
    {
        int ret = str.indexOf("\"w\":");
        str.remove(0,ret).remove("\"w\":\"");
        ret = str.indexOf("\"");
        str.remove(ret,str.length()-1);
    }
    if(str == "打开菜单")
    {
        ui->listWidget->show();
        timer_menu.stop();
        ui->textBrowser->setText(0);
        ui->textBrowser->hide();
    }
    else if(str == "打开图库")
    {
        w6.show();
        this->hide();
        timer_menu.stop();
        ui->textBrowser->setText(0);
        ui->textBrowser->hide();
    }
    else if(str == "播放音乐")
    {
        w8.show();
        this->hide();
        timer_menu.stop();
        ui->textBrowser->setText(0);
        ui->textBrowser->hide();
    }
    else if(str == "播放视频")
    {
        w9.show();
        this->hide();
        timer_menu.stop();
        ui->textBrowser->setText(0);
        ui->textBrowser->hide();
    }
    else if(str == "退出语音")
    {
        timer_menu.stop();
        ui->textBrowser->setText(0);
        ui->textBrowser->hide();
    }
    else if(str == "和机器人聊天")
    {
        w7.show();
        this->hide();
        timer_menu.stop();
        ui->textBrowser->setText(0);
        ui->textBrowser->hide();
    }
    else
    {
        ui->textBrowser->setText("对不起，暂不支持这项功能。");
    }
}
