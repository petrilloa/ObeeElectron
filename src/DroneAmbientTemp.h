
#include "lib/Adafruit_DHT.h"
/* DroneDigital.h */
#include "Drone.h"


#ifndef _droneambienttemp_h
#define _droneambienttemp_h

class DroneAmbientTemp : public Drone
{
    public:
        DroneAmbientTemp();

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

        DHT dhtSensor;
};

#endif
