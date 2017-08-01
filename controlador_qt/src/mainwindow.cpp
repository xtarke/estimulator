#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "i2cmodule.h"

#define DEBUG

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    i2comm = new i2cModule();
}

MainWindow::~MainWindow()
{
    delete i2comm;
    delete ui;
}

void MainWindow::on_pushButtonSend_clicked()
{
    int index = ui->comboBox->currentIndex();
    int data = ui->lineEdit->text().toInt();

#ifdef DEBUG
    std::cout << "Enviado " <<  data << " para prop: " << index << "\n";
#endif

}

void MainWindow::on_pushButtonOpen_clicked()
{
    uint8_t addr = ui->lineEditAddr->text().toInt();

    int ret = i2comm->openDev(addr);

    if (ret > 0){
        ui->pushButtonSend->setEnabled(true);
        ui->pushButtonStart->setEnabled(true);
        ui->pushButtonStop->setEnabled(true);
    }

}
