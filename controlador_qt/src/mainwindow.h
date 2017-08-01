#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "i2cmodule.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonSend_clicked();

    void on_pushButtonOpen_clicked();

private:
    Ui::MainWindow *ui;
    i2cModule *i2comm;
};

#endif // MAINWINDOW_H
