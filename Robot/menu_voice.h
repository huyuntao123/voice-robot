#ifndef MENU_VOICE_H
#define MENU_VOICE_H

#include <QMainWindow>

namespace Ui {
class menu_voice;
}

class menu_voice : public QMainWindow
{
    Q_OBJECT

public:
    explicit menu_voice(QWidget *parent = 0);
    ~menu_voice();

private slots:
    void on_pushButton_clicked();

private:
    Ui::menu_voice *ui;
};

#endif // MENU_VOICE_H
