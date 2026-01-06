// ================= PID PARAMETERS =================
float Kp = 1.0;
float Ki = 0.0;
float Kd = 0.0;

// ================= PID VARIABLES =================
float setpoint = 0.0;     // Desired value
float input = 0.0;        // Measured value
float output = 0.0;       // Controller output

float error = 0.0;
float prevError = 0.0;
float integral = 0.0;

unsigned long lastTime = 0;
const unsigned long sampleTime = 100; // ms

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  lastTime = millis();
}

// ================= LOOP =================
void loop() {
  unsigned long now = millis();

  if (now - lastTime >= sampleTime) {
    float dt = (now - lastTime) / 1000.0;

    // --------- READ INPUT (sensor placeholder) ---------
    input = readInput();   // <-- replace with your sensor

    // --------- PID COMPUTATION ---------
    error = setpoint - input;

    integral += error * dt;
    integral = constrain(integral, -1000, 1000); // anti-windup

    float derivative = (error - prevError) / dt;
    prevError = error;

    output = Kp * error + Ki * integral + Kd * derivative;

    // --------- APPLY OUTPUT (actuator placeholder) ---------
    applyOutput(output);   // <-- replace with your actuator

    // --------- DEBUG ---------
    Serial.print("SP: ");
    Serial.print(setpoint);
    Serial.print(" | IN: ");
    Serial.print(input);
    Serial.print(" | OUT: ");
    Serial.println(output);

    lastTime = now;
  }
}

// ================= PLACEHOLDER FUNCTIONS =================
float readInput() {
  // Replace with sensor reading
  // Example: analogRead(A0)
  return 0.0;
}

void applyOutput(float value) {
  // Replace with actuator command
  // Example: analogWrite(PWM_PIN, value)
}

