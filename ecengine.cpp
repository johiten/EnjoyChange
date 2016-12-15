#include "ecengine.h"
#include <QDebug>
#include <string>
#include <sstream>
#include <windows.h>
#include <QTime>
#include <QCoreApplication>
#include "spppinclude/vector.h"
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



    while(readyToCollect){
        EE_DataUpdateHandle(0, hData);
        EE_DataGetNumberOfSample(hData,&nSamplesTaken);

        qDebug() << "Updated " << nSamplesTaken;

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

        }
        mypause(SAMPLETIMEMS);
    }
}

ECEngine::ECEngine(QObject *parent):
    QObject(parent),
    userID(0),
    secs(1.0),
    datarate(0),
    readyToCollect(false),
    nSamplesTaken(0),
    userAdded(false){

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

        //QTest::qSleep(500);
        //Sleep(500);

        QTime t;
        t.start();
        while(t.elapsed()<500)
            QCoreApplication::processEvents();

        qDebug()<<"EE_EngineConnect not ready.";
    }


    eEvent = EE_EmoEngineEventCreate();
    eState = EE_EmoStateCreate();
    hData = EE_DataCreate();
    EE_DataSetBufferSizeInSec(secs*2);

    while(!readyToCollect){
        if(EE_EngineGetNextEvent(eEvent) == EDK_OK){
            eventType = EE_EmoEngineEventGetType(eEvent);
            EE_EmoEngineEventGetUserId(eEvent, &userID);

            if (eventType == EE_UserAdded) {
                qDebug() << "User added";
                EE_DataAcquisitionEnable(userID,true);
                readyToCollect = true;
            }
        }
    }
}
