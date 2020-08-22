#ifndef MUSIC_H
#define MUSIC_H

#include <QMainWindow>

#include <QFileDialog>
#include <QProcess>
#include <QDebug>
#include <QStringList>
#include <QTimer>
#include <QPixmap>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

namespace Ui {
class Music;
}

class Music : public QMainWindow
{
    Q_OBJECT

public:
    explicit Music(QWidget *parent = 0);
    ~Music();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void music_pic_lrc();



    void update_slider();

    void lrc();



    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_11_clicked();

    void on_listWidget_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderPressed();

    void on_horizontalSlider_sliderReleased();

    void on_pushButton_8_clicked();

    void Speech_Recognition();

    void on_verticalSlider_sliderMoved(int position);

    void on_pushButton_9_clicked();

    void v_mute();


private:
    Ui::Music *ui;
};

#endif // MUSIC_H
