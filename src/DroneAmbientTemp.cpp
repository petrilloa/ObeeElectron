#include "DroneAmbientTemp.h"

DroneAmbientTemp::DroneAmbientTemp()
{
    }

void DroneAmbientTemp::SetUpSensor(sensor oSensor)
{
    _sensor.droneID = oSensor.droneID;
    _sensor.type = oSensor.type;
    _sensor.pin = oSensor.pin;
    _sensor.pin2 = oSensor.pin2;
    _sensor.buzzerNotificationID = oSensor.buzzerNotificationID;
    _sensor.rgbNotificationID = oSensor.rgbNotificationID;
    _sensor.notificationElapsedTime = oSensor.notificationElapsedTime;
    _sensor.fieldID = oSensor.fieldID;
    _sensor.mode = oSensor.mode;
    _sensor.workerID = oSensor.workerID;
    _sensor.workerElapsedTime = oSensor.workerElapsedTime;
    _sensor.oledID = oSensor.oledID;

    pinMode(_sensor.pin, INPUT);

    _sensor_event.timestamp = millis();
    _sensor_event.lastChange = _sensor_event.timestamp;
    _sensor_event.value = _sensor.inverted ? 1:0;
    _sensor_event.lastValue = _sensor_event.value;
    _sensor_event.changed = false;

    _sensor_event.acumulatedNotification = 0;

    _sensor_event.triggerPublish = false;

    //dhtSensor = new DHT(_sensor.pin, "DHT11");
    Log.info("Sensor Temp/Hum: " + String(_sensor.pin));
    Log.info("B - Sensor Temp/Hum: " + String(oSensor.pin));

    dhtSensor.setup(_sensor.pin, 11);
    dhtSensor.begin();
}

void DroneAmbientTemp::GetSensor(sensor *oSensor)
{
    /* Clear the event */
    //Serial.println("Digital Get Sensor 1");

    memset(oSensor, 0, sizeof(sensor));

    oSensor->droneID = _sensor.droneID;
    oSensor->type = _sensor.type;
    oSensor->pin = _sensor.pin;
    oSensor->pin2 = _sensor.pin2;
    oSensor->buzzerNotificationID = _sensor.buzzerNotificationID;
    oSensor->rgbNotificationID = _sensor.rgbNotificationID;
    oSensor->notificationElapsedTime = _sensor.notificationElapsedTime;
    oSensor->fieldID = _sensor.fieldID;
    oSensor->mode = _sensor.mode;
    oSensor->workerID = _sensor.workerID;
    oSensor->workerElapsedTime = _sensor.workerElapsedTime;
    oSensor->notificationFieldID = _sensor.notificationFieldID;
    oSensor->inverted = _sensor.inverted;
    oSensor->oledID = _sensor.oledID;

    //Serial.println("Digital Get Sensor 2");

}

void DroneAmbientTemp::GetEvent(sensor_event *oEvent)
{
  /* Clear the event */
  memset(oEvent, 0, sizeof(sensor_event));

  //Serial.println("GetEventTemp_1");
  //Read the sensor to get Value and Times
  float tempTemporal =  read();

  //Read error
  if (tempTemporal != -127)
    _sensor_event.value = tempTemporal;
  else
    _sensor_event.value =  _sensor_event.lastValue;

  //TODO:
  //Check notification
  if(activeFor(_sensor.notificationElapsedTime))
      _sensor_event.triggerNotification = true;
  else
      _sensor_event.triggerNotification = false;

  //Check worker
  if(_sensor.workerID != -1 && activeFor(_sensor.workerElapsedTime))
      _sensor_event.triggerWorker = true;
  else
      _sensor_event.triggerWorker = false;

  //Para Tempertura, si hay un delta de 5°, es NECESARIO Publicar!
  //Usa el valor absoluto
  if(abs(_sensor_event.value -_sensor_event.lastPublishedValue) > 5)
  {
      _sensor_event.triggerPublish = true;
  }
  else
  {
      _sensor_event.triggerPublish = false;
  }

  //Copy event
  /* Copy object to return */
  oEvent->value = _sensor_event.value;
  oEvent->lastValue = _sensor_event.lastValue;
  oEvent->acumulatedValue = _sensor_event.acumulatedValue;
  oEvent->changed = _sensor_event.changed;

  oEvent->lastPublishedValue = _sensor_event.lastPublishedValue;

  oEvent->timestamp = _sensor_event.timestamp;
  oEvent->lastChange = _sensor_event.lastChange;
  oEvent->triggerNotification = _sensor_event.triggerNotification;
  oEvent->triggerWorker = _sensor_event.triggerWorker;

  //Serial.println("_Event value: " + String(_sensor_event.value));
  //Serial.println("_Event acumulated: " + String(_sensor_event.acumulatedValue));

}

float DroneAmbientTemp::read()
{
  //Serial.println("TempRead_1");
  //Serial.println("Pin2: " + String(_sensor.pin2));
  //delay(1000);

  // Sensor readings may also be up to 2 seconds 'old' (its a
  // very slow sensor)
  float readValue = 0;

  if(_sensor.pin2 == 1)
  {
    readValue = dhtSensor.getTempCelcius();
    Log.info("Read AMBIENT-TEMP");
  }
  else if (_sensor.pin2 == 2)
  {
    readValue = dhtSensor.getHumidity();
    Log.info("Read AMBIENT-HUM");
  }


  Log.info("Temp/Hum PIN: " + String(_sensor.pin));
  Log.info("Temp/Hum VALUE: " + String(readValue));
  //Serial.println("Temperature: " + String(readValue));

  return readValue;

}

int DroneAmbientTemp::wasActive(void)
{
    return _sensor_event.value == 1 && _sensor_event.changed;
}

int DroneAmbientTemp::activeFor(int ms)
{
    return (_sensor_event.value == 1 && _sensor_event.timestamp - _sensor_event.lastChange >= ms) ? 1 : 0;
}

void DroneAmbientTemp::Publish(sensor_event *oEvent)
{
      oEvent->value = _sensor_event.value;
      //Save LastPublished
      _sensor_event.lastPublishedValue = _sensor_event.value;
}
