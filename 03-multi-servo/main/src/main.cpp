#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include "network-info.h"
 
const int servoCount = 4;
const int servoPins[servoCount] = {13, 14, 27, 26};
Servo servoMotors[servoCount];
 
WiFiServer server(3333);
WiFiClient computer;
String receivedText = "";
 
 
void moveServo(int servoIndex, int targetAngle) {
 
  targetAngle = constrain(targetAngle, 0, 180);
 
  servoMotors[servoIndex].write(targetAngle);
}
 
 
void setup() {
  Serial.begin(9600);
 
  for (int servoIndex = 0; servoIndex < servoCount; servoIndex++) {
    servoMotors[servoIndex].setPeriodHertz(50);
    servoMotors[servoIndex].attach(servoPins[servoIndex], 500, 2400);
    moveServo(servoIndex, 90);
  }
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(networkName, networkPassword);
  Serial.print("Connecting to WiFi ..");
 
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  WiFi.setSleep(false);
  Serial.println(WiFi.localIP());
 
  server.begin();
}
 
 
void loop() {
 
  if (!computer || !computer.connected()) {
    computer = server.available();
    return;
  }
 
  if (computer.available() == 0) {
    return;
  }
 
  receivedText = computer.readStringUntil('\n');
  receivedText.trim();
  Serial.println(receivedText);
 
  if (receivedText.length() == 0 || !isDigit(receivedText.charAt(0))) {
    return;
  }
 
  int startOfNumber = 0;
  for (int servoIndex = 0; servoIndex < servoCount; servoIndex++) {
 
    if (startOfNumber >= (int)receivedText.length()) {
      break;
    }
 
    int commaPosition = receivedText.indexOf(',', startOfNumber);
    if (commaPosition == -1) {
      commaPosition = receivedText.length();
    }
 
    int targetAngle = receivedText.substring(startOfNumber, commaPosition).toInt();
    moveServo(servoIndex, targetAngle); 
    startOfNumber = commaPosition + 1;
  }
}