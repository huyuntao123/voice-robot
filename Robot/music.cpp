#include "music.h"
#include "ui_music.h"

QString filePath_1;   //音乐路径
QList<QFileInfo> *fileInfo_1;

QString filePath_2;   //图片路径
QList<QFileInfo> *fileInfo_2;

QString filePath_3;   //歌词路径
QList<QFileInfo> *fileInfo_3;

QProcess *process;  //外部进程

int i_mus = 2;      //音乐路径标志（去除前两个无用路径）
int flag = 0;   //播放标志
int p_p = 0;   //继续——暂停标志
int j = 0;      //播放模式标志
int pos_2;      //音乐实时时间
int p_seek = 0;
int p_time = 0;
int mute = 0;

QString lrc_str;  //歌词
QStringList lrc_list;

QTimer timer;
QTimer timer_slider;
QTimer timer_voice;
QTimer timer_mute;
QString music_len;
Music::Music(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Music)
{
    ui->setupUi(this);
    this->setGeometry(400,200,900,506);

    process = new QProcess(this);

    connect(ui->pushButton_5, SIGNAL(clicked(bool)), this, SLOT(music_pic_lrc()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), this, SLOT(music_pic_lrc()));
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), this, SLOT(music_pic_lrc()));
    connect(ui->pushButton_8, SIGNAL(clicked(bool)), this, SLOT(Speech_Recognition()));
    connect(ui->listWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(music_pic_lrc()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(lrc()));
    connect(&timer_slider, SIGNAL(timeout()), this, SLOT(update_slider()));
    connect(&timer_mute, SIGNAL(timeout()), this, SLOT(v_mute()));

    //获取音乐路径
    filePath_1 = "/home/gec/Music";
    QDir *dir=new QDir(filePath_1);
    QStringList filter;
    fileInfo_1=new QList<QFileInfo>(dir->entryInfoList(filter));

    //播放列表
    for(int n = 2; n < fileInfo_1->size() ; n++)
    {
        ui->listWidget->addItem( fileInfo_1->at(n).filePath()
                                 .remove("/home/gec/Music/")
                                 .remove(".mp3"));
    }


    ui->listWidget->hide();
    ui->textBrowser->hide();

    //获取专辑图片路径
    filePath_2 = "/home/gec/Music_pic";
    dir=new QDir(filePath_2);
    fileInfo_2=new QList<QFileInfo>(dir->entryInfoList(filter));
    QPixmap pic;
    pic.load(fileInfo_2->at(i_mus).filePath());
    pic = pic.scaled(ui->label->width(), ui->label->height());
    ui->label->setPixmap(pic);
    ui->verticalSlider->setRange(0,100);  //音量范围
    ui->verticalSlider->hide();
    ui->verticalSlider->setValue(60);   //初始音量
}

Music::~Music()
{
    delete ui;
}

void Music::on_pushButton_clicked()
{
    this->parentWidget()->show();
    this->hide();
}

void Music::on_pushButton_5_clicked()   //播放
{
    if(flag == 0)
    {
        QString m_play = QString("mplayer -slave -quiet %1 & ").arg(fileInfo_1->at(i_mus).filePath());
        qDebug() << m_play;
        process->start(m_play);//开启进程，相同于system函数使用命令

        ui->pushButton_5->setIcon(QIcon("/home/gec/image/pause.png"));
        ui->listWidget->setCurrentRow(i_mus-2);
        flag = 1;
        timer.start(100);
        timer_slider.start(100);
    }

    else
    {
        process->write("pause\n");
        if(p_p == 0)
        {
            ui->pushButton_5->setIcon(QIcon("/home/gec/image/play.png"));
            p_p = 1;
            timer.stop();
            timer_slider.stop();
        }
        else
        {
            ui->pushButton_5->setIcon(QIcon("/home/gec/image/pause.png"));
            p_p = 0;
            timer.start(100);
            timer_slider.start(100);
        }
    }
}

void Music::music_pic_lrc()
{
    //图片
    QPixmap pic;
    pic.load(fileInfo_2->at(i_mus).filePath());
    pic = pic.scaled(ui->label->width(), ui->label->height());
    ui->label->setPixmap(pic);

    //歌词
    filePath_3 = "/home/gec/lrc";
    QDir *dir=new QDir(filePath_3);
    QStringList filter;
    fileInfo_3=new QList<QFileInfo>(dir->entryInfoList(filter));
    int fd = open(fileInfo_3->at(i_mus).filePath().toStdString().c_str(), O_RDONLY);
    if(fd == -1)
    {
        perror("open lrc error");
        exit(0);
    }
    char buf[10240] = {};
    read(fd, buf, sizeof(buf));
    ::close(fd);
    lrc_str =QString(buf);
    lrc_str.remove("\t").remove("\r").remove("\n");
    lrc_str.remove("]");
    lrc_list = lrc_str.split("[");
}

void Music::update_slider() //进度条实时显示
{
    ui->horizontalSlider->setValue(pos_2);
}

void Music::lrc()   //歌词与时间显示
{
    bool ok;
    int sec ,min;
    process->write("get_time_pos\n");
    process->write("get_time_length\n");
    QString data_str = process->readAll();
    if(data_str.startsWith("ANS_TIME_POSITION"))
    {
        QStringList pos_length = data_str.split("\n");

        //当前时间
        QString pos = QString(pos_length[0]);
        pos.remove("ANS_TIME_POSITION=");
        QStringList pos_1 = pos.split(".");
        pos_2 = QString(pos_1[0]).toInt(&ok, 10);
        sec = pos_2%60;
        min = pos_2/60;
        pos = QString("%1:%2").arg(min,2,10,QChar('0')).arg(sec,2,10,QChar('0'));
        for(int a = 0;a<lrc_list.length()-1;a++)
        {
            QString str = QString(lrc_list[a]);
            int ret = str.indexOf(".");
            QString substr=str.mid(0,ret);
            if( substr <= pos)
            {
                str.remove(0,8);
                ui->label_2->setText(str);
            }
        }

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
        ui->label_3->setText(time_p_l);

    }
}

void Music::on_pushButton_2_clicked()   //上一首
{
    if(j == 0)
    {
        if(i_mus == 2)
        {
            i_mus = fileInfo_1->size()-1;

        }
        else
        {
            i_mus--;
        }
    }

    else if(j == 1)
    {
        i_mus =rand()%(fileInfo_1->size()-2) + 2;
    }
    process->close();
    QString m_play = QString("mplayer -slave -quiet %1 & ").arg(fileInfo_1->at(i_mus).filePath());
    qDebug() << m_play;
    process->start(m_play);
    ui->label_2->clear();
    ui->listWidget->setCurrentRow(i_mus-2);
    ui->pushButton_5->setIcon(QIcon("/home/gec/image/pause.png"));
    flag = 1;
    p_p = 0;
    timer.start(100);
    timer_slider.start(100);
}

void Music::on_pushButton_3_clicked()
{
    if(j == 0)
    {
        if(i_mus == fileInfo_1->size()-1)
        {
            i_mus = 2;
        }
        else
        {
            i_mus++;
        }
    }
    else if(j == 1)
    {
        i_mus =rand()%(fileInfo_1->size()-2) + 2;
    }

    process->close();
    QString m_play = QString("mplayer -slave -quiet %1 & ").arg(fileInfo_1->at(i_mus).filePath());
    qDebug() << m_play;
    process->start(m_play);
    ui->label_2->clear();
    ui->listWidget->setCurrentRow(i_mus-2);
    ui->pushButton_5->setIcon(QIcon("/home/gec/image/pause.png"));
    flag = 1;
    p_p = 0;
    timer.start(100);
    timer_slider.start(100);
}

void Music::on_pushButton_4_clicked()   //模式按钮
{
    if(j == 0 )
    {
        j = 1 ;
        ui->pushButton_4->setIcon(QIcon("/home/gec/image/shuffle.png"));
    }
    else if(j == 1)
    {
        j = 2 ;
        ui->pushButton_4->setIcon(QIcon("/home/gec/image/repeat1.png"));
    }
    else if(j == 2)
    {
        j = 0 ;
        ui->pushButton_4->setIcon(QIcon("/home/gec/image/repeat.png"));
    }
}

void Music::on_pushButton_11_clicked()  //列表按钮
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

void Music::on_listWidget_clicked() //列表点击事件
{
    process->close();
    i_mus = ui->listWidget->currentRow() + 2;//当前单击选中ListWidget控件的行号（第几行）
    QString m_play = QString("mplayer -slave -quiet %1 & ").arg(fileInfo_1->at(i_mus).filePath());
    qDebug() << m_play;
    process->start(m_play);
    QPixmap pic;    //图片
    pic.load(fileInfo_2->at(i_mus).filePath());
    pic = pic.scaled(ui->label->width(), ui->label->height());
    ui->label_2->clear();
    ui->label->setPixmap(pic);
    ui->pushButton_5->setIcon(QIcon("/home/gec/image/pause.png"));
    flag = 1;
    p_p = 0;
    timer.start(100);
    timer_slider.start(100);
}

void Music::on_pushButton_6_clicked()   //停止
{
    timer.stop();
    timer_slider.stop();
    timer_voice.stop();
    ui->pushButton_5->setIcon(QIcon("/home/gec/image/play.png"));
    ui->listWidget->clearSelection();
    ui->horizontalSlider->setValue(0);
    ui->label_2->clear();
    ui->label_3->clear();
    if(flag == 1)
    {
        process->write("quit\n");
        flag = 0;
        p_p = 0;
    }
}

void Music::on_pushButton_7_clicked()   //歌词按钮
{
    if(ui->label_2->isHidden())
    {
        ui->label_2->show();
    }
    else
    {
        ui->label_2->hide();
    }
}

void Music::on_horizontalSlider_sliderMoved(int position)   //拖动进度条
{
    QString str = QString("seek "+QString::number(position)+" 2\n");
    process->write(str.toStdString().c_str());
    ui->horizontalSlider->setValue(position);
}

void Music::on_horizontalSlider_sliderPressed()
{
    timer_slider.stop();
}

void Music::on_horizontalSlider_sliderReleased()
{
    timer_slider.start(100);
}

void Music::on_pushButton_8_clicked()
{
    if(ui->textBrowser->isHidden())
    {
        //禁用按钮
        ui->pushButton_11->setDisabled(true);
        ui->pushButton_2->setDisabled(true);
        ui->pushButton_3->setDisabled(true);
        ui->pushButton_4->setDisabled(true);
        ui->pushButton_5->setDisabled(true);
        ui->pushButton_7->setDisabled(true);
        ui->pushButton_8->setIcon(QIcon("/home/gec/image/keyboard.png"));
        ui->textBrowser->show();
        connect(&timer_voice, SIGNAL(timeout()), this, SLOT(Speech_Recognition()));
        timer_voice.start(7000);
    }
    else
    {
        //启用按钮
        ui->pushButton_11->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_7->setEnabled(true);
        timer_voice.stop();
        ui->textBrowser->setText(0);
        ui->pushButton_8->setIcon(QIcon("/home/gec/image/voice.png"));
        ui->textBrowser->hide();
    }
}

void Music::Speech_Recognition()
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
    if(str == "播放音乐" || str == "继续播放" || str == "暂停播放")
    {
        QTimer *pTimer = new QTimer(this);
        connect(pTimer,SIGNAL(timeout()),this,SLOT(on_pushButton_5_clicked()));
        pTimer->setSingleShot(true);
        pTimer->start(10);
        QTimer *pTimer_1 = new QTimer(this);
        connect(pTimer_1,SIGNAL(timeout()),this,SLOT(music_pic_lrc()));
        pTimer_1->setSingleShot(true);
        pTimer_1->start(10);
    }

    else if ( str == "下一首")
    {
        QTimer *pTimer = new QTimer(this);
        connect(pTimer,SIGNAL(timeout()),this,SLOT(on_pushButton_3_clicked()));
        pTimer->setSingleShot(true);
        pTimer->start(10);
        QTimer *pTimer_1 = new QTimer(this);
        connect(pTimer_1,SIGNAL(timeout()),this,SLOT(music_pic_lrc()));
        pTimer_1->setSingleShot(true);
        pTimer_1->start(10);
    }

    else if( str == "上一首")
    {
        QTimer *pTimer = new QTimer(this);
        connect(pTimer,SIGNAL(timeout()),this,SLOT(on_pushButton_2_clicked()));
        pTimer->setSingleShot(true);
        pTimer->start(10);
        QTimer *pTimer_1 = new QTimer(this);
        connect(pTimer_1,SIGNAL(timeout()),this,SLOT(music_pic_lrc()));
        pTimer_1->setSingleShot(true);
        pTimer_1->start(10);
    }

    else if( str == "随机播放")
    {
        j = 1 ;
        ui->pushButton_4->setIcon(QIcon("/home/gec/image/shuffle.png"));
    }

    else if( str == "循环播放")
    {
        j = 0 ;
        ui->pushButton_4->setIcon(QIcon("/home/gec/image/repeat.png"));
    }

    else if( str == "单曲循环")
    {
        j = 2 ;
        ui->pushButton_4->setIcon(QIcon("/home/gec/image/repeat1.png"));
    }

    else if( str == "打开播放列表")
    {
        ui->listWidget->show();
    }

    else if( str == "关闭播放列表")
    {
        ui->listWidget->hide();
    }

    else if( str == "打开歌词")
    {
        ui->label_2->show();
    }

    else if( str ==  "关闭歌词")
    {
        ui->label_2->hide();
    }

    else if( str == "停止播放")
    {
        QTimer *pTimer = new QTimer(this);
        connect(pTimer,SIGNAL(timeout()),this,SLOT(on_pushButton_6_clicked()));
        pTimer->setSingleShot(true);
        pTimer->start(10);
    }

    else if( str == "退出语音")
    {
        ui->pushButton_11->setEnabled(true);
        ui->pushButton_2->setEnabled(true);
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
        ui->pushButton_5->setEnabled(true);
        ui->pushButton_7->setEnabled(true);
        timer_voice.stop();
        ui->textBrowser->setText(0);
        ui->pushButton_8->setIcon(QIcon("/home/gec/image/voice.png"));
        ui->textBrowser->hide();
    }
}



void Music::on_verticalSlider_sliderMoved(int position)
{
    QString str = QString("volume "+QString::number(position)+" 2\n");
    process->write(str.toStdString().c_str());
    ui->horizontalSlider->setValue(position);
    if(position == 0)
    {
        ui->pushButton_9->setIcon(QIcon("/home/gec/image/jingyin.png"));
    }
    else
    {
        ui->pushButton_9->setIcon(QIcon("/home/gec/image/sound.png"));
    }
}

void Music::on_pushButton_9_clicked()
{
    timer_mute.start(10);

    p_seek++;
}

void Music::v_mute()
{
    p_time++;
    if(p_time == 30)
    {
        timer_mute.stop();
        qDebug() << "双击失败";
        if(ui->verticalSlider->isHidden())
        {
            ui->verticalSlider->show();
        }
        else
        {
            ui->verticalSlider->hide();
        }
        p_seek = 0;
        p_time = 0;

    }
    if(p_seek == 2)
    {
        timer_mute.stop();
        qDebug() << "双击成功";
        process->write("mute\n");
        if(mute == 0)
        {
            ui->pushButton_9->setIcon(QIcon("/home/gec/image/jingyin.png"));
            ui->verticalSlider->setValue(0);
            mute = 1;
        }
        else
        {
           ui->pushButton_9->setIcon(QIcon("/home/gec/image/sound.png"));
           ui->verticalSlider->setValue(60);
           mute = 0;
        }

        p_seek = 0;
        p_time = 0;


    }
}

