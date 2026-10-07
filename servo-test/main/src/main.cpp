#include <Arduino.h>
#include <ESP32Servo.h>
 
// SETUP
const int servoPin = 13;
const int baudRate = 9600;
const int startPosition = 90;
 
Servo servoMotor;
 
void setup() {
  Serial.begin(baudRate);
  servoMotor.setPeriodHertz(50);
  servoMotor.attach(servoPin, 500, 2400);
  servoMotor.write(startPosition);
}
 
void loop() {
  
  if (Serial.available() > 0) {
    String incomingText = Serial.readStringUntil('\n');
    incomingText.trim();
 
    
    if (incomingText.length() > 0 && isDigit(incomingText.charAt(0))) { 
      int targetPosition = incomingText.toInt();  
      targetPosition = constrain(targetPosition, 0, 180); 
 
      // OUTPUT
      servoMotor.write(targetPosition); 
      Serial.println("Moved to: " + String(targetPosition)); 
    }
  }
}
