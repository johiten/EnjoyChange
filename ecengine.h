#ifndef ECENGINE_H
#define ECENGINE_H

#include <QObject>
#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"
#include "spppinclude/vector.h"
#include "complex.h"
#include <QTimer>
using namespace std;
using namespace splab;

#define SAMPLETIMEMS 1000
//采样时间固定为1s,能保证0~64Hz中1Hz的分辨率。
#define REALTIMEUPDATETIME 33
//30fps is good enough for this application, so every 33 ms checks the update

class ECEngine : public QObject
{
    Q_OBJECT
public:
    explicit ECEngine(QObject *parent = 0);
    ~ECEngine();

    void initEngine();//initialize member variables
    void computeBaseLine();


signals:
    void bufferUpdated();
    void buffer128();

public slots:
    //void updateBuffer();//SLOT
    void updateDQ();
    void updateDeviceInfo();//battery and other infoes.
    void computeFFT();

private:
    int                 batteryLevel, maxBatteryLevel;//ES_GetBatteryChargeLevel()
    int                 headsetOn;//ES_GetHeadsetOn()
    EE_SignalStrength_t wirelessSignalStatus;//ES_GetWirelessSignalStatus()
    EE_EEG_ContactQuality_t contactQuality[20];//ES_GetContactQualityFromAllChannels()

    EmoEngineEventHandle eEvent;//EE_EmoEngineEventCreate()
    EmoStateHandle      eState;//EE_EmoStateCreate()
    DataHandle          hData;//EE_DataCreate()
    EE_Event_t          eventType;//EE_EmoEngineEventGetType(eEvent);
    unsigned int        userID;//EE_DataAcquisitionEnable(userID,true);    see example 11
    float               secs;
    unsigned int        datarate;
    bool                readyToCollect;
    unsigned int        nSamplesTaken;                                     //see example 11
    bool                userAdded;
    double *            bufferHead[16];
    double  bufferAF3[520],bufferF7[520],bufferF3[520],bufferFC5[520],bufferT7[520],bufferP7[520],bufferO1[520],
            bufferAF4[520],bufferF8[520],bufferF4[520],bufferFC6[520],bufferT8[520],bufferP8[520],bufferO2[520];
    double sumOfAF3, sumOfAF4, sumOfF3, sumOfF4, sumOfF7, sumOfF8, sumOfFC5, sumOfFC6, sumOfT7, sumOfT8, sumOfP7, sumOfP8, sumOfO1, sumOfO2;//for baselineCorrecting

public:
    Vector<double>      rawBuffer_AF3;
    Vector<complex<double> >  fft_AF3;
    Vector<double>      rawBuffer_F7;
    Vector<complex<double> >  fft_F7;
    Vector<double>      rawBuffer_F3;
    Vector<complex<double> >  fft_F3;
    Vector<double>      rawBuffer_FC5;
    Vector<complex<double> >  fft_FC5;
    Vector<double>      rawBuffer_T7;
    Vector<complex<double> >  fft_T7;
    Vector<double>      rawBuffer_P7;
    Vector<complex<double> >  fft_P7;
    Vector<double>      rawBuffer_O1;
    Vector<complex<double> >  fft_O1;
    Vector<double>      rawBuffer_O2;
    Vector<complex<double> >  fft_O2;
    Vector<double>      rawBuffer_P8;
    Vector<complex<double> >  fft_P8;
    Vector<double>      rawBuffer_T8;
    Vector<complex<double> >  fft_T8;
    Vector<double>      rawBuffer_FC6;
    Vector<complex<double> >  fft_FC6;
    Vector<double>      rawBuffer_F4;
    Vector<complex<double> >  fft_F4;
    Vector<double>      rawBuffer_F8;
    Vector<complex<double> >  fft_F8;
    Vector<double>      rawBuffer_AF4;
    Vector<complex<double> >  fft_AF4;

    QTimer *timer_1s_1;
    QTimer *timerFrameUpdate;

    double baseLineAF3, baseLineAF4,
        baseLineF7,baseLineF8,
        baseLineF3,baseLineF4,
        baseLineFC5,baseLineFC6,
        baseLineT7,baseLineT8,
        baseLineP7,baseLineP8,
        baseLineO1,baseLineO2;

    int bufferCount;
    double  bufferAF3x[1300],bufferF7x[1300],bufferF3x[1300],bufferFC5x[1300],bufferT7x[1300],bufferP7x[1300],bufferO1x[1300],
            bufferAF4x[1300],bufferF8x[1300],bufferF4x[1300],bufferFC6x[1300],bufferT8x[1300],bufferP8x[1300],bufferO2x[1300];
};

#endif // ECENGINE_H
