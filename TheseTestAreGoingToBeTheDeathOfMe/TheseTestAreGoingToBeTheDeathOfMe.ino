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


void setup() 
{
  Serial.begin(9600); // Start serial communication at 9600 baud
 
  while (!Serial) { ; } // Wait for serial port to connect (for Leonardo/Micro/Zero)
  Serial.print("18Arduino is ready.\n");

}

void loop() 
{
   if (Serial.available() > 0) {
    
    char ReceivedChar = Serial.read();//Read data in the Buffer

    switch(ReceivedChar)
    {
      case 'A' : Serial.print("Character A  Received OK\n");break;
      case 'B' : Serial.print("Character B  Received OK\n");break;
      case 'C' : Serial.print("Character C  Received OK\n");break;
      default  : Serial.print("Invalid Character Received\n");
      
    }
    delay(500);
  }
}
