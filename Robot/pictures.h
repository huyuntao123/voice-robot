#ifndef PICTURES_H
#define PICTURES_H

#include <QMainWindow>

#include <QDebug>
#include <QProcess>
#include <QFileDialog>
#include <QTimer>
#include <QDebug>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

namespace Ui {
class pictures;
}

class pictures : public QMainWindow
{
    Q_OBJECT

public:
    explicit pictures(QWidget *parent = 0);
    ~pictures();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void push();

    void ro_speech();

    void on_pushButton_5_clicked();

private:
    Ui::pictures *ui;
};

#endif // PICTURES_H
