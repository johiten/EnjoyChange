#include "mainwindow.h"
#include "ecengine.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    engine(new ECEngine)
{
    ui->setupUi(this);
    //engine->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete engine;
}
void MainWindow::startEngine(){
    engine->start();
}
