#include "signalpanel.h"
#include "ui_signalpanel.h"
#include <qdebug.h>
#include "ecengine.h"
#include "spppinclude/vector.h"
#include "spppinclude/vectormath.h"
#include <QPainter>
#include <deque>

class ECEngine;
using namespace splab;
using namespace std;

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
    int pos = enginePoint->bufferCount-640;
    if(pos<0){
        pos+=640;
    }
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    paintOneChannel(enginePoint->bufferAF3x, enginePoint->baseLineAF3, pos, 1, painter);
    paintOneChannel(enginePoint->bufferAF4x, enginePoint->baseLineAF4, pos, 2, painter);
    paintOneChannel(enginePoint->bufferF7x, enginePoint->baseLineF7, pos, 3, painter);
    paintOneChannel(enginePoint->bufferF8x, enginePoint->baseLineF8, pos, 4, painter);

    painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));
    paintOneChannel(enginePoint->bufferF3x, enginePoint->baseLineF3, pos, 5, painter);
    paintOneChannel(enginePoint->bufferF4x, enginePoint->baseLineF4, pos, 6, painter);
    paintOneChannel(enginePoint->bufferFC5x, enginePoint->baseLineFC5, pos, 7, painter);
    paintOneChannel(enginePoint->bufferFC6x, enginePoint->baseLineFC6, pos, 8, painter);
    painter.setPen(QPen(Qt::blue, 1, Qt::SolidLine));
    paintOneChannel(enginePoint->bufferO1x, enginePoint->baseLineO1, pos, 9, painter);
    paintOneChannel(enginePoint->bufferO2x, enginePoint->baseLineO2, pos, 10, painter);
    painter.setPen(QPen(Qt::green, 1, Qt::SolidLine));
    paintOneChannel(enginePoint->bufferT7x, enginePoint->baseLineT7, pos, 11, painter);
    paintOneChannel(enginePoint->bufferT8x, enginePoint->baseLineT8, pos, 12, painter);

    painter.setPen(QPen(Qt::yellow, 1, Qt::SolidLine));
    paintOneChannel(enginePoint->bufferP7x, enginePoint->baseLineP7, pos, 13, painter);
    paintOneChannel(enginePoint->bufferP8x, enginePoint->baseLineP8, pos, 14, painter);

}

void SignalPanel::paintOneChannel(double *signal, double baseLine, int pos, int row, QPainter &painter){
    //qDebug()<<"paintOneChannel: "<<v[0]-baseLine<<"  "<<pos;

    int yOffset = row*60+20;
    int pre = signal[pos]-baseLine;//qDebug()<<pre<<"  "<<baseLine;
    for(int i=1;i<640;i++){
        painter.drawLine(2*i-2,yOffset+pre,2*i,yOffset+signal[i+pos]-baseLine);
        pre = signal[i+pos]-baseLine;
    }
}
