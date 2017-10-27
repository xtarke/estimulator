#include <iostream>
#include <stdint.h>
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
    int addr = ui->comboBox_channel->currentIndex();

    uint8_t cmd_raw[] = {PKG_INIT, 0xff, 0, 0};
    uint8_t i2cAddr;

#ifdef DEBUG
    std::cout << "Enviado " <<  data << " para prop: " <<
                 index << " (" << ui->comboBox->currentText().toStdString() << ")\n";
#endif

    cmd_raw[0] = PKG_INIT;
    cmd_raw[2] = (uint8_t) (data >> 8);
    cmd_raw[3] = (uint8_t) data;

    switch (index){
        // Frequencia de pulsos
        case 0:
            cmd_raw[1] = PULSE_FREQ;
            break;

        //Tempo em alta dos pulsos
        case 1:
            cmd_raw[1] = PULSE_HTIME;
            break;

        //Tempo em baixa dos pulsos
        case 2:
            cmd_raw[1] = PULSE_LTIME;
            break;

        //Frequencia de burst
        case 3:
            cmd_raw[1] = BURST_FREQ;
            break;

        //Tempo em alta do burst
        case 4:
            cmd_raw[1] = BURST_HTIME;
            break;

        //Tempo em baixa do burst
        case 5:
            cmd_raw[1] = BURST_LTIME;
            break;

        //Tempo de subida
        case 6:
            cmd_raw[1] = RISE_TIME;
            break;

        //Tempo de plato
        case 7:
            cmd_raw[1] = STABLE_TIME;
            break;

        //Tempo de descida
        case 8:
            cmd_raw[1] = FALL_TIME;
            break;

        //Tempo de repouso
        case 9:
            cmd_raw[1] = IDLE_TIME;
            break;

        //Amplitude
        case 10:
            cmd_raw[1] = AMPL;
            break;

        //Padrao de pulso
        case 11:
            cmd_raw[1] = PULSE_PATT;
            break;

        default:
            break;
    }

    switch (addr) {
    case 0:
        i2cAddr = ui->lineEditAddr->text().toInt();
        break;
    case 1:
        i2cAddr = ui->lineEditAddr_2->text().toInt();
        break;
    default:
        break;
    }

    if (i2comm->get_i2cAddr() != i2cAddr)
        i2comm->change_i2cAddr(i2cAddr);

    i2comm->sendDev(cmd_raw, sizeof(cmd_raw));
}



void MainWindow::on_pushButtonOpen_clicked()
{
    uint8_t addr = ui->lineEditAddr->text().toInt();

    int ret = i2comm->openDev(addr);

    if (ret >= 0){
        ui->pushButtonSend->setEnabled(true);
        ui->pushButtonStart->setEnabled(true);
        ui->pushButtonStop->setEnabled(true);
        ui->pushButtonOpen->setEnabled(false);
        ui->pushButtonClose->setEnabled(true);
    }
}

void MainWindow::on_pushButtonStart_clicked()
{
    uint8_t i2cAddr = 0;
    int addr = ui->comboBox_channel->currentIndex();

    switch (addr) {
    case 0:
        i2cAddr = ui->lineEditAddr->text().toInt();
        break;
    case 1:
        i2cAddr = ui->lineEditAddr_2->text().toInt();
        break;
    default:
        break;
    }

    if (i2comm->get_i2cAddr() != i2cAddr)
        i2comm->change_i2cAddr(i2cAddr);

    uint8_t cmd[] = {PKG_INIT, START_CMD, 0, 0};
    i2comm->sendDev(cmd, sizeof(cmd));
}

void MainWindow::on_pushButtonStop_clicked()
{
    uint8_t i2cAddr = 0;
    int addr = ui->comboBox_channel->currentIndex();

    switch (addr) {
    case 0:
        i2cAddr = ui->lineEditAddr->text().toInt();
        break;
    case 1:
        i2cAddr = ui->lineEditAddr_2->text().toInt();
        break;
    default:
        break;
    }

    if (i2comm->get_i2cAddr() != i2cAddr)
        i2comm->change_i2cAddr(i2cAddr);

    uint8_t cmd[] = {PKG_INIT, STOP_CMD, 0, 0};
    i2comm->sendDev(cmd, sizeof(cmd));
}

void MainWindow::on_pushButtonClose_clicked()
{
    int ret = i2comm->closeDev();

    if (ret !=0 )
        std::cerr << "Error closing connection\n";

    ui->pushButtonSend->setEnabled(false);
    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonStop->setEnabled(false);
    ui->pushButtonOpen->setEnabled(true);
    ui->pushButtonClose->setEnabled(false);
}
