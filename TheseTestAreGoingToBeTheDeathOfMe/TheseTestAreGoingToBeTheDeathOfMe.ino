// // Arduino Echo Program (Serial Round-Trip)
// void setup() {
//   Serial.begin(9600);
//   while (!Serial) {
//     ; // wait for serial port (for boards like Leonardo)
//   }
//   Serial.println("Arduino is ready.");
// }

// void loop() {
//   if (Serial.available()) {
//     String received = Serial.readStringUntil('\n');  // Read user message
//     received.trim();  // Remove newline/extra spaces
//     Serial.print("Arduino received: ");
//     Serial.println(received);
//   }
// }

//Code is available on our GitHub 
//Arduino side Code 
void setup() 
{
 Serial.begin(9600); // Start serial communication at 9600 baud
 while (!Serial) { ; } // Wait for serial port to connect (for Leonardo/Micro/Zero)
}
void loop() 
{
  Serial.println("Hello from Arduino!"); // Send message to PC
  delay(1000); // Wait for 1 second
}
