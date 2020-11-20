// oneM2M Education SDK for Arduino WiFi V1.00
// Copyright (c) 2020, WINIKIM. All Rights Reserved.
// Author: Winikim

#include <ESP8266WiFi.h>
#include <winikim.h>
#include <ArduinoJson.h>

/////////////////////////////////////////////////////////////////////
// Environment variables for Using IoT Platform.
const char ssid[] = "Your_WiFi_SSID";
const char wifi_pass[] = "Your_WiFi_Password";
const char cseId[] = "oneM2M Platform CSE ID";
const char mqttSvr[] = "Platform's_MQTT_Server_Address_DO_NOT_ADD_PORT";
const char aeId[] = "aeId of oneM2M";
const char mqttUsername[] = "AE_ID";
const char mqttPassword[] = "AE_ID";
/////////////////////////////////////////////////////////////////////

WiFiClient net;
////ArduinoJson
const size_t CAPACITY = JSON_OBJECT_SIZE(1024);
StaticJsonDocument<CAPACITY> doc;
////
unsigned long lastMillis = 0;

bool wifiStatus = false;
bool mqttStatus = false;
int LED_PIN = 12;

void setup()
{

  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  WiFi.begin(ssid, wifi_pass);

  winikimBegin(net, aeId, mqttSvr, cseId, messageReceived);

  if (WiFi.status() != WL_CONNECTED)
  {
    wifiStatus = false;
  }
  else
  {
    wifiStatus = true;
  }
}


void loop()
{
  // Running Lotte IoT Platform code in background.
  winikimLoop();

  // If platform connection is disconnected, this function reconnect MQTT.
  if (!winikimConnected())
  {
    mqttStatus = false;
    mqttConnect();
  }
}

// Get Commands from Platform via MQTT.
void messageReceived(String &topic, String &payload)
{
  
  deserializeJson(doc, payload);
  JsonObject req_root = doc.as<JsonObject>();
  String command = req_root["pc"]["m2m:sgn"]["nev"]["rep"]["m2m:cin"]["con"];
  doc.clear();

  if (command.equals("ON"))
  {
    digitalWrite(LED_PIN, HIGH); 
    
  }
  if (command.equals("OFF"))
  {
    digitalWrite(LED_PIN, LOW); 
  }
}

// Connect MQTT & WiFi
void mqttConnect()
{
  Serial.print("\nchecking wifi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    wifiStatus = false;
    delay(1000);
  }
  wifiStatus = true;

  //MQTT reconnect
  if (wifiStatus)
  {
    Serial.print("\nWinikim connecting...");
    while (!winikimConnect(aeId, mqttUsername, mqttPassword))
    {
      delay(1000);
    }
    Serial.println("\nConnected!");
    mqttStatus = true;

    winikimSubscribe();
    Serial.print("\nSuccess subscribe()");
  }
}