/* oBee.h */
#include "Drone.h"
#include "DroneSwitch.h"
#include "DroneTemperature.h"
#include "DroneDigital.h"
#include "DroneAnalog.h"
#include "DroneAmbientTemp.h"

#include "oBeeRGB.h"
#include "oBeeSound.h"
#include "oBeeOled.h"
#include "Worker.h"

#include "lib/LinkedList.h"
#include "lib/SparkJson/ArduinoJson.h"

#ifndef _oBee_h
#define _oBee_h

const int PIN_Buzzer = D6;
const int PIN_RGB = D0;

const int PIN_D11 = D3;
const int PIN_D12 = D2;
const int PIN_D21 = D5;
const int PIN_D22 = D4;
const int PIN_D31 = C1;
const int PIN_D32 = C0;
const int PIN_A11 = B2;
const int PIN_A12 = B3;
const int PIN_A21 = B4;
const int PIN_A22 = B5;
const int AUX_0 = 0;
const int AUX_1 = 1;
const int AUX_2 = 2;

const int N_PIXELS = 3;

class oBee
{
    public:
        oBee();

        oBeeRGB oRGB;
        oBeeSound oSound;
        oBeeOled oOled;

        void Update();
        //void NotificationComplete();

        void SetUpDrone(JsonObject& json);
        void SetUpDroneSwitch(sensor oSensor);
        void SetUpDroneTemperature(sensor oSensor);
        void SetUpDroneDigital(sensor oSensor);
        void SetUpDroneAnalog(sensor oSensor);
        void SetUpDroneAmbientTemp(sensor oSensor);

        void ClearLists();

        LinkedList<DroneSwitch*> droneSwitchList = LinkedList<DroneSwitch*>();
        LinkedList<DroneTemperature*> droneTemperatureList = LinkedList<DroneTemperature*>();
        LinkedList<DroneDigital*> droneDigitalList = LinkedList<DroneDigital*>();
        LinkedList<DroneAnalog*> droneAnalogList = LinkedList<DroneAnalog*>();
        LinkedList<DroneAmbientTemp*> droneAmbientTempList = LinkedList<DroneAmbientTemp*>();

        //DronePresence;
        //DroneLight;
        //DroneButton; ???
        //DroneLine1;
        //DroneLine2;
        //DroneLine3;


        //DroneDistance;
        //DroneAmbientTemperature;
        //DroneAmbientHumidity
        //DroneTemperature1;
        //DroneTemperature2;
        //DroneSoilMoisture1;
        //DroneSoilMoisture2;
        //DroneRain;
        //DroneWaterLevel1;
        //DroneWaterLevel2;
        //DroneSound;
        //DroneLightLevel;
        //DroneAmp;
        //DroneVolt;
        //DroneGas;


        int GetPinValue(String strPIN);

        void SetUpWorker(JsonObject& json);
        LinkedList<Worker*> workerList = LinkedList<Worker*>();


        void HandleWorker(sensor oSensor, sensor_event oEvent);
        void HandleNotification(sensor oSensor, sensor_event oEvent);

        void RGBNotification(int id);
        void BzzrNotification(int id);

        void NotificationComplete();

        uint32_t color1, color2, lastColor;

        long msLast = 0;

    private:

        unsigned int _channelNumber;
        String writeAPIKey;
        String buzzerTallBackID;
        String rgbTallBackID;

        OneWire oneWire1;
        OneWire oneWire2;
        OneWire oneWire3;
        OneWire oneWire4;
        OneWire oneWire5;
        OneWire oneWire6;
        OneWire oneWire7;
        OneWire oneWire8;
        OneWire oneWire9;
        OneWire oneWire10;

};


#endif
