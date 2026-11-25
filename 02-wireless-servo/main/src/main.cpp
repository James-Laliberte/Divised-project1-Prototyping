#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include "network-info.h"

Servo servoMotor;
const int servoPin = 13;

WiFiServer server(3333);
WiFiClient computer;
String receivedText = "";


void moveServo(int targetAngle) {

  targetAngle = constrain(targetAngle, 0, 180);

  servoMotor.write(targetAngle);
}


void setup() {
  Serial.begin(9600);
  
  servoMotor.setPeriodHertz(50);
  servoMotor.attach(servoPin, 500, 2400);
  moveServo(90);

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

  if (receivedText.length() == 0 || !isDigit(receivedText.charAt(0))) {
    return;
  }

  int targetAngle = receivedText.toInt();

  moveServo(targetAngle);
  Serial.println("Moved to: " + String(targetAngle)); 
}