#include <AFMotor.h>
#include <Servo.h>

// Motor for arm
AF_DCMotor armMotor(3);
// Motor for belt
AF_DCMotor beltMotor(4);
// Solenoid
AF_DCMotor solenoid(1);

// Servomotor for flag
Servo flagServo;
// Position of the Servomotor arm
int pos = 0;

// Pins for color sensor
#define S0 2
#define S1 3
#define S2 A2
#define S3 10
#define sensorOut 11

// Calibration Values
int redMin = 30; // Red minimum value
int redMax = 65; // Red maximum value
int greenMin = 53; // Green minimum value
int greenMax = 130; // Green maximum value
int blueMin = 43; // Blue minimum value
int blueMax = 118; // Blue maximum value

// Variables for Color Pulse Width Measurements
int redPW = 0;
int greenPW = 0;
int bluePW = 0;

// Variables for final Color values
int redValue;
int greenValue;
int blueValue;

// Variables for decimal conversion
int decimalNumber; //to store user input
int i = 0;
int binaryNumber[4];

// Sets pins and values for laser-trip system
int ldr1  = 1; // analogue pin
int ldr1_value = 0;
int ldr2  = 0; // analogue pin
int ldr2_value = 0;
int ldrmain = 3; // analogue pin
int ldrmain_value = 0;
int p = 0;

void setup() {

  // Set S0 - S3 as outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Set Sensor output as input
  pinMode(sensorOut, INPUT);
  
  // Set Frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  // Pin for LED light
  pinMode (13, OUTPUT);

  // Pin for Servomotor with flag
  flagServo.attach(9);
  
  // Setup Serial Monitor
  Serial.begin(9600);

}

void loop() {

  Serial.println("Please input a decimal number to convert to binary:");
  takeInput();
  decimalToBinary();

  delay(2000);

  mainSolenoid();

  beltMotor.setSpeed(speed(100));
  beltMotor.run(FORWARD); 

  ldr1_value = analogRead(ldr1);
  ldr2_value = analogRead(ldr2);
  // ldr3_value = analogRead(ldr3);

  // pads with extra 0's, so all numbers are represented as 4 bits
  for (int k = 0; k < sizeof(binaryNumber) / sizeof(int); k++) {
    ldr1_value = analogRead(ldr1);
    ldr2_value = analogRead(ldr2);

    if ((ldr1_value > 300) && (ldr2_value > 300)) {
      k--;
      digitalWrite(13, HIGH);
      delay(14000);
      digitalWrite(13, LOW);
    } else if ((binaryNumber[k] == 1) && (detectBlackDisc() == true) && (ldr2_value > 300)) {
        selectDisk();
    } else if ((binaryNumber[k] == 0) && (detectWhiteDisc() == true) && (ldr2_value > 300)) {
        selectDisk();
    } else if ((detectRandomColor() == true) && (ldr2_value > 300)) {
      Serial.println("Disk is not a recognized color");
      digitalWrite(13, HIGH);
      delay(1000);
      digitalWrite(13, LOW);
      k--;
    } else {
      k--;
    }
  }

  beltMotor.setSpeed(speed(0));

  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    flagServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    flagServo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  Serial.println("Go again? (y/n)");

  while (!Serial.available()) {
    // Wait for input
  }

  String answer = Serial.readStringUntil('\n');
  answer.trim();  // Remove whitespace

  while (!answer.equals("y")) { 
    if (answer.equals("n")) {
      while (true) {
        // Stops program
      }
    }
    Serial.println("Please input y or n.");
    while (!Serial.available()) {
    // Wait for input
    }
    answer = Serial.readStringUntil('\n');
    answer.trim();
  } 
}

// Function gets every fourth disc off of main belt
void mainSolenoid() {
  if (ldrmain_value > 300) {
    if (p % 4 == 0) {
      solenoid.setSpeed(255);
      solenoid.run(FORWARD);

      delay(1120);
      solenoid.setSpeed(speed(0));
    }
    p++;
  }

// Function turns on arm
void selectDisk() {
  delay(8150);

  armMotor.setSpeed(speed(80));
  armMotor.run(BACKWARD);

  delay(1200);
  armMotor.setSpeed(speed(0));

}

// Converts arbitrary motor speed to a percentage
int  speed(int percent)
{
  return map(percent, 0, 100, 0, 255);
}

//Waits for user input 
void Hold() {
  while (Serial.available() == 0) {}
}

//Takes user input 
void takeInput() {
  Hold(); 
  decimalNumber = Serial.parseInt();
  if (decimalNumber == 0 && Serial.readString() != "0") {
    Serial.println("Please input an integer in the range 0-15:");
    takeInput();
  } else if (decimalNumber < 0 ||  decimalNumber > 15) {
    Serial.println("This was outside of the allowed range");
    takeInput(); 
  }
}

// Converts decimal to binary
void decimalToBinary() {
  for (int n = 0; n <= sizeof(binaryNumber) / sizeof(int); n++) {
    binaryNumber[n] = 0;
  }
  // Reset the index variable i
  i = 0;

  while (decimalNumber > 0) {
    int remainder = decimalNumber % 2;
    binaryNumber[i] = remainder;
    decimalNumber = decimalNumber / 2;
    i++;
  }

  Serial.print("Binary number: ");

  for (int j = i - 1; j >= 0; j--) {
    Serial.print(binaryNumber[j]);
  }
  Serial.println();
}

// Boolean function to detect black discs
bool detectBlackDisc() {
  
  // Read Red value
  redPW = getRedPW();
  // Map to value from 0-255
  redValue = map(redPW, redMin,redMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Read Green value
  greenPW = getGreenPW();
  // Map to value from 0-255
  greenValue = map(greenPW, greenMin,greenMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Read Blue value
  bluePW = getBluePW();
  // Map to value from 0-255
  blueValue = map(bluePW, blueMin,blueMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  if ((blueValue + greenValue + redValue) < 150) {
    return true;
  } else {
    return false;
  }

}

// Boolean function to detect white discs
bool detectWhiteDisc() {
  
  // Read Red value
  redPW = getRedPW();
  // Map to value from 0-255
  redValue = map(redPW, redMin,redMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Read Green value
  greenPW = getGreenPW();
  // Map to value from 0-255
  greenValue = map(greenPW, greenMin,greenMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Read Blue value
  bluePW = getBluePW();
  // Map to value from 0-255
  blueValue = map(bluePW, blueMin,blueMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  if ((blueValue + greenValue + redValue) > 700) {
    return true;
  } else {
    return false;
  }
  
}

bool detectRandomColor() {
  // Read Red value
  redPW = getRedPW();
  // Map to value from 0-255
  redValue = map(redPW, redMin,redMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Read Green value
  greenPW = getGreenPW();
  // Map to value from 0-255
  greenValue = map(greenPW, greenMin,greenMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  // Read Blue value
  bluePW = getBluePW();
  // Map to value from 0-255
  blueValue = map(bluePW, blueMin,blueMax,255,0);
  // Delay to stabilize sensor
  delay(200);
  
  int summed = (blueValue + greenValue + redValue);
  if (summed > 150 && summed < 700) {
    return true;
  } else {
    return false;
  }
}

// Function to read Red Pulse Widths
int getRedPW() {

  // Set sensor to read Red only
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}

// Function to read Green Pulse Widths
int getGreenPW() {

  // Set sensor to read Green only
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}

// Function to read Blue Pulse Widths
int getBluePW() {

  // Set sensor to read Blue only
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Define integer to represent Pulse Width
  int PW;
  // Read the output Pulse Width
  PW = pulseIn(sensorOut, LOW);
  // Return the value
  return PW;

}
