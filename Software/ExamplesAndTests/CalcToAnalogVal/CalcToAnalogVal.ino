String word2;
String preWord;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  int ans;
  float desireVolt;
  Serial.println("(From Arduino) - Please type a number between 0 and 24: ");
  if (Serial.available() > 0) {
    // Read data until a newline character is received
    Serial.print("(From Arduino) - Please Enter a Desired Voltage Value (0-24V): ");
    desireVolt = Serial.parseFloat();
    Serial.print("Desired Voltage: ");
    Serial.println(desireVolt);
    
    // Do the Math (My Calculation)
    ans = floor(((desireVolt)/24) * 255);

    //Print the Answer
    Serial.print("(From Arduino) - The number is: ");
    Serial.println(ans);
  }
  // word2 = Serial.readString();
  // if (word2 != preWord && word2 != "\0") {
  //   preWord = word2;
  // }
  // Serial.print("From Arduino ");
  // Serial.println(preWord);
  // // } 

}
