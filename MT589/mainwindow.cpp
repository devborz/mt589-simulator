#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "emulator.hpp"
#include <string>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("MT589");
    QRegularExpression reg("[0-1]+");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(reg, this);
    ui->functionLineEdit->setValidator(validator);
    ui->ciLineEdit->setValidator(validator);
    ui->iLineEdit->setValidator(validator);
    ui->mLineEdit->setValidator(validator);
    ui->kLineEdit->setValidator(validator);
    ui->riLineEdit->setValidator(validator);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::clearInputs() {
    ui->functionLineEdit->setText("");
    ui->ciLineEdit->setText("");
    ui->iLineEdit->setText("");
    ui->mLineEdit->setText("");
    ui->kLineEdit->setText("");
    ui->riLineEdit->setText("");
}

void MainWindow::on_stepButton_clicked()
{

}


void MainWindow::on_minusButton_clicked()
{
    ui->listWidget->removeItemWidget(ui->listWidget->itemAt(listitems.size() - 1, 0));
    listitems.pop_back();
}


void MainWindow::on_plusButton_clicked()
{
    std::cout << "hello";
//    int f = ui->functionLineEdit->text().toInt();
//    int i = ui->iLineEdit->text().toInt();
//    int k = ui->kLineEdit->text().toInt();
//    int m = ui->mLineEdit->text().toInt();
//    int ri = ui->riLineEdit->text().toInt();
//    int ci = ui->ciLineEdit->text().toInt();
    CommandItem command = CommandItem(0, 0, 0, 0, 0, 0);
    command.setText(QString(command.prepareText().c_str()));
    listitems.push_back(command);
    ui->listWidget->addItem(&command);
}


void MainWindow::on_runButton_clicked()
{

}



