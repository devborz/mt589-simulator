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
//    update_on_cpu_data();

    handleInputState();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_stepButton_clicked()
{

    if ((model.currentCommandIndex > model.listitems.size() - 1) || model.isExecuting) {
        return;
    }
    CommandItem* command = model.listitems[model.currentCommandIndex];
    mk.execute_cpe(command->f, command->k.to_ulong(), command->i.to_ulong(), command->m.to_ulong(), command->ri.to_ulong());
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
    std::bitset<7> cpeFunc = getFromFunc(ui->boxCPE->currentText().toStdString());
    std::bitset<4> reg = getFromReg(ui->boxREG->currentText().toStdString(), getRGroup(ui->boxREG->currentIndex()));
    std::bitset<2> fic = getFromFIC(ui->boxFC1->currentText().toStdString());
    std::bitset<2> foc = getFromFOC(ui->boxFC2->currentText().toStdString());
    std::bitset<7> jumpMask = getFromJump(ui->boxJUMP->currentText().toStdString());
    std::bitset<7> command_adress = std::bitset<7>(ui->commandAddressEdit->text().toStdString());
    std::bitset<8> i = std::bitset<8>(ui->iLineEdit->text().toStdString());
    std::bitset<8> k = std::bitset<8>(ui->kLineEdit->text().toStdString());
    std::bitset<8> m = std::bitset<8>(ui->mLineEdit->text().toStdString());
    std::bitset<1> ri   = std::bitset<1>(ui->riLineEdit->text().toStdString());
    std::bitset<1> ci = std::bitset<1>(ui->ciLineEdit->text().toStdString());

    std::bitset<7> buf(reg.to_string());
    CommandItem* item = new CommandItem(buf | cpeFunc, i, k, m, ci, ri);
    item->setText(item->prepareText().c_str());
    model.listitems.push_back(item);
    ui->listWidget->addItem(item);
    clearInputs();
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
    ui->ciLineEdit->setText("");
    ui->iLineEdit->setText("");
    ui->mLineEdit->setText("");
    ui->kLineEdit->setText("");
    ui->riLineEdit->setText("");
}

void MainWindow::update_on_cpu_data() {
  for (size_t i = 0; i < 10; ++i) {
      regLCDs[i]->display(mk.MEM[i]);
  }
  ui->regMAR->display(mk.MAR);
  ui->regT->display(mk.MEM[T]);
  ui->regAC->display(mk.MEM[AC]);
  ui->COlcd->display(mk.CO);
  ui->ROlcd->display(mk.RO);

  std::cout << mk.MAR << " " << mk.MEM[T] << " " << mk.MEM[AC] << " " << mk.CO << " " << mk.RO;
}

void MainWindow::on_boxCPE_currentIndexChanged(int index)
{
    int r_group = getRGroup(index);
    std::vector<std::string> regs;
    std::vector<std::string> addresses;
    getRGroupRegs(r_group, regs, addresses);

    QStringList list;
    for (auto& reg: regs) {
        list << reg.c_str();
    }
    for (int i = ui->boxREG->count() - 1; i >= 0; --i) {
        ui->boxREG->removeItem(i);
    }
    ui->boxREG->addItems(list);
    ui->kLineEdit->setText(getKFromFunc(index).c_str());
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

    haveEmptyLineEdit = command_adress.size() < 7
            or i.size() < 8 or k.size() < 8 or m.size() < 8 or ri.empty() or ci.empty();
    ui->plusButton->setEnabled(!haveEmptyLineEdit);
}

void MainWindow::on_commandAddressEdit_textEdited(const QString &arg1)
{
    handleInputState();
}


void MainWindow::on_iLineEdit_textEdited(const QString &arg1)
{
    handleInputState();
}


void MainWindow::on_kLineEdit_textEdited(const QString &arg1)
{
    handleInputState();
}


void MainWindow::on_mLineEdit_textEdited(const QString &arg1)
{
    handleInputState();
}


void MainWindow::on_riLineEdit_textEdited(const QString &arg1)
{
    handleInputState();
}


void MainWindow::on_ciLineEdit_textEdited(const QString &arg1)
{
    handleInputState();
}
