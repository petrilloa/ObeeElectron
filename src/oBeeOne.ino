#include "oBee.h"
#include "lib/SparkJson/ArduinoJson.h"
#include "lib/PowerCheck.h"

SYSTEM_THREAD(ENABLED);

oBee oBeeOne;
PowerCheck power;
FuelGauge fuel;

double publishTime = 1200000;
//Usado para los sensores de temperatura. Debe ser menor o igual al PublishTime.
//Se utiliza si el tiempo de Publish es muy grande, para detectar "EVENTOS"
double poolingTime = 30000;

unsigned long ms;
unsigned long msLast;
unsigned long msLastPooling;

//Obtener desde System
String oBeeID = "1";

bool readyToPublish = false;
bool loopWarmUp = false;

bool eventToPublish = false;


typedef struct
{
  int fieldID;
  float value;
} fieldValue; //Usado para publicar valores

LinkedList<fieldValue*> fieldList = LinkedList<fieldValue*>(); //Listado de valores a publicar


/************Setup**************/
/*******************************/

SerialLogHandler logHandler;

bool powerVariable = true;

void setup() {

    Serial.begin(9600);

    power.setup();
    //TODO: Testear modo de funcionamiento SEMI-AUTOMATICO
    //waitFor(Particle.connected, 10000);

    //if(Particle.connected)
    //  Log.info("Particle CONNECTED");
    //else
    //  Log.info("Particle NOT CONNECTED");


    Particle.subscribe(System.deviceID() +"/hook-response/Firebase", FireBaseHandler, MY_DEVICES);
    Particle.variable("publishTime", publishTime);

    //Particle.variable("losantId", losantId);

    Particle.function("rgbTrigger",TriggerRGBNotification); //Funcion para ALARMA RGB
    Particle.function("bzzTrigger",TriggeBzzrNotification); //funcion para ALARMA Sonido
    Particle.function("reset", Reset); //Funcion para resetear

    Particle.function("setup", SetupObee); //Se llama desde la Web para tomar nueva Configuracion

    ms = 0;
    msLast = 0;

    //Obtener ID de Obee
    oBeeID = System.deviceID();

    //Intenta tomar desde la EEPROM, sino, invoca a FireBase
    SetupObee("nothing");
}

/******* Handler of Publish DEVICE Name********/
int SetupObee(String var)
{
  //Obtener de la EEPROM
  int addr = 10;
  String fullMessage;
  char charBuf[1024];

  EEPROM.get(addr, charBuf);

  fullMessage = String(charBuf);

  //Si esta VACIA la EEPROM - o se INVOCA reload (desde WEB) - Llamar a FireBase
  if (fullMessage == "" || var=="reload")
  {
    Log.info("Setup Obee EEPROM: Empty - Calling FireBase");
    //Obtener configuracion
    //String data =  "{\"id\":\"1\"}";
    String dataFirebase = "{\"id\":\"" + oBeeID + "\"}";
    Particle.publish("Firebase", dataFirebase, PRIVATE);

    //Limpar Drone y Workers
    oBeeOne.ClearLists();
  }
  else
  {
    Log.info("Setup Obee EEPROM:" + fullMessage);
    SetupObeeDetail(fullMessage);
  }

  Log.info("Setup Obee");

  return 0;
}

/******* Handler of FireBase********/
String strStart = "{\"oBee";
String strEnd = "}}";
String fullMessage;

void FireBaseHandler(const char *event, const char *data) {
  // Handle the webhook response
  Log.info("***********FireBaseHandler***********");
  Log.info(String(data));

  //Test if final data
  if (String(data).startsWith(strStart))
  {
    fullMessage = String(data);
    Log.info("***********START***********");

    //Si no termina, salir y esperar el resto
    if(!String(data).endsWith(strEnd))
    {
      Log.info("***********CONTINUE***********");
      return;
    }
    else
    {
      Log.info("***********END***********");
    }
  }
  else if (!String(data).endsWith(strEnd))
  {
    //Agrego data al mensaje total
    fullMessage += String(data);
    Log.info("***********LOOP***********");
    return;
    //Salgo de la rutina y se vuelve a llamar hasta que se completa el mensaje
  }
  else{
    //Completo el mensaje
    fullMessage += String(data);
    Log.info("***********END***********");
  }

  Log.info("FullMessage: " + fullMessage);

  //Write to EEPROM
  int addr = 10;
  //int lenght = sizeof(fullMessage);
  char charBuf[1024];
  fullMessage.toCharArray(charBuf, 1024);

  EEPROM.put(addr, charBuf);
  Log.info("PUT EEPROM");

  //String fullMessageEE;
  //EEPROM.get(addr, fullMessageEE);
  //Log.info("Setup Obee EEPROM:" + fullMessageEE);

  SetupObeeDetail(fullMessage);
}

void SetupObeeDetail(String fullMessage)
{
  StaticJsonBuffer<2048> jsonBuffer;
  //char *dataCopy = strdup(data);
  //char *dataCopy = fullMessage.c_str();
  char *dataCopy = strdup(fullMessage);

  JsonObject& root = jsonBuffer.parseObject(dataCopy);

  if (!root.success()) {
    Log.info("parseObject() failed - root");
    return;
  }

  int pos1 = fullMessage.indexOf("oBee");
  int pos2 = fullMessage.indexOf(":");

  String strId = fullMessage.substring (pos1, pos2-1);   // get Name
  Log.info("srtID:" + strId);

  JsonObject& oBeeJS = root[strId];  // query root
  if (!oBeeJS.success()) {
    Log.error("parseObject() failed - oBee");
    return;
  }

  //Get variables
  publishTime = (int)oBeeJS["t"];

  String strNode;

  //Check for Drones
  for(int i=1; i < 11; i++)
  {
    if (oBeeJS.containsKey("d" + String(i)))
    {
        //strNode = oBeeJS["d" + String(i)].asString();
        JsonObject& oBeeNode = oBeeJS["d" + String(i)];  // query root
        if (!oBeeNode.success()) {
          Log.error("parseObject() failed - oBee Node");
          return;
        }

        Log.info("Drone finded!");
        oBeeOne.SetUpDrone(oBeeNode);
    }
  }

  //Check for Workers
  for(int i=1; i < 11; i++)
  {
    if (oBeeJS.containsKey("w" + String(i)))
    {
        //strNode = oBeeJS["w" + String(i)].asString();
        JsonObject& oBeeWorker = oBeeJS["w" + String(i)];  // query root
        if (!oBeeWorker.success()) {
          Log.error("parseObject() failed - oBee Worker");
          return;
        }

        Log.info("Worker finded!");

        oBeeOne.SetUpWorker(oBeeWorker);
    }
  }

  //readyToPublish
  readyToPublish = true;


}

/*Public Function Particle API */
/*******************************/

int TriggerRGBNotification(String command)
{
  oBeeOne.RGBNotification(command.toInt());
  return 1;
}

int TriggeBzzrNotification(String command)
{
  oBeeOne.BzzrNotification(command.toInt());
  return 1;
}

int Reset(String command)
{
  System.reset();
  return 1;
}

/*******************************/

/*********Main loop ************/
/*******************************/

bool hasPower = true;

void loop() {

    ms = millis();

    oBeeOne.Update();

    HandleDroneSwitch();
    HandleDroneDigital();
    HandleDroneAnalog();
    HandleDroneTemperature();
    HandleDroneAmbientTemp();

    //Publish to the cloud
    //Hacer 1er loop de warmup
    if (readyToPublish)
    {
      if (!loopWarmUp)
        loopWarmUp = true;
      else
        Publish();
    }

    //Log.info("Loop");

}
/*******************************/

void HandleDroneSwitch()
{
  sensor oSensor;
  sensor_event oEvent;

  int listSize = oBeeOne.droneSwitchList.size();

  for (int h = 0; h < listSize; h++)
  {
    DroneSwitch *droneSwitch;

    droneSwitch = oBeeOne.droneSwitchList.get(h);

    droneSwitch->GetSensor(&oSensor);
    droneSwitch->GetEvent(&oEvent);

    oBeeOne.HandleWorker(oSensor, oEvent);
    oBeeOne.HandleNotification(oSensor, oEvent);

    //IF Publish time o Cambio???
    if(ms - msLast > publishTime || oEvent.triggerPublish)
    {
      //Hay un evento para PUBLICAR
      eventToPublish = oEvent.triggerPublish;


      sensor_event oEvent;
      droneSwitch->Publish(&oEvent);

      //GetValue
      //Serial.println("SetField-" + String(oSensor.fieldID) + ": "+ String(oEvent.value));

      //Add to collection - LOSANT
      fieldValue *oValue = new fieldValue();

      oValue->fieldID = oSensor.fieldID;
      oValue->value = oEvent.value;

      fieldList.add(oValue);

      //Check notification publish
      if(oSensor.notificationFieldID != 0)
      {
        //Add to collection - LOSANT
        fieldValue *oValue = new fieldValue();
        oValue->fieldID = oSensor.notificationFieldID;
        oValue->value = oEvent.acumulatedNotification;

        fieldList.add(oValue);
      }
    }
  }
}

void HandleDroneDigital()
{
  sensor oSensor;
  sensor_event oEvent;

  int listSize = oBeeOne.droneDigitalList.size();

  for (int h = 0; h < listSize; h++)
  {
    DroneDigital *droneDigital;

    //Serial.println("GetDrone: " + String(h));
    droneDigital = oBeeOne.droneDigitalList.get(h);

    //Serial.println("3");
    droneDigital->GetSensor(&oSensor);
    //Serial.println("4");
    droneDigital->GetEvent(&oEvent);
    //Serial.println("5");

    oBeeOne.HandleWorker(oSensor, oEvent);
    oBeeOne.HandleNotification(oSensor, oEvent);
    //Serial.println("6");

    //IF Publish time || Event Generated (Check THIS)
    if(ms - msLast > publishTime || oEvent.triggerPublish)
    {
      //Hay un evento para PUBLICAR
      eventToPublish = oEvent.triggerPublish;

      droneDigital->Publish(&oEvent);

      //Add to collection - LOSANT
      fieldValue *oValue = new fieldValue();

      oValue->fieldID = oSensor.fieldID;
      oValue->value = oEvent.value;

      fieldList.add(oValue);

      //Check notification publish
      if(oSensor.notificationFieldID != 0)
      {
        //Add to collection - LOSANT
        fieldValue *oValue = new fieldValue();

        oValue->fieldID = oSensor.notificationFieldID;
        oValue->value = oEvent.acumulatedNotification;
        fieldList.add(oValue);
      }
    }
  }
}

void HandleDroneAnalog()
{
  sensor oSensor;
  sensor_event oEvent;

  int listSize = oBeeOne.droneAnalogList.size();

  for (int h = 0; h < listSize; h++)
  {
  DroneAnalog *droneAnalog;

  droneAnalog = oBeeOne.droneAnalogList.get(h);

  droneAnalog->GetSensor(&oSensor);
  droneAnalog->GetEvent(&oEvent);

  oBeeOne.HandleWorker(oSensor, oEvent);
  oBeeOne.HandleNotification(oSensor, oEvent);

  //IF Publish time
    if(ms - msLast > publishTime)
    {
        droneAnalog->Publish(&oEvent);

        //GetValue
        //Serial.println("SetField-" + String(oSensor.fieldID) + ": "+ String(oEvent.value));

        //Add to collection - LOSANT
        fieldValue *oValue = new fieldValue();

        oValue->fieldID = oSensor.fieldID;
        oValue->value = oEvent.value;

        fieldList.add(oValue);

    }
  }
}

void HandleDroneTemperature()
{
  sensor oSensor;
  sensor_event oEvent;

  int listSize = oBeeOne.droneTemperatureList.size();

  for (int h = 0; h < listSize; h++)
  {
    //IF Publish time - FOR TEMPERATURE ONLY WHEN PUBLISH! Evitar saturar el BUS OneWIRE -
    //Testear con varios sensores...
    if(ms - msLastPooling > poolingTime || ms - msLast > publishTime)
    {
      DroneTemperature *droneTemperature;

      droneTemperature = oBeeOne.droneTemperatureList.get(h);

      droneTemperature->GetSensor(&oSensor);
      droneTemperature->GetEvent(&oEvent);

      oBeeOne.HandleWorker(oSensor, oEvent);
      oBeeOne.HandleNotification(oSensor, oEvent);

      msLastPooling = ms;

      if(ms - msLast > publishTime || oEvent.triggerPublish)
      {
        //Hay un evento para PUBLICAR
        eventToPublish = oEvent.triggerPublish;

        droneTemperature->Publish(&oEvent);

        //GetValue
        //Serial.println("SetField-" + String(oSensor.fieldID) + ": "+ String(oEvent.value));

        //Add to collection - LOSANT
        fieldValue *oValue = new fieldValue();

        oValue->fieldID = oSensor.fieldID;
        oValue->value = oEvent.value;

        fieldList.add(oValue);
      }

    }
  }
}

void HandleDroneAmbientTemp()
{
  sensor oSensor;
  sensor_event oEvent;

  int listSize = oBeeOne.droneAmbientTempList.size();

  for (int h = 0; h < listSize; h++)
  {
    //IF Publish time - FOR TEMPERATURE ONLY WHEN PUBLISH! Evitar saturar el BUS -
    //Testear con varios sensores...

    //Log.info("elapsedTime: " + String(ms - msLast));

    if(ms - msLastPooling > poolingTime || ms - msLast > publishTime)
    {
      DroneAmbientTemp *droneAmbientTemp;

      droneAmbientTemp = oBeeOne.droneAmbientTempList.get(h);

      droneAmbientTemp->GetSensor(&oSensor);
      droneAmbientTemp->GetEvent(&oEvent);

      oBeeOne.HandleWorker(oSensor, oEvent);
      oBeeOne.HandleNotification(oSensor, oEvent);

      msLastPooling = ms;

      Log.info("elapsedTime: " + String(ms - msLast));

      if(ms - msLast > publishTime || oEvent.triggerPublish)
      {
        //Hay un evento para PUBLICAR
        eventToPublish = oEvent.triggerPublish;

        droneAmbientTemp->Publish(&oEvent);

        //GetValue
        Log.info("SetField-" + String(oSensor.fieldID) + ": "+ String(oEvent.value));
        //GetValue
        Log.info("SetField-" + String(oSensor.notificationFieldID) + ": "+ String(oEvent.acumulatedNotification));

        //Add to collection - LOSANT
        fieldValue *oValue = new fieldValue();

        oValue->fieldID = oSensor.fieldID;
        oValue->value = oEvent.value;

        fieldList.add(oValue);

        //Add to collection - LOSANT
        fieldValue *oValueH = new fieldValue();

        //AmbientTemp uses NotificacionField
        oValueH->fieldID = oSensor.notificationFieldID;
        oValueH->value = oEvent.acumulatedNotification;

        fieldList.add(oValueH);
      }
    }
  }
}


/*********Publish to the cloud************/
/*******************************/
int randomNumber(int minVal, int maxVal)
{
  // int rand(void); included by default from newlib
  return rand() % (maxVal-minVal+1) + minVal;
}

void Publish()
{
    //Si paso el tiempo de Publicacion, todo lo que esta en la LISTA se publica.
    //Incluir tema de Bateria y Carga
    if (Particle.connected)
    {
      //Check for Power ADDED
      bool publishPowerFailure = false;
      bool publishPowerConnected = false;

      bool isPower = power.getHasPower();

      //Si no tiene poder, y la variable hasPower era TRUE, significa que se detecto CORTE - Hay que publicar
      if (!isPower && hasPower)
      {
        publishPowerFailure = true;
        Log.info("LOST Power - Publish");
      }

      //Si tiene poder, y antes NO tenia, es que recupero la alimentacion, Hay que publicar
      if (isPower && !hasPower)
      {
        publishPowerConnected = true;
        Log.info("Connected Power - Publish");
      }

      hasPower = isPower;

      if(ms - msLast > publishTime || eventToPublish || publishPowerFailure || publishPowerConnected)
      {
          if (eventToPublish)
            Log.info("PUBLISH: Event to Publish");
          else
            Log.info("PUBLISH: Publish Time");

          msLast = ms;

          StaticJsonBuffer<1024> jsonBuffer;
          JsonObject& root = jsonBuffer.createObject();

          //TODO: Mejorar esto!
          for (int i = 0; i < fieldList.size(); i++)
          {
              fieldValue *oValue = fieldList.get(i);

              if(oValue->fieldID == 1)
              {
                root["f1"] = "";
              }
              else if (oValue->fieldID == 2)
              {
                root["f2"] = "";
              }
              else if (oValue->fieldID == 3)
              {
                root["f3"] = "";
              }
              else if (oValue->fieldID == 4)
              {
                root["f4"] = "";
              }
              else if (oValue->fieldID == 5)
              {
                root["f5"] = "";
              }
              else if (oValue->fieldID == 6)
              {
                root["f6"] = "";
              }
              else if (oValue->fieldID == 7)
              {
                root["f7"] = "";
              }
              else if (oValue->fieldID == 8)
              {
                root["f8"] = "";
              }
              else if (oValue->fieldID == 9)
              {
                root["f9"] = "";
              }
              else if (oValue->fieldID == 10)
              {
                root["f10"] = "";
              }
          }

          //SI tiene PODER NO es NECESARIO PUBLICAR Valor
          //Publica en los siguientes ESCENARIOS: Poder perdido, Poder Conectado, Cuando pasa el tiempo de publicacion y NO tiene poder.
          if (publishPowerFailure || publishPowerConnected || !isPower)
          {
            //Battery 0 to 100
            float batLevel = fuel.getSoC();
            root["b"] = batLevel;
            //Stats TRUE or FALSE; Has Power
            bool hasPower = power.getHasPower();
            root["s"] = hasPower ? "1" : "0";
          }


          for (int i = 0; i < fieldList.size(); i++)
          {
            fieldValue *oValue = fieldList.get(i);
            //root["f" + String(oValue->fieldID)] = oValue->value;
            int fieldID = oValue->fieldID;
            float value = oValue->value;

            root["f" + String(fieldID)] = value;

            Log.info("Field: " + String(fieldID) + " Value: " + String(value));

            //Delete object from Memory
            delete oValue;
          }

          // Get JSON string.
          char buffer[1024];
          root.printTo(Serial);
          root.printTo(buffer, sizeof(buffer));

          //Serial.println("---------------");

          //Publicacion - Losant toma el evento del API de Particle
          Particle.publish("Publish", buffer , PRIVATE, NO_ACK);

          //Clear the values
          fieldList.clear();

          eventToPublish = false;

      }
    }
    else //Particle NOT connected
    {
      //Si no tenia conexion al momento de PUBLICAR - Elimar LISTA INFORMAR y esperar el siguiente evento
      Log.error("CANT Publish - Particle NOT connected");
      //Clear the values
      fieldList.clear();
    }
}
