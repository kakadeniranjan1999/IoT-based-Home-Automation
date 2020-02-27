/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>



/****************************************
 * Define Constants
 ****************************************/

#define TOKEN "A1E-jnfnWZ1lmLu52NyG3AHy8sf58xGGhoKDGOiiWQQoA6hibAmPr1XzVDMd" // Your Ubidots TOKEN
#define WIFINAME "Redmi 4" //Your SSID
#define WIFIPASS "qazwsxedc" // Your Wifi Pass
int sec=16;
float sensor=A0;
float gas_value;
int trigPin = 14;
int echoPin = 5;
int led1=15;
int led2=12;
float duration, distance;
char auth[] = "aaa99c83ee1843c79bb471d4ffafdd27";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Redmi 4";
char pass[] = "qazwsxedc";
WidgetMap myMap(V1);

Ubidots client(TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if ((char)payload[0]=='1')
  {
    digitalWrite(15,HIGH);
  }
  else
  {
    digitalWrite(15,LOW);
  }
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(sensor,INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(sec,OUTPUT);
  client.ubidotsSubscribe("esp8266","button");
  client.ubidotsSubscribe("esp8266","motor");
  Blynk.begin(auth, ssid, pass);
  digitalWrite(sec,HIGH);

  }

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()){
      client.reconnect();
      client.ubidotsSubscribe("esp8266","door-security"); //Insert the dataSource and Variable's Labels
      client.ubidotsSubscribe("esp8266","button");
      client.ubidotsSubscribe("esp8266","water-level");
      client.ubidotsSubscribe("esp8266","gas-sensor");
      client.ubidotsSubscribe("esp8266","motor");
      }
   if(digitalRead(sec)==1)
   {
    Serial.print("Alert!!!");
    client.add("door-security", 0);
    client.ubidotsPublish("esp8266");
    int index = 1;
    float lat = 51.5074;
    float lon = 0.1278;
    myMap.location(index, lat, lon, "value");
  }
  else
  {
    client.add("door-security", 1);
    client.ubidotsPublish("esp8266");
    myMap.clear();
   
  }

    gas_value=analogRead(sensor);
    client.add("gas-sensor",gas_value);
    client.ubidotsPublish("esp8266");

    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration=pulseIn(echoPin, HIGH);
    distance =(((duration/2)/29.1)*4);
    client.add("water-level",distance);
    client.ubidotsPublish("esp8266");
  
    client.loop();
    delay(1000);
  }
