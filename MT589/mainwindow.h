#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLCDNumber>
#include <ListModels.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Model {

public:
    Model() { }

    std::vector<CommandItem*> listitems = {};

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

    void on_funcList_itemActivated(QListWidgetItem *item);

    void on_funcList_itemClicked(QListWidgetItem *item);

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_listWidget_pressed(const QModelIndex &index);

    void on_listWidget_currentRowChanged(int currentRow);

    void on_funcList_currentRowChanged(int currentRow);

private:
    Ui::MainWindow *ui;
    CPE cpe = CPE();

    std::vector<QLCDNumber*> regLCDs = {};

    int selectedCommand = -1;

    Model model = Model();
};

#endif // MAINWINDOW_H
