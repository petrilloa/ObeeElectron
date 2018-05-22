// This #include statement was automatically added by the Particle IDE.
#include "lib/spark-dallas-temperature.h"

// This #include statement was automatically added by the Particle IDE.
#include "lib/OneWire.h"

/* DroneSwitch.h */
#include "Drone.h"


#ifndef _dronetemperature_h
#define _dronetemperature_h

class DroneTemperature : public Drone
{
    public:
        DroneTemperature();

        virtual void SetUpSensor(sensor);
        virtual void SetUpSensor(sensor, OneWire*);
        virtual void GetEvent(sensor_event*);
        virtual void GetSensor(sensor*);
        virtual void Publish(sensor_event*);


    private:

        sensor _sensor;
        sensor_event _sensor_event;

        float read();
        int wasActive();
        int activeFor(int ms);

        float temperature;

        DallasTemperature dallasTemp;
};

#endif
