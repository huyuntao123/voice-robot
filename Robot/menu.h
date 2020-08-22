#ifndef MENU_H
#define MENU_H

#include <QMainWindow>
#include <pictures.h>
#include <chat.h>
#include <music.h>
#include <video.h>
#include <wallpaper.h>
#include <QTimer>
#include <QDebug>
#include <QFileDialog>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

namespace Ui {
class menu;
}

class menu : public QMainWindow
{
    Q_OBJECT

public:
    explicit menu(QWidget *parent = 0);
    ~menu();

private slots:
    void on_pushButton_clicked();

    void on_listWidget_activated();

    void on_pushButton_6_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void menu_voice();

private:
    Ui::menu *ui;
    pictures w6;
    chat     w7;
    Music    w8;
    Video    w9;
    wallpaper w10;

};

#endif // MENU_H
