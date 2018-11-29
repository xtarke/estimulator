#include <iostream>
#include <stdint.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "i2cmodule.h"
#include <QTimer>
#include <QDateTime>
#include <QFile>
#include <QDebug>

#define DEBUG

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    curveAquiEna = false;
    controlEna = false;
    ui->setupUi(this);
    i2comm = new i2cModule();

    timerAd = new QTimer(this);
    timerFile = new QTimer(this);
    timerStopAq = new QTimer(this);
    timerControl = new QTimer(this);

    outFile = new QFile(this);
    outFileStream = new QTextStream(outFile);

    connect(timerAd, SIGNAL(timeout()), this, SLOT(onTimerAdTimeout()));
    connect(timerFile, SIGNAL(timeout()), this, SLOT(onTimerFileTimeout()));
    connect(timerControl, SIGNAL(timeout()), this, SLOT(onTimerControlTimeout()));

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
        ui->pushButtonAd->setEnabled(true);
        ui->pushButtonCurva->setEnabled(true);
    }
}

void MainWindow::start_estimulator(){
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

void MainWindow::stop_estimulator(){
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

void MainWindow::on_pushButtonStart_clicked()
{

    start_estimulator();


    //    uint8_t i2cAddr = 0;
//    int addr = ui->comboBox_channel->currentIndex();

//    switch (addr) {
//    case 0:
//        i2cAddr = ui->lineEditAddr->text().toInt();
//        break;
//    case 1:
//        i2cAddr = ui->lineEditAddr_2->text().toInt();
//        break;
//    default:
//        break;
//    }

//    if (i2comm->get_i2cAddr() != i2cAddr)
//        i2comm->change_i2cAddr(i2cAddr);

//    uint8_t cmd[] = {PKG_INIT, START_CMD, 0, 0};
//    i2comm->sendDev(cmd, sizeof(cmd));
}

void MainWindow::on_pushButtonStop_clicked()
{

    stop_estimulator();


//    uint8_t i2cAddr = 0;
//    int addr = ui->comboBox_channel->currentIndex();

//    switch (addr) {
//    case 0:
//        i2cAddr = ui->lineEditAddr->text().toInt();
//        break;
//    case 1:
//        i2cAddr = ui->lineEditAddr_2->text().toInt();
//        break;
//    default:
//        break;
//    }

//    if (i2comm->get_i2cAddr() != i2cAddr)
//        i2comm->change_i2cAddr(i2cAddr);

//    uint8_t cmd[] = {PKG_INIT, STOP_CMD, 0, 0};
//    i2comm->sendDev(cmd, sizeof(cmd));
}

void MainWindow::on_pushButtonAd_clicked(){

    /* Current on/off state */
    static bool on = false;

    if (on == false){
        timerAd->setInterval(100);
        timerAd->start();
        on = true;
        ui->pushButtonAd->setChecked(true);
    }else {
        timerAd->stop();
        ui->pushButtonAd->setChecked(false);
        on = false;
    }
}

void::MainWindow::onTimerAdTimeout(){

    uint8_t my_I2cAddr = ui->lineEditAddr_AD->text().toInt();
    uint8_t curr_I2cAddr = i2comm->get_i2cAddr();

    if (curr_I2cAddr != my_I2cAddr)
        i2comm->change_i2cAddr(my_I2cAddr);

    uint8_t cmd[4] = {0x01};
    i2comm->sendReceiveDev(cmd, 1);

    ui->lcdNumber->display((double)cmd[0]);

    i2comm->change_i2cAddr(curr_I2cAddr);
}

void::MainWindow::onTimerFileTimeout(){

    uint8_t my_I2cAddr = ui->lineEditAddr_AD->text().toInt();
    uint8_t curr_I2cAddr = i2comm->get_i2cAddr();

    if (curr_I2cAddr != my_I2cAddr)
        i2comm->change_i2cAddr(my_I2cAddr);

    uint8_t cmd[4] = {0x01};
    i2comm->sendReceiveDev(cmd, 4);


    if (outFile->isOpen()){
        *outFileStream << QDateTime::currentMSecsSinceEpoch() << ";"  << (uint32_t)cmd[0] << "\n";
    }


    i2comm->change_i2cAddr(curr_I2cAddr);
}

int MainWindow::getPos(){
    uint8_t my_I2cAddr = ui->lineEditAddr_AD->text().toInt();
    uint8_t curr_I2cAddr = i2comm->get_i2cAddr();

    if (curr_I2cAddr != my_I2cAddr)
        i2comm->change_i2cAddr(my_I2cAddr);

    uint8_t cmd[4] = {0x01};
    i2comm->sendReceiveDev(cmd, 4);

    i2comm->change_i2cAddr(curr_I2cAddr);

    return (int)cmd[0];
}


void MainWindow::sendControl(float uk){
    int addr = ui->comboBox_channel->currentIndex();
    int data = (int)uk;

    uint8_t cmd_raw[] = {PKG_INIT, 0xff, 0, 0};
    uint8_t i2cAddr;

    cmd_raw[0] = PKG_INIT;
    cmd_raw[2] = (uint8_t) (data >> 8);
    cmd_raw[3] = (uint8_t) data;

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

void::MainWindow::onTimerControlTimeout(){
    /* x[k] values */
    float uk = 0;
    float angulo = 0;
    float userRef = 0;

    userRef = ui->lineEditRef->text().toFloat();
    angulo  = (float) getPos();

    uk = (userRef + ref_)*controlGain_0 - angulo*controlGain_1 + angulo_*controlGain_2 + uk_;

    if (uk < 0)
        uk = 0;
    if (uk > maxUk)
        uk = maxUk;

    //std::cout <<  "----------------------------" << std::endl;
    //std::cout <<  QDateTime::currentMSecsSinceEpoch() << ":" << angulo <<  ":" << uk << " : " << (userRef + ref_) << std::endl;
    //std::cout <<  QDateTime::currentMSecsSinceEpoch() << ":" << angulo << std::endl;

    /* Save x[k-1] values */
    uk_ = uk;
    angulo_ = angulo;
    ref_ = userRef;

    ui->lcdNumberUk->display(uk);
    ui->lcdNumber->display(angulo);
    sendControl(uk);
}

void::MainWindow::on_pushButtonControl_clicked(){
    int aqTime = ui->lineEditTs->text().toInt();

    float Ts = ui->lineEditTs->text().toFloat()/1000;
    float Tc = ui->lineEditTc->text().toFloat()/1000;
    float K = ui->lineEditK->text().toFloat();

    maxUk = ui->lineEditRefMaxUk->text().toFloat();

    controlGain_0 = K*Ts/2;           //K*Ts / 2
    controlGain_1 = K*(Ts+2*Tc)/2;    //K*(Ts + 2Tc)/2
    controlGain_2 = K*(2*Tc-Ts)/2;    //K*(2Tc - Ts)/2

    /* x[k-1] values */
    ref_ = getPos();
    uk_  = 0;
    angulo_ = ref_;

    if (!controlEna){
         std::cout << controlGain_0 << " : " << controlGain_1 << " " << controlGain_2 << std::endl;
        controlEna = true;
        timerControl->setTimerType(Qt::PreciseTimer);
        timerControl->setInterval(aqTime);
        timerControl->start();

    }else{
        timerControl->stop();
        controlEna = false;
    }

}

void::MainWindow::onTimerStopAqTimeout(){
    curveAquiEna = false;

    timerFile->stop();
    ui->pushButtonAd->setDisabled(false);
    ui->pushButtonCurva->setChecked(false);

    if (outFile->isOpen())
        outFile->close();

    stop_estimulator();

}

void::MainWindow::on_pushButtonCurva_clicked(){

    bool opened = i2comm->isOpen();

    QString fileName = QDateTime::currentDateTime().toString("ddMMyyyy-hhmmss") + ".csv";
    std::cout << "Recording: " << fileName.toStdString() << std::endl;

    if (!opened){
        std::cerr << "I2C not openend!\n";
        return;
    }

    int aqInterval = ui->lineEditAddr_AqT->text().toInt();
    int aqTime = ui->lineEditAddr_Aq->text().toInt();

    if (curveAquiEna == false){
        timerFile->setInterval(aqInterval);
        curveAquiEna = true;

        outFile->setFileName(fileName);
        outFile->open(QIODevice::WriteOnly | QIODevice::Text);

        QTimer::singleShot(aqTime*1000, this, SLOT(onTimerStopAqTimeout()));

        ui->pushButtonAd->setDisabled(true);
        ui->pushButtonCurva->setChecked(true);
        timerFile->start();


        start_estimulator();

    }else {
        timerFile->stop();
        ui->pushButtonAd->setChecked(false);
        curveAquiEna = false;

        ui->pushButtonAd->setDisabled(false);
        ui->pushButtonCurva->setChecked(false);
        outFile->close();


        stop_estimulator();

    }
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
    ui->pushButtonCurva->setEnabled(false);
    ui->pushButtonAd->setEnabled(false);
}
