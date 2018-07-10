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

    void onTimerAdTimeout();
    void onTimerFileTimeout();
    void onTimerStopAqTimeout();

    //void on_pushButtonSend_clicked_OLD();

private:
    Ui::MainWindow *ui;
    i2cModule *i2comm;
    QTimer *timerAd;
    QTimer *timerFile;
    QTimer *timerStopAq;

    bool curveAquiEna;
    QFile *outFile;
    QTextStream *outFileStream;

    #define PKG_INIT 0x7E
    #define PKG_PAYLOAD_SIZE 3

    enum CMD_TYPE {START_CMD, STOP_CMD, PULSE_FREQ, PULSE_HTIME, PULSE_LTIME,
                    BURST_FREQ, BURST_HTIME, BURST_LTIME, RISE_TIME, STABLE_TIME,
                    FALL_TIME, IDLE_TIME, AMPL, PULSE_PATT};

    void start_estimulator();
    void stop_estimulator();

};

#endif // MAINWINDOW_H
