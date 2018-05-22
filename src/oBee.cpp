#include "oBee.h"

oBee::oBee()
{
    //oRGB();
    //oRGB(N_PIXELS, PIN_RGB, WS2812);
    //oRGB(3, 1, WS2812);
    //oRGB.Setup(N_PIXELS, PIN_RGB);
    oRGB.begin();
    oRGB.Repeat = true;

    //Default anim
    oRGB.RainbowCycle(5);
    //color1 = oRGB.Color(255,0,0);
    //color2 = oRGB.Color(0,0,255);

    //oRGB.Scanner(color1, 100);
    //oRGB.TheaterChase(color1, color2, 100, FORWARD);

    oSound.pin = PIN_Buzzer;
    char *song = oSound.song_startup;
    oSound.Begin(song);

    //TODO: Poner en variable y usar para todo los One_Wire
    //oneWire.Setup(PIN_D12);
}

void oBee::Update()
{
    oRGB.Update();
    oSound.Update();
}

void oBee::SetUpDrone(JsonObject& root)
{
    Log.info("SetupDrone-DETAIL");

    //int posID = str.indexOf(":");

    //String strId = str.substring (1, posID);   // get 01
    //Log.info("SetupDrone-ID:" + strId );

    String charPin1 = root["p"].asString();
    String charPin2 = root["a"].asString();

    Log.info("Sensor AUX: " + charPin2);

    //int droneID = strId.toInt();
    int pin1 = GetPinValue(charPin1);
    int pin2 = GetPinValue(charPin2);
    int fieldID = root["f"];
    int buzzerNotificationID = root["b"];
    int rgbNotificationID = root["r"];
    int notificationTime = root["x"];
    int workerID = root["w"];

    //int workerTimer = strWTimer.toInt();
    int workerTimer = root["y"];

    int inverted = root["z"];
    int notificationfieldID = root["n"];

    int oledID = root["o"];

    //Valor default
    sensor_type dtype = SENSOR_TYPE_SWITCH;

    if(root["t"] == 1)
    {
        dtype = SENSOR_TYPE_SWITCH;
    }
    else if (root["t"] == 2)
    {
        dtype = SENSOR_TYPE_TEMP;
    }
    else if (root["t"] == 3)
    {
        dtype = SENSOR_TYPE_DIGITAL;
    }
    else if (root["t"] == 4)
    {
        dtype = SENSOR_TYPE_ANALOG;
    }
    else if (root["t"] == 5)
    {
        dtype = SENSOR_TYPE_AMBIENT_TEMP;
    }

    //TODO: droneMode dmode = dmPULSE;
    sensor_mode dmode = SENSOR_MODE_PULSE;


    dmode = SENSOR_MODE_PULSE;

    sensor oSensor;

    //oSensor.droneID = droneID;
    oSensor.type = dtype;
    oSensor.pin = pin1;
    oSensor.pin2 = pin2;

    Log.info("Sensor PIN: " + String(oSensor.pin));
    Log.info("Sensor PIN2: " + String(oSensor.pin2));

    oSensor.buzzerNotificationID = buzzerNotificationID;
    oSensor.rgbNotificationID = rgbNotificationID;
    oSensor.notificationElapsedTime = notificationTime;
    oSensor.fieldID = fieldID;

    oSensor.mode = dmode;
    oSensor.workerID = workerID;
    Log.info("WorkerID: " + String(workerID));

    oSensor.workerElapsedTime = workerTimer;

    if(inverted == 1)
    {
      oSensor.inverted = true;
      Log.info("Inverted TRUE: " + String(oSensor.inverted));
    }
    else
    {
      oSensor.inverted = false;
      Log.info("Inverted FALSE: " + String(oSensor.inverted));
    }

    oSensor.notificationFieldID = notificationfieldID;
    oSensor.oledID = oledID;


    Log.info("SensorPIN: " + String(oSensor.pin));

    switch (oSensor.type)
    {
        case SENSOR_TYPE_SWITCH:
            SetUpDroneSwitch(oSensor);
            break;
        case SENSOR_TYPE_TEMP:
            SetUpDroneTemperature(oSensor);
            break;
        case SENSOR_TYPE_DIGITAL:
            SetUpDroneDigital(oSensor);
            break;
        case SENSOR_TYPE_ANALOG:
            SetUpDroneAnalog(oSensor);
            break;
        case SENSOR_TYPE_AMBIENT_TEMP:
            SetUpDroneAmbientTemp(oSensor);
            break;
    }


}

void oBee::SetUpDroneSwitch(sensor oSensor)
{
    DroneSwitch *oDroneSwitch = new DroneSwitch();
    oDroneSwitch->SetUpSensor(oSensor);
    droneSwitchList.add(oDroneSwitch);
}

void oBee::SetUpDroneTemperature(sensor oSensor)
{
    //TODO: Ver si lo dejamos aca
    //Check PORTS (A11, A12, D11, D12, etc - Para diferente BUS de OneWire)
    switch (oSensor.pin)
    {
        case PIN_D11:
            oneWire1.Setup(oSensor.pin);
            break;
        case PIN_D12:
            oneWire2.Setup(oSensor.pin);
            break;
        case PIN_D21:
            oneWire3.Setup(oSensor.pin);
            break;
        case PIN_D22:
            oneWire4.Setup(oSensor.pin);
            break;
        case PIN_D31:
            oneWire5.Setup(oSensor.pin);
            break;
        case PIN_D32:
            oneWire6.Setup(oSensor.pin);
            break;
        case PIN_A11:
            oneWire7.Setup(oSensor.pin);
            break;
        case PIN_A12:
            oneWire8.Setup(oSensor.pin);
            break;
        case PIN_A21:
            oneWire9.Setup(oSensor.pin);
            break;
        case PIN_A22:
            oneWire10.Setup(oSensor.pin);
            break;
    }

    //oneWire.Setup(oSensor.pin);

    DroneTemperature *oDroneTemperature = new DroneTemperature;

    switch (oSensor.pin)
    {
        case PIN_D11:
            oDroneTemperature->SetUpSensor(oSensor, &oneWire1);
            break;
        case PIN_D12:
            oDroneTemperature->SetUpSensor(oSensor, &oneWire2);
            break;
        case PIN_D21:
            oDroneTemperature->SetUpSensor(oSensor, &oneWire3);
            break;
        case PIN_D22:
            oDroneTemperature->SetUpSensor(oSensor, &oneWire4);
            break;
        case PIN_D31:
            oDroneTemperature->SetUpSensor(oSensor, &oneWire5);
            break;
        case PIN_D32:
            oDroneTemperature->SetUpSensor(oSensor, &oneWire6);
            break;
        case PIN_A11:
            oDroneTemperature->SetUpSensor(oSensor, &oneWire7);
            break;
        case PIN_A12:
            oDroneTemperature->SetUpSensor(oSensor, &oneWire8);
            break;
        case PIN_A21:
            oDroneTemperature->SetUpSensor(oSensor, &oneWire9);
            break;
        case PIN_A22:
            oDroneTemperature->SetUpSensor(oSensor, &oneWire10);
            break;
    }


    droneTemperatureList.add(oDroneTemperature);
}

void oBee::SetUpDroneDigital(sensor oSensor)
{
  DroneDigital *oDroneDigital = new DroneDigital();
  oDroneDigital->SetUpSensor(oSensor);
  droneDigitalList.add(oDroneDigital);
}

void oBee::SetUpDroneAnalog(sensor oSensor)
{
  DroneAnalog *oDroneAnalog = new DroneAnalog();
  oDroneAnalog->SetUpSensor(oSensor);
  droneAnalogList.add(oDroneAnalog);
}

void oBee::SetUpDroneAmbientTemp(sensor oSensor)
{
  DroneAmbientTemp *oDroneAmbientTemp = new DroneAmbientTemp();
  oDroneAmbientTemp->SetUpSensor(oSensor);
  droneAmbientTempList.add(oDroneAmbientTemp);
}

void oBee::ClearLists()
{
  //Clear list to initial SetUp
  droneSwitchList.clear();
  droneTemperatureList.clear();
  droneDigitalList.clear();
  droneAnalogList.clear();
  droneAmbientTempList.clear();
  workerList.clear();
}

int oBee::GetPinValue(String strPIN)
{
    int pinValue;

    if(strPIN == "D11")
        pinValue = PIN_D11;
    else if (strPIN == "D12")
        pinValue = PIN_D12;
    else if (strPIN == "D21")
        pinValue = PIN_D21;
    else if (strPIN == "D22")
        pinValue = PIN_D22;
    else if (strPIN == "D31")
        pinValue = PIN_D31;
    else if (strPIN == "D32")
        pinValue = PIN_D32;
    else if (strPIN == "A11")
        pinValue = PIN_A11;
    else if (strPIN == "A12")
        pinValue = PIN_A12;
    else if (strPIN == "A21")
        pinValue = PIN_A21;
    else if (strPIN == "A22")
        pinValue = PIN_A22;
    else if (strPIN == "AUX0")
            pinValue = AUX_0;
    else if (strPIN == "AUX1")
            pinValue = AUX_1;
    else if (strPIN == "AUX2")
            pinValue = AUX_2;
    else
            pinValue = strPIN.toInt();


    return pinValue;
}

void oBee::SetUpWorker(JsonObject& root)
{
    //CreateWorker("ID:00-PIN:D11-BZZR:1-RGB:1-TYPE:M-TIMER:005");
    Log.info("SetupWorker-DETAIL");

    //String charPin1 = root["p"].asString();
    //String charPin2 = root["a"].asString()

    String charPin1 = root["p"].asString();

    int workerID = root["i"];
    int pin = GetPinValue(charPin1);

    //String tallbackID = strTallBackID;
    int buzzerNotificationID = root["b"];
    int rgbNotificationID = root["r"];
    int autoTimer = root["x"];
    String strType = root["t"].asString();

    worker_type wtype = WORKER_MANUAL;

    if(strType == "M")
    {
        wtype = WORKER_MANUAL;
    }
    else
    {
        wtype = WORKER_AUTO;
    }

    Worker *worker = new Worker();

    worker->workerID = workerID;
    worker->pin = pin;
    //worker.tallBAckID
    worker->buzzerNotificationID = buzzerNotificationID;
    worker->rgbNotificationID = rgbNotificationID;
    worker->autoTimer = autoTimer;
    worker->type = wtype;

    worker->SetUpWorker();

    Log.info("WorkerID: " + String(worker->workerID));

    workerList.add(worker->workerID-1, worker);

}

void oBee::HandleWorker(sensor oSensor, sensor_event oEvent)
{
    if (oSensor.workerID != 0) //Check if has a Worker asociated
    {
      //Log.info("Worker: " + String(oSensor.workerID));
      int active = -1;
      int workerAutoOff = 0;

      Worker *worker = workerList.get(oSensor.workerID-1);
      workerAutoOff = worker->autoTimer;

      if(oEvent.triggerWorker)
      {
          active = 1;
          //Log.info("Worker Active 1");
      }
      else if (!oEvent.triggerWorker && workerAutoOff == 0) //if value is 0 -> OFF
      {
         active = 0;
         //Log.info("Active 0");
      }

      if (active == 1)
      {
        //Serial.println("WorkerID_ON: " + String(worker->workerID));
        worker->WorkerON();
      }
      else if(active == 0)
      {
          worker->WorkerOFF();
      }
      //If active -1 -> Check AutoWorker
      worker->AutoOffWorker();
    }
}

void oBee::HandleNotification(sensor oSensor, sensor_event oEvent)
{
    if (oEvent.triggerNotification)
    {
        //Serial.println("Notification");

        if(oSensor.rgbNotificationID != 0)
          RGBNotification(oSensor.rgbNotificationID);

        if(oSensor.buzzerNotificationID != 0)
          BzzrNotification(oSensor.buzzerNotificationID);
    }
}

void oBee::RGBNotification(int id)
{
  if(!oRGB.Running)
  {
    color1 = oRGB.Color(255,0,0);
    color2 = oRGB.Color(0,0,255);

    switch(id)
    {
      case 0:
        oRGB.RainbowCycle(5);
        break;
      case 1:
        //Notification 1
        color1 = oRGB.Color(100, 55, 0);
        color2 = oRGB.Color(0, 160, 255);
        oRGB.Repeat = false;
        oRGB.TheaterChase(color1, color2, 100, FORWARD);
        break;
      case 2:
        //Notification 2
        color1 = oRGB.Color(255, 0, 139);
        color2 = oRGB.Color(167, 255, 0);
        oRGB.Repeat = false;
        oRGB.TheaterChase(color1, color2, 100, FORWARD);
        break;
      case 3:
        //Notification 3
        color1 = oRGB.Color(255, 0, 50);
        color2 = oRGB.Color(0, 50, 255);
        oRGB.Repeat = false;
        oRGB.TheaterChase(color1, color2, 100, FORWARD);
        break;
      case 4:
        //Alert 1
        color1 = oRGB.Color(255, 113, 0);
        oRGB.Repeat = false;
        oRGB.Scanner(color1, 100);
        break;
      case 5:
        //Alert 2
        color1 = oRGB.Color(2, 96, 255);
        oRGB.Repeat = false;
        oRGB.Scanner(color1, 100);
        break;
      case 6:
        //Alert 2
        color1 = oRGB.Color(24, 255, 0);
        oRGB.Repeat = false;
        oRGB.Scanner(color1, 100);
        break;
    };
  }

}

void oBee::BzzrNotification(int id)
{
  if (!oSound.IsPlaying())
  {
    char *song;
    switch(id)
    {
      case 1:
          song = oSound.song_notification1;
          break;
      case 2:
          song = oSound.song_notification2;
          break;
      case 3:
          song = oSound.song_notification3;
          break;
      case 4:
          song = oSound.song_alamr1;
          break;
      case 5:
          song = oSound.song_alamr2;
          break;
      case 6:
          song = oSound.song_alamr3;
          break;
      case 7:
          song = oSound.song_error;
          break;
      case 8:
          song = oSound.song_custom1;
          break;
      case 9:
          song = oSound.song_custom2;
          break;
      case 10:
          song = oSound.song_custom3;
          break;
    }
    oSound.Begin(song);
  }

}
