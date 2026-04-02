
// Signed long serial communication with Simulink
// Packet format: #sXXXXXX,sXXXXXX\n
//   '#'      = start-of-frame marker
//   s        = sign: '+' or '-'
//   XXXXXX   = up to 6 digits (supports values up to ±999999)
//   ','      = separator between the two values
//   '\n'     = end-of-frame terminator

long v_motor   = 123;    // Value 1 (e.g. motor voltage command)
long p_encoder = -456;   // Value 2 (e.g. encoder position, can be negative)

char tx_buf[20];  // Transmit buffer: "#sXXXXXX,sXXXXXX\n" = 18 chars + null
char rx_buf[20];  // Receive  buffer

void setup() {
  Serial.begin(500000);
  Serial.println("Beginning");
}

void loop() {

  // ---- Send data to Simulink ----
  // Format: #sXXXXXX,sXXXXXX\n
  // ---->>>> read encoder / sensor here <<<<----
  snprintf(tx_buf, sizeof(tx_buf), "#%+07ld,%+07ld\n", v_motor, p_encoder);
  //   %+07ld  =>  sign always printed (+/-), zero-padded, 6 digits, long int
  //   Result example:  #+000123,-000456\n
  Serial.print(tx_buf);

  // ---- Read data from Simulink ----
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    Serial.print("GOT: ");
    Serial.println(line);   // echo back for debugging

    // Expect the '#' start marker
    if (line.length() > 1 && line.charAt(0) == '#') {
      // Parse the two signed longs after the '#'
      // sscanf handles the +/- sign automatically with %ld
      int parsed = sscanf(line.c_str() + 1, "%ld,%ld", &v_motor, &p_encoder);

      if (parsed == 2) {
        // ---->>>> use v_motor and p_encoder here <<<<----
        // e.g. analogWrite(MOTOR_PIN, map(v_motor, -999999, 999999, 0, 255));
      }
      // If parsed != 2, the packet was malformed — silently discard it
    }
  }

  delay(10);
}
