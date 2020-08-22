#include <SoftwareSerial.h>
#include "TinyGPS++.h"

SoftwareSerial ss(10, 11); // RX, TX
SoftwareSerial ssGPS(9, 8); // RX, TX

TinyGPSPlus gps;

#define btn 7
#define led 13

boolean check_GPS = false;
String data_GPS = "";

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  ssGPS.begin(9600);

  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  ss.print("+++");
  delay(300);
  while (ss.available() != 0){
    digitalWrite(led, HIGH);
    Serial.print(ss.read());
  }
  ss.print("AT+ROLE=STA");
  delay(300);
  while (ss.available() != 0){
    digitalWrite(led, HIGH);
    Serial.print(ss.read());
  }
  Serial.println("Xong 1");
  ss.print("AT+AP=DCSVN,WPA2,03021930");
  delay(300);
  while (ss.available() != 0){
    digitalWrite(led, HIGH);
    Serial.print(ss.read());
  }
  Serial.println("Xong 2");
  ss.print("AT+SOCK=TCPC,9000,192.168.137.163,8080");       // IP LAN, PORT
  delay(300);
  while (ss.available() != 0){
    digitalWrite(led, HIGH);
    Serial.print(ss.read());
  }
  Serial.println("Xong 3");
  ss.print("AT+RST");
  delay(1000);
  while (ss.available() != 0){
    digitalWrite(led, HIGH);
    Serial.print(ss.read());
  }
  Serial.println("Xong 4");
}

void loop() {
  while (ssGPS.available())
  {
    gps.encode(ssGPS.read());
    Serial.print("...");
  }
  if (gps.location.isUpdated())
  {
    Serial.println("Latitude:");
    Serial.println(gps.location.lat(), 6);
    Serial.println("Longitude:");
    Serial.println(gps.location.lng(), 6);
    data_GPS = ("Warning " + String(gps.location.lat(), 6) + " - " + String(gps.location.lng(), 6));
    Serial.println(data_GPS);
  }

  if (!digitalRead(btn)) {
    while (!digitalRead(btn));
    ss.print("Warning");
    digitalWrite(led, HIGH);
    for (int i = 0; i < 5; i++) {
      ss.print(data_GPS);
      delay(500);
    }
    digitalWrite(led, LOW);
    Serial.println("Send");
  }
}
