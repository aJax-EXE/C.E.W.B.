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

// Encoder on pins 2 and 3
CEWBEncoder enc1(2, 3, ENC2X);

// ── Change this to set the encoder limit (0 = disabled) ───
#define TARGET_POS 0
// ──────────────────────────────────────────────────────────

bool limit_hit = false;

void setup() {
  enc1.begin();
  Serial.begin(115200);
}

void loop() {

  // Wait for trigger byte from MATLAB ('R')
  if (Serial.available()) {
    Serial.read();  // consume trigger byte — content doesn't matter

    // Read encoder
    long p_encoder = enc1.getCount();

    // Check limit
    if (TARGET_POS != 0 && !limit_hit) {
      bool passed = (p_encoder >= TARGET_POS) ||
                    (p_encoder <= -TARGET_POS);
      if (passed) {
        limit_hit = true;
      }
    }

    // Reset limit if encoder returns close to zero
    if (abs(p_encoder) < 5) {
      limit_hit = false;
    }

    // Send response: #±XXXXXX,±XXXXXX:B\n
    // First value  = encoder count
    // Second value = TARGET_POS (for reference in MATLAB)
    // B            = 1 if limit reached, 0 otherwise
    char tx_buf[24];
    snprintf(tx_buf, sizeof(tx_buf), "#%+07ld,%+07ld:%d\n",
             p_encoder, (long)TARGET_POS, limit_hit ? 1 : 0);
    Serial.print(tx_buf);
  }
}
