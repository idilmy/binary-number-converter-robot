// Multi-solenoid cycle routine by Jeremy S. Cook
// W/ Arduino motor shield. Requires library found here: https://github.com/adafruit/Adafruit-Motor-Shield-library
// Modified from Public domain Adafruit Motor shield library

#include <AFMotor.h>

AF_DCMotor solenoid(1);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");

  solenoid.run(RELEASE); 
  // turn on motor
  solenoid.setSpeed(255);
  
}

void loop() {
  solenoid.run(BACKWARD);
  delay(250);
  solenoid.run(RELEASE);
  delay(5000);
}

int speed(int percent) {
  return map(percent, 0, 100, 0, 255);
}
