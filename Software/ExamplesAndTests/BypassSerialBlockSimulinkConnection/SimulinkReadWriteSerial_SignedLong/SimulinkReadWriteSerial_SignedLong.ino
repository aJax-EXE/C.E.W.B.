
// Signed long serial communication with Simulink
// Packet format: #sXXXXXX,sXXXXXX\n
//   '#'      = start-of-frame marker
//   s        = sign: '+' or '-'
//   XXXXXX   = up to 6 digits (supports values up to ±999999)
//   ','      = separator between the two values
//   '\n'     = end-of-frame terminator

long v_motor   = 0;
long p_encoder = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    
    // Only respond if it starts with #
    if (line.length() > 0 && line.charAt(0) == '#') {
      long v = 0;
      long p = 0;
      int parsed = sscanf(line.c_str(), "#%ld,%ld", &v, &p);
      if (parsed == 2) {
        v_motor   = v;
        p_encoder = p;
      }

      // Send response immediately after receiving
      char tx_buf[20];
      snprintf(tx_buf, sizeof(tx_buf), "#%+07ld,%+07ld\n", v_motor, p_encoder);
      Serial.print(tx_buf);
    }
  }
  // No delay — just wait for next request
}
