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
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFEROFFSET 640
//128*5=640
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

ECEngine::ECEngine(QObject *parent)://构造函数，初始化那几个简单的变量
    QObject(parent),
    userID(0),
    secs(1.0),
    datarate(0),
    readyToCollect(false),
    nSamplesTaken(0),
    userAdded(false),
    simuMode(SIMU),
    readyToDisplay640(false),
    displayBufferCount(0),
    analyseBufferCount(0){

    initEngine();
}

ECEngine::~ECEngine(){
    delete this;
}

void ECEngine::initEngine(){

    srand((int)time(0));


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

    sumOfAF3=sumOfAF4=sumOfF3=sumOfF4=sumOfF7=sumOfF8=sumOfFC5=sumOfFC6=sumOfT7=sumOfT8=sumOfP7=sumOfP8=sumOfO1=sumOfO2=4000;

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
    timerFrameUpdate = new QTimer(this);
    connect( timerFrameUpdate, SIGNAL(timeout()), this, SLOT(updateDQ()));
    //connect( timer_1s_1, SIGNAL(timeout()), this, SLOT(updateBuffer()) );
    //connect( timer_1s_1, SIGNAL(timeout()), this, SLOT(updateDeviceInfo()) );
    timerFrameUpdate->start(REALTIMEUPDATETIME);

    //timer_1s_1->start(SAMPLETIMEMS);

    connect( this, SIGNAL(buffer256()), this, SLOT(computeFFT()));
}


void ECEngine::computeBaseLine(){
    baseLineAF3 = sumOfAF3/640;   //rawBuffer_AF3 -= baseLineAF3;
    baseLineAF4 = sumOfAF4/640;   //rawBuffer_AF4 -= baseLineAF4;
    baseLineF7 = sumOfF7/640;     //rawBuffer_F7 -= baseLineF7;
    baseLineF8 = sumOfF8/640;     //rawBuffer_F8 -= baseLineF8;
    baseLineF3 = sumOfF3/640;     //rawBuffer_F3 -= baseLineF3;
    baseLineF4 = sumOfF4/640;     //rawBuffer_F4 -= baseLineF4;
    baseLineFC5 = sumOfFC5/640;   //rawBuffer_FC5 -= baseLineFC5;
    baseLineFC6 = sumOfFC6/640;   //rawBuffer_FC6 -= baseLineFC6;
    baseLineT7 = sumOfT7/640;     //rawBuffer_T7 -= baseLineT7;
    baseLineT8 = sumOfT8/640;     //rawBuffer_T8 -= baseLineT8;
    baseLineP7 = sumOfP7/640;     //rawBuffer_P7 -= baseLineP7;
    baseLineP8 = sumOfP8/640;     //rawBuffer_P8 -= baseLineP8;
    baseLineO1 = sumOfO1/640;     //rawBuffer_O1 -= baseLineO1;
    baseLineO2 = sumOfO2/640;     //rawBuffer_O2 -= baseLineO2;
}

void ECEngine::updateDQ(){
    //simuMode here is very useful  for debuging
    if(simuMode){
        nSamplesTaken = 4;
        for(int i=0;i<nSamplesTaken;i++){
            bufferAF3[i] = 3900+(rand()%60);
            bufferAF4[i] = 3900+(rand()%60);
            bufferF7[i] = 3900+(rand()%60);
            bufferF8[i] = 3900+(rand()%60);
            bufferF3[i] = 3900+(rand()%60);
            bufferF4[i] = 3900+(rand()%60);
            bufferFC5[i] = 3900+(rand()%60);
            bufferFC6[i] = 3900+(rand()%60);
            bufferT7[i] = 3900+(rand()%60);
            bufferT8[i] = 3900+(rand()%60);
            bufferP7[i] = 3900+(rand()%60);
            bufferP8[i] = 3900+(rand()%60);
            bufferO1[i] = 3900+(rand()%60);
            bufferO2[i] = 3900+(rand()%60);
        }
    }

    if(readyToCollect){
        if(!simuMode)EE_DataUpdateHandle(0, hData);
        if(!simuMode)EE_DataGetNumberOfSample(hData,&nSamplesTaken);

        //qDebug() << "Updated buffer: " << nSamplesTaken;

        if (nSamplesTaken > 0) {
            if(!simuMode)EE_DataGetMultiChannels(hData, targetChannelList, 14, bufferHead, nSamplesTaken);//get data
            for(int i=0;i<nSamplesTaken;i++){
                if(displayBufferCount+i<=640){
                    sumOfAF3 += bufferAF3[i];sumOfAF3 -= displayBufferAF3[displayBufferCount+i];
                    displayBufferAF3[displayBufferCount+i+BUFFEROFFSET] = displayBufferAF3[displayBufferCount+i] = bufferAF3[i];

                    sumOfF7 += bufferF7[i];sumOfF7 -= displayBufferF7[displayBufferCount+i];
                    displayBufferF7[displayBufferCount+i+BUFFEROFFSET] = displayBufferF7[displayBufferCount+i] = bufferF7[i];

                    sumOfF3 += bufferF3[i];sumOfF3 -= displayBufferF3[displayBufferCount+i];
                    displayBufferF3[displayBufferCount+i+BUFFEROFFSET] = displayBufferF3[displayBufferCount+i] = bufferF3[i];

                    sumOfFC5 += bufferFC5[i];sumOfFC5 -= displayBufferFC5[displayBufferCount+i];
                    displayBufferFC5[displayBufferCount+i+BUFFEROFFSET] = displayBufferFC5[displayBufferCount+i] = bufferFC5[i];

                    sumOfT7 += bufferT7[i];sumOfT7 -= displayBufferT7[displayBufferCount+i];
                    displayBufferT7[displayBufferCount+i+BUFFEROFFSET] = displayBufferT7[displayBufferCount+i] = bufferT7[i];

                    sumOfP7 += bufferP7[i];sumOfP7 -= displayBufferP7[displayBufferCount+i];
                    displayBufferP7[displayBufferCount+i+BUFFEROFFSET] = displayBufferP7[displayBufferCount+i] = bufferP7[i];

                    sumOfO1 += bufferO1[i];sumOfO1 -= displayBufferO1[displayBufferCount+i];
                    displayBufferO1[displayBufferCount+i+BUFFEROFFSET] = displayBufferO1[displayBufferCount+i] = bufferO1[i];

                    sumOfAF4 += bufferAF4[i];sumOfAF4 -= displayBufferAF4[displayBufferCount+i];
                    displayBufferAF4[displayBufferCount+i+BUFFEROFFSET] = displayBufferAF4[displayBufferCount+i] = bufferAF4[i];

                    sumOfF8 += bufferF8[i];sumOfF8 -= displayBufferF8[displayBufferCount+i];
                    displayBufferF8[displayBufferCount+i+BUFFEROFFSET] = displayBufferF8[displayBufferCount+i] = bufferF8[i];

                    sumOfF4 += bufferF4[i];sumOfF4 -= displayBufferF4[displayBufferCount+i];
                    displayBufferF4[displayBufferCount+i+BUFFEROFFSET] = displayBufferF4[displayBufferCount+i] = bufferF4[i];

                    sumOfFC6 += bufferFC6[i];sumOfFC6 -= displayBufferFC6[displayBufferCount+i];
                    displayBufferFC6[displayBufferCount+i+BUFFEROFFSET] = displayBufferFC6[displayBufferCount+i] = bufferFC6[i];

                    sumOfT8 += bufferT8[i];sumOfT8 -= displayBufferT8[displayBufferCount+i];
                    displayBufferT8[displayBufferCount+i+BUFFEROFFSET] = displayBufferT8[displayBufferCount+i] = bufferT8[i];

                    sumOfP8 += bufferP8[i];sumOfP8 -= displayBufferP8[displayBufferCount+i];
                    displayBufferP8[displayBufferCount+i+BUFFEROFFSET] = displayBufferP8[displayBufferCount+i] = bufferP8[i];

                    sumOfO2 += bufferO2[i];sumOfO2 -= displayBufferO2[displayBufferCount+i];
                    displayBufferO2[displayBufferCount+i+BUFFEROFFSET] = displayBufferO2[displayBufferCount+i] = bufferO2[i];
                }
                else{
                    sumOfAF3 += bufferAF3[i];sumOfAF3 -= displayBufferAF3[displayBufferCount+i];
                    displayBufferAF3[displayBufferCount+i] = displayBufferAF3[displayBufferCount+i-BUFFEROFFSET] = bufferAF3[i];

                    sumOfF7 += bufferF7[i];sumOfF7 -= displayBufferF7[displayBufferCount+i];
                    displayBufferF7[displayBufferCount+i] = displayBufferF7[displayBufferCount+i-BUFFEROFFSET] = bufferF7[i];

                    sumOfF3 += bufferF3[i];sumOfF3 -= displayBufferF3[displayBufferCount+i];
                    displayBufferF3[displayBufferCount+i] = displayBufferF3[displayBufferCount+i-BUFFEROFFSET] = bufferF3[i];

                    sumOfFC5 += bufferFC5[i];sumOfFC5 -= displayBufferFC5[displayBufferCount+i];
                    displayBufferFC5[displayBufferCount+i] = displayBufferFC5[displayBufferCount+i-BUFFEROFFSET] = bufferFC5[i];

                    sumOfT7 += bufferT7[i];sumOfT7 -= displayBufferT7[displayBufferCount+i];
                    displayBufferT7[displayBufferCount+i] = displayBufferT7[displayBufferCount+i-BUFFEROFFSET] = bufferT7[i];

                    sumOfP7 += bufferP7[i];sumOfP7 -= displayBufferP7[displayBufferCount+i];
                    displayBufferP7[displayBufferCount+i] = displayBufferP7[displayBufferCount+i-BUFFEROFFSET] = bufferP7[i];

                    sumOfO1 += bufferO1[i];sumOfO1 -= displayBufferO1[displayBufferCount+i];
                    displayBufferO1[displayBufferCount+i] = displayBufferO1[displayBufferCount+i-BUFFEROFFSET] = bufferO1[i];

                    sumOfAF4 += bufferAF4[i];sumOfAF4 -= displayBufferAF4[displayBufferCount+i];
                    displayBufferAF4[displayBufferCount+i] = displayBufferAF4[displayBufferCount+i-BUFFEROFFSET] = bufferAF4[i];

                    sumOfF8 += bufferF8[i];sumOfF8 -= displayBufferF8[displayBufferCount+i];
                    displayBufferF8[displayBufferCount+i] = displayBufferF8[displayBufferCount+i-BUFFEROFFSET] = bufferF8[i];

                    sumOfF4 += bufferF4[i];sumOfF4 -= displayBufferF4[displayBufferCount+i];
                    displayBufferAF4[displayBufferCount+i] = displayBufferAF4[displayBufferCount+i-BUFFEROFFSET] = bufferF4[i];

                    sumOfFC6 += bufferFC6[i];sumOfFC6 -= displayBufferFC6[displayBufferCount+i];
                    displayBufferFC6[displayBufferCount+i] = displayBufferFC6[displayBufferCount+i-BUFFEROFFSET] = bufferFC6[i];

                    sumOfT8 += bufferT8[i];sumOfT8 -= displayBufferT8[displayBufferCount+i];
                    displayBufferT8[displayBufferCount+i] = displayBufferT8[displayBufferCount+i-BUFFEROFFSET] = bufferT8[i];

                    sumOfP8 += bufferP8[i];sumOfP8 -= displayBufferP8[displayBufferCount+i];
                    displayBufferP8[displayBufferCount+i] = displayBufferP8[displayBufferCount+i-BUFFEROFFSET] = bufferP8[i];

                    sumOfO2 += bufferO2[i];sumOfO2 -= displayBufferO2[displayBufferCount+i];
                    displayBufferO2[displayBufferCount+i] = displayBufferO2[displayBufferCount+i-BUFFEROFFSET] = bufferO2[i];
                }
            }
            computeBaseLine();

            for(int i=0;i<nSamplesTaken;i++){//copy bufferXX to analyseBufferXX
                analyseBufferAF3[analyseBufferCount+i]=bufferAF3[i];
                analyseBufferAF4[analyseBufferCount+i]=bufferAF4[i];
                analyseBufferF7[analyseBufferCount+i]=bufferF7[i];
                analyseBufferF8[analyseBufferCount+i]=bufferF8[i];
                analyseBufferF3[analyseBufferCount+i]=bufferF3[i];
                analyseBufferF4[analyseBufferCount+i]=bufferF4[i];
                analyseBufferFC5[analyseBufferCount+i]=bufferFC5[i];
                analyseBufferFC6[analyseBufferCount+i]=bufferFC6[i];
                analyseBufferT7[analyseBufferCount+i]=bufferT7[i];
                analyseBufferT8[analyseBufferCount+i]=bufferT8[i];
                analyseBufferP7[analyseBufferCount+i]=bufferP7[i];
                analyseBufferP8[analyseBufferCount+i]=bufferP8[i];
                analyseBufferO1[analyseBufferCount+i]=bufferO1[i];
                analyseBufferO2[analyseBufferCount+i]=bufferO2[i];
            }
            analyseBufferCount+=nSamplesTaken;
            if(analyseBufferCount>=255){//if 256 samples were got, emit buffer256
                analyseBufferCount=0;
                rawBuffer_AF3 = Vector<double>(256, analyseBufferAF3);
                rawBuffer_F7  = Vector<double>(256, analyseBufferF7);
                rawBuffer_F3  = Vector<double>(256, analyseBufferF3);
                rawBuffer_FC5 = Vector<double>(256, analyseBufferFC5);
                rawBuffer_T7  = Vector<double>(256, analyseBufferT7);
                rawBuffer_P7  = Vector<double>(256, analyseBufferP7);
                rawBuffer_O1  = Vector<double>(256, analyseBufferO1);
                rawBuffer_O2  = Vector<double>(256, analyseBufferO2);
                rawBuffer_P8  = Vector<double>(256, analyseBufferP8);
                rawBuffer_T8  = Vector<double>(256, analyseBufferT8);
                rawBuffer_FC6 = Vector<double>(256, analyseBufferFC6);
                rawBuffer_F4  = Vector<double>(256, analyseBufferF4);
                rawBuffer_F8  = Vector<double>(256, analyseBufferF8);
                rawBuffer_AF4 = Vector<double>(256, analyseBufferAF4);
                emit buffer256();
            }

            displayBufferCount+=nSamplesTaken;
            if(readyToDisplay640==false && displayBufferCount>=640){
                readyToDisplay640 = true;
            }
            if(readyToDisplay640) emit bufferUpdated();
            if(displayBufferCount>=640)displayBufferCount=0;
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
    qDebug()<<"---- computeFFT  ----";
    fft_AF3 = fft(rawBuffer_AF3);
    int deltaPower,thetaPower,alphaPower,betaPower,gammaPower;
    deltaPower=thetaPower=alphaPower=betaPower=gammaPower=0;
    for(int i=1;i<=7;i++){//power of delta band, 0.5~3.5 Hz
        deltaPower+=(fft_AF3[i].imag()*fft_AF3[i].imag()+fft_AF3[i].real()*fft_AF3[i].real());
    }
    for(int i=8;i<=15;i++){//power of theta band, 4~7.5 Hz
        thetaPower+=(fft_AF3[i].imag()*fft_AF3[i].imag()+fft_AF3[i].real()*fft_AF3[i].real());
    }
    for(int i=16;i<=26;i++){//power of alpha band, 8~13 Hz
        alphaPower+=(fft_AF3[i].imag()*fft_AF3[i].imag()+fft_AF3[i].real()*fft_AF3[i].real());
    }
    for(int i=28;i<=52;i++){//power of beta band, 14~26
        betaPower+=(fft_AF3[i].imag()*fft_AF3[i].imag()+fft_AF3[i].real()*fft_AF3[i].real());
    }
    for(int i=60;i<=120;i++){//power of gamma band, 30~60
        gammaPower+=(fft_AF3[i].imag()*fft_AF3[i].imag()+fft_AF3[i].real()*fft_AF3[i].real());
    }
    qDebug()<<" Delta "<<deltaPower<<" Theta "<<thetaPower<<" Alpha "<<alphaPower<<" Beta "<<betaPower<<" Gamma "<<gammaPower;

    qDebug()<<"---------";
}
