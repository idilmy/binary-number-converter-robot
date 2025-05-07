#include <AFMotor.h>

AF_DCMotor motor3(3);

void setup() {
  Serial.begin(9600);

}

void loop() {
  motor3.setSpeed(speed(50));
  motor3.run(FORWARD);

}

int  speed(int percent)
{
  return map(percent, 0, 100, 0, 255);
}
