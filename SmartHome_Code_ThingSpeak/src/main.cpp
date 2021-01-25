#include <Arduino.h>
#include "SoftwareSerial.h"

//Defining the RX and TX for the esp8266 module
#define RX 2
#define TX 3

//calling softwareSerial library with parameter of RX and TX
SoftwareSerial esp8266(RX, TX);

//Define Information for the api and esp8266
String AP = "MortyMcFly2020";
String PASS = "Valorant2020!";
String API = "HF1HUWFS9LR9I59N";
String HOST = "api.thingspeak.com";
String PORT = "80";

//define other variables
int countTrueCommand;
int countTimeCommand;
boolean found = false;

void estup()
{
  Serial.begin(9600);
  esp8266.begin(115200);
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\""+AP+"\",\""+PASS+"\"",20,"OK");
}

void loop()
{
  String getData = "GET /update?api_key="+API+"&field1="+getTemperatureValue()+"&field2="+getHumidityValue();
  sendCommand("AT+CIPMUX=1",5,"OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
  sendCommand("AT+CIPSEND=0," +String(getData.length()+4), 4, ">");
  esp8266.println(getData);
  delay(1500);
  countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");
}

void sendCommand(String command, int maxTime, char readReplay[])
{
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1))
  {
    esp8266.println(command);
    if (esp8266.find(readReplay))
    {
      found = true;
      break;
    }
    countTimeCommand++;
  }
  if (found == true)
  {
    Serial.println("Con");
    countTrueCommand++;
    countTimeCommand = 0;
  }
  if (found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }
  found = false;
}

String getTemperatureValue(){
  //float temperature = dhtobject.getTemperature();
  Serial.print("Temperature(C) = ");
  //Serial.println(temperature);
  //return String(temperature);
}

String getHumidityValue(){
  //float humidity = dhtobject.getHumidity();
  Serial.print("Humidity in % = ");
  //Serial.println(humidity);
  //return String(humidity);
}