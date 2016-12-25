#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ecengine.h"
#include "signalpanel.h"
#include "brainpowerpanel.h"
#include "deviceinfopanel.h"
#include "training.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);//construct all parts of the app.
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ECEngine *engine;//数据获取、数据处理、计时功能，不包含gui
    SignalPanel *signalPanel;//show raw EEG and fft results. //GUI
    BrainPowerPanel *brainPowerPanel;//show brain power on a brain map. //GUI
    DeviceInfoPanel *deviceInfoPanel;//show basic device info. //GUI
    Training *training;//设定训练目标和训练时间，计算分数，显示分数. //a dialog for setting, compute data and show it. //GUI and Logic
};

#endif // MAINWINDOW_H
