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
#define SIMU false

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
    void buffer256();

public slots:
    //void updateBuffer();//SLOT
    void updateDQ();
    void updateDeviceInfo();//battery and other infoes.
    void computeFFT();

private:

    bool simuMode;      //if simuMode is true, using simulator data.
    bool readyToDisplay640;

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
    double  displayBufferAF3[1300],displayBufferF7[1300],displayBufferF3[1300],displayBufferFC5[1300],displayBufferT7[1300],displayBufferP7[1300],displayBufferO1[1300],
            displayBufferAF4[1300],displayBufferF8[1300],displayBufferF4[1300],displayBufferFC6[1300],displayBufferT8[1300],displayBufferP8[1300],displayBufferO2[1300];
    double  analyseBufferAF3[300],analyseBufferF7[300],analyseBufferF3[300],analyseBufferFC5[300],analyseBufferT7[300],analyseBufferP7[300],analyseBufferO1[300],
            analyseBufferAF4[300],analyseBufferF8[300],analyseBufferF4[300],analyseBufferFC6[300],analyseBufferT8[300],analyseBufferP8[300],analyseBufferO2[300];
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

    int displayBufferCount, analyseBufferCount;

};

#endif // ECENGINE_H
