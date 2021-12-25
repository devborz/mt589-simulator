#ifndef COMMANDMODEWINDOW_H
#define COMMANDMODEWINDOW_H

#include <QMainWindow>
#include <romwindow.h>
#include <mainwindow.h>

namespace Ui {
class CommandModeWindow;
}

class CommandModeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CommandModeWindow(QWidget *parent = nullptr);
    ~CommandModeWindow();

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

private:
    Ui::CommandModeWindow *ui;

    ROMWindow* romWindow = new ROMWindow();

    std::vector<QTableWidgetItem*> items;

    MK589 mk;
};

#endif // COMMANDMODEWINDOW_H
