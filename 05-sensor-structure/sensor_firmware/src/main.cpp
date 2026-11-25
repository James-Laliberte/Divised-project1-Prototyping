#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <Adafruit_VL53L0X.h>
#include "network-info.h"

Adafruit_VL53L0X sensor1;
Adafruit_VL53L0X sensor2;
Adafruit_VL53L0X sensor3;
Adafruit_VL53L0X sensor4;
WiFiServer server(3333);
WiFiClient computer;

int distance1 = -1;
int distance2 = -1;
int distance3 = -1;
int distance4 = -1;
unsigned long lastSendTime = 0;


void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(networkName, networkPassword);
  Serial.print("Connecting to WiFi \n");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println(WiFi.localIP());
  WiFi.setSleep(false);
  server.begin();

  Wire.begin(17, 15);

  pinMode(27, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(27, LOW);
  digitalWrite(14, LOW);
  digitalWrite(26, LOW);
  digitalWrite(13, LOW);
  delay(10);

  digitalWrite(27, HIGH);
  delay(10);
  if (!sensor1.begin(0x30)) {
    Serial.println("sensor1 not found");
    while (true) { delay(10); }
  }

  digitalWrite(14, HIGH);            // [4] sensor 2 on, still at 0x29
  delay(10);
  if (!sensor2.begin(0x31)) {        // [4] DECISION: move it to 0x31
    Serial.println("sensor2 not found");
    while (true) { delay(10); }
  }

  digitalWrite(26, HIGH);            // [4] sensor 3 on, still at 0x29
  delay(10);
  if (!sensor3.begin(0x32)) {        // [4] DECISION: move it to 0x32
    Serial.println("sensor3 not found");
    while (true) { delay(10); }
  }

  digitalWrite(13, HIGH);            // [4] sensor 4 on, still at 0x29
  delay(10);
  if (!sensor4.begin(0x33)) {        // [4] DECISION: move it to 0x33
    Serial.println("sensor4 not found");
    while (true) { delay(10); }
  }

  sensor1.startRangeContinuous(50);  // [5] SETUP: each sensor reads on its own every 50 ms
  sensor2.startRangeContinuous(50);
  sensor3.startRangeContinuous(50);
  sensor4.startRangeContinuous(50);
}


void loop() {

  if (sensor1.isRangeComplete()) {                   // [6] DECISION: new reading ready?
    int sensor1Reading = sensor1.readRangeResult();  // [7] INPUT: collect it, no waiting
    if (sensor1.readRangeStatus() == 0) {            // [8] DECISION: good reading?
      distance1 = sensor1Reading;                    // [9] stored
    } else {
      distance1 = -1;                                // [9] out of range
    }
  }

  if (sensor2.isRangeComplete()) {
    int sensor2Reading = sensor2.readRangeResult();
    if (sensor2.readRangeStatus() == 0) {
      distance2 = sensor2Reading;
    } else {
      distance2 = -1;
    }
  }

  if (sensor3.isRangeComplete()) {
    int sensor3Reading = sensor3.readRangeResult();
    if (sensor3.readRangeStatus() == 0) {
      distance3 = sensor3Reading;
    } else {
      distance3 = -1;
    }
  }

  if (sensor4.isRangeComplete()) {
    int sensor4Reading = sensor4.readRangeResult();
    if (sensor4.readRangeStatus() == 0) {
      distance4 = sensor4Reading;
    } else {
      distance4 = -1;
    }
  }

  // [10] DECISION: if no laptop is connected, check for one and start again
  if (!computer || !computer.connected()) {
    computer = server.available();
    if (computer) {
      computer.setNoDelay(true);
    }
    return;
  }

  // [11] DECISION: only send every 100 ms
  if (millis() - lastSendTime < 100) {
    return;
  }
  lastSendTime = millis();

  // [12] OUTPUT: send all four readings as one line, e.g. "243,-1,88,512"
  computer.print(distance1);
  computer.print(",");
  computer.print(distance2);
  computer.print(",");
  computer.print(distance3);
  computer.print(",");
  computer.println(distance4);
  Serial.printf("%d,%d,%d,%d\n", distance1, distance2, distance3, distance4);   // test only
}