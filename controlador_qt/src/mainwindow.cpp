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
    int addr = ui->comboBox_channel->currentIndex();

    QString cmd;
    uint8_t cmd_raw[10];
    uint8_t i2cAddr;

#ifdef DEBUG
    std::cout << "Enviado " <<  data << " para prop: " <<
                 index << " (" << ui->comboBox->currentText().toStdString() << ")\n";
#endif

    switch (index){
        // Frequencia de pulsos
        case 0:
            cmd = "FP0" + ui->lineEdit->text();
            break;

        //Tempo em alta dos pulsos
        case 1:
            cmd = "TAP" + ui->lineEdit->text();
            break;

        //Tempo em baixa dos pulsos
        case 2:
            cmd = "TBP" + ui->lineEdit->text();
            break;

        //Frequencia de burst
        case 3:
            cmd = "FB0" + ui->lineEdit->text();
            break;

        //Tempo em alta do burst
        case 4:
            cmd = "TAB" + ui->lineEdit->text();
            break;

        //Tempo em baixa do burst
        case 5:
            cmd = "TBB" + ui->lineEdit->text();
            break;

        //Tempo de subida
        case 6:
            cmd = "TS0" + ui->lineEdit->text();
            break;

        //Tempo de plato
        case 7:
            cmd = "PL0" + ui->lineEdit->text();
            break;

        //Tempo de descida
        case 8:
            cmd = "TD0" + ui->lineEdit->text();
            break;

        //Tempo de repouso
        case 9:
            cmd = "RF0" + ui->lineEdit->text();
            break;

        //Amplitude
        case 10:
            cmd = "AMP" + ui->lineEdit->text();
            break;

        //Padrao de pulso
        case 11:
            cmd = "CP0" + ui->lineEdit->text();
            break;

        default:
            break;
    }

    std::cout << cmd.toStdString() << " tam cmd: " << cmd.size() << "\n";

    if (cmd.size() > 7){
        std::cerr << "Valor deve estar [0000,9999]\n Comando invalido\n";
        return;
    }

    QByteArray byte_data = cmd.toUtf8();
    for (int i=0; i < 7; i++){
        cmd_raw[i] = byte_data[i];
#ifdef DEBUG
        std::cout << (char)byte_data[i]  << "\n";
#endif
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

    i2comm->sendDev(cmd_raw, 7);
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
    uint8_t cmd[10] = "STR1234";
    i2comm->sendDev(cmd, 7);
}

void MainWindow::on_pushButtonStop_clicked()
{
    uint8_t cmd[10] = "STP5678";
    i2comm->sendDev(cmd, 7);
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
