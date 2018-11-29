#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
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
    void on_pushButtonStart_clicked();
    void on_pushButtonStop_clicked();
    void on_pushButtonClose_clicked();
    void on_pushButtonAd_clicked();
    void on_pushButtonCurva_clicked();
    void on_pushButtonControl_clicked();

    void onTimerAdTimeout();
    void onTimerFileTimeout();
    void onTimerStopAqTimeout();
    void onTimerControlTimeout();

    //void on_pushButtonSend_clicked_OLD();

private:
    Ui::MainWindow *ui;
    i2cModule *i2comm;
    QTimer *timerAd;
    QTimer *timerFile;
    QTimer *timerStopAq;
    QTimer *timerControl;

    bool curveAquiEna;
    bool controlEna;
    QFile *outFile;
    QTextStream *outFileStream;

    float controlGain_0;    //K*Ts / 2
    float controlGain_1;    //K*(Ts + 2Tc)/2
    float controlGain_2;    //K*(2Tc - Ts)/2

    float maxUk;

    /* x[k-1] values */
    float ref_ = 0;
    float uk_  = 0;
    float angulo_ = 0;

    #define PKG_INIT 0x7E
    #define PKG_PAYLOAD_SIZE 3

    enum CMD_TYPE {START_CMD, STOP_CMD, PULSE_FREQ, PULSE_HTIME, PULSE_LTIME,
                    BURST_FREQ, BURST_HTIME, BURST_LTIME, RISE_TIME, STABLE_TIME,
                    FALL_TIME, IDLE_TIME, AMPL, PULSE_PATT};

    void start_estimulator();
    void stop_estimulator();
    int getPos();
    void sendControl(float uk);

};

#endif // MAINWINDOW_H
