#ifndef ROMWINDOW_H
#define ROMWINDOW_H

#include <QDialog>

namespace Ui {
class ROMWindow;
}

class ROMWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ROMWindow(QWidget *parent = nullptr);
    ~ROMWindow();

private:
    Ui::ROMWindow *ui;
};

#endif // ROMWINDOW_H
