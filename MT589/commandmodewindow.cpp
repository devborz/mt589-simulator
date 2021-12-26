#include "commandmodewindow.h"
#include "ui_commandmodewindow.h"
#include <filemanager.h>
#include <mainwindow.h>

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

//    fm::programm_data data = fm::get_data("");

    ui->ramWidget->setHorizontalHeaderLabels({"DATA"});
    ui->ramWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    for (size_t i = 0; i < mk.ram.size; ++i) {
            QTableWidgetItem* item = new QTableWidgetItem();
            items.push_back(item);
            item->setData(Qt::ItemDataRole::EditRole, 0);
            ui->ramWidget->setItem(i, 0, item);
    }
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
//    QString filename = QFileDialog::getOpenFileName(this, tr("Save project"),
//                                                    "~/prog.rom",
//                                                    tr("*.rom"));


//    fm::programm_data data = fm::get_data(filename.toStdString());

//    model.startPoint = Point(data.start_row, data.start_col);
//    setItemColor(model.startPoint);
//    mk = data.mk;
//    for (size_t row = 0; row < 32; ++row) {
//        for (size_t col = 0; col < 16; ++col) {
//            setItemColor(Point(row, col));
//        }
//    }
//    fillInputs();
//    model.current_filename = filename.toStdString();
}


void CommandModeWindow::on_open_microcommand_mode_triggered()
{

    MainWindow* window = new MainWindow();
    window->show();
     this->hide();
}


void CommandModeWindow::on_resetButton_clicked()
{

}


void CommandModeWindow::on_stepButton_clicked()
{

}


void CommandModeWindow::on_runButton_clicked()
{

}


void CommandModeWindow::on_endButton_clicked()
{

}


void CommandModeWindow::on_ramWidget_cellChanged(int row, int column)
{
    WORD word = parseHex(items[row]->text().toStdString());
    mk.ram.write(row + 1, word);
}

