# C.E.W.B.: The Open Source Teaching Platform for Control Systems
**ECEN4620/MCEN4085: Senior Design Project - University of Colorado Boulder**  
**Sponsors:** Talles Santos  
&emsp;&emsp;&emsp;&emsp;&emsp; Nathan McNeil  
**Students:** [Alyssa Jackson](https://github.com/aJax-EXE) (Electrical and Computer Engineering)  
&emsp;&emsp;&emsp;&emsp;&emsp;Eric Pollert (Mechanical Engineering)  
&emsp;&emsp;&emsp;&emsp;&emsp;[Isaiah Knox](https://github.com/iknox001) (Mechanical Engineering)  

---

## Table of Contents
1. [Introduction](#introduction)
2. [Project Overview](#project-overview)
3. [System Specifications](#system-specifications)
4. [Repository Structure](#repository-structure)
5. [Hardware](#hardware)
   - [Circuit Design](#circuit-design)
   - [Accessory Storage](#accessory-storage)
   - [Device Deck](#device-deck)
   - [Motor Housing](#motor-housing)
   - [Electrical Compartment](#electrical-compartment)
   - [Attachments](#attachments)
6. [Software](#software)
   - [Dependencies](#dependencies)
   - [Arduino Code](#arduino-code)
   - [MATLAB/Simulink Interface](#matlabsimulink-interface)
   - [Custom Libraries](#custom-libraries)
7. [Getting Started](#getting-started)
   - [Hardware Setup](#hardware-setup)
   - [Software Setup](#software-setup)
   - [Running Your First Experiment](#running-your-first-experiment)
8. [Lab Experiments](#lab-experiments)
   - [Angular Position Control (Inertia Disc)](#angular-position-control-inertia-disc)
   - [Inverted Pendulum Balancing](#inverted-pendulum-balancing)
9. [Results](#results)
10. [Known Limitations](#known-limitations)
11. [Next Steps](#next-steps)
12. [Contributing](#contributing)
13. [License](#license)

---

## Introduction

C.E.W.B. (Control Engineering Workbench) is a low-cost, open-source electromechanical teaching platform designed for undergraduate control systems laboratories. It was developed as a direct functional replacement for the Quanser Qube-Servo 2, a $5,000 proprietary device, at a projected per-unit production cost of $763.68.

The device supports the same core laboratory experiments as the Qube-Servo 2, including angular position control with an inertia disc and inverted pendulum balancing. It is designed to be self-contained, portable, and fully compatible with MATLAB and Simulink.

All design files, code, and documentation are publicly available in this repository so that the device can be reproduced by any institution without reliance on proprietary hardware or software.

---

## Project Overview

This project set out to design a low-cost, open-source replacement for the Quanser Qube-Servo 2, a $5,000 device used in control systems classes. The target was a fully functional prototype delivered by May 2026 at under $1,000 per unit, capable of supporting the same lab experiments, including angular position control with an inertia disc and inverted pendulum balancing, while being self-contained, portable, and compatible with MATLAB/Simulink.

The design integrates all components into a Nanuk 905 protective case with built-in storage for attachments, cables, and the power supply. The system uses an Arduino microcontroller, a motor driver, and a custom PCB, with two interchangeable magnetic attachments — an inertia disc and a pendulum — mirroring the Qube-Servo 2's functionality. The projected per-unit production cost of $763.68 comes in well under the $1,000 target.

The device met most implementation requirements. Data transmission, MATLAB/Simulink compatibility, third-party manufacturability, self-contained storage, and transport protection all passed evaluation. Open-source documentation, including CAD files, a bill of materials, and assembly instructions, has been prepared and made publicly available.

However, the device did not fully replicate the closed-loop control performance of the Qube-Servo 2. Arduino's data transmission rate is approximately an order of magnitude slower than the proprietary controller used in the Qube-Servo 2, causing significant control loop delays and excessive overshoot during inertia disc testing. As a result, the pendulum balancing test was not attempted.

The primary recommended next step is to replace the Arduino with a faster microcontroller, retest the system using the same control experiments, and proceed to a small production run once performance requirements are confirmed to be met.

---

## System Specifications

The following table lists the key performance parameters of the C.E.W.B. system and its Qube-Servo 2 equivalents for reference.

| Parameter | C.E.W.B. | Qube-Servo 2 |
|---|---|---|
| Motor | Maxon DC-max 26 S Ø26 mm | Maxon (discontinued) |
| Nominal input voltage | 24 V | 18 V |
| Nominal torque | 22.0 mN-m | 22.0 mN-m |
| Nominal speed | 3050 RPM | 3050 RPM |
| Encoder line count | 512 lines/rev | 512 lines/rev |
| Encoder resolution (quadrature) | 2048 counts/rev | 2048 counts/rev |
| Encoder resolution (deg) | 0.176 deg/count | 0.176 deg/count |
| Encoder resolution (rad) | 0.00307 rad/count | 0.00307 rad/count |
| Motor controller | Adafruit DRV8871 | PWM amplifier |
| Microcontroller | Arduino Uno R3 | Proprietary |
| Data transmission rate | ~50 Hz (USB serial) | ~500 Hz (proprietary) |
| Output voltage range | ±24 V | ±10 V (recommended) |
| Per-unit cost (production) | $763.68 | $5,000.00 |

---

## Repository Structure

```
C.E.W.B./
├── Hardware/
│   ├── 3DModels/
│   │   └── CurrentVersion/       # CAD files for all custom parts
│   ├── PCB/                      # PCB schematic and layout files
│   └── BOM/                      # Bill of materials
├── Software/
│   ├── Arduino/
│   │   ├── WorkingModel/         # Main Arduino sketch
│   │   └── ArduinoTests/         # Diagnostic and test sketches
│   │       ├── EncoderQuadTest/
│   │       └── MotorRampSpeedTest/
│   ├── MATLAB/
│   │   ├── ArduinoSerial.m       # MATLAB System block for Simulink
│   │   └── Libraries/
│   │       ├── CEWBEncoder/      # Custom encoder library
│   │       └── CEWBFunctions/    # Motor and utility functions
│   └── Simulink/
│       └── PIDController.slx     # Example Simulink model
├── Documentation/
│   ├── DesignReport.pdf
│   └── TechnicalInstructions.pdf
└── README.md
```

---

## Hardware

### Circuit Design

The electrical system is built around an Arduino Uno R3 microcontroller and an Adafruit DRV8871 motor driver, both connected through a custom PCB. The PCB acts as a junction point between the motor, encoders, and microcontroller, consolidating all wiring and reducing the risk of loose connections. A 24V Mean Well GST90A24-P1M power supply provides power to the system via a front-panel power port. A front-panel USB-B port connects the Arduino to the user's laptop for data transmission.

The PCB is insulated from the aluminum mounting panel using an acrylic backing plate to prevent electrical interference.

### Accessory Storage

The accessory storage compartment is made up of two pieces of hardboard and one piece of foam with cutouts for the attachments to fit into. The rear panel of hardboard has magnets glued into it which allow the attachments to be secured in their respective locations during storage. Hardboard was selected for the front and rear panel as it is a light and cheap but sturdy material. Foam was chosen for the middle panel as it is low density and can fill the space between the boards without increasing the weight significantly. The three panels are glued together with super glue and are fixed into the top of the Nanuk 905 case using thread forming screws.

### Device Deck

The main part of the deck of the device is a bent piece of aluminum sheet metal which is screwed into the Nanuk 905 case using thread forming screws and serves as a mounting plate for the rest of the components. This part is bent the way it is in order to create a compartment for storing the power supply and the data transmission cable. The top surface of this panel has two ports. A USB port where the data transmission cable is plugged in and a power port where the device is connected to the power supply. The ports were added to clean up the visual appearance of the device and remove the need for cables to be plugged directly into the electrical compartment. On the right side of the panel there is a cavity with a u-shaped bent piece of aluminum sheet metal mounted to the bottom of the panel which is used both as a mounting location for the motor housing and as a barrier between the user and the internal components of the device. The cavity is required to allow the motor housing to fold into the device when it is being stored. The trough contains a hole to allow wires to pass through between the motor housing and the electrical compartment and this hole is lined with a silicone grommet which protects the wires passing through the hole from fraying when the motor housing is moved between its storage and operation positions.

### Motor Housing
The motor housing consists of 3 main structural components which are all aluminum sheet metal. The two motor housing legs give the housing the necessary height required for the use of the pendulum attachment and also serve as the location where the hinge is mounted to the housing which allows the housing to move between operational and storage positions. The hinge consists of a partially threaded socket head screw, nuts, and nylon washers. The partially threaded screw was chosen to allow for a smooth surface for the hinge to turn on. The nylon washers were used to secure the housing in place during operation but still allow for rotation given enough force to slightly deform the nylon. The two washers at the end are jammed together to secure the hinge at a fixed tightness calibrated based on ensuring the hinge can be rotated but is still secured during operation. The third structural component is the motor mounting plate and has several components attached to it as well as a bend that serves as a handle when switching the device between positions. This plate is attached at the top of the two motor legs using bolts and locknuts and has the motor mounted to the underside of it as the name implies using machine screws which are secured directly into threaded holes in the top of the motor. The motor is from Maxon and was chosen to replicate the motor of the Qube-Servo 2 as closely as possible given that motor is no longer available. An optical encoder is mounted to the rear axle of the motor using a double sided adhesive which allows the encoder to be secured firmly to the rear of the motor without the use of screws as there are no mounting locations on the rear of the motor. The attachment interface is a small machined aluminum disc that is mounted to the end of the top shaft of the motor using a set screw. This component has magnets on it which allow the attachments to be fixed to the motor shaft during use. The motor plate also has a small elbow macaroni shaped component mounted to it near the motor. This component was 3D printed and serves as a stop to ensure that the pendulum doesn't over rotate and damage its wiring. The mounting plate also has a port for a 3.5 mm plug which is used to plug in the encoder from the pendulum attachment while it is being used.

### Electrical Compartment
The electrical compartment is placed in between the bend in the top panel of the device deck and the trough. The electrical components including the motor driver, Arduino, and PCB are mounted to the backside of the bend of the panel using nuts and bolts and a piece of acrylic sized to match the PCB with the purpose of insulating the exposed circuitry of the PCB from the aluminum panel to ensure the panel doesn't cause interference.

### Attachments
The two attachments are the inertia disc and the pendulum. The inertia disc is a machined aluminum disc which adds inertia to the motor for use in certain projects. The disc has lines engraved into the top which allow the user of the disc to track the position of the motor during use and confirm their project is successful. The disc also has four magnets on the back which allow the disc to be secured to the attachment interface during use and the attachment storage compartment when stored. The pendulum consists of 3 machined components. The pendulum cylinder is machined out of aluminum and is the mounting location for the pendulum and has magnets on one side for mounting to the attachment interface and storage compartment. The cylinder has two bearings press fit into its internal hole which allow the pendulum axle to freely rotate. The pendulum axle is made out of steel and is inserted through the two bearings in the pendulum cylinder and is secured using two retaining rings which fit into grooves in the axle placed in order to be pressed against the bearings. An optical encoder is also attached to the pendulum cylinder using machine screws and is used to track the position of the axle during pendulum projects. The encoder is connected to a 3.5 mm cable which is plugged into a jack in the motor mounting plate and allows the encoder data to be transmitted to the electrical compartment. The pendulum arm is mounted on the far end of the axle from the cylinder using a set screw and acts as a weight to create tipping motion for the pendulum for programming to compensate for in the inverted pendulum project.

---

## Software

### Dependencies

**Arduino IDE (1.8.x recommended)**
- CEWBEncoder (included in `Software/MATLAB/Libraries/CEWBEncoder/`)
- CEWBFunctions (included in `Software/MATLAB/Libraries/CEWBFunctions/`)

**MATLAB/Simulink**
- MATLAB R2024b or earlier (note: R2025b has known compatibility issues — see [Known Limitations](#known-limitations))
- Simulink
- No additional toolboxes required — serial communication is handled via a custom `matlab.System` object

### Arduino Code

The main Arduino sketch (`WorkingModel.ino`) implements a simple request/response serial protocol. Simulink sends a voltage command, the Arduino executes it on the motor and responds with the current encoder count and applied voltage.

**Packet format:**

| Direction | Format | Example |
|---|---|---|
| Simulink → Arduino | `#sXXXXXX\n` | `#+037500\n` (3.75V × 10000) |
| Arduino → Simulink | `#sXXXXXX,sXXXXXX\n` | `#+000512,+037500\n` |

Voltage is transmitted as a scaled integer (real volts × 10000) to preserve 4 decimal places without transmitting a decimal point. Conversion is handled by `makeVoltage()` in CEWBFunctions.

The Arduino also implements a **500ms watchdog** — if no packet is received within 500ms (e.g. if Simulink is stopped or the USB cable is disconnected), the motor is cut automatically.

### MATLAB/Simulink Interface

Communication between Simulink and the Arduino is handled by `ArduinoSerial.m`, a `matlab.System` subclass that runs a 200Hz background timer independently of the Simulink step rate. This decouples serial timing from Simulink's solver, allowing smoother encoder data with less blocking.

**Block interface:**

| Port | Direction | Description |
|---|---|---|
| `v_in` | Input | Voltage command (−6 to +6 V) |
| `enc_out` | Output | Raw encoder count |
| `v_out` | Output | Voltage currently applied (for scope) |

**Configurable block parameters (set by double-clicking the block):**

| Parameter | Default | Description |
|---|---|---|
| `COMPort` | `/dev/ttyACM0` | Serial port (Linux) or `COM3` (Windows) |
| `BaudRate` | `115200` | Serial baud rate |
| `TimerPeriod` | `0.005` | Background timer period in seconds (0.005 = 200Hz) |

To convert `enc_out` to angle, wire it through a Gain block set to `(2*pi)/2048` for radians or `360/2048` for degrees.

### Custom Libraries

**CEWBEncoder**  
Interrupt-driven quadrature encoder library supporting ENC1X, ENC2X, and ENC4X decoding modes using Arduino pin change interrupts (PCINT). Supports up to 8 encoders simultaneously.

Key functions:
```cpp
CEWBEncoder enc(pinA, pinB, ENC4X);  // constructor
enc.begin();                          // enable interrupts
enc.getCount();                       // returns long count
enc.getDirection();                   // returns -1, 0, or 1
enc.resetCount();                     // resets count to zero
```

**CEWBFunctions**  
Utility functions for motor voltage conversion.

Key functions:
```cpp
voltToPWM(float voltage);   // converts voltage magnitude to PWM (0-255)
PWMToVolt(uint8_t PWM);     // converts PWM back to voltage
makeVoltage(long value);    // descales integer (value / 10000.0) to float volts
```

Note: `voltToPWM` maps voltage against a 24V maximum (`maxVolt = 24`). Passing values above 24V returns 255. Always pass `fabsf(voltage)` — direction is handled separately by `spinMotor`.

---

## Getting Started

### Hardware Setup

1. Connect the USB-B cable from the device's front panel USB port to your laptop.
2. Connect the power cable to the device's front panel power port and plug into the wall.
3. Raise the motor housing to its upright operating position.
4. Attach the desired attachment (inertia disc or pendulum) to the motor interface magnetically.
5. If using the pendulum attachment, plug the 3.5 mm encoder cable into the jack on the motor plate.

### Software Setup

**Arduino:**
1. Install Arduino IDE 1.8.x.
2. Copy `CEWBEncoder` and `CEWBFunctions` folders from `Software/MATLAB/Libraries/` into your Arduino libraries directory:
   - Linux (Snap): `~/snap/arduino/85/Arduino/libraries/`
   - Linux (AppImage/standard): `~/Arduino/libraries/`
   - Windows: `Documents/Arduino/libraries/`
3. Open `Software/Arduino/WorkingModel/WorkingModel.ino` in Arduino IDE.
4. Select **Tools → Board → Arduino Uno** and the correct port under **Tools → Port**.
5. Upload the sketch.

**MATLAB/Simulink:**
1. Place `ArduinoSerial.m` in the same folder as your Simulink `.slx` model file — Simulink looks for it by name in the current working directory.
2. Open your Simulink model.
3. Add a **MATLAB System** block (Simulink → User-Defined Functions → MATLAB System).
4. Double-click the block and select `ArduinoSerial` from the dropdown.
5. Set `COMPort` to match your Arduino's port (check Arduino IDE → Tools → Port).
6. Set the Simulink solver to **Fixed-step**, step size `0.02` (Ctrl+E → Solver).
7. Press **Play** — the block handles serial port opening, the background timer, and cleanup automatically.

### Running Your First Experiment

A simple open-loop voltage test:

1. Add a **Signal Generator** block (square wave, amplitude 3, frequency 0.5 Hz).
2. Wire it to `v_in` on the ArduinoSerial block.
3. Add a **Gain** block after `enc_out` with value `360/2048` to convert counts to degrees.
4. Connect both the Gain output and `v_out` to a **Scope** block.
5. Press **Play**. The motor should respond to the square wave and the scope should display the angular position and applied voltage.

---

## Lab Experiments

### Angular Position Control (Inertia Disc)

**Objective:** Command the inertia disc to move between angular positions (e.g. +1 rad and −1 rad) while minimizing overshoot using a PID controller.

**Simulink wiring:**
```
Constant (setpoint, rad) ──► Sum(+−) ──► PID ──► Saturation(±6V) ──► v_in ──► ArduinoSerial
                                ▲                                                      │
                                └──────────── Gain(2π/2048) ◄──── enc_out ────────────┘
```

**Recommended starting PID gains:**

| Gain | Starting Value |
|---|---|
| Kp | 0.1 |
| Ki | 0.0 |
| Kd | 0.1 (use filtered derivative: `10s/(s+10)`) |

> **Important:** Do not use Simulink's built-in `Derivative` block — it has infinite gain at high frequencies and will amplify serial timing noise, causing unstable motor behavior. Use a `Transfer Fcn` block with numerator `[10 0]` and denominator `[1 10]` instead.

**Note:** Due to the Arduino's ~20ms serial round-trip latency, aggressive PID gains will cause instability. Start with low Kp and no derivative, then increase gradually.

### Inverted Pendulum Balancing

> **Status: Not yet validated.** The inverted pendulum experiment requires faster closed-loop performance than the current Arduino-based system can reliably provide. This experiment is planned for a future hardware revision using a faster microcontroller. See [Next Steps](#next-steps).

---

## Results

The device met the following design requirements:

| Requirement | Target | Result |
|---|---|---|
| Accurate data transmission | Pass | ✅ Pass |
| MATLAB/Simulink compatibility | Pass | ✅ Pass |
| Cost target | < $1,000 | ✅ $763.68 |
| Self-contained storage | Pass | ✅ Pass |
| Transport protection | Pass | ✅ Pass (Nanuk 905, ASTM D-4169 DC-18 certified) |
| Third-party manufacturability | Pass | ✅ Pass |
| Software updatability | Pass | ✅ Pass (on supported MATLAB versions) |
| Qube function replication | Pass | ❌ Partial — inertia disc overshoot; pendulum not attempted |
| Motor and encoder performance | Comparable | ❌ Not met under closed-loop conditions |

---

## Known Limitations

**Serial latency (~20ms round-trip)**  
The Arduino Uno communicates over USB serial at approximately 50Hz, compared to the Qube-Servo 2's ~500Hz proprietary interface. This introduces significant control loop delay, causing excessive overshoot at higher PID gains. The background timer in `ArduinoSerial.m` mitigates this partially by running at 200Hz independently of Simulink, but the underlying serial round-trip remains the bottleneck.

**MATLAB R2025b incompatibility**  
The system does not operate correctly on the latest MATLAB R2025b release. Use R2024b or an earlier supported version. Compatibility on future versions is expected to require minor code or syntax updates.

**Encoder noise at high motor speeds**  
At high voltages, motor vibration can introduce noise on the encoder signal lines, causing occasional miscounts. Adding 10–100nF ceramic capacitors between each encoder signal pin and GND reduces this significantly.

**voltToPWM maps against 24V maximum**  
The `voltToPWM` function in CEWBFunctions maps input voltage against a 24V maximum. Voltages between ±6V — the recommended operating range — use only 25% of the available PWM range. If more motor resolution is needed, change `maxVolt` in `CEWBFunctions.cpp` to match your operating voltage range.

---

## Next Steps

The primary recommended next step is to replace the Arduino Uno with a faster microcontroller — such as a Raspberry Pi Pico, STM32, or Teensy 4.x — that supports higher-speed serial or USB communication. This would reduce control loop latency from ~20ms to ~1ms, bringing performance in line with the Qube-Servo 2 and making the inverted pendulum experiment feasible.

After the hardware revision, the following steps are planned:
1. Retest angular position control with inertia disc using the same PID experiments
2. Attempt inverted pendulum balancing
3. Assemble a small production run for deployment in the control systems course
4. Provide a prototype unit to the course instructor for curriculum integration

---

## Contributing

This project is open-source and contributions are welcome. To contribute:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/your-feature`)
3. Commit your changes (`git commit -m 'Add your feature'`)
4. Push to the branch (`git push origin feature/your-feature`)
5. Open a pull request

For questions or technical support, contact:
- **Dr. Talles Santos** (project sponsor): Talles.Santos@colorado.edu
- **US Digital** (encoders): support@usdigital.com | 800-736-0194
- **Maxon Motors**: info.us@maxongroup.com | (508) 677-0520

---

## License

This project is open-source. All CAD files, code, and documentation are made publicly available for reproduction and modification. See the repository for license details.
