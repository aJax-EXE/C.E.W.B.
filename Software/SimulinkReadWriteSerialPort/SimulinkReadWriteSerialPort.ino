
int v_motor = 0123;
int p_encoder = 0456;
int int_aux = 0;
char str_aux[11];                // each number is defined as a string "XXXX,XXXX\n", where X is a digit [0:9]
  
void setup() {
  Serial.begin(115200);
  Serial.println("Beginning");
}

void loop() {

    // ---- Send data to Linux ----
      
    // ---->>>> read encoder here <<<<<---- 
    sprintf (str_aux, "%04d,%04d\n", v_motor , p_encoder );
    Serial.print(str_aux);
  
    // ---- Read data from Linux ----
    if (Serial.available()) {
      String line = Serial.readStringUntil('\n');
      sscanf(line.c_str(), "%04d,%04d", &v_motor, &p_encoder);
      // ---->>>> send voltage to the motor <<<<<----
      
    }
    delay(1);
    
}
