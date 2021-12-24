#include "commandmodewindow.h"
#include "ui_commandmodewindow.h"

CommandModeWindow::CommandModeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CommandModeWindow)
{
    ui->setupUi(this);
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

