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
    //training(new Training),
    deviceInfoPanel(new DeviceInfoPanel)
{
    ui->setupUi(this);
    signalPanel->linkEngine(engine);
    //connect(engine->timer_1s_1,SIGNAL(timeout()),signalPanel,SLOT(update()));
/////////////////////    set layout and add widgets
    ui->gridLayout->addWidget(signalPanel, 0, 1, 2, 1);
    ui->gridLayout->addWidget(brainPowerPanel, 1, 0);
    ui->gridLayout->addWidget(deviceInfoPanel, 0, 0);
    ui->gridLayout->setColumnStretch(0,1);
    ui->gridLayout->setColumnStretch(1,5);
    ui->gridLayout->setRowStretch(0,1);
    ui->gridLayout->setRowStretch(1,1);
    //this->setLayout(ui->verticalLayout);
/////////////////////    connect signals with slots
    connect(engine->timer_1s_1, SIGNAL(timeout()), signalPanel, SLOT(update()));

}

MainWindow::~MainWindow(){
}

