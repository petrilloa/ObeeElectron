#include "Worker.h"

void Worker::SetUpWorker()
{
    pinMode(pin, OUTPUT);

    //LOW to turn ON the RELAY
    WorkerOFF();
}

void Worker::WorkerON()
{
    //Serial.println("WorkerON:" + String(workerID));
    value = HIGH;

    //Worker ON TIME
    timestamp = millis();

    //LOW to turn ON the RELAY
    digitalWrite(pin, LOW);
}

void Worker::WorkerOFF()
{
    //Serial.println("WorkerOFF:" + String(workerID));
    value = LOW;

    //LOW to turn ON the RELAY
    digitalWrite(pin, HIGH);
}

void Worker::AutoOffWorker()
{
    if(autoTimer != -1 && millis()-timestamp > autoTimer)
        WorkerOFF();

}
