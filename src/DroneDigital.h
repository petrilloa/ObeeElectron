/* DroneDigital.h */
#include "Drone.h"


#ifndef _dronedigital_h
#define _dronedigital_h

class DroneDigital : public Drone
{
    public:
        DroneDigital();

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
