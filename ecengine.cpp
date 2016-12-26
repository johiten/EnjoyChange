#include "ecengine.h"
#include <QDebug>
#include <string>
#include <sstream>
#include <windows.h>
#include <QTime>
#include <QCoreApplication>
#include "spppinclude/vector.h"
#include "spppinclude/vectormath.h"
#include "spppinclude/fft.h"
using namespace std;
using namespace splab;

EE_DataChannel_t targetChannelList[] = {
        ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7,
        ED_P7, ED_O1, ED_O2, ED_P8, ED_T8,
        ED_FC6, ED_F4, ED_F8, ED_AF4
};

void mypause(int ms){
    QTime t;
    t.start();
    while(t.elapsed()<ms)
        QCoreApplication::processEvents();
}

void ECEngine::start(){
    initEngine();
}

ECEngine::ECEngine(QObject *parent)://构造函数，初始化那几个简单的变量
    QObject(parent),
    userID(0),
    secs(1.0),
    datarate(0),
    readyToCollect(false),
    nSamplesTaken(0),
    userAdded(false){

    initEngine();
}

ECEngine::~ECEngine(){
    delete this;
}

void ECEngine::initEngine(){
    bufferHead[0]=bufferAF3;
    bufferHead[1]=bufferF7;
    bufferHead[2]=bufferF3;
    bufferHead[3]=bufferFC5;
    bufferHead[4]=bufferT7;
    bufferHead[5]=bufferP7;
    bufferHead[6]=bufferO1;
    bufferHead[7]=bufferO2;
    bufferHead[8]=bufferP8;
    bufferHead[9]=bufferT8;
    bufferHead[10]=bufferFC6;
    bufferHead[11]=bufferF4;
    bufferHead[12]=bufferF8;
    bufferHead[13]=bufferAF4;

    while(EE_EngineConnect()!=EDK_OK){
        mypause(300);
        qDebug()<<"EE_EngineConnect not ready. Connecting...";//todo:改为显示在gui中
    }

    eEvent = EE_EmoEngineEventCreate();
    eState = EE_EmoStateCreate();
    hData = EE_DataCreate();
    EE_DataSetBufferSizeInSec(secs*2);//Buffer in Emotiv

    while(!readyToCollect){
        if(EE_EngineGetNextEvent(eEvent) == EDK_OK){
            eventType = EE_EmoEngineEventGetType(eEvent);
            EE_EmoEngineEventGetUserId(eEvent, &userID);

            if (eventType == EE_UserAdded) {
                EE_DataAcquisitionEnable(userID,true);
                readyToCollect = true;
            }
        }
    }

    timer_1s_1 = new QTimer(this);
    connect( timer_1s_1, SIGNAL(timeout()), this, SLOT(updateBuffer()) );
    connect( timer_1s_1, SIGNAL(timeout()), this, SLOT(updateDeviceInfo()) );
    timer_1s_1->start(SAMPLETIMEMS);

    connect( this, SIGNAL(bufferUpdated()), this, SLOT(computeFFT()));
}


void ECEngine::baseLineCorrect(){
    baseLineAF3 = sum(rawBuffer_AF3)/128;   rawBuffer_AF3 -= baseLineAF3;
    baseLineAF4 = sum(rawBuffer_AF4)/128;   rawBuffer_AF4 -= baseLineAF4;
    baseLineF7 = sum(rawBuffer_F7)/128;     rawBuffer_F7 -= baseLineF7;
    baseLineF8 = sum(rawBuffer_F8)/128;     rawBuffer_F8 -= baseLineF8;
    baseLineF3 = sum(rawBuffer_F3)/128;     rawBuffer_F3 -= baseLineF3;
    baseLineF4 = sum(rawBuffer_F4)/128;     rawBuffer_F4 -= baseLineF4;
    baseLineFC5 = sum(rawBuffer_FC5)/128;   rawBuffer_FC5 -= baseLineFC5;
    baseLineFC6 = sum(rawBuffer_FC6)/128;   rawBuffer_FC6 -= baseLineFC6;
    baseLineT7 = sum(rawBuffer_T7)/128;     rawBuffer_T7 -= baseLineT7;
    baseLineT8 = sum(rawBuffer_T8)/128;     rawBuffer_T8 -= baseLineT8;
    baseLineP7 = sum(rawBuffer_P7)/128;     rawBuffer_P7 -= baseLineP7;
    baseLineP8 = sum(rawBuffer_P8)/128;     rawBuffer_P8 -= baseLineP8;
    baseLineO1 = sum(rawBuffer_O1)/128;     rawBuffer_O1 -= baseLineO1;
    baseLineO2 = sum(rawBuffer_O2)/128;     rawBuffer_O2 -= baseLineO2;
}

void ECEngine::updateBuffer(){
    if(readyToCollect){
        EE_DataUpdateHandle(0, hData);
        EE_DataGetNumberOfSample(hData,&nSamplesTaken);

        qDebug() << "Updated buffer: " << nSamplesTaken;

        if (nSamplesTaken != 0) {
            EE_DataGetMultiChannels(hData, targetChannelList, 14, bufferHead, nSamplesTaken);
            nSamplesTaken = 128;
            rawBuffer_AF3 = Vector<double>(nSamplesTaken, bufferHead[0]);
            rawBuffer_F7  = Vector<double>(nSamplesTaken, bufferHead[1]);
            rawBuffer_F3  = Vector<double>(nSamplesTaken, bufferHead[2]);
            rawBuffer_FC5  = Vector<double>(nSamplesTaken, bufferHead[3]);
            rawBuffer_T7  = Vector<double>(nSamplesTaken, bufferHead[4]);
            rawBuffer_P7  = Vector<double>(nSamplesTaken, bufferHead[5]);
            rawBuffer_O1  = Vector<double>(nSamplesTaken, bufferHead[6]);
            rawBuffer_O2  = Vector<double>(nSamplesTaken, bufferHead[7]);
            rawBuffer_P8  = Vector<double>(nSamplesTaken, bufferHead[8]);
            rawBuffer_T8  = Vector<double>(nSamplesTaken, bufferHead[9]);
            rawBuffer_FC6  = Vector<double>(nSamplesTaken, bufferHead[10]);
            rawBuffer_F4  = Vector<double>(nSamplesTaken, bufferHead[11]);
            rawBuffer_F8  = Vector<double>(nSamplesTaken, bufferHead[12]);
            rawBuffer_AF4  = Vector<double>(nSamplesTaken, bufferHead[13]);
            baseLineCorrect();
            emit bufferUpdated();
        }
    }
}

void ECEngine::updateDeviceInfo(){
    int state = EE_EngineGetNextEvent(eEvent);
    if (state == EDK_OK) {
        eventType = EE_EmoEngineEventGetType(eEvent);
        if(eventType == EE_EmoStateUpdated){
            EE_EmoEngineEventGetEmoState(eEvent, eState);
            ES_GetBatteryChargeLevel(eState, &batteryLevel, &maxBatteryLevel);
            headsetOn = ES_GetHeadsetOn(eState);
            wirelessSignalStatus = ES_GetWirelessSignalStatus(eState);
            ES_GetContactQualityFromAllChannels(eState, contactQuality, 14);
        }

    }
}

void ECEngine::computeFFT(){
//    qDebug()<<"---- computeFFT  ----";
//    fft_AF3 = fft(rawBuffer_AF3);//charge Emotiv..
//    qDebug()<<"compute over";
//    for(int i=0;i<5;i++)
//    qDebug()<<(double)fft_AF3[i].real()<<" "<<(double)fft_AF3[i].imag();
//    fft_AF4 = fft(rawBuffer_AF4);
//    for(int i=0;i<5;i++)
//    qDebug()<<fft_AF4[i].real()<<" "<<fft_AF4[i].imag();
//    qDebug()<<"---------";
}
