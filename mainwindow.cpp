#include "mainwindow.h"
#include "ecengine.h"
#include "ui_mainwindow.h"
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)//,
    //engine(new ECEngine)
{
    ui->setupUi(this);
    //engine->start();
    ///////////////////FOR TEST ONLY.
    displayDataPos1 = displayDataPos2 = displayDataTmp = 0;
    timer = new QTimer(this);
    timer->setInterval(100);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerEvent()));
    timer->start();
    //////////////////
}

MainWindow::~MainWindow()
{
    delete ui;
    delete engine;
}
void MainWindow::startEngine(){
    engine->start();
}

void MainWindow::drawCurves(QPainter &painter){
    painter.setPen(QPen(Qt::red));
    displayDataPos2 = displayDataTmp;
    displayDataTmp = displayDataPos1;
    painter.drawLine(QPoint(-4, -displayDataPos2), QPoint(0, -displayDataPos1));
}

void MainWindow::paintEvent(QPaintEvent *){
    QPainter painter(this);
    painter.setViewport(0,0,this->width(),this->height());
    painter.setWindow(QRect(-1000, -25, 1001, 26));
    drawCurves(painter);
}

void MainWindow::timerEvent(){
    scroll(-4, 0);
    displayDataPos1 = rand()%25;
}
