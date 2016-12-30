#include "signalpanel.h"
#include "ui_signalpanel.h"
#include <qdebug.h>
#include "ecengine.h"
#include "spppinclude/vector.h"
#include "spppinclude/vectormath.h"
#include <QPainter>

class ECEngine;
using namespace splab;

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
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    paintOneChannel(enginePoint->rawBuffer_AF3, enginePoint->baseLineAF3, 1, painter);

    painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));
    paintOneChannel(enginePoint->rawBuffer_AF4, enginePoint->baseLineAF4, 2, painter);

    painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    paintOneChannel(enginePoint->rawBuffer_O1, enginePoint->baseLineO1, 3, painter);

    painter.setPen(QPen(Qt::green, 1, Qt::SolidLine));
    paintOneChannel(enginePoint->rawBuffer_O2, enginePoint->baseLineO2, 4, painter);

    painter.setPen(QPen(Qt::yellow, 1, Qt::SolidLine));
    paintOneChannel(enginePoint->rawBuffer_T7, enginePoint->baseLineT7, 5, painter);

}

void SignalPanel::paintOneChannel(Vector<double> v, double baseLine, int pos, QPainter &painter){
    if(v.size()==0)return;
    int yOffset = pos*100;
    int pre = v[0]-baseLine;
    for(int i=1;i<v.size();i++){
        painter.drawLine(15*i-15,yOffset+pre*3,15*i,yOffset+3*(v[i]-baseLine));
        pre = v[i]-baseLine;
    }
}
