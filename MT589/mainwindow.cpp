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
    FuncListItem::prepareFunctions();
    for (FuncListItem* func: FuncListItem::items) {
        func->setText(func->name.c_str());
        ui->funcList->addItem(func);
    }
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


void MainWindow::on_stepButton_clicked()
{
    if ((model.currentCommandIndex > model.listitems.size() - 1) && !model.isExecuting) {
        return;
    }
    CommandItem* command = model.listitems[model.currentCommandIndex];
    cpe.fetch(command->f, command->i, command->k, command->m, command->ci, command->ri);
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
//            CommandItem* item = model.listitems[selectedCommand];
//            item->setSelected(true);
        } else {
            selectedCommand = -1;
            ui->listWidget->clearSelection();
        }
    }
}

void MainWindow::on_runButton_clicked()
{
//    if ((model.currentCommandIndex > model.listitems.size() - 1) && !model.isExecuting) {
//        return;
//    }
//    for (size_t i = model.currentCommandIndex; i < model.listitems.size(); ++i)
//    CommandItem* command = model.listitems[model.currentCommandIndex];
//    cpe.fetch(command->f, command->i, command->k, command->m, command->ci, command->ri);
//    cpe.decode();
//    cpe.execute();
//    update_on_cpu_data();
//    model.currentCommandIndex += 1;
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

    CommandItem* command = new CommandItem(fvec, i, k, ci, ri, m);
    command->setText(QString(command->prepareText().c_str()));

    model.listitems.push_back(command);
    ui->listWidget->addItem(command);

}

void MainWindow::on_funcList_itemActivated(QListWidgetItem *item)
{
    FuncListItem* func = dynamic_cast<FuncListItem*>(item);
    ui->functionLineEdit->setText(func->get_func().c_str());
    ui->kLineEdit->setText(func->get_k().c_str());
}


void MainWindow::on_funcList_itemClicked(QListWidgetItem *item)
{
    FuncListItem* func = dynamic_cast<FuncListItem*>(item);
    ui->functionLineEdit->setText(func->get_func().c_str());
    ui->kLineEdit->setText(func->get_k().c_str());
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
//    CommandItem* command = dynamic_cast<CommandItem*>(item);
//    for (CommandItem item: model.listitems) {
//        if item
//    }
}


void MainWindow::on_listWidget_pressed(const QModelIndex &index)
{
    selectedCommand = index.row();
}


void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    selectedCommand = currentRow;
}


void MainWindow::on_funcList_currentRowChanged(int currentRow)
{
    FuncListItem* func = FuncListItem::items[currentRow];
    ui->functionLineEdit->setText(func->get_func().c_str());
    ui->kLineEdit->setText(func->get_k().c_str());
}

