#ifndef _worker_h
#define _worker_h

#include "application.h"

typedef enum
{
    WORKER_MANUAL   = (1),
    WORKER_AUTO     = (2)
} worker_type;


class Worker
{
    public:

    Worker(){}

    int workerID; //Must be 1 to 4
    int pin;
    String tallBackID; //To determine Thingspeak TallBackID
    int value; //0 to LOW - 1 to HI DIGITAL


    int buzzerNotificationID;
    int rgbNotificationID;

    worker_type type;

    int autoTimer; //In AUTO mode defines the duration off the HIGH value. -1 manteins value

    void SetUpWorker();

    void WorkerON();
    void WorkerOFF();

    long unsigned timestamp;

    void AutoOffWorker(); //Check if must turn off worker
};

#endif
