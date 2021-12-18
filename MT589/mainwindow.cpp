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

    QStringList horlist;
    for (size_t i = 0; i < 16; ++i) {
        horlist << std::to_string(i).c_str();
    }
    QStringList verlist;
    for (size_t i = 0; i < 32; ++i) {
        verlist << std::to_string(i).c_str();
    }
    ui->tableWidget->setHorizontalHeaderLabels(horlist);
    ui->tableWidget->setVerticalHeaderLabels(verlist);
    clearInputs();
    setupRegs();
    setLCDsColor();
    setupBoxes();
//    update_on_cpu_data();

    handleInputState();

    for (size_t i = 0; i < 32; ++i) {
        std::vector<QTableWidgetItem*> row;
        for (size_t j = 0; j < 16; ++j) {
            QTableWidgetItem* item = new QTableWidgetItem();
            row.push_back(item);
            ui->tableWidget->setItem(i, j, item);
        }
        items.push_back(row);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_stepButton_clicked()
{
    if (nextCol == -1 && nextRow == - 1) {
        nextRow = startRow;
        nextCol = startColumn;
    }
    microcommand command = mk.rom.read(nextRow, nextCol);
    mk.execute_cpe(command.F, command.K, 0, command.M, command.CI);
    mk.mcu.connect_data(command.AC, 0, mk.CO, command.FC);
    mk.mcu.execute();
    mk.mcu.decode_adr();

    if (command.empty) {
        items[nextRow][nextCol]->setBackground(QBrush(Qt::transparent));
    } else {
        items[nextRow][nextCol]->setBackground(QBrush(Qt::blue));
    }

    nextRow = mk.mcu.get_row_adr();
    nextCol = mk.mcu.get_col_adr();
    items[nextRow][nextCol]->setBackground(QBrush(Qt::yellow));
    update_on_cpu_data();

}

void MainWindow::on_runButton_clicked()
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
//    ui->iLineEdit->setValidator(validator);
    ui->mLineEdit->setValidator(validator);
    ui->kLineEdit->setValidator(validator);
    ui->riLineEdit->setValidator(validator);
    ui->startAddressEdit->setValidator(validator);

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
    std::vector<std::string> acs = {
        "0000000",
        "0100000",
        "0110000",
        "1110000"
    };
    ui->commandAddressEdit->setText(acs[0].c_str());
}

void MainWindow::clearInputs() {
    ui->ciLineEdit->setText("0");
    ui->mLineEdit->setText("00000000");
    ui->ramcLineEdit->setText("00");
    ui->kLineEdit->setText("");
    ui->riLineEdit->setText("0");
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
  ui->Dlcd->display(mk.MEM[AC]);
  ui->Alcd->display(mk.MAR);
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
    std::vector<std::string> acs = {
        "0000000",
        "0100000",
        "0110000",
        "1110000",
    };
    ui->commandAddressEdit->setText(acs[index].c_str());
}

void MainWindow::handleInputState() {
    std::string cpeFunc = ui->boxCPE->currentText().toStdString();

    std::string reg = ui->boxREG->currentText().toStdString();
    std::string fic = ui->boxFC1->currentText().toStdString();
    std::string foc = ui->boxFC2->currentText().toStdString();
    std::string jump = ui->boxJUMP->currentText().toStdString();

    bool haveEmptyLineEdit = false;
    std::string address_control = ui->commandAddressEdit->text().toStdString();
//    std::string i = ui->iLineEdit->text().toStdString();
    std::string k = ui->kLineEdit->text().toStdString();
    std::string m = ui->mLineEdit->text().toStdString();
    std::string ri  = ui->riLineEdit->text().toStdString();
    std::string ci = ui->ciLineEdit->text().toStdString();

    haveEmptyLineEdit = address_control.size() < 7
             or k.size() < 8 or m.size() < 8 or ri.empty() or ci.empty();
    ui->saveButton->setEnabled(!haveEmptyLineEdit);
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

void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    fillInputs();
}

void MainWindow::fillInputs() {
    int currentRow = ui->tableWidget->currentRow();
    int currentColumn = ui->tableWidget->currentColumn();

    microcommand command = mk.rom.read(currentRow, currentColumn);

    ui->boxCPE->setCurrentIndex(command.index_F);
    ui->boxFC1->setCurrentIndex(command.index_FIC);
    ui->boxFC2->setCurrentIndex(command.index_FOC);
    ui->boxJUMP->setCurrentIndex(command.index_Jump);

    ui->kLineEdit->setText(std::bitset<8>(command.K).to_string().c_str());
    ui->commandAddressEdit->setText(command.address_control.c_str());
    ui->ciLineEdit->setText(std::to_string(command.CI).c_str());
    ui->mLineEdit->setText(std::bitset<8>(command.M).to_string().c_str());
}


void MainWindow::on_saveButton_clicked()
{
    std::bitset<7> cpeFunc = getFromFunc(ui->boxCPE->currentText().toStdString());
    std::bitset<4> reg = getFromReg(ui->boxREG->currentText().toStdString(), getRGroup(ui->boxREG->currentIndex()));
    std::bitset<2> fic = getFromFIC(ui->boxFC1->currentText().toStdString());
    std::bitset<2> foc = getFromFOC(ui->boxFC2->currentText().toStdString());
    std::bitset<7> jumpMask = getFromJump(ui->boxJUMP->currentText().toStdString());
    std::bitset<7> address_control = std::bitset<7>(ui->commandAddressEdit->text().toStdString());
//    std::bitset<8> i = std::bitset<8>(ui->iLineEdit->text().toStdString());
    std::bitset<8> k = std::bitset<8>(ui->kLineEdit->text().toStdString());
    std::bitset<8> m = std::bitset<8>(ui->mLineEdit->text().toStdString());
    std::bitset<1> ri   = std::bitset<1>(ui->riLineEdit->text().toStdString());
    std::bitset<1> ci = std::bitset<1>(ui->ciLineEdit->text().toStdString());

    std::bitset<7> buf("111" + reg.to_string());

    clearInputs();
    microcommand command;
    command.F = buf & cpeFunc;

    command.index_F = ui->boxCPE->currentIndex();
    command.index_FIC = ui->boxFC1->currentIndex();
    command.index_FOC = ui->boxFC2->currentIndex();
    command.index_Jump = ui->boxJUMP->currentIndex();
    command.address_control = address_control.to_string();
    command.index_REG = ui->boxREG->currentIndex();
    command.M = m.to_ulong();
    command.CI = ci.to_ulong();

    std::bitset<4> fc_buf(foc.to_string());
    fc_buf = (fc_buf << 2) | std::bitset<4>(fic.to_string());
    command.FC = fc_buf.to_ulong();

    command.AC = jumpMask | address_control;

    command.K = k.to_ulong();
    command.empty = false;

    int currentRow = ui->tableWidget->currentRow();
    int currentColumn = ui->tableWidget->currentColumn();
    if (currentRow >= 0 and currentRow < 32 and currentColumn >= 0 and currentColumn < 16) {
        if (currentRow != startRow || currentColumn != startColumn) {
            items[currentRow][currentColumn]->setBackground(QBrush(Qt::blue));
        }
        mk.rom.write(currentRow, currentColumn, command);
    }

    ui->tableWidget->clearSelection();
}

void MainWindow::on_clearButton_clicked()
{
    microcommand command;
    int currentRow = ui->tableWidget->currentRow();
    int currentColumn = ui->tableWidget->currentColumn();
    if (currentRow >= 0 and currentRow < 32 and currentColumn >= 0 and currentColumn < 16) {
        if (currentRow != startRow || currentColumn != startColumn) {
            items[currentRow][currentColumn]->setBackground(QBrush(Qt::transparent));
        }
        mk.rom.write(currentRow, currentColumn, command);
    }
}

void MainWindow::on_startAddressEdit_textEdited(const QString &arg1)
{

}

void MainWindow::on_loadButton_clicked()
{
    std::bitset<8> addr = std::bitset<8>(ui->startAddressEdit->text().toStdString());
    mk.mcu.connect_data(std::bitset<7>(), addr, 0, 0);
    mk.mcu.load();
    mk.mcu.decode_adr();
    if (!(startRow == -1 && startColumn == -1)) {
        items[startRow][startColumn]->setBackground(QBrush(Qt::transparent));
    }
    startRow = mk.mcu.get_row_adr();
    startColumn = mk.mcu.get_col_adr();
    items[startRow][startColumn]->setBackground(QBrush(Qt::yellow));
    ui->tableWidget->clearSelection();
}


void MainWindow::on_ramcLineEdit_textEdited(const QString &arg1)
{
    handleInputState();
}

