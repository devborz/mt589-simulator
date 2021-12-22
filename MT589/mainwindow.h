#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLCDNumber>
#include <ListModels.h>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QAbstractItemView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Address in matrix
struct Point {
    Point() {
        this->row = -1;
        this->col = -1;
    }

    Point(int row, int col) {
        this->row = row;
        this->col = col;
    }

    int row;

    int col;

    bool isNull() {
        bool isNull = (row == -1) && (col == -1);
        return isNull;
    }

    static Point nullPoint() {
        return Point();
    }

    friend bool operator== (Point &lhs, Point &rhs) {
        return (lhs.row == rhs.row) && (lhs.col == rhs.col);
    }

    friend bool operator!= (Point &lhs, Point &rhs) {
        return (lhs.row != rhs.row) || (lhs.col != rhs.col);
    }
};

// Mode
enum Mode {
    editing,
    running
};

// UI Model
class Model {

    Mode mode = editing;

public:
    Model() { }

    Mode getMode() {
        return mode;
    }

    void setMode(Mode mode) {
        this->currentPoint = Point::nullPoint();
        this->mode = mode;
    }

    Point currentPoint = Point::nullPoint();

    Point startPoint = Point::nullPoint();
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

    void showCurrentCommand();

    void setItemColor(Point point);

    void configUIMode();

    void setMode(Mode mode);

    void changeCurrentPoint(Point last, Point currentPoint);

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

    void on_ramcLineEdit_textEdited(const QString &arg1);

    void on_endButton_clicked();

    void on_tableWidget_cellClicked(int row, int column);

private:
    Ui::MainWindow *ui;

    MK589 mk;

    std::vector<QLCDNumber*> regLCDs = {};

    Model model = Model();



    std::vector<std::vector<QTableWidgetItem*>> items;

    QBrush commandColor = QBrush(Qt::blue);

    QBrush startColor = QBrush(Qt::green);

    QBrush currentRunningColor = QBrush(Qt::red);

    QBrush transparentColor = QBrush(Qt::transparent);
};

#endif // MAINWINDOW_H
