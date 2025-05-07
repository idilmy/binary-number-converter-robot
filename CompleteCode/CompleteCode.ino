#include <AFMotor.h>
#include <Servo.h>

// Motor for arm
AF_DCMotor armMotor(3);
// Motor for belt
AF_DCMotor beltMotor(4);
// Motot for solenoid
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

//Fix piston taking
int count = 0;

// Set pins and values for laser-trip system
int ldr1  = 1; // analogue pin
int ldr1_value = 0;
int ldr2  = 0; // analogue pin
int ldr2_value = 0;
int ldrmain = 3; // analogue pin 
int ldrmain_value = 0;  

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

  // Prepare solenoid
  solenoid.run(RELEASE);
  
  // Setup Serial Monitor
  Serial.begin(9600);

}

void loop() {
  count = 0;
  Serial.println();
  Serial.println("Please input a decimal number to convert to binary:");
  takeInput();
  decimalToBinary();

  delay(2000);

  // Start the conveyor belt
  beltMotor.setSpeed(speed(100));
  beltMotor.run(FORWARD); 

  Piston(); //Take the first disk

  // Iterate through the binary number 
  for (int k = 0; k < sizeof(binaryNumber) / sizeof(int); k++) {
    ldr1_value = analogRead(ldr1); // Read from the secondary laser 
    ldr2_value = analogRead(ldr2); // Read from the the main laser 

    if ((ldr1_value > 300) && (ldr2_value > 300)) { // If both lasers are triggered at the same time 
      Serial.println("Unrecognized object or disks too close together"); 
      k--;
      digitalWrite(13, HIGH); // Turn on the LED to signify error
      delay(14000); // Wait for the large object to move away from the secondary laser
      digitalWrite(13, LOW); // Turn off the LED
      Piston();
      //  Serial.println("Two:");
      //  Serial.print(ldr2_value);
      //  Serial.println();
      //  Serial.println("One:");
      //  Serial.print(ldr1_value);
    } else if ((binaryNumber[k] == 1) && (detectBlackDisc() == true) && (ldr2_value > 300)) { 
        Serial.println("Taking black disk");
        count += 1;
        selectDisk();
        Piston();
    } else if ((binaryNumber[k] == 0) && (detectWhiteDisc() == true) && (ldr2_value > 300)) {
        Serial.println("Taking white disk");
        count += 1;
        selectDisk();
        Piston();
    } else if ((detectRandomColor() == true) && (ldr2_value > 300)) { // Movement is detected but the object is neither black, nor white
        Serial.println("Disk is not a recognized color");
        digitalWrite(13, HIGH); // Turn on the LED to signify error
        delay(1000); 
        digitalWrite(13, LOW); // Turn off the LED
        k--;
        Piston();
    } else if ((binaryNumber[k] == 1) && (detectWhiteDisc() == true) && (ldr2_value > 300)) {
        Serial.println("Wrong color");
        k--;
        Piston(); 
    } else if ((binaryNumber[k]== 0 ) && (detectBlackDisc() == true) && (ldr2_value > 300)) {
        Serial.println("Wrong color");
        k--;
        Piston(); 
    } else { // No movement detected 
      //  Serial.println("Two:");
      //  Serial.print(ldr2_value);
      //  Serial.println();
      //  Serial.println("One:");
      //  Serial.print(ldr1_value);
        k--;
    }
  }

  beltMotor.setSpeed(speed(0)); //Stop the belt  
  Flag();  // Wave the flag
  Repeat(); // Ask the user whether to restart or stop
}


// Function turns on arm
void selectDisk() {
  delay(6220); // Wait for the disk to reach the arm 

  armMotor.setSpeed(speed(80)); // Turn the arm on 
  armMotor.run(BACKWARD);
 
  delay(950); 
  armMotor.setSpeed(speed(0)); // Turn the arm off 

}


//Wait for user input 
void Hold() {
  while (!Serial.available()) {
    // Wait 
  }
}

// Takes user input 
void takeInput() {
  Hold(); 
  String input = Serial.readString(); 
  input.trim(); // Remove whitespace 
  decimalNumber = input.toInt(); // Try to parse the input to int 
  if (decimalNumber == 0 && !input.equals("0")) { // extra condition, since toInt() parses strings as 0 
    Serial.println("This doesn't seem like an integer. Please input an integer in the range 0-15:");
    digitalWrite(13, HIGH); // Turn on the LED to signify error
    delay(1000); 
    digitalWrite(13, LOW); // Turn off the LED
    takeInput();
  } else if (decimalNumber < 0 ||  decimalNumber > 15) { // Outside of the allowed range 
    Serial.println("This was outside of the allowed range");
    digitalWrite(13, HIGH); // Turn on the LED to signify error
    delay(1000); 
    digitalWrite(13, LOW); // Turn off the LED
    takeInput(); 
  }
}

// Converts decimal to binary
void decimalToBinary() {
  for (int n = 0; n <= sizeof(binaryNumber) / sizeof(int); n++) {
    binaryNumber[n] = 0; // Reset the previous binary number
  }

  i = 0;// Reset the index variable i

  while (decimalNumber > 0) { // Standard decimal to binary conversion 
    int remainder = decimalNumber % 2;
    binaryNumber[i] = remainder;
    decimalNumber = decimalNumber / 2;
    i++;
  }

  Serial.print("Binary number: ");

  // Pad with 0s if necessary, to make it 4-bit 
  for (int j = 3; j>=i; j--) {
    Serial.print("0");
  }
  
  // Print the binary number 
  for (int j = i - 1; j >= 0; j--) {
    Serial.print(binaryNumber[j]);
  }
  Serial.println();
}

// Take a disk from the demo conveyor belt 
void Piston() {
  if (count < 4) {
    //Serial.print("push");
    ldrmain_value = analogRead(ldrmain);
    while (ldrmain_value < 300) {
      ldrmain_value = analogRead(ldrmain);
    }
    delay(2450);
    solenoid.setSpeed(120);
    solenoid.run(FORWARD);
    delay(1000);
    solenoid.run(RELEASE);
    delay(5000); 
  }
}

//Function to wave the victory flag 
void Flag() { 
  Serial.println("Waving the flag!");  // Report on internal state 
  for (pos = 0; pos <= 180; pos += 1) { // Go from 0 degrees to 180 degrees
    flagServo.write(pos);              
    delay(15);                      
  }
  for (pos = 180; pos >= 0; pos -= 1) { // Go from 180 degrees to 0 degrees
    flagServo.write(pos);              
    delay(15);                       
  }
}

// Repeat or stop program 
void Repeat() {
  Serial.println("Go again? (y/n)");
  Hold(); // Wait for input 
  String answer = Serial.readStringUntil('\n');
  answer.trim();  // Remove whitespace

  while (!answer.equals("y")) { 
    if (answer.equals("n")) {
      Serial.print("Goodbye!");
      while (true) {
        // Stop program
      }
    }
    // In case of incorrect input
    Serial.println("Please input y or n.");
    Hold();
    answer = Serial.readStringUntil('\n');
    answer.trim();
  } 
}


// Read the values from the color sensor and return the RGB value 
int findColor() {
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

  return (redValue + greenValue + blueValue);
}

// Boolean function to detect black discs
bool detectBlackDisc() {
  int sum = findColor();
 
  if (sum < 150) {
    return true;
  } else {
    return false;
  }

}

// Boolean function to detect white discs
bool detectWhiteDisc() {
  int sum = findColor();

  if (sum > 700) {
    return true;
  } else {
    return false;
  }
  
}

// Boolean function to detect colors that are not black or white
bool detectRandomColor() {
  int sum = findColor();

  if (sum > 150 && sum < 700) {
    return true;
  } else {
    return false;
  }
}

// Convert arbitrary motor speed to a percentage
int  speed(int percent) {
  return map(percent, 0, 100, 0, 255);
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