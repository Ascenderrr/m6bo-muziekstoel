#include <Servo.h>
#include <math.h> // for sin()

Servo blueServo1;   // original blue servo
Servo blueServo2;   // new blue servo
Servo spinServo;    // continuous rotation servo

const int buttonPin = 2;
const int trigPin = 3;
const int echoPin = 4;

// Blue servo smooth shake parameters
const int shakeCenter = 90;
const int shakeAmplitude = 15;   
const float shakeFrequency = 0.005; 
unsigned long startTime = 0;

// Ultrasonic sensor timing
unsigned long lastUltrasonicTime = 0;
const int ultrasonicInterval = 150; 
long distance = 0;
bool objectIsClose = false;

// Serial monitor timing
unsigned long lastSerialTime = 0;
const int serialInterval = 150; // print every 150ms

// 360° servo spin speed
const int spinStop = 90;       
const int spinSpeed = 140;     

void setup() {
  Serial.begin(9600);

  blueServo1.attach(9);
  blueServo2.attach(6);
  spinServo.attach(5);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  blueServo1.write(shakeCenter);
  blueServo2.write(shakeCenter);
  spinServo.write(spinStop);

  startTime = millis();
  Serial.println("System Ready");
}

void loop() {
  unsigned long currentTime = millis();

  // -------- BUTTON --------
  bool buttonReleased = (digitalRead(buttonPin) == HIGH);

  // -------- ULTRASONIC SENSOR --------
  if (currentTime - lastUltrasonicTime >= ultrasonicInterval) {
    lastUltrasonicTime = currentTime;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH, 30000); 
    if (duration > 0) {  
      distance = duration * 0.034 / 2;  
      objectIsClose = (distance <= 8);
    }
  }

  // -------- BLUE SERVOS (SMOOTH UP-AND-DOWN SHAKE) --------
  if (buttonReleased && objectIsClose) {
    float elapsed = currentTime - startTime;
    int offset = shakeAmplitude * sin(2 * 3.1416 * shakeFrequency * elapsed);
    int servoPos = shakeCenter + offset;

    blueServo1.write(servoPos);
    blueServo2.write(servoPos);
  } else {
    blueServo1.write(shakeCenter);
    blueServo2.write(shakeCenter);
    startTime = currentTime; 
  }

  // -------- CONTINUOUS ROTATION SERVO --------
  if (buttonReleased) {
    spinServo.write(spinSpeed);
  } else {
    spinServo.write(spinStop);
  }

  // -------- SERIAL MONITOR (THROTTLED) --------
  if (currentTime - lastSerialTime >= serialInterval) {
    lastSerialTime = currentTime;
    Serial.print("Button Released: ");
    Serial.print(buttonReleased);
    Serial.print(" | Distance: ");
    Serial.print(distance);
    Serial.print(" cm");
    Serial.print(" | Object Close: ");
    Serial.println(objectIsClose);
  }

  delay(1); // smooth loop
}
