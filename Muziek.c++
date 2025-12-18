#include <Servo.h>
 
#define buttonPin 5
Servo servoButton;
const int buttonStart = 90;
const int buttonShakeRange = 45;
const int shakeDelay = 50;
 
#define trigPin 9
#define echoPin 8
Servo servoDistance1;
Servo servoDistance2;
const int stopPulseDistance = 1500;
const int forwardPulseDistance = 2000;
const int triggerDistance = 5;
 
void setup() {
  Serial.begin(9600);
 
  pinMode(buttonPin, INPUT_PULLUP);
  servoButton.attach(10);
  servoButton.write(buttonStart);
 
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servoDistance1.attach(6);
  servoDistance2.attach(7);
  servoDistance1.writeMicroseconds(stopPulseDistance);
  servoDistance2.writeMicroseconds(stopPulseDistance);
}
 
void loop() {
  int buttonState = digitalRead(buttonPin);
  bool buttonSpinning = false;
 
  if (buttonState == LOW) {
    Serial.println("Button Pressed");
    servoButton.write(buttonStart);
    buttonSpinning = false;
  } else {
    Serial.println("Button Released");
    buttonSpinning = true;
 
    servoButton.write(buttonStart - buttonShakeRange);
    delay(shakeDelay);
    servoButton.write(buttonStart + buttonShakeRange);
    delay(shakeDelay);
    servoButton.write(buttonStart);
  }
 

  if (buttonSpinning) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
 
    long duration = pulseIn(echoPin, HIGH);
    long distance = (duration / 2) / 29.1;
 
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
 
    if (distance >= 0 && distance <= triggerDistance) {
      servoDistance1.writeMicroseconds(forwardPulseDistance);
      servoDistance2.writeMicroseconds(forwardPulseDistance);
    } else {
      servoDistance1.writeMicroseconds(stopPulseDistance);
      servoDistance2.writeMicroseconds(stopPulseDistance);
    }
  } else {
    servoDistance1.writeMicroseconds(stopPulseDistance);
    servoDistance2.writeMicroseconds(stopPulseDistance);
  }
 
  delay(50);
}

