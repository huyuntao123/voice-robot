#include "menu_voice.h"
#include "ui_menu_voice.h"

menu_voice::menu_voice(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::menu_voice)
{
    ui->setupUi(this);
}

menu_voice::~menu_voice()
{
    delete ui;
}

void menu_voice::on_pushButton_clicked()
{
    this->parentWidget()->show();
    this->hide();
}
