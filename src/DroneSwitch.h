/* DroneSwitch.h */
#include "Drone.h"


#ifndef _droneswitch_h
#define _droneswitch_h

class DroneSwitch : public Drone
{
    public:
        DroneSwitch();

        void setDeBounceTime(int dbTime); //Specific for this sensor

        virtual void SetUpSensor(sensor);
        virtual void GetEvent(sensor_event*);
        virtual void GetSensor(sensor*);
        virtual void Publish(sensor_event*);
        

    private:

        sensor _sensor;
        sensor_event _sensor_event;
        int _dbTime = 120;               //Default debounce time in miliSeconds

        float read();
        int wasActive();
        int activeFor(int ms);


};

#endif
