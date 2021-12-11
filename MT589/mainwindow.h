#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class CommandItem: public QListWidgetItem {
public:
    int f;
    int i;
    int k;
    int ci;
    int ri;
    int m;

    CommandItem(int f, int i, int k, int ci, int ri, int m) {
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
        result += std::to_string(f);
        result += "I=";
        result += std::to_string(i);
        result += "K=";
        result += std::to_string(k);
        result += "CI=";
        result += std::to_string(ci);
        result += "RI=";
        result += std::to_string(ri);
        result += "M=";
        result += std::to_string(m);
        return result;
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_stepButton_clicked();

    void on_minusButton_clicked();

    void on_plusButton_clicked();

    void on_runButton_clicked();

    void clearInputs();

private:
    Ui::MainWindow *ui;

    std::vector<CommandItem> listitems = {};
};
#endif // MAINWINDOW_H
