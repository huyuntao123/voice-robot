#include "video.h"
#include "ui_video.h"

int v_pos;
int i_vid = 2;  //路径标志
int play = 0;  //播放标志
int v_p_p = 0;  //暂停标志
int v_p_seek = 0;
int v_p_time = 0;
int speed = 0;
int mute_vid = 0;   //静音标志

QString vidPath;   //视频路径
QList<QFileInfo> *vidInfo;
QTimer timer_slid;
QTimer timer_vid;
QTimer timer_seek;
QTimer timer_speed;
QTimer timer_v_v;
QTimer timer_mut;

QProcess *process_vid;  //外部进程

Video::Video(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Video)
{
    ui->setupUi(this);
    this->setGeometry(400,200,900,506);

    process_vid = new QProcess(this);

    connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(Speech_Recognition()));
    connect(&timer_vid, SIGNAL(timeout()), this, SLOT(v_time()));
    connect(&timer_slid, SIGNAL(timeout()), this, SLOT(update_slider()));
    connect(&timer_seek, SIGNAL(timeout()), this, SLOT(v_seek()));
    connect(&timer_speed, SIGNAL(timeout()), this, SLOT(v_speed()));
    connect(&timer_v_v, SIGNAL(timeout()), this, SLOT(Speech_Recognition()));
    connect(&timer_mut, SIGNAL(timeout()), this, SLOT(v_mute()));


    vidPath = "/home/gec/Videos";
    QDir *dir=new QDir(vidPath);
    QStringList filter;
    vidInfo=new QList<QFileInfo>(dir->entryInfoList(filter));

    //播放列表
    for(int n = 2; n < vidInfo->size() ; n++)
    {
        ui->listWidget->addItem( vidInfo->at(n).filePath()
                                 .remove("/home/gec/Videos/")
                                 .remove(".mp4"));
    }

    ui->listWidget->hide();
    ui->textBrowser->hide();
    ui->verticalSlider->setRange(0,100);  //音量范围
    ui->verticalSlider->hide();
    ui->verticalSlider->setValue(60);   //初始音量
}

Video::~Video()
{
    delete ui;
}

void Video::on_pushButton_1_clicked()   //播放
{
    if(play == 0)
    {
        ui->label->show();
        //后台播放视频
        QString v_play = QString("mplayer -slave -quiet %1 -wid %2 & ").arg(vidInfo->at(i_vid).filePath()).arg(QString::number(ui->label->winId()));
        qDebug() << v_play;
        process_vid->start(v_play);//开启进程，相同于system函数使用命令
        ui->pushButton_1->setIcon(QIcon("/home/gec/image/pause.png"));
        play = 1;
        timer_vid.start(100);
        timer_slid.start(100);
    }
    else
    {
        process_vid->write("pause\n");
        if(v_p_p == 0)
        {
            ui->pushButton_1->setIcon(QIcon("/home/gec/image/play.png"));
            v_p_p = 1;
            timer_vid.stop();
            timer_slid.stop();
        }
        else
        {
            ui->pushButton_1->setIcon(QIcon("/home/gec/image/pause.png"));
            v_p_p = 0;
            timer_vid.start(100);
            timer_slid.start(100);
        }

    }
}


void Video::on_listWidget_clicked()     //列表点击
{
    process_vid->close();
    i_vid = ui->listWidget->currentRow() + 2;//当前单击选中ListWidget控件的行号（第几行）
    QString v_play = QString("mplayer -slave -quiet %1 -wid %2 & ").arg(vidInfo->at(i_vid).filePath()).arg(QString::number(ui->label->winId()));
    qDebug() << v_play;
    process_vid->start(v_play);//开启进程，相同于system函数使用命令
    ui->label->clear();
    ui->pushButton_1->setIcon(QIcon("/home/gec/image/pause.png"));
    timer_vid.start(100);
    timer_slid.start(100);
    speed = 0;
    play = 1;
    v_p_p = 0;
}

void Video::on_pushButton_8_clicked()   //列表按钮
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

void Video::on_pushButton_11_clicked()  //上一个
{
    if(i_vid == 2)
    {
        i_vid = vidInfo->size()-1;

    }
    else
    {
        i_vid--;
    }
    process_vid->close();
    QString v_play = QString("mplayer -slave -quiet %1 -wid %2 & ").arg(vidInfo->at(i_vid).filePath()).arg(QString::number(ui->label->winId()));
    qDebug() << v_play;
    process_vid->start(v_play);//开启进程，相同于system函数使用命令
    ui->pushButton_1->setIcon(QIcon("/home/gec/image/pause.png"));
    ui->listWidget->setCurrentRow(i_vid-2);
    ui->label->clear();
    ui->label->show();
    timer_vid.start(100);
    timer_slid.start(100);
    speed = 0;
    play = 1;
    v_p_p = 0;
}

void Video::on_pushButton_12_clicked()  //下一个
{
    if(i_vid == vidInfo->size()-1)
    {
        i_vid = 2;
    }
    else
    {
        i_vid++;
    }
    process_vid->close();
    QString v_play = QString("mplayer -slave -quiet %1 -wid %2 & ").arg(vidInfo->at(i_vid).filePath()).arg(QString::number(ui->label->winId()));
    qDebug() << v_play;
    process_vid->start(v_play);//开启进程，相同于system函数使用命令
    ui->pushButton_1->setIcon(QIcon("/home/gec/image/pause.png"));
    ui->listWidget->setCurrentRow(i_vid-2);
    ui->label->clear();
    ui->label->show();
    timer_vid.start(100);
    timer_slid.start(100);
    speed = 0;
    play = 1;
    v_p_p = 0;
}

void Video::on_pushButton_14_clicked()  //停止
{
    ui->pushButton_1->setIcon(QIcon("/home/gec/image/play.png"));
    ui->listWidget->clearSelection();
    ui->horizontalSlider->setValue(0);
    ui->label->hide();
    ui->label_2->clear();

    if(play == 1)
    {
        process_vid->write("quit\n");
        speed = 0;
        play = 0;
        v_p_p = 0;
        timer_vid.stop();
        timer_slid.stop();
    }
}

void Video::on_verticalSlider_sliderMoved(int position) //音量滑动
{
    QString str = QString("volume "+QString::number(position)+" 2\n");
    process_vid->write(str.toStdString().c_str());
    ui->horizontalSlider->setValue(position);
    if(position == 0)
    {
        ui->pushButton_2->setIcon(QIcon("/home/gec/image/jingyin.png"));
    }
    else
    {
        ui->pushButton_2->setIcon(QIcon("/home/gec/image/sound.png"));
    }
}

void Video::on_horizontalSlider_sliderMoved(int position)   //进度条滑动
{
    QString str = QString("seek "+QString::number(position)+" 2\n");
    process_vid->write(str.toStdString().c_str());
    ui->horizontalSlider->setValue(position);
}

void Video::on_horizontalSlider_sliderPressed()
{
    timer_slid.stop();
}

void Video::on_horizontalSlider_sliderReleased()
{
    timer_slid.start(100);
}

void Video::update_slider()     //实时显示进度
{
    ui->horizontalSlider->setValue(v_pos);
}

void Video::v_time()    //显示总时长
{
    bool ok;
    int sec ,min;
    process_vid->write("get_time_pos\n");
    process_vid->write("get_time_length\n");
    QString data_str = process_vid->readAll();
    if(data_str.startsWith("ANS_TIME_POSITION"))
    {
        QStringList pos_length = data_str.split("\n");

        //当前时间
        QString pos = QString(pos_length[0]);
        pos.remove("ANS_TIME_POSITION=");
        QStringList pos_1 = pos.split(".");
        v_pos = QString(pos_1[0]).toInt(&ok, 10);
        sec = v_pos%60;
        min = v_pos/60;
        pos = QString("%1:%2").arg(min,2,10,QChar('0')).arg(sec,2,10,QChar('0'));

        //总时间
        QString length = pos_length[1];
        length.remove("ANS_LENGTH=");
        QStringList length_1 = length.split(".");
        int length_2 = QString(length_1[0]).toInt(&ok, 10);
        sec = length_2%60;
        min = length_2/60;
        length = QString("%1:%2").arg(min,2,10,QChar('0')).arg(sec,2,10,QChar('0'));
        QString time_p_l = pos + "/" +length;
        ui->horizontalSlider->setRange(0,length_2);
        ui->label_2->setText(time_p_l);

    }
}

void Video::v_seek()        //快进与倍速响应
{
    v_p_time++;
    if(v_p_time >= 50)
    {
        timer_seek.stop();
        qDebug() << "双击失败";
        process_vid->write("seek 10\n");
        v_p_seek = 0;
        v_p_time = 0;
    }
    if(v_p_seek == 2)
    {
        if( speed == 5)
        {
            return;
        }
        speed++;

        timer_seek.stop();
        qDebug() << "双击成功";
        QString v_speed = QString("speed_set 1.%1\n").arg(speed);
        qDebug() << v_speed;
        process_vid->write(v_speed.toStdString().c_str());
        v_p_seek = 0;
        v_p_time = 0;

    }
}

void Video::on_pushButton_15_clicked()
{
    timer_seek.start(10);

    v_p_seek++;
}

void Video::on_pushButton_clicked()
{
    timer_speed.start(10);

    v_p_seek++;
}

void Video::v_speed()
{
    v_p_time++;
    if(v_p_time >= 50)
    {
        timer_speed.stop();
        qDebug() << "双击失败";
        process_vid->write("seek -10\n");
        v_p_seek = 0;
        v_p_time = 0;
    }
    if(v_p_seek == 2)
    {
        if( speed == 0)
        {
            return;
        }

        speed--;

        timer_speed.stop();
        qDebug() << "双击成功";
        QString v_speed = QString("speed_set 1.%1\n").arg(speed);
        qDebug() << v_speed;
        process_vid->write(v_speed.toStdString().c_str());
        v_p_seek = 0;
        v_p_time = 0;
    }
}

void Video::on_pushButton_2_clicked()   //音量
{
    timer_mut.start(10);

    v_p_seek++;
}

void Video::v_mute()
{
    v_p_time++;
    if(v_p_time == 30)
    {
        timer_mut.stop();
        qDebug() << "双击失败";
        if(ui->verticalSlider->isHidden())
        {
            ui->verticalSlider->show();
        }
        else
        {
            ui->verticalSlider->hide();
        }
        v_p_seek = 0;
        v_p_time = 0;
    }
    if(v_p_seek == 2)
    {
        timer_mut.stop();
        qDebug() << "双击成功";
        process_vid->write("mute\n");
        if(mute_vid == 0)
        {
            ui->pushButton_2->setIcon(QIcon("/home/gec/image/jingyin.png"));
            ui->verticalSlider->setValue(0);
            mute_vid = 1;
        }
        else
        {
           ui->pushButton_2->setIcon(QIcon("/home/gec/image/sound.png"));
           ui->verticalSlider->setValue(60);
           mute_vid = 0;
        }
        v_p_seek = 0;
        v_p_time = 0;
    }
}

void Video::on_pushButton_3_clicked()
{
    if(ui->textBrowser->isHidden())
    {
        //禁用按钮
        ui->pushButton->setDisabled(true);
        ui->pushButton_1->setDisabled(true);
        ui->pushButton_8->setDisabled(true);
        ui->pushButton_11->setDisabled(true);
        ui->pushButton_12->setDisabled(true);
        ui->pushButton_15->setDisabled(true);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_3->setIcon(QIcon("/home/gec/image/keyboard.png"));
        ui->textBrowser->show();
        ui->verticalSlider->hide();
        connect(&timer_v_v, SIGNAL(timeout()), this, SLOT(Speech_Recognition()));
        timer_v_v.start(8000);
    }
    else
    {
        //启用按钮
        ui->pushButton->setEnabled(true);
        ui->pushButton_1->setEnabled(true);
        ui->pushButton_8->setEnabled(true);
        ui->pushButton_11->setEnabled(true);
        ui->pushButton_12->setEnabled(true);
        ui->pushButton_15->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        timer_v_v.stop();
        ui->textBrowser->setText(0);
        ui->pushButton_3->setIcon(QIcon("/home/gec/image/voice.png"));
        ui->textBrowser->hide();
    }
}

void Video::Speech_Recognition()
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
        ui->textBrowser->setText("识别失败，请重新录入语音！");
        return;
    }
    else
    {
        int ret = str.indexOf("\"w\":");
        str.remove(0,ret).remove("\"w\":\"");
        ret = str.indexOf("\"");
        str.remove(ret,str.length()-1);
    }
    if(str == "播放视频" || str == "继续播放" || str == "暂停播放")
    {
        QTimer *pTimer = new QTimer(this);
        connect(pTimer,SIGNAL(timeout()),this,SLOT(on_pushButton_1_clicked()));
        pTimer->setSingleShot(true);
        pTimer->start(10);
    }
    else if ( str == "下一个视频")
    {
        QTimer *pTimer = new QTimer(this);
        connect(pTimer,SIGNAL(timeout()),this,SLOT(on_pushButton_12_clicked()));
        pTimer->setSingleShot(true);
        pTimer->start(10);
    }
    else if ( str == "上一个视频")
    {
        QTimer *pTimer = new QTimer(this);
        connect(pTimer,SIGNAL(timeout()),this,SLOT(on_pushButton_11_clicked()));
        pTimer->setSingleShot(true);
        pTimer->start(10);
    }
    else if ( str == "停止播放")
    {
        QTimer *pTimer = new QTimer(this);
        connect(pTimer,SIGNAL(timeout()),this,SLOT(on_pushButton_14_clicked()));
        pTimer->setSingleShot(true);
        pTimer->start(10);
    }
    else if( str == "打开播放列表")
    {
        ui->listWidget->show();
    }

    else if( str == "关闭播放列表")
    {
        ui->listWidget->hide();
    }
    else if ( str == "音量加")
    {
        process_vid->write("volume 2000\n");
    }
    else if( str == "音量减")
    {
        process_vid->write("volume -2000\n");
    }
    else if( str == "快进")
    {
        process_vid->write("seek 10\n");
    }
    else if( str == "快退")
    {
        process_vid->write("seek -10\n");
    }
    else if( str == "退出语音")
    {
        ui->pushButton->setEnabled(true);
        ui->pushButton_1->setEnabled(true);
        ui->pushButton_8->setEnabled(true);
        ui->pushButton_11->setEnabled(true);
        ui->pushButton_12->setEnabled(true);
        ui->pushButton_15->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        timer_v_v.stop();
        ui->textBrowser->setText(0);
        ui->pushButton_3->setIcon(QIcon("/home/gec/image/voice.png"));
        ui->textBrowser->hide();
    }
}

void Video::on_pushButton_4_clicked()
{
    this->parentWidget()->show();
    this->hide();
}
