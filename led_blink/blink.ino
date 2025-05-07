void setup() {
pinMode (2, OUTPUT); // to set the OUTPUT mode of pin number 8.
}

void loop() {
digitalWrite (2, HIGH);   
delay(1000);  // 1 second = 1 x 1000 milliseconds  
digitalWrite (2, LOW);  
delay(500);  // 0.5 second = 0.5 x 1000 milliseconds  

}
