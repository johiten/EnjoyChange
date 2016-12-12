#include "ecengine.h"

ECEngine::ECEngine(QObject *parent):
    QObject(parent),
    userID(0),
    secs(1.0),
    datarate(0),
    readyToCollect(false),
    nSamplesTaken(0)

{

}
