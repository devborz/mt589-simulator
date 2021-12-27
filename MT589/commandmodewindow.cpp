#include "commandmodewindow.h"
#include "ui_commandmodewindow.h"
#include <filemanager.h>
#include <mainwindow.h>

#include <QFileDialog>


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

    ui->ramWidget->setHorizontalHeaderLabels({"PROGRAM"});
    ui->ramWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    for (size_t i = 0; i < mk.ram.size; ++i) {
            QTableWidgetItem* item = new QTableWidgetItem();
            items.push_back(item);
            //item->setData(Qt::ItemDataRole::EditRole, 0);
            ui->ramWidget->setItem(i, 0, item);
    }
    loaded = true;
}

CommandModeWindow::~CommandModeWindow()
{
    delete ui;
}

void CommandModeWindow::on_open_rom_triggered()
{
    romWindow->show();
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
    QString filename = QFileDialog::getOpenFileName(this, tr("Save project"),
                                                    "~/prog.rom",
                                                    tr("*.rom"));


    fm::programm_data data = fm::get_data(filename.toStdString());

//    model.startPoint = Point(data.start_row, data.start_col);
//    setItemColor(model.startPoint);
    mk = data.mk;
//    for (size_t row = 0; row < 32; ++row) {
//        for (size_t col = 0; col < 16; ++col) {
//            setItemColor(Point(row, col));
//        }
//    }
//    fillInputs();
//    model.current_filename = filename.toStdString();
}
void CommandModeWindow::setupRegs() {
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

void CommandModeWindow::update_on_cpu_data() {
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

void CommandModeWindow::on_open_microcommand_mode_triggered()
{

    MainWindow* window = new MainWindow();
    window->show();
     this->hide();
}


void CommandModeWindow::on_resetButton_clicked()
{
    mk.reset();
    mk.load(0b10100000);
    update_on_cpu_data();
    //ui->ramWidget->cur
}


void CommandModeWindow::on_stepButton_clicked()
{
    bool is_loadmem_prog_running = true;
    while(is_loadmem_prog_running) {
        size_t r = mk.get_row_adr();
        size_t c = mk.get_col_adr();
        auto command = mk.rom.read(r, c);
        if (command.CS == 0b1 and command.RW == 0b0) {
            // read
            command.M = mk.ram.read(mk.MAR);
        } else {
            command.M = 0x00;
        }

        if (command.LD == 0b1) {
            is_loadmem_prog_running = false;
        }
        mk.do_fetch_decode_execute_cycle(command);
        if (command.CS == 0b1 and command.RW == 0b1) {
            // write
            items[mk.MAR]->setText(toHex(mk.D).c_str());
        }
    }
    size_t current_row = mk.get_row_adr();
    size_t current_col = mk.get_col_adr();
    while (current_row != 0 && current_col != 10) {
        auto command = mk.rom.read(current_row, current_col);
        if (command.CS == 0b1 and command.RW == 0b0) {
            // read
            command.M = mk.ram.read(mk.MAR);
        } else {
            command.M = 0x00;
        }

        mk.do_fetch_decode_execute_cycle(command);
        if (command.CS == 0b1 and command.RW == 0b1) {
            // write
            mk.WRITING = true; // заменить на проверку ED и EA( а их сначала нужно добавить в микрокоманду)
            items[mk.MAR]->setText(toHex(mk.D).c_str());
            mk.WRITING = false;
        }
        current_row = mk.get_row_adr();
        current_col = mk.get_col_adr();
    }
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

    if (isa_commands[cmd] > 0xFF) {
        return isa_commands[cmd];
    }
    WORD com = isa_commands[cmd];
    WORD arg;
    ss >> arg;
    com = (com << 8) | arg;
    return com;
}

void CommandModeWindow::on_ramWidget_cellChanged(int row, int column)
{
    if(mk.WRITING) {
        auto data = parseHex(items[row]->text().toStdString());
        mk.ram.write(row, data);
        return;
    }
    WORD word = parseCommand(items[row]->text().toStdString());
    mk.ram.write(row, word);
}

