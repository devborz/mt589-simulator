#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLCDNumber>
#include <ListModels.h>
#include <QTableWidgetItem>

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

    // Help functions

    void setupRegs();

    void setLCDsColor();

    void setupBoxes();

    void clearInputs();

    void update_on_cpu_data();

    void handleInputState();

    void fillInputs();

private slots:
    void on_stepButton_clicked();

    void on_runButton_clicked();

    void on_listWidget_currentRowChanged(int currentRow);

    void on_boxCPE_currentIndexChanged(int index);

    void on_boxREG_currentIndexChanged(int index);

    void on_boxFC1_currentIndexChanged(int index);

    void on_boxFC2_currentIndexChanged(int index);

    void on_boxJUMP_currentIndexChanged(int index);

    void on_commandAddressEdit_textEdited(const QString &arg1);

    void on_iLineEdit_textEdited(const QString &arg1);

    void on_kLineEdit_textEdited(const QString &arg1);

    void on_mLineEdit_textEdited(const QString &arg1);

    void on_riLineEdit_textEdited(const QString &arg1);

    void on_ciLineEdit_textEdited(const QString &arg1);

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

    void on_saveButton_clicked();

    void on_clearButton_clicked();

    void on_startAddressEdit_textEdited(const QString &arg1);

    void on_loadButton_clicked();

private:
    Ui::MainWindow *ui;

    MK589 mk;

    std::vector<QLCDNumber*> regLCDs = {};

    int selectedCommand = 0;

    Model model = Model();

    std::vector<std::vector<QTableWidgetItem*>> items;

    int startRow = -1;

    int startColumn = -1 ;

    int nextRow = -1;

    int nextCol = -1;
};

#endif // MAINWINDOW_H
