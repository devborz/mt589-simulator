#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <util.hpp>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("MT589");

    setupRegs();
    setLCDsColor();
    setupBoxes();
    update_on_cpu_data();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_stepButton_clicked()
{
    if ((model.currentCommandIndex > model.listitems.size() - 1) && !model.isExecuting) {
        return;
    }
    CommandItem* command = model.listitems[model.currentCommandIndex];
//    cpe.fetch(command->f, command->i, command->k, command->m, command->ci, command->ri);
    cpe.decode();
    cpe.execute();
    update_on_cpu_data();
    model.currentCommandIndex += 1;
}


void MainWindow::on_minusButton_clicked()
{

    if (selectedCommand < model.listitems.size() && selectedCommand != -1) {
        CommandItem* item = model.listitems[selectedCommand];
        model.listitems.erase(model.listitems.begin() + selectedCommand);

        delete item;
        if (selectedCommand < model.listitems.size()) {

        } else {
            selectedCommand = -1;
            ui->listWidget->clearSelection();
        }
    }
}

void MainWindow::on_runButton_clicked()
{

}

void MainWindow::on_plusButton_clicked()
{

}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    selectedCommand = currentRow;
}

// HELP FUNCTIONS

void MainWindow::setupRegs() {
    QRegularExpression reg("[0-1]+");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(reg, this);

    ui->ciLineEdit->setValidator(validator);
    ui->iLineEdit->setValidator(validator);
    ui->mLineEdit->setValidator(validator);
    ui->kLineEdit->setValidator(validator);
    ui->riLineEdit->setValidator(validator);

   regLCDs.push_back(ui->reg0);
   regLCDs.push_back(ui->reg1);
   regLCDs.push_back(ui->reg2);
   regLCDs.push_back(ui->reg3);
   regLCDs.push_back(ui->reg4);
   regLCDs.push_back(ui->reg5);
   regLCDs.push_back(ui->reg6);
   regLCDs.push_back(ui->reg7);
   regLCDs.push_back(ui->reg8);
   regLCDs.push_back(ui->reg9);
}

void MainWindow::setLCDsColor() {
    QPalette palette = regLCDs[0]->palette();
    palette.setColor(QPalette::Normal, QPalette::WindowText, Qt::green);
    for (size_t i = 0; i < 10; ++i) {
        regLCDs[i]->setPalette(palette);
    }
    ui->regMAR->setPalette(palette);
    ui->regT->setPalette(palette);
    ui->regAC->setPalette(palette);
    ui->Xlcd->setPalette(palette);
    ui->Ylcd->setPalette(palette);
    ui->COlcd->setPalette(palette);
    ui->ROlcd->setPalette(palette);
    ui->Alcd->setPalette(palette);
    ui->Dlcd->setPalette(palette);
}

void MainWindow::setupBoxes() {

    FuncListItem::prepareFunctions();
    QStringList list;
    for (FuncListItem* func: FuncListItem::items) {
        list << func->name.c_str();
    }
    ui->boxCPE->addItems(list);

    ui->boxREG->addItems({
                             "R0",
                             "R1",
                             "R2",
                             "R3",
                             "R4",
                             "R5",
                             "R6",
                             "R7",
                             "R8",
                             "R9",
                             "MAR",
                             "T",
                             "AC"
                         });
    ui->boxFC1->addItems({
                             "SCZ",
                             "STZ",
                             "STC",
                             "HCZ"
                         });
    ui->boxFC2->addItems({
                             "FF0",
                             "FFC",
                             "FFZ",
                             "FFI"
                         });

    ui->boxJUMP->addItems({
                              "JCC",
                              "JZR",
                              "JCR",
                              "JCE"
                          });
}

void MainWindow::clearInputs() {
//    ui->functionLineEdit->setText("");
    ui->ciLineEdit->setText("");
    ui->iLineEdit->setText("");
    ui->mLineEdit->setText("");
    ui->kLineEdit->setText("");
    ui->riLineEdit->setText("");
}

void MainWindow::update_on_cpu_data() {
  for (size_t i = 0; i < 10; ++i) {
      regLCDs[i]->display(cpe.MEM[i]);
  }
  ui->regMAR->display(cpe.MAR);
  ui->regT->display(cpe.MEM[T]);
  ui->regAC->display(cpe.MEM[AC]);
  ui->Xlcd->display(cpe.X);
  ui->Ylcd->display(cpe.Y);
  ui->COlcd->display(cpe.CO);
  ui->ROlcd->display(cpe.RO);
}

void MainWindow::on_boxCPE_currentIndexChanged(int index)
{

}


void MainWindow::on_boxREG_currentIndexChanged(int index)
{

}


void MainWindow::on_boxFC1_currentIndexChanged(int index)
{

}


void MainWindow::on_boxFC2_currentIndexChanged(int index)
{

}


void MainWindow::on_boxJUMP_currentIndexChanged(int index)
{

}

void MainWindow::handleInputState() {
    std::string cpeFunc = ui->boxCPE->currentText().toStdString();
    // fetch r group and change regs list if needed

    std::string reg = ui->boxREG->currentText().toStdString();
    std::string fic = ui->boxFC1->currentText().toStdString();
    std::string foc = ui->boxFC2->currentText().toStdString();
    std::string jump = ui->boxJUMP->currentText().toStdString();

    bool haveEmptyLineEdit = false;
    std::string command_adress = ui->commandAddressEdit->text().toStdString();
    std::string i = ui->iLineEdit->text().toStdString();
    std::string k = ui->kLineEdit->text().toStdString();
    std::string m = ui->mLineEdit->text().toStdString();
    std::string ri   = ui->riLineEdit->text().toStdString();
    std::string ci = ui->ciLineEdit->text().toStdString();

    haveEmptyLineEdit = command_adress.empty() or i.empty() or k.empty() or m.empty() or ri.empty() or ci.empty();
    ui->plusButton->setEnabled(!haveEmptyLineEdit);
}


