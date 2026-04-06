// Signed long serial communication with Simulink
// NEW Packet format: #sXXXXXX,sXXXXXX:b\n
//   '#'      = start-of-frame marker
//   s        = sign: '+' or '-'
//   XXXXXX   = up to 6 digits (supports values up to ±999999)
//   ','      = separator between the two values
//   ':'      = separator bit for the encoder value and the confirmation bit
//   b        = confirmation bit: 1 if value has reached its limit, 0 if the value has not yet
//   '\n'     = end-of-frame terminator 


// Custom Libraries
#include <CEWBEncoder.h>
#include <CEWBFunctions.h>

// Motor Drive PWM Pins and Values
#define MotorIn1 9   // Orange Wire
#define MotorIn2 10  // Blue Wire
uint8_t PWMVal = 0;

// Encoder
CEWBEncoder enc1(2, 3, ENC2X);

long v_motor   = 0;
long p_encoder = 0;
bool reachedLim = 0;

// Setup
void setup() {
  pinMode(MotorIn1, OUTPUT);
  pinMode(MotorIn2, OUTPUT);
  enc1.begin();
  Serial.begin(115200);
}

// Main Loop
void loop() {
  // ---- Read encoder ----
  long p_encoder = enc1.getCount();
  
  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    
    
    // Only respond if it starts with #
    if (line.length() > 0 && line.charAt(0) == '#') {
      long v = 0;
      long p = 0;
      bool check = 0;
      int parsed = sscanf(line.c_str(), "#%ld,%ld:%d", &v, &p, &check);
      if (parsed == 3) {
        v_motor   = v;
        p_encoder = p;
        reachedLim = check;
        
        // Checking if the limit has been reached or not
        if (reachedLim == 1) {
          // Begin stopping the motor here  
        }
        else {
          // Keep the motor running/ do nothing  
        }

      }

      // Send response immediately after receiving
      char tx_buf[20];
      snprintf(tx_buf, sizeof(tx_buf), "#%+07ld,%+07ld\n", v_motor, p_encoder);
      Serial.print(tx_buf);
    }
  }
}
