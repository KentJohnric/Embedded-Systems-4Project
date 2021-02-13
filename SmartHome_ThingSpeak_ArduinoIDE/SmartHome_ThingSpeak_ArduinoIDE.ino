#include "SoftwareSerial.h"
#include <DHT.h>

//Defining the RX and TX for the esp8266 module
#define RX 2
#define TX 3
#define DHTTYPE DHT11
#define DHTPIN 5
const int DOOR_SENSOR_PIN = 13;
int IRSensor = 8;
int doorState;
//const int trigPin = 9;
//const int echoPin = 10;
//float duration, distance;
//calling softwareSerial library with parameter of RX and TX
SoftwareSerial esp8266(RX, TX);

//Define Information for the api and esp8266
String AP = "MortyMcFly2020"; //Wifi Name
String PASS = "Valorant2020!"; //Wifi Password
String API = "F1SPFJ9UXAT8I8XA"; //The user ThingSpeak Channel API
String HOST = "api.thingspeak.com"; //ThingSpeak API 
String PORT = "80";

//define other variables
int countTrueCommand;
int countTimeCommand;
boolean found = false;
DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(9600);
  esp8266.begin(115200);
  pinMode(DOOR_SENSOR_PIN, INPUT_PULLUP);
  //pinMode(trigPin, OUTPUT);
  //pinMode(echoPin, INPUT);
  pinMode(IRSensor, INPUT);
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\""+AP+"\",\""+PASS+"\"",20,"OK");
}

void loop()
{
  String getData = "GET /update?api_key="+API+"&field1="+getTemperatureValue()+"&field2="+getHumidityValue()+"&field3="+getPhotoSens()+"&field4="+getDoorSens()+"%field5="+getMotionSen();
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
    Serial.println("Connected");
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
  float temperature = dht.readTemperature();
  Serial.print("Temperature(C) = ");
  Serial.println(temperature);
  return String(temperature);
}

String getHumidityValue(){
  float humidity = dht.readHumidity();
  Serial.print("Humidity in % = ");
  Serial.println(humidity);
  return String(humidity);
}

String getPhotoSens(){
  int photoSenValue = analogRead(A0);
  Serial.print("PhotoSensor Value = ");
  Serial.println(photoSenValue);
  return String(photoSenValue);
}

String getDoorSens(){
  doorState = digitalRead(DOOR_SENSOR_PIN);
  if(doorState == 1){
    Serial.println("Door is Open");
  }
  else{
    Serial.println("Door is Closed");
  }
  return String(doorState);
}

String getMotionSen(){
  int statusSensor = digitalRead(IRSensor);
  if(statusSensor == 1){
    Serial.println("Motion Detected");
  }
  else{
    Serial.println("Motion not Detected");
  }
  return String(statusSensor);
}
