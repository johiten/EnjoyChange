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
#include <deque>
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
    userAdded(false),
    bufferCount(0){

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

    dqAF3.resize(128, 4000.0);dqAF4.resize(128, 4000.0);
    dqF3.resize(128, 4000.0);dqF4.resize(128, 4000.0);
    dqF7.resize(128, 4000.0);dqF8.resize(128, 4000.0);
    dqFC5.resize(128, 4000.0);dqFC6.resize(128, 4000.0);
    dqT7.resize(128, 4000.0);dqT8.resize(128, 4000.0);
    dqP7.resize(128, 4000.0);dqP8.resize(128, 4000.0);
    dqO1.resize(128, 4000.0);dqO2.resize(128, 4000.0);

    sumOfAF3=sumOfAF4=sumOfF3=sumOfF4=sumOfF7=sumOfF8=sumOfFC5=sumOfFC6=sumOfT7=sumOfT8=sumOfP7=sumOfP8=sumOfO1=sumOfO2=128*4000;

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
    timerDQUpdate = new QTimer(this);
    connect( timerDQUpdate, SIGNAL(timeout()), this, SLOT(updateDQ()));
    //connect( timer_1s_1, SIGNAL(timeout()), this, SLOT(updateBuffer()) );
    connect( timer_1s_1, SIGNAL(timeout()), this, SLOT(updateDeviceInfo()) );
    timerDQUpdate->start(REALTIMEUPDATETIME);
    timer_1s_1->start(SAMPLETIMEMS);

    connect( this, SIGNAL(bufferUpdated()), this, SLOT(computeFFT()));
}


void ECEngine::baseLineCorrect(){
    baseLineAF3 = sumOfAF3/128;   rawBuffer_AF3 -= baseLineAF3;
    baseLineAF4 = sumOfAF4/128;   rawBuffer_AF4 -= baseLineAF4;
    baseLineF7 = sumOfF7/128;     rawBuffer_F7 -= baseLineF7;
    baseLineF8 = sumOfF8/128;     rawBuffer_F8 -= baseLineF8;
    baseLineF3 = sumOfF3/128;     rawBuffer_F3 -= baseLineF3;
    baseLineF4 = sumOfF4/128;     rawBuffer_F4 -= baseLineF4;
    baseLineFC5 = sumOfFC5/128;   rawBuffer_FC5 -= baseLineFC5;
    baseLineFC6 = sumOfFC6/128;   rawBuffer_FC6 -= baseLineFC6;
    baseLineT7 = sumOfT7/128;     rawBuffer_T7 -= baseLineT7;
    baseLineT8 = sumOfT8/128;     rawBuffer_T8 -= baseLineT8;
    baseLineP7 = sumOfP7/128;     rawBuffer_P7 -= baseLineP7;
    baseLineP8 = sumOfP8/128;     rawBuffer_P8 -= baseLineP8;
    baseLineO1 = sumOfO1/128;     rawBuffer_O1 -= baseLineO1;
    baseLineO2 = sumOfO2/128;     rawBuffer_O2 -= baseLineO2;
}

void ECEngine::updateDQ(){
    if(readyToCollect){
        EE_DataUpdateHandle(0, hData);
        EE_DataGetNumberOfSample(hData,&nSamplesTaken);

        qDebug() << "Updated buffer: " << nSamplesTaken;

        if (nSamplesTaken > 0) {
            EE_DataGetMultiChannels(hData, targetChannelList, 14, bufferHead, nSamplesTaken);
            for(int i=0;i<nSamplesTaken;i++){
                sumOfAF3 += bufferHead[0][i];  dqAF3.push_back(bufferHead[0][i]);sumOfAF3 -=dqAF3.front();  dqAF3.pop_front();  bufferAF3x[bufferCount+i]=bufferHead[0][i];
                sumOfF7  += bufferHead[1][i];  dqF7.push_back(bufferHead[1][i]); sumOfF7  -=dqF7.front();   dqF7.pop_front();   bufferF7x[bufferCount+i]=bufferHead[1][i];
                sumOfF3  += bufferHead[2][i];  dqF3.push_back(bufferHead[2][i]); sumOfF3  -=dqF3.front();   dqF3.pop_front();   bufferF3x[bufferCount+i]=bufferHead[2][i];
                sumOfFC5 += bufferHead[3][i];  dqFC5.push_back(bufferHead[3][i]);sumOfFC5 -=dqFC5.front();  dqFC5.pop_front();  bufferFC5x[bufferCount+i]=bufferHead[3][i];
                sumOfT7  += bufferHead[4][i];  dqT7.push_back(bufferHead[4][i]); sumOfT7  -=dqT7.front();   dqT7.pop_front();   bufferT7x[bufferCount+i]=bufferHead[4][i];
                sumOfP7  += bufferHead[5][i];  dqP7.push_back(bufferHead[5][i]); sumOfP7  -=dqP7.front();   dqP7.pop_front();   bufferP7x[bufferCount+i]=bufferHead[5][i];
                sumOfO1  += bufferHead[6][i];  dqO1.push_back(bufferHead[6][i]); sumOfO1  -=dqO1.front();   dqO1.pop_front();   bufferO1x[bufferCount+i]=bufferHead[6][i];
                sumOfO2  += bufferHead[7][i];  dqO2.push_back(bufferHead[7][i]); sumOfO2  -=dqO2.front();   dqO2.pop_front();   bufferO2x[bufferCount+i]=bufferHead[7][i];
                sumOfP8  += bufferHead[8][i];  dqP8.push_back(bufferHead[8][i]); sumOfP8  -=dqP8.front();   dqP8.pop_front();   bufferP8x[bufferCount+i]=bufferHead[8][i];
                sumOfT8  += bufferHead[9][i];  dqT8.push_back(bufferHead[9][i]); sumOfT8  -=dqT8.front();   dqT8.pop_front();   bufferT8x[bufferCount+i]=bufferHead[9][i];
                sumOfFC6 += bufferHead[10][i]; dqFC6.push_back(bufferHead[10][i]);sumOfFC6-=dqFC6.front();  dqFC6.pop_front();  bufferFC6x[bufferCount+i]=bufferHead[10][i];
                sumOfF4  += bufferHead[11][i]; dqF4.push_back(bufferHead[11][i]);sumOfF4  -=dqF4.front();   dqF4.pop_front();   bufferF4x[bufferCount+i]=bufferHead[11][i];
                sumOfF8  += bufferHead[12][i]; dqF8.push_back(bufferHead[12][i]);sumOfF8  -=dqF8.front();   dqF8.pop_front();   bufferF8x[bufferCount+i]=bufferHead[12][i];
                sumOfAF4 += bufferHead[13][i]; dqAF4.push_back(bufferHead[13][i]);sumOfAF4-=dqAF4.front();  dqAF4.pop_front();  bufferAF4x[bufferCount+i]=bufferHead[13][i];


            }
            bufferCount+=nSamplesTaken;
            if(bufferCount>=128){
                bufferCount=0;
                rawBuffer_AF3 = Vector<double>(128, bufferAF3x);
                rawBuffer_F7  = Vector<double>(128, bufferF7x);
                rawBuffer_F3  = Vector<double>(128, bufferF3x);
                rawBuffer_FC5 = Vector<double>(128, bufferFC5x);
                rawBuffer_T7  = Vector<double>(128, bufferT7x);
                rawBuffer_P7  = Vector<double>(128, bufferP7x);
                rawBuffer_O1  = Vector<double>(128, bufferO1x);
                rawBuffer_O2  = Vector<double>(128, bufferO2x);
                rawBuffer_P8  = Vector<double>(128, bufferP8x);
                rawBuffer_T8  = Vector<double>(128, bufferT8x);
                rawBuffer_FC6 = Vector<double>(128, bufferFC6x);
                rawBuffer_F4  = Vector<double>(128, bufferF4x);
                rawBuffer_F8  = Vector<double>(128, bufferF8x);
                rawBuffer_AF4 = Vector<double>(128, bufferAF4x);
                baseLineCorrect();
                emit bufferUpdated();
            }
        }
    }
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
