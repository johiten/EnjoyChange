#ifndef ECENGINE_H
#define ECENGINE_H

#include <QObject>
#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"
using namespace std;

EE_DataChannel_t targetChannelList[] = {
        ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7,
        ED_P7, ED_O1, ED_O2, ED_P8, ED_T8,
        ED_FC6, ED_F4, ED_F8, ED_AF4
    };

class ECEngine : public QObject
{
    Q_OBJECT
public:
    explicit ECEngine(QObject *parent = 0);
    ~ECEngine();

signals:

public slots:


private:
    int                 batteryLevel, maxBatteryLevel;//ES_GetBatteryChargeLevel()
    int                 headsetOn;//ES_GetHeadsetOn()
    EE_SignalStrength_t wirelessSignalStatus;//ES_GetWirelessSignalStatus()
    qint32              contactQuality[20];//ES_GetContactQualityFromAllChannels()

    EmoEngineEventHandle eEvent;//EE_EmoEngineEventCreate()
    EmoStateHandle      eState;//EE_EmoStateCreate() and EE_EngineGetNextEvent()
    DataHandle          hData;//EE_DataCreate()
    EE_Event_t          eventType;//EE_EmoEngineEventGetType(eEvent);
    unsigned int        userID;//EE_DataAcquisitionEnable(userID,true);    see example 11
    float               secs;
    unsigned int        datarate;
    bool                readyToCollect;
    unsigned int        nSamplesTaken;                                     //see example 11


};

#endif // ECENGINE_H
