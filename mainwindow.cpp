#include "mainwindow.h"
#include "ecengine.h"
#include "ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    engine(new ECEngine),
    signalPanel(new SignalPanel),
    brainPowerPanel(new BrainPowerPanel),
    training(new Training),
    deviceInfoPanel(new DeviceInfoPanel)
{
    ui->setupUi(this);
/////////////////////    set layout and add widgets
    //ui->verticalLayout->addWidget(signalPanel);
    //this->setLayout(ui->verticalLayout);
/////////////////////    connect signals with slots
    //connect(engine->timer_1s_1, SIGNAL(timeout()), signalPanel, SLOT(update()));

}

MainWindow::~MainWindow(){
}

