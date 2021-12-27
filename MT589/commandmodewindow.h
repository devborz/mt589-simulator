#ifndef COMMANDMODEWINDOW_H
#define COMMANDMODEWINDOW_H

#include <QLCDNumber>
#include <QMainWindow>
#include <romwindow.h>
#include <QTableWidgetItem>
#include <emulator.hpp>
#include <sstream>

namespace Ui {
class CommandModeWindow;
}

class CommandModeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CommandModeWindow(QWidget *parent = nullptr);
    ~CommandModeWindow();

    MK589 mk;

private slots:
    void on_open_rom_triggered();

    void on_save_triggered();

    void on_save_as_triggered();

    void on_open_triggered();

    void on_open_microcommand_mode_triggered();

    void on_resetButton_clicked();

    void on_stepButton_clicked();

    void on_runButton_clicked();

    void on_endButton_clicked();

    void on_ramWidget_cellChanged(int row, int column);

    void update_on_cpu_data();

    void setupRegs();

    void changeCurrentRow(WORD oldRow, WORD newRow);

private:
    Ui::CommandModeWindow *ui;

    bool loaded = false;

    std::vector<QLCDNumber*> regLCDs = {};

    ROMWindow* romWindow = new ROMWindow();

    std::vector<QTableWidgetItem*> items;

    std::string toHex(unsigned int value);

    unsigned int parseHex(const std::string& str);

    WORD parseCommand(const std::string& str);

    std::map<std::string, WORD> isa_commands = {
        {"INCA", 0x0100},
        {"MOVA", 0x03},
        {"ADDA", 0x02},
        {"POP", 0x0500},
        {"PUSH", 0x04},
        {"IS", 0x0600},
    };
    std::map<std::string, std::string> isa_regs = {
        {"REG0", "PC"},
        {"REG1", "SP"},
    };

    WORD* PC;
};

#endif // COMMANDMODEWINDOW_H
