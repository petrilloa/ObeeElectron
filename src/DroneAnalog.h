/* DroneAnalog.h */
#include "Drone.h"


#ifndef _droneanalog_h
#define _droneanalog_h

class DroneAnalog : public Drone
{
    public:
        DroneAnalog();

        virtual void SetUpSensor(sensor);
        virtual void GetEvent(sensor_event*);
        virtual void GetSensor(sensor*);
        virtual void Publish(sensor_event*);


    private:

        sensor _sensor;
        sensor_event _sensor_event;
        int _dbTime = 50;

        float read();
        int wasActive();
        int activeFor(int ms);


};

#endif
