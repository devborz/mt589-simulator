﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <util.hpp>
#include <QPalette>
#include <QtGui>
#include <QFileDialog>
#include <filemanager.h>
#include <commandmodewindow.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("MT589");

    clearInputs();
    setupRegs();
    setLCDsColor();
    setupBoxes();
//    update_on_cpu_data();

    handleInputState();

    setupMatrix();
    setupRAM();

    configUIMode();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupMatrix() {
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
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    for (size_t i = 0; i < 32; ++i) {
        std::vector<QTableWidgetItem*> row;
        for (size_t j = 0; j < 16; ++j) {
            QTableWidgetItem* item = new QTableWidgetItem();
            row.push_back(item);
            ui->tableWidget->setItem(i, j, item);
        }
        matrixItems.push_back(row);
    }
}

void MainWindow::setupRAM() {
    QStringList verlist;
    QRegularExpression reg("[0-9]+");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(reg, this);
    for (size_t i = 0; i < mk.ram.size; ++i) {
        verlist << std::to_string(i).c_str();
    }

    ui->ramWidget->setHorizontalHeaderLabels({"DATA"});
    ui->ramWidget->setVerticalHeaderLabels(verlist);
    ui->ramWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    for (size_t i = 0; i < mk.ram.size; ++i) {
            QTableWidgetItem* item = new QTableWidgetItem();
            ramItems.push_back(item);
            item->setData(Qt::ItemDataRole::EditRole, 0);
            ui->ramWidget->setItem(i, 0, item);
    }
}

void MainWindow::on_stepButton_clicked()
{
    if (model.getMode() == editing) {
        setMode(running);
    }
    if (model.currentPoint.isNull()) {
        model.currentPoint = model.startPoint;
    }
    Point currentPoint = model.currentPoint;
    microcommand command = mk.rom.read(currentPoint.row, currentPoint.col);
    std::string ac = command.AC.to_string();

    if (command.RAMC == 0b01) {
        // read
        command.M = mk.ram.read(mk.MAR);
    } else {
        command.M = 0x00;
    }

    mk.do_fetch_decode_execute_cycle(command);
    if (command.RAMC == 0b00) {
            // write
        ramItems[mk.MAR]->setText(std::to_string(mk.D).c_str());
    }

    Point nextPoint = Point(mk.get_row_adr(), mk.get_col_adr());

    model.currentPoint = nextPoint;
    ui->rowLcd->display(model.currentPoint.row);
    ui->colLcd->display(model.currentPoint.col);

    configUIMode();
    changeCurrentPoint(currentPoint, nextPoint);
    update_on_cpu_data();

}

void MainWindow::on_runButton_clicked()
{
    if (model.getMode() == editing) {
        setMode(running);
    }
    if (model.currentPoint.isNull()) {
        model.currentPoint = model.startPoint;
    }
    ui->stepButton->setEnabled(false);
    while (model.getMode() == running) {
        Point currentPoint = model.currentPoint;
        microcommand command = mk.rom.read(currentPoint.row, currentPoint.col);

        mk.do_fetch_decode_execute_cycle(command);

        Point nextPoint = Point(mk.get_row_adr(), mk.get_col_adr());

        model.currentPoint = nextPoint;

        configUIMode();
        changeCurrentPoint(currentPoint, nextPoint);
        update_on_cpu_data();
    }
    ui->stepButton->setEnabled(true);
}

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
//    selectedCommand = currentRow;
}

// HELP FUNCTIONS

void MainWindow::setupRegs() {
    QRegularExpression reg("[0-9]+");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(reg, this);

    ui->mLineEdit->setValidator(validator);
//    ui->kLineEdit->setValidator(validator);
    ui->startAddressEdit->setValidator(validator);
    ui->commandAddressEdit->setValidator(validator);

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
    QStringList list;
    for (std::string& func: model.mnemonics) {
        list << func.c_str();
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
                              "JCE",
                              "JFL",
                              "JCF",
                              "JZF",
                              "JPR",
                              "JLL",
                              "JRL",
                              "JPX"
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
    ui->mLineEdit->setText("00000000");
    ui->wrCheckBox->setChecked(true);
    ui->loadCheckBox->setChecked(false);
    ui->ceCheckBox->setChecked(true);
    ui->kLineEdit->setText("");
}

void MainWindow::update_on_cpu_data() {
  for (size_t i = 0; i < 10; ++i) {
      regLCDs[i]->display(mk.MEM[i]);
  }
  ui->regMAR->display(mk.MAR);
  ui->regT->display(mk.MEM[T]);
  ui->regAC->display(mk.MEM[AC]);

  if (mk.is_performing_right_rot) {
      ui->ROlcd->display(mk.RO);
      ui->COlcd->display("-");
  } else {
      ui->COlcd->display(mk.CO);
      ui->ROlcd->display("-");
  }

  ui->Dlcd->display(mk.MEM[AC]);
  ui->Alcd->display(mk.MAR);
}

void MainWindow::on_boxCPE_currentIndexChanged(int index)
{
    ui->fLineEdit->setText(model.funcs[index].c_str());
    std::string k =  model.ks[index];
    ui->kLineEdit->setText(toHex(std::bitset<16>(k+k+k+k+k+k+k+k).to_ulong()).c_str());
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
        "1000000",
        "1010000",

        "1011000",

        "1100000",

        "1101000",

        "1111100",

        "1111000"
    };
    ui->commandAddressEdit->setText(acs[index].c_str());
}

void MainWindow::handleInputState() {

    bool haveEmptyLineEdit = false;
    std::string ac = ui->commandAddressEdit->text().toStdString();
    std::string i = ui->mLineEdit->text().toStdString();
    std::string k = ui->kLineEdit->text().toStdString();
    std::string f = ui->fLineEdit->text().toStdString();

    haveEmptyLineEdit = ac.size() < 7 or f.size() < 7
             or i.size() < 6 or k.size() < 6;
    ui->saveButton->setEnabled(!haveEmptyLineEdit && !model.currentPoint.isNull());
    ui->clearButton->setEnabled(!model.currentPoint.isNull());

    if (ui->startAddressEdit->text().toStdString().size() < 8) {
        ui->loadButton->setEnabled(false);
    } else {
        ui->loadButton->setEnabled(true);
    }
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
    auto previousPoint = model.currentPoint;
    if (model.getMode() == editing) {
        model.currentPoint = Point(currentRow, currentColumn);
        changeCurrentPoint(previousPoint, model.currentPoint);
        if (previousPoint.isNull()) {
            ui->tableWidget->setCurrentIndex(QModelIndex(ui->tableWidget->model()->index(currentRow, currentColumn)));
        }
        configUIMode();
    }
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    auto previousPoint = model.currentPoint;
    if (model.getMode() == editing) {
        model.currentPoint = Point(row, column);
        changeCurrentPoint(previousPoint, model.currentPoint);
        if (previousPoint.isNull()) {
            ui->tableWidget->setCurrentIndex(QModelIndex(ui->tableWidget->model()->index(row, column)));
        }
        configUIMode();
    }
}

void MainWindow::fillInputs() {
    if (model.currentPoint.isNull()) {
        ui->boxCPE->setCurrentIndex(0);
        ui->boxFC1->setCurrentIndex(0);
        ui->boxFC2->setCurrentIndex(0);
        ui->boxJUMP->setCurrentIndex(0);

        ui->kLineEdit->setText("0x0000");
//        ui->iLineEdit->setText("0x0000");
        ui->mLineEdit->setText("0x0000");
        ui->fLineEdit->setText("0001001");
        ui->wrCheckBox->setChecked(false);
        ui->loadCheckBox->setChecked(false);
        ui->ceCheckBox->setChecked(false);
        on_boxJUMP_currentIndexChanged(0);
    } else {
        auto point = model.currentPoint;

        microcommand command = mk.rom.read(point.row, point.col);

        if (command.empty) {
            ui->boxCPE->setCurrentIndex(0);
            ui->boxFC1->setCurrentIndex(0);
            ui->boxFC2->setCurrentIndex(0);
            ui->boxJUMP->setCurrentIndex(0);

            ui->kLineEdit->setText("0x0000");
            ui->mLineEdit->setText("0x0000");
            ui->fLineEdit->setText("0001001");
            ui->wrCheckBox->setChecked(false);
            ui->loadCheckBox->setChecked(false);
            ui->ceCheckBox->setChecked(false);
            on_boxJUMP_currentIndexChanged(0);
        } else {
            ui->boxCPE->setCurrentIndex(command.index_F);
            ui->boxFC1->setCurrentIndex(command.index_FIC);
            ui->boxFC2->setCurrentIndex(command.index_FOC);
            ui->boxJUMP->setCurrentIndex(command.index_Jump);

            ui->kLineEdit->setText(toHex(command.K).c_str());
            ui->commandAddressEdit->setText(command.address_control.c_str());
            ui->mLineEdit->setText(toHex(command.I).c_str());
            ui->fLineEdit->setText(command.F.to_string().c_str());
            ui->wrCheckBox->setChecked(command.RW == 1);
            ui->loadCheckBox->setChecked(command.LD == 1);
            ui->ceCheckBox->setChecked(command.CS == 1);
        }
    }
}


void MainWindow::on_saveButton_clicked()
{
    std::bitset<7> f = std::bitset<7>(ui->fLineEdit->text().toStdString().c_str());

    int fic = ui->boxFC1->currentIndex();
    int foc = ui->boxFC2->currentIndex();

    std::bitset<7> jumpMask = getFromJump(ui->boxJUMP->currentText().toStdString());
    std::bitset<7> address_control = std::bitset<7>(ui->commandAddressEdit->text().toStdString());
    WORD i = parseHex(ui->mLineEdit->text().toStdString());
    WORD k = parseHex(ui->kLineEdit->text().toStdString());

    microcommand command;
    command.F = f;
    command.I = i;
    command.K = k;
    command.empty = false;

    command.index_F = ui->boxCPE->currentIndex();
    command.index_FIC = ui->boxFC1->currentIndex();
    command.index_FOC = ui->boxFC2->currentIndex();
    command.index_Jump = ui->boxJUMP->currentIndex();
    command.address_control = address_control.to_string();
    command.RW = ui->wrCheckBox->isChecked() ? 1 : 0;
    command.LD = ui->loadCheckBox->isChecked() ? 1 : 0;
    command.CS = ui->ceCheckBox->isChecked() ? 1 : 0;

    BYTE fc_buf = ((foc << 2) + fic) & 0b1111;
    std::string str = std::bitset<4>(fc_buf).to_string();
    command.FC = fc_buf;

    command.AC = jumpMask | address_control;

    Point currentPoint = model.currentPoint;

    if (!currentPoint.isNull()) {
        mk.rom.write(currentPoint.row, currentPoint.col, command);
    }

    setItemColor(currentPoint);

    handleInputState();
}

void MainWindow::on_clearButton_clicked()
{
    microcommand command;
    Point currentPoint = model.currentPoint;

    if (!currentPoint.isNull()) {
        mk.rom.write(currentPoint.row, currentPoint.col, command);
    }

    setItemColor(currentPoint);
    ui->tableWidget->clearSelection();
}

void MainWindow::on_startAddressEdit_textEdited(const QString &arg1)
{
    handleInputState();
}

void MainWindow::on_loadButton_clicked()
{
    std::bitset<8> addr = std::bitset<8>(ui->startAddressEdit->text().toStdString());
    mk.load(addr); // fix load (with X connection, not x from argument);

    auto currentStartPoint = model.startPoint;

    auto newStartPoint = Point(mk.get_row_adr(), mk.get_col_adr());

    model.startPoint = newStartPoint;

    setItemColor(currentStartPoint);
    setItemColor(newStartPoint);
    configUIMode();
}

void MainWindow::on_ramcLineEdit_textEdited(const QString &arg1)
{
    handleInputState();
}

void MainWindow::showCurrentCommand() {
//    items[]
}

void MainWindow::setItemColor(Point point) {
    if (point.isNull()) {
        return;
    }
    microcommand command = mk.rom.read(point.row, point.col);
    QTableWidgetItem* item = matrixItems[point.row][point.col];
    if (model.getMode() == running) {
        if (point == model.currentPoint) {
            item->setBackground(currentRunningColor);
        } else if (point == model.startPoint) {
            item->setBackground(startColor);
        } else if (command.empty) {
            item->setBackground(transparentColor);
        } else {
            item->setBackground(commandColor);
        }
    } else {
        if (point == model.startPoint) {
            item->setBackground(startColor);
        } else if (command.empty) {
            item->setBackground(transparentColor);
        } else {
            item->setBackground(commandColor);
        }
    }
}

void MainWindow::configUIMode() {
    if (model.getMode() == running) {
        ui->inputFrame->setEnabled(false);
        ui->startAddressFrame->setEnabled(false);
        ui->endButton->setEnabled(true);

        Point point = model.currentPoint;
        if (!point.isNull()) {
            microcommand command = mk.rom.read(point.row, point.col);
            if (command.empty) {
                setMode(editing);
                configUIMode();
            }
        }
    } else {
        ui->inputFrame->setEnabled(true);
        ui->startAddressFrame->setEnabled(true);
        ui->endButton->setEnabled(false);

        ui->saveButton->setEnabled(!model.currentPoint.isNull());
        ui->clearButton->setEnabled(!model.currentPoint.isNull());

        ui->stepButton->setEnabled(!model.startPoint.isNull());
        ui->runButton->setEnabled(!model.startPoint.isNull());
    }
}

void MainWindow::on_endButton_clicked()
{
    setMode(editing);
}

void MainWindow::setMode(Mode mode) {
    auto currentPoint = model.currentPoint;
    model.setMode(mode);
    ui->tableWidget->clearSelection();
    changeCurrentPoint(currentPoint, model.currentPoint);
    configUIMode();
}

void MainWindow::changeCurrentPoint(Point last, Point currentPoint) {
    setItemColor(last);
    setItemColor(currentPoint);
    fillInputs();
}


void MainWindow::on_fLineEdit_textEdited(const QString &arg1)
{
    handleInputState();
}



void MainWindow::on_ramWidget_cellChanged(int row, int column)
{
    mk.ram.write(row, ramItems[row]->text().toUInt());
}


void MainWindow::on_resetButton_clicked()
{
    on_endButton_clicked();
    model.currentPoint = Point::nullPoint();
    ui->rowLcd->display(model.currentPoint.row);
    ui->colLcd->display(model.currentPoint.col);
    mk.reset();
}


void MainWindow::on_save_file_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save project"),
                               "~/prog.rom",
                               tr("*.rom"));
   fm::save(filename.toStdString(), this->mk, model.startPoint.row, model.startPoint.col, MT::microcommand);
   model.current_filename = filename.toStdString();
}


void MainWindow::on_open_file_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Save project"),
                                                    "~/prog.rom",
                                                    tr("*.rom"));


    fm::programm_data data = fm::get_data(filename.toStdString());

    model.startPoint = Point(data.start_row, data.start_col);
    setItemColor(model.startPoint);
    mk = data.mk;
    for (size_t row = 0; row < 32; ++row) {
        for (size_t col = 0; col < 16; ++col) {
            setItemColor(Point(row, col));
        }
    }
    fillInputs();
    model.current_filename = filename.toStdString();
}


void MainWindow::on_save_file_triggered()
{
    if (model.current_filename.empty()) {
        QString filename = QFileDialog::getSaveFileName(this, tr("Save project"),
                                   "~/prog.rom",
                                   tr("*.rom"));
       fm::save(filename.toStdString(), this->mk, model.startPoint.row, model.startPoint.col, MT::microcommand);
       model.current_filename = filename.toStdString();
    } else {
        fm::save(model.current_filename, this->mk, model.startPoint.row, model.startPoint.col, MT::microcommand);
    }
}


void MainWindow::on_open_command_mode_triggered()
{
    CommandModeWindow* window = new CommandModeWindow();
    window->show();
     this->hide();
}

