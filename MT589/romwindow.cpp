#include "romwindow.h"
#include "ui_romwindow.h"

ROMWindow::ROMWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ROMWindow)
{
    ui->setupUi(this);
}

ROMWindow::~ROMWindow()
{
    delete ui;
}
