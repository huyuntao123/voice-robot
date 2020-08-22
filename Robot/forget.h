#ifndef FORGET_H
#define FORGET_H

#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <QMainWindow>

namespace Ui {
class forget;
}

class forget : public QMainWindow
{
    Q_OBJECT

public:
    explicit forget(QWidget *parent = 0);
    ~forget();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::forget *ui;
};

#endif // FORGET_H
