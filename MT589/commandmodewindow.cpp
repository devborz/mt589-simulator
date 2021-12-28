#include "commandmodewindow.h"
#include "ui_commandmodewindow.h"
#include <filemanager.h>
#include <mainwindow.h>
#include <QFileDialog>
#include <QBrush>


std::string CommandModeWindow::toHex(unsigned int value) {
    std::stringstream stream;
    stream << std::hex << value;
    std::string str = stream.str();
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    while (str.size() < 4) {
        str = "0" + str;
    }
    str = "0x" + str;
    return str;
}

unsigned int CommandModeWindow::parseHex(const std::string& str) {
    std::stringstream stream;
    stream << std::hex << str;
    unsigned int value = 0;
    stream >> value;
    return value;
}

CommandModeWindow::CommandModeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CommandModeWindow)
{
    ui->setupUi(this);
    setupRegs();
//    fm::programm_data data = fm::get_data("");
    QStringList verlist;

    for (size_t i = 0; i < mk.ram.size; ++i) {
        verlist << std::to_string(i).c_str();
    }

    ui->ramWidget->setVerticalHeaderLabels(verlist);
    ui->ramWidget->setHorizontalHeaderLabels({"PROGRAM"});
    ui->ramWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    for (size_t i = 0; i < mk.ram.size; ++i) {
            QTableWidgetItem* item = new QTableWidgetItem();
            items.push_back(std::shared_ptr<QTableWidgetItem>(item));
            //item->setData(Qt::ItemDataRole::EditRole, 0);
            ui->ramWidget->setItem(i, 0, item);
    }
    loaded = true;
    PC = mk.MEM;

    on_resetButton_clicked();
}

CommandModeWindow::~CommandModeWindow()
{
    delete ui;
}

void CommandModeWindow::on_open_rom_triggered()
{

    romWindow.show();
    romWindow.mk = mk;
    romWindow.setupItems();
}


void CommandModeWindow::on_save_triggered()
{

//    if (model.current_filename.empty()) {
//        QString filename = QFileDialog::getSaveFileName(this, tr("Save project"),
//                                   "~/prog.rom",
//                                   tr("*.rom"));
//       fm::save(filename.toStdString(), this->mk, model.startPoint.row, model.startPoint.col, MT::microcommand);
//       model.current_filename = filename.toStdString();
//    } else {
//        fm::save(model.current_filename, this->mk, model.startPoint.row, model.startPoint.col, MT::microcommand);
//    }
}


void CommandModeWindow::on_save_as_triggered()
{

    //    QString filename = QFileDialog::getSaveFileName(this, tr("Save project"),
    //                               "~/prog.rom",
    //                               tr("*.rom"));
    //   fm::save(filename.toStdString(), this->mk, model.startPoint.row, model.startPoint.col, MT::microcommand);
    //   model.current_filename = filename.toStdString();
}


void CommandModeWindow::on_open_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open project"),
                                                    "~/Desktop/prog.rom",
                                                    tr("*.rom"));


    fm::programm_data data = fm::get_data(filename.toStdString());

//    model.startPoint = Point(data.start_row, data.start_col);
//    setItemColor(model.startPoint);
    mk = data.mk;
    PC = mk.MEM;
//    for (size_t row = 0; row < 32; ++row) {
//        for (size_t col = 0; col < 16; ++col) {
//            setItemColor(Point(row, col));
//        }
//    }
//    fillInputs();
//    model.current_filename = filename.toStdString();
}
void CommandModeWindow::setupRegs() {
   regLCDs.push_back(std::shared_ptr<QLCDNumber>(ui->reg0));
   regLCDs.push_back(std::shared_ptr<QLCDNumber>(ui->reg1));
   regLCDs.push_back(std::shared_ptr<QLCDNumber>(ui->reg2));
   regLCDs.push_back(std::shared_ptr<QLCDNumber>(ui->reg3));
   regLCDs.push_back(std::shared_ptr<QLCDNumber>(ui->reg4));
   regLCDs.push_back(std::shared_ptr<QLCDNumber>(ui->reg5));
   regLCDs.push_back(std::shared_ptr<QLCDNumber>(ui->reg6));
   regLCDs.push_back(std::shared_ptr<QLCDNumber>(ui->reg7));
   regLCDs.push_back(std::shared_ptr<QLCDNumber>(ui->reg8));
   regLCDs.push_back(std::shared_ptr<QLCDNumber>(ui->reg9));
}

void CommandModeWindow::update_on_cpu_data() {
  for (size_t i = 0; i < 10; ++i) {
      regLCDs[i]->display(mk.MEM[i]);
  }
  ui->regMAR->display(mk.MAR);
  ui->regT->display(mk.MEM[T]);
  ui->regAC->display(mk.MEM[AC]);

  if (mk.RO) {
      ui->ROlcd->display(mk.RO.value());
  } else {
      ui->ROlcd->display("-");
  }
  if (mk.CO) {
      ui->COlcd->display(mk.CO.value());
  } else {
      ui->COlcd->display("-");
  }
  if (mk.D) {
      ui->Dlcd->display(mk.D.value());
  } else {
      ui->Dlcd->display("-");
  }
  if (mk.A) {
      ui->Alcd->display(mk.A.value());
  } else {
      ui->Alcd->display("-");
  }
}

void CommandModeWindow::on_open_microcommand_mode_triggered()
{

    MainWindow* window = new MainWindow();
    window->mk = mk;
    window->show();
    this->hide();
    window->setupItems();
    window->fillInputs();
}


void CommandModeWindow::on_resetButton_clicked()
{
    WORD oldRow = *PC;
    mk.reset();
    mk.load(0b10100000);
    update_on_cpu_data();

    changeCurrentRow(oldRow, *PC);
}


void CommandModeWindow::on_stepButton_clicked()
{
    bool is_loadmem_prog_running = true;
    WORD oldRow = *PC;
    while (is_loadmem_prog_running) {
        size_t r = mk.get_row_adr();
        size_t c = mk.get_col_adr();
        auto command = mk.rom.read(r, c);
        if (command.empty) { return; }
        if (command.CS == 0b1 and command.RW == 0b0 and command.EA == 0b1) {
            // read
            command.M = mk.ram.read(mk.MAR);
        } else {
            command.M = 0x00;
        }

        if (command.LD == 0b1) {
            is_loadmem_prog_running = false;
        }
        mk.do_fetch_decode_execute_cycle(command);
//        if (command.CS == 0b1 and command.RW == 0b1 and mk.EA == 0b1 and mk.ED == 0b1) {
//            // write
//            items[mk.A.value()]->setText(toHex(mk.D.value()).c_str());
//        }
    }
    size_t current_row = mk.get_row_adr();
    size_t current_col = mk.get_col_adr();

    while (current_row != 0 && current_col != 10) {
        auto command = mk.rom.read(current_row, current_col);
        if (command.empty) { break; }
        if (command.CS == 0b1 and command.RW == 0b0 and command.EA == 0b1) {
            // read
            command.M = mk.ram.read(mk.MAR);
        } else {
            command.M = 0x00;
        }
        mk.do_fetch_decode_execute_cycle(command);
        if (command.CS == 0b1 and command.RW == 0b1 and mk.EA == 0b1 and mk.ED == 0b1) {
            // write
            mkwrite = true;
            items[mk.A.value()]->setText(toHex(mk.D.value()).c_str());
            mkwrite = false;
        }
        current_row = mk.get_row_adr();
        current_col = mk.get_col_adr();
    }
    WORD newRow = *PC;
    changeCurrentRow(oldRow, newRow);
    update_on_cpu_data();
}


void CommandModeWindow::on_runButton_clicked()
{

}


void CommandModeWindow::on_endButton_clicked()
{

}

WORD CommandModeWindow::parseCommand(const std::string& str) {
    if (!loaded) {
        return 0;
    }
    std::stringstream ss(str);
    std::string cmd;
    ss >> cmd;

    if (isa_commands.count(cmd) == 0) {
        return 0;
    }
    if (isa_commands[cmd] > 0xFF) {
        return isa_commands[cmd];
    }
    WORD com = isa_commands[cmd];
    std::string arg;
    ss >> arg;
    com = (com << 8) | (arg.empty() ? 0 : parseHex(arg));
    return com;
}

void CommandModeWindow::on_ramWidget_cellChanged(int row, int column) {
    std::string rowContent = items[row]->text().toStdString();
    if (rowContent.empty()) { return; }
    // (mk.EA == 0b1 and mk.ED == 0b1) {
    if (mkwrite) {
        auto data = parseHex(rowContent);
        mk.ram.write(row, data);
        return;
    }
    WORD word = 0;
    if (row < 200) {
        word = parseCommand(rowContent);
        if (!word) {
            items[row]->setText("");
        }
    } else {
        word = parseHex(rowContent);
    }
    mk.ram.write(row, word);
}

void CommandModeWindow::changeCurrentRow(WORD oldRow, WORD newRow) {
    items[oldRow]->setBackground(QBrush(Qt::transparent));
    items[newRow]->setBackground(QBrush(Qt::red));
}
