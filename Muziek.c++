#include <Servo.h>
#include <math.h>

Servo blueServo1;
Servo blueServo2;
Servo spinServo;   // continuous rotation servo

const int buttonPin = 2;
const int trigPin = 3;
const int echoPin = 4;

// Blue servo shake parameters
const int shakeCenter = 90;
const int shakeAmplitude = 15;
const float shakeFrequency = 0.005;
unsigned long startTime = 0;

// Ultrasonic sensor
unsigned long lastUltrasonicTime = 0;
const int ultrasonicInterval = 150;
long distance = 0;
bool objectIsClose = false;

// Serial monitor timing
unsigned long lastSerialTime = 0;
const int serialInterval = 150;

// 360° servo microseconds
const int spinStop = 1500;     // STOP
const int spinFull = 2000;     // FULL SPEED FORWARD

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
  spinServo.writeMicroseconds(spinStop);

  startTime = millis();
  Serial.println("System Ready");
}

void loop() {
  unsigned long currentTime = millis();

  // -------- BUTTON --------
  bool buttonPressed = (digitalRead(buttonPin) == LOW); // Button pressed means LOW
  bool buttonReleased = (digitalRead(buttonPin) == HIGH); // Button released means HIGH

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

  // -------- CONTINUOUS ROTATION SERVO --------
  // Activate when BUTTON is RELEASED and OBJECT is CLOSE
  if (buttonReleased && objectIsClose) {
    spinServo.writeMicroseconds(spinFull);   // FULL SPEED
  } else {
    spinServo.writeMicroseconds(spinStop);   // STOP
  }

  // -------- BLUE SERVOS (SHAKE) --------
  // If spin servo is active, turn off blue servos
  if (spinServo.readMicroseconds() == spinFull) {
    blueServo1.write(shakeCenter);
    blueServo2.write(shakeCenter);
  } else {
    // Activate blue servos when BUTTON is RELEASED
    if (buttonReleased) {
      float elapsed = currentTime - startTime;
      int offset = shakeAmplitude * sin(2 * PI * shakeFrequency * elapsed);
      int servoPos = shakeCenter + offset;

      blueServo1.write(servoPos);
      blueServo2.write(servoPos);
    } else {
      blueServo1.write(shakeCenter);
      blueServo2.write(shakeCenter);
      startTime = currentTime;
    }
  }

  // -------- SERIAL MONITOR --------
  if (currentTime - lastSerialTime >= serialInterval) {
    lastSerialTime = currentTime;
    Serial.print("Button Released: ");
    Serial.print(buttonReleased);
    Serial.print(" | Distance: ");
    Serial.print(distance);
    Serial.print(" cm | Object Close: ");
    Serial.println(objectIsClose);
  }

  delay(1);
}
