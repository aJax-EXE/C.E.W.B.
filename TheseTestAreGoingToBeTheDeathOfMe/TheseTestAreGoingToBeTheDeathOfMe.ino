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
  Serial.println("Arduino is ready.");

}

void loop() 
{
   if (Serial.available() > 0) {
    
    char ReceivedChar = Serial.read();//Read data in the Buffer

    switch(ReceivedChar)
    {
      case 'A' : Serial.println("26Character A  Received OK");break;
      case 'B' : Serial.println("26Character B  Received OK");break;
      case 'C' : Serial.println("26Character C  Received OK");break;
      default  : Serial.println("28Invalid Character Received");
      
    }
    delay(500);
  }
}
