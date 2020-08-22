#ifndef WALLPAPER_H
#define WALLPAPER_H

#include <QDebug>
#include <QProcess>
#include <QFileDialog>
#include <QMainWindow>

namespace Ui {
class wallpaper;
}

class wallpaper : public QMainWindow
{
    Q_OBJECT

public:
    explicit wallpaper(QWidget *parent = 0);
    ~wallpaper();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::wallpaper *ui;
};

#endif // WALLPAPER_H
