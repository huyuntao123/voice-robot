#ifndef VIDEO_H
#define VIDEO_H

#include <QMainWindow>

#include <QTimer>
#include <QDebug>
#include <QProcess>
#include <QFileDialog>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

namespace Ui {
class Video;
}

class Video : public QMainWindow
{
    Q_OBJECT

public:
    explicit Video(QWidget *parent = 0);
    ~Video();

private slots:
    void on_pushButton_1_clicked();

    void on_listWidget_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_14_clicked();

    void on_verticalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderPressed();

    void on_horizontalSlider_sliderReleased();

    void update_slider();

    void v_time();

    void v_seek();

    void on_pushButton_15_clicked();

    void on_pushButton_clicked();

    void v_speed();

    void on_pushButton_2_clicked();

    void v_mute();

    void on_pushButton_3_clicked();

    void Speech_Recognition();

    void on_pushButton_4_clicked();

private:
    Ui::Video *ui;
};

#endif // VIDEO_H
