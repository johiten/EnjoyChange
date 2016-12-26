#include "signalpanel.h"
#include "ui_signalpanel.h"
#include <qdebug.h>
#include "ecengine.h"
#include <QPainter>

class ECEngine;

SignalPanel::SignalPanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SignalPanel)
{
    ui->setupUi(this);
}

SignalPanel::~SignalPanel()
{
    delete ui;
}

void SignalPanel::linkEngine(ECEngine *p){
    enginePoint = p;
}

void SignalPanel::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
//    int pre = enginePoint->rawBuffer_AF3[0];
//    for(int i=1;i<enginePoint->rawBuffer_AF3.size();i++){
//        painter.drawLine(5*i,pre*3,5*i+5,3*enginePoint->rawBuffer_AF3[i]);
//        pre = enginePoint->rawBuffer_AF3[i];
//        //qDebug()<<data[i];
//    }
    //qDebug()<<"painted...";
}

void SignalPanel::myUpdate(){
    qDebug()<<"update!!....";
    update();
}
