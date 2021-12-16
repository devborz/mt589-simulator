#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <util.hpp>

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

//    regLCDs.push_back(ui->lcdReg0);
//    regLCDs.push_back(ui->lcdReg1);
//    regLCDs.push_back(ui->lcdReg2);
//    regLCDs.push_back(ui->lcdReg3);
//    regLCDs.push_back(ui->lcdReg4);
//    regLCDs.push_back(ui->lcdReg5);
//    regLCDs.push_back(ui->lcdReg6);
//    regLCDs.push_back(ui->lcdReg7);
//    regLCDs.push_back(ui->lcdReg8);
//    regLCDs.push_back(ui->lcdReg9);
//    regLCDs.push_back(ui->lcdReg10);
//    regLCDs.push_back(ui->lcdReg11);
    update_on_cpu_data();
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

void MainWindow::update_on_cpu_data() {
  
}


void MainWindow::on_stepButton_clicked()
{
    if ((model.currentCommandIndex > model.listitems.size() - 1) && !model.isExecuting) {
        return;
    }
    CommandItem command = model.listitems[model.currentCommandIndex];
    cpe.fetch(command.f, command.i, command.k, command.m, command.ci, command.ri);
    cpe.decode();
    cpe.execute();
    update_on_cpu_data();
    model.currentCommandIndex += 1;
}


void MainWindow::on_minusButton_clicked()
{
    ui->listWidget->removeItemWidget(ui->listWidget->itemAt(model.listitems.size() - 1, 0));
    model.listitems.pop_back();
}

void MainWindow::on_runButton_clicked()
{

}

void MainWindow::on_plusButton_clicked()
{
    std::string fstr = ui->functionLineEdit->text().toStdString();
    std::vector<BYTE> fvec= {};
    for (char&  ch: fstr) {
        std::string str = "";
        str += ch;
        int i = std::stoi(str);
        fvec.insert(fvec.begin(), i);
    }
    int i = fromStringBin(ui->iLineEdit->text().toStdString());
    int k = fromStringBin(ui->kLineEdit->text().toStdString());
    int m = fromStringBin(ui->mLineEdit->text().toStdString());
    int ri = fromStringBin(ui->riLineEdit->text().toStdString());
    int ci = fromStringBin(ui->ciLineEdit->text().toStdString());

    CommandItem command = CommandItem(fvec, i, k, ci, ri, m);
    command.setText(QString(command.prepareText().c_str()));

    model.listitems.push_back(command);
    ui->listWidget->addItem(command.prepareText().c_str());

}
