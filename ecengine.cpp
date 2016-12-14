#include "ecengine.h"
#include <QDebug>
#include <string>
#include <sstream>
#include <windows.h>
#include <QTime>
#include <QCoreApplication>
using namespace std;

EE_DataChannel_t targetChannelList[] = {
        ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7,
        ED_P7, ED_O1, ED_O2, ED_P8, ED_T8,
        ED_FC6, ED_F4, ED_F8, ED_AF4
};

ECEngine::start(){
    while(EE_EngineConnect()!=EDK_OK){

        //QTest::qSleep(500);
        //Sleep(500);

        QTime t;
        t.start();
        while(t.elapsed()<500)
            QCoreApplication::processEvents();

        qDebug()<<"EE_EngineConnect not ready.";
    }
    //initTargetChannelList();
    eEvent = EE_EmoEngineEventCreate();
    eState = EE_EmoStateCreate();
    hData = EE_DataCreate();
    EE_DataSetBufferSizeInSec(secs*2);

    while(!readyToCollect){
        qDebug()<<"hi";
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

    while(readyToCollect){
        //string output="";
        //stringstream ss;
        //ss.Init();
        EE_DataUpdateHandle(0, hData);
        EE_DataGetNumberOfSample(hData,&nSamplesTaken);

        qDebug() << "Updated " << nSamplesTaken;

//        if (nSamplesTaken != 0) {
//            double ** buffer = new double*[14];
//            for (int i=0; i<14; i++)
//                buffer[i] = new double[nSamplesTaken];

//            EE_DataGetMultiChannels(hData, targetChannelList, 14, buffer, nSamplesTaken);

//            for (int sampleIdx=0 ; sampleIdx<(int)nSamplesTaken ; ++ sampleIdx) {
//                for (int i = 0 ; i<14 ; i++) {
//                    ss<<buffer[i][sampleIdx];
//                    output = output+" "+ss.str();
//                    //ss.Init();


//                    //qDebug() << buffer[i][sampleIdx] << ",";
//                }
//                qDebug()<<output.c_str();
//            }
//            for (int i=0; i<14; i++)
//                delete buffer[i];
//            delete buffer;
//        }
        //QTest::qSleep(500);
        QTime t;
        t.start();
        while(t.elapsed()<500)
            QCoreApplication::processEvents();
    }
}

ECEngine::ECEngine(QObject *parent):
    QObject(parent),
    userID(0),
    secs(1.0),
    datarate(0),
    readyToCollect(false),
    nSamplesTaken(0),
    userAdded(false)

{



}

ECEngine::~ECEngine(){
    delete this;
}

//bool ECEngine::initTargetChannelList(){
//    targetChannelList[] = {
//            ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7,
//            ED_P7, ED_O1, ED_O2, ED_P8, ED_T8,
//            ED_FC6, ED_F4, ED_F8, ED_AF4
//    };
//    return true;
//}
