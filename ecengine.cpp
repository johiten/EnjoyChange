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

    sumOfAF3=sumOfAF4=sumOfF3=sumOfF4=sumOfF7=sumOfF8=sumOfFC5=sumOfFC6=sumOfT7=sumOfT8=sumOfP7=sumOfP8=sumOfO1=sumOfO2=0;

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

    //connect( this, SIGNAL(bufferUpdated()), this, SLOT(computeFFT()));
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
    if(readyToCollect){
        EE_DataUpdateHandle(0, hData);
        EE_DataGetNumberOfSample(hData,&nSamplesTaken);

        //qDebug() << "Updated buffer: " << nSamplesTaken;

        if (nSamplesTaken > 0) {
            EE_DataGetMultiChannels(hData, targetChannelList, 14, bufferHead, nSamplesTaken);//get data
            for(int i=0;i<nSamplesTaken;i++){
                if(bufferCount+i<=640){
                    sumOfAF3 += bufferAF3[i];sumOfAF3 -= bufferAF3x[bufferCount+i];
                    bufferAF3x[bufferCount+i+BUFFEROFFSET] = bufferAF3x[bufferCount+i] = bufferAF3[i];

                    sumOfF7 += bufferF7[i];sumOfF7 -= bufferF7x[bufferCount+i];
                    bufferF7x[bufferCount+i+BUFFEROFFSET] = bufferF7x[bufferCount+i] = bufferF7[i];

                    sumOfF3 += bufferF3[i];sumOfF3 -= bufferF3x[bufferCount+i];
                    bufferF3x[bufferCount+i+BUFFEROFFSET] = bufferF3x[bufferCount+i] = bufferF3[i];

                    sumOfFC5 += bufferFC5[i];sumOfFC5 -= bufferFC5x[bufferCount+i];
                    bufferFC5x[bufferCount+i+BUFFEROFFSET] = bufferFC5x[bufferCount+i] = bufferFC5[i];

                    sumOfT7 += bufferT7[i];sumOfT7 -= bufferT7x[bufferCount+i];
                    bufferT7x[bufferCount+i+BUFFEROFFSET] = bufferT7x[bufferCount+i] = bufferT7[i];

                    sumOfP7 += bufferP7[i];sumOfP7 -= bufferP7x[bufferCount+i];
                    bufferP7x[bufferCount+i+BUFFEROFFSET] = bufferP7x[bufferCount+i] = bufferP7[i];

                    sumOfO1 += bufferO1[i];sumOfO1 -= bufferO1x[bufferCount+i];
                    bufferO1x[bufferCount+i+BUFFEROFFSET] = bufferO1x[bufferCount+i] = bufferO1[i];

                    sumOfAF4 += bufferAF4[i];sumOfAF4 -= bufferAF4x[bufferCount+i];
                    bufferAF4x[bufferCount+i+BUFFEROFFSET] = bufferAF4x[bufferCount+i] = bufferAF4[i];

                    sumOfF8 += bufferF8[i];sumOfF8 -= bufferF8x[bufferCount+i];
                    bufferF8x[bufferCount+i+BUFFEROFFSET] = bufferF8x[bufferCount+i] = bufferF8[i];

                    sumOfF4 += bufferF4[i];sumOfF4 -= bufferF4x[bufferCount+i];
                    bufferF4x[bufferCount+i+BUFFEROFFSET] = bufferF4x[bufferCount+i] = bufferF4[i];

                    sumOfFC6 += bufferFC6[i];sumOfFC6 -= bufferFC6x[bufferCount+i];
                    bufferFC6x[bufferCount+i+BUFFEROFFSET] = bufferFC6x[bufferCount+i] = bufferFC6[i];

                    sumOfT8 += bufferT8[i];sumOfT8 -= bufferT8x[bufferCount+i];
                    bufferT8x[bufferCount+i+BUFFEROFFSET] = bufferT8x[bufferCount+i] = bufferT8[i];

                    sumOfP8 += bufferP8[i];sumOfP8 -= bufferP8x[bufferCount+i];
                    bufferP8x[bufferCount+i+BUFFEROFFSET] = bufferP8x[bufferCount+i] = bufferP8[i];

                    sumOfO2 += bufferO2[i];sumOfO2 -= bufferO2x[bufferCount+i];
                    bufferO2x[bufferCount+i+BUFFEROFFSET] = bufferO2x[bufferCount+i] = bufferO2[i];
                }
                else{
                    sumOfAF3 += bufferAF3[i];sumOfAF3 -= bufferAF3x[bufferCount+i];
                    bufferAF3x[bufferCount+i] = bufferAF3x[bufferCount+i-BUFFEROFFSET] = bufferAF3[i];

                    sumOfF7 += bufferF7[i];sumOfF7 -= bufferF7x[bufferCount+i];
                    bufferF7x[bufferCount+i] = bufferF7x[bufferCount+i-BUFFEROFFSET] = bufferF7[i];

                    sumOfF3 += bufferF3[i];sumOfF3 -= bufferF3x[bufferCount+i];
                    bufferF3x[bufferCount+i] = bufferF3x[bufferCount+i-BUFFEROFFSET] = bufferF3[i];

                    sumOfFC5 += bufferFC5[i];sumOfFC5 -= bufferFC5x[bufferCount+i];
                    bufferFC5x[bufferCount+i] = bufferFC5x[bufferCount+i-BUFFEROFFSET] = bufferFC5[i];

                    sumOfT7 += bufferT7[i];sumOfT7 -= bufferT7x[bufferCount+i];
                    bufferT7x[bufferCount+i] = bufferT7x[bufferCount+i-BUFFEROFFSET] = bufferT7[i];

                    sumOfP7 += bufferP7[i];sumOfP7 -= bufferP7x[bufferCount+i];
                    bufferP7x[bufferCount+i] = bufferP7x[bufferCount+i-BUFFEROFFSET] = bufferP7[i];

                    sumOfO1 += bufferO1[i];sumOfO1 -= bufferO1x[bufferCount+i];
                    bufferO1x[bufferCount+i] = bufferO1x[bufferCount+i-BUFFEROFFSET] = bufferO1[i];

                    sumOfAF4 += bufferAF4[i];sumOfAF4 -= bufferAF4x[bufferCount+i];
                    bufferAF4x[bufferCount+i] = bufferAF4x[bufferCount+i-BUFFEROFFSET] = bufferAF4[i];

                    sumOfF8 += bufferF8[i];sumOfF8 -= bufferF8x[bufferCount+i];
                    bufferF8x[bufferCount+i] = bufferF8x[bufferCount+i-BUFFEROFFSET] = bufferF8[i];

                    sumOfF4 += bufferF4[i];sumOfF4 -= bufferF4x[bufferCount+i];
                    bufferAF4x[bufferCount+i] = bufferAF4x[bufferCount+i-BUFFEROFFSET] = bufferF4[i];

                    sumOfFC6 += bufferFC6[i];sumOfFC6 -= bufferFC6x[bufferCount+i];
                    bufferFC6x[bufferCount+i] = bufferFC6x[bufferCount+i-BUFFEROFFSET] = bufferFC6[i];

                    sumOfT8 += bufferT8[i];sumOfT8 -= bufferT8x[bufferCount+i];
                    bufferT8x[bufferCount+i] = bufferT8x[bufferCount+i-BUFFEROFFSET] = bufferT8[i];

                    sumOfP8 += bufferP8[i];sumOfP8 -= bufferP8x[bufferCount+i];
                    bufferP8x[bufferCount+i] = bufferP8x[bufferCount+i-BUFFEROFFSET] = bufferP8[i];

                    sumOfO2 += bufferO2[i];sumOfO2 -= bufferO2x[bufferCount+i];
                    bufferO2x[bufferCount+i] = bufferO2x[bufferCount+i-BUFFEROFFSET] = bufferO2[i];
                }
            }

            int offset2=0;
            if(bufferCount<128&&bufferCount+nSamplesTaken>=128){
                offset2 = 640;
                rawBuffer_AF3 = Vector<double>(128, bufferAF3x+offset2);
                rawBuffer_F7  = Vector<double>(128, bufferF7x+offset2);
                rawBuffer_F3  = Vector<double>(128, bufferF3x+offset2);
                rawBuffer_FC5 = Vector<double>(128, bufferFC5x+offset2);
                rawBuffer_T7  = Vector<double>(128, bufferT7x+offset2);
                rawBuffer_P7  = Vector<double>(128, bufferP7x+offset2);
                rawBuffer_O1  = Vector<double>(128, bufferO1x+offset2);
                rawBuffer_O2  = Vector<double>(128, bufferO2x+offset2);
                rawBuffer_P8  = Vector<double>(128, bufferP8x+offset2);
                rawBuffer_T8  = Vector<double>(128, bufferT8x+offset2);
                rawBuffer_FC6 = Vector<double>(128, bufferFC6x+offset2);
                rawBuffer_F4  = Vector<double>(128, bufferF4x+offset2);
                rawBuffer_F8  = Vector<double>(128, bufferF8x+offset2);
                rawBuffer_AF4 = Vector<double>(128, bufferAF4x+offset2);
                computeBaseLine();
                emit buffer128();
            }
            else if(bufferCount<256&&bufferCount+nSamplesTaken>=256){
                offset2=128;
                rawBuffer_AF3 = Vector<double>(128, bufferAF3x+offset2);
                rawBuffer_F7  = Vector<double>(128, bufferF7x+offset2);
                rawBuffer_F3  = Vector<double>(128, bufferF3x+offset2);
                rawBuffer_FC5 = Vector<double>(128, bufferFC5x+offset2);
                rawBuffer_T7  = Vector<double>(128, bufferT7x+offset2);
                rawBuffer_P7  = Vector<double>(128, bufferP7x+offset2);
                rawBuffer_O1  = Vector<double>(128, bufferO1x+offset2);
                rawBuffer_O2  = Vector<double>(128, bufferO2x+offset2);
                rawBuffer_P8  = Vector<double>(128, bufferP8x+offset2);
                rawBuffer_T8  = Vector<double>(128, bufferT8x+offset2);
                rawBuffer_FC6 = Vector<double>(128, bufferFC6x+offset2);
                rawBuffer_F4  = Vector<double>(128, bufferF4x+offset2);
                rawBuffer_F8  = Vector<double>(128, bufferF8x+offset2);
                rawBuffer_AF4 = Vector<double>(128, bufferAF4x+offset2);
                computeBaseLine();
                emit buffer128();
            }
            else if(bufferCount<384&&bufferCount+nSamplesTaken>=384){
                offset2=256;
                rawBuffer_AF3 = Vector<double>(128, bufferAF3x+offset2);
                rawBuffer_F7  = Vector<double>(128, bufferF7x+offset2);
                rawBuffer_F3  = Vector<double>(128, bufferF3x+offset2);
                rawBuffer_FC5 = Vector<double>(128, bufferFC5x+offset2);
                rawBuffer_T7  = Vector<double>(128, bufferT7x+offset2);
                rawBuffer_P7  = Vector<double>(128, bufferP7x+offset2);
                rawBuffer_O1  = Vector<double>(128, bufferO1x+offset2);
                rawBuffer_O2  = Vector<double>(128, bufferO2x+offset2);
                rawBuffer_P8  = Vector<double>(128, bufferP8x+offset2);
                rawBuffer_T8  = Vector<double>(128, bufferT8x+offset2);
                rawBuffer_FC6 = Vector<double>(128, bufferFC6x+offset2);
                rawBuffer_F4  = Vector<double>(128, bufferF4x+offset2);
                rawBuffer_F8  = Vector<double>(128, bufferF8x+offset2);
                rawBuffer_AF4 = Vector<double>(128, bufferAF4x+offset2);
                computeBaseLine();
                emit buffer128();
            }
            else if(bufferCount<512&&bufferCount+nSamplesTaken>=512){
                offset2=384;
                rawBuffer_AF3 = Vector<double>(128, bufferAF3x+offset2);
                rawBuffer_F7  = Vector<double>(128, bufferF7x+offset2);
                rawBuffer_F3  = Vector<double>(128, bufferF3x+offset2);
                rawBuffer_FC5 = Vector<double>(128, bufferFC5x+offset2);
                rawBuffer_T7  = Vector<double>(128, bufferT7x+offset2);
                rawBuffer_P7  = Vector<double>(128, bufferP7x+offset2);
                rawBuffer_O1  = Vector<double>(128, bufferO1x+offset2);
                rawBuffer_O2  = Vector<double>(128, bufferO2x+offset2);
                rawBuffer_P8  = Vector<double>(128, bufferP8x+offset2);
                rawBuffer_T8  = Vector<double>(128, bufferT8x+offset2);
                rawBuffer_FC6 = Vector<double>(128, bufferFC6x+offset2);
                rawBuffer_F4  = Vector<double>(128, bufferF4x+offset2);
                rawBuffer_F8  = Vector<double>(128, bufferF8x+offset2);
                rawBuffer_AF4 = Vector<double>(128, bufferAF4x+offset2);
                computeBaseLine();
                emit buffer128();
            }
            else if(bufferCount<640&&bufferCount+nSamplesTaken>=640){
                offset2=512;
                rawBuffer_AF3 = Vector<double>(128, bufferAF3x+offset2);
                rawBuffer_F7  = Vector<double>(128, bufferF7x+offset2);
                rawBuffer_F3  = Vector<double>(128, bufferF3x+offset2);
                rawBuffer_FC5 = Vector<double>(128, bufferFC5x+offset2);
                rawBuffer_T7  = Vector<double>(128, bufferT7x+offset2);
                rawBuffer_P7  = Vector<double>(128, bufferP7x+offset2);
                rawBuffer_O1  = Vector<double>(128, bufferO1x+offset2);
                rawBuffer_O2  = Vector<double>(128, bufferO2x+offset2);
                rawBuffer_P8  = Vector<double>(128, bufferP8x+offset2);
                rawBuffer_T8  = Vector<double>(128, bufferT8x+offset2);
                rawBuffer_FC6 = Vector<double>(128, bufferFC6x+offset2);
                rawBuffer_F4  = Vector<double>(128, bufferF4x+offset2);
                rawBuffer_F8  = Vector<double>(128, bufferF8x+offset2);
                rawBuffer_AF4 = Vector<double>(128, bufferAF4x+offset2);
                computeBaseLine();
                emit buffer128();
            }
            bufferCount+=nSamplesTaken;
            emit bufferUpdated();
            if(bufferCount>=640)bufferCount=0;
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
