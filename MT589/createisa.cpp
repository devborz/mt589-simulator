#include "createisa.h"
#include "ui_createisa.h"
#include "filemanager.h"
#include <QFileDialog>

CreateISA::CreateISA(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateISA)
{
    ui->setupUi(this);
    prepare();

}

void CreateISA::prepare() {

    ui->regsTableWidget->setHorizontalHeaderLabels({"Default name", "New name"});
   for (size_t i = 0; i < 9; ++i) {
       QTableWidgetItem* item = new QTableWidgetItem;
       ui->regsTableWidget->setItem(i, 0, item);
       regsDNames.push_back(item);
   }
   for (size_t i = 0; i < 9; ++i) {
       QTableWidgetItem* item = new QTableWidgetItem;
       ui->regsTableWidget->setItem(i, 1, item);
       regsNNames.push_back(item);
   }

   ui->commandsTableWidget->setHorizontalHeaderLabels({"Command", "Address"});
   for (size_t i = 0; i < 31; ++i) {
       QTableWidgetItem* item = new QTableWidgetItem;
       ui->commandsTableWidget->setItem(i, 0, item);
       commandNames.push_back(item);
   }
   for (size_t i = 0; i < 31; ++i) {
       QTableWidgetItem* item = new QTableWidgetItem;
       item->setData(Qt::ItemDataRole::EditRole, 0);
       ui->commandsTableWidget->setItem(i, 1, item);
       commandAddresses.push_back(item);
   }
}

CreateISA::~CreateISA()
{
    delete ui;
}

void CreateISA::on_saveButton_clicked()
{
    fm::isa_data data;
    for (size_t i = 0; i < regsDNames.size(); ++i) {
        std::string regDName = regsDNames[i]->text().toStdString();
        std::string regNName = regsNNames[i]->text().toStdString();
        if (!regDName.empty() && !regNName.empty()) {
            data.isa_regs[regDName] = regNName;
        }
    }

    for (size_t i = 0; i < commandNames.size(); ++i) {
        std::string command = commandNames[i]->text().toStdString();
        size_t address = commandAddresses[i]->text().toUInt();
        if (!command.empty()) {
            data.isa_regs[command] = address;
        }
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("Save ISA"),
                               "~/prog.isad",
                               tr("*.isad"));
    fm::save_isa(filename.toStdString(), data);
}

