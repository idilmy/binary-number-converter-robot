int binaryNumber[20];
int i = 0;

void setup() {
  Serial.begin(9600);
  
}

void loop() {

  Serial.println("Please input a decimal number you wish to convert to binary:");

  while (Serial.available() == 0) {
  }

  int decimalNumber = Serial.parseInt();

  while (decimalNumber > 0) {
    int remainder = decimalNumber % 2;
    binaryNumber[i] = remainder;
    decimalNumber = decimalNumber / 2;
    i++;
  }

  Serial.print("Binary number: ");
  for(int j = i - 1; j >= 0; j--) {
    Serial.print(binaryNumber[j]);
  }

  i=0;
  memset(binaryNumber, 0, sizeof(binaryNumber));

  Serial.println();

}