#ifndef CREATEISA_H
#define CREATEISA_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class CreateISA;
}

class CreateISA : public QDialog
{
    Q_OBJECT

public:
    explicit CreateISA(QWidget *parent = nullptr);
    ~CreateISA();

    CreateISA& operator=(const CreateISA &rhs) {
        // Guard self assignment
        if (this == &rhs)
            return *this;
        this->regsDNames = rhs.regsDNames;
        this->regsNNames = rhs.regsNNames;
        this->commandNames = rhs.commandNames;
        this->commandAddresses = rhs.commandAddresses;
        return *this;
    }

    void prepare();

private slots:
    void on_saveButton_clicked();

private:
    Ui::CreateISA *ui;

   std::vector<QTableWidgetItem*> regsDNames;

   std::vector<QTableWidgetItem*> regsNNames;

   std::vector<QTableWidgetItem*> commandNames;

   std::vector<QTableWidgetItem*> commandAddresses;
};

#endif // CREATEISA_H
