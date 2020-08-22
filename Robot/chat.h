#ifndef CHAT_H
#define CHAT_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDebug>
#include <string.h>
#include <unistd.h>
#include <QTimer>
#include <QMessageBox>
#include <QFileDialog>
#include <QMainWindow>
#include <QNetworkReply>

namespace Ui {
class chat;
}

class chat : public QMainWindow
{
    Q_OBJECT

public:
    explicit chat(QWidget *parent = 0);
    ~chat();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void ro_chat();

    void replyFinished(QNetworkReply *);

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void ro_clock();

    void clo_time();

    void cm_time();

    void on_pushButton_5_clicked();

    void on_listWidget_clicked();

private:
    Ui::chat *ui;
};

#endif // CHAT_H
