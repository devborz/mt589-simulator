#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <emulator.hpp>
#include <QLCDNumber>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class CommandItem: public QListWidgetItem {
public:
    std::vector<BYTE> f;
    int i;
    int k;
    int ci;
    int ri;
    int m;

    CommandItem(std::vector<BYTE> f, int i, int k, int ci, int ri, int m) {
        this->f = f;
        this->i= i;
        this->k = k;
        this->ci = ci;
        this->ri = ri;
        this->m = m;
    }


    std::string prepareText()  {
        std::string result = "";
        result += "F=";
        for (const auto& fi : f) {
            result += std::to_string(fi);
        }
        result += " I=";
        result += std::to_string(i);
        result += " K=";
        result += std::to_string(k);
        result += " CI=";
        result += std::to_string(ci);
        result += " RI=";
        result += std::to_string(ri);
        result += " M=";
        result += std::to_string(m);
        return result;
    }
};

class Model {

public:
    Model() { }

    std::vector<CommandItem> listitems = {};

    size_t currentCommandIndex = 0;

    bool isExecuting = false;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void add_cpe_funcs();

    void add_fc1_funcs();

    void add_fc2_funcs();

    void add_jump_funcs();

    void clearInputs();

    void update_on_cpu_data();

private slots:
    void on_stepButton_clicked();

    void on_minusButton_clicked();

    void on_runButton_clicked();

    void on_plusButton_clicked();



private:
    Ui::MainWindow *ui;
    CPE cpe = CPE();

    std::vector<QLCDNumber*> regLCDs = {};

    std::map<std::string, int> cpe_funcs;

     std::map<std::string, int> fc1_funcs;

      std::map<std::string, int> fc2_funcs;

       std::map<std::string, int> jump_funcs;

    Model model = Model();
};

#endif // MAINWINDOW_H
