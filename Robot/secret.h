#ifndef SECRET_H
#define SECRET_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>

namespace Ui {
class secret;
}

class secret : public QMainWindow
{
    Q_OBJECT

public:
    explicit secret(QWidget *parent = 0);
    ~secret();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::secret *ui;
};

#endif // SECRET_H
