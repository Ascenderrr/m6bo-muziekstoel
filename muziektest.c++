#include <Servo.h>
 
Servo s;
 
void setup() {
  s.attach(2);
 
  // Full speed (one direction)
 
s.writeMicroseconds(2000); // full speed forward
}
 
void loop() {
}