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

[The CEWB](/Images/IMG_6527.jpg)  
*The CEWB, fully built*  
&emsp;  


## Introduction
C.E.W.B. (name pending; refered to as 'CEWB' throughout the README) is a low-cost, open-source electromechanical teaching platform designed for teaching and applying control systems in a real-world platform. It was developed as a direct functional replacement for the Quanser Qube-Servo 2, a $5,000 proprietary device that is now outdated and needs an old version of MATLAB to even use.

The device supports the same core laboratory experiments as the Qube-Servo 2, including angular position control with an inertia disc and inverted pendulum balancing. It is designed to be self-contained, portable, and fully compatible with MATLAB and Simulink.

All design files, code, and documentation are publicly available in this repository so that the device can be reproduced by any institution without reliance on proprietary hardware or software.

---

## Project Overview
This project set out to design a low-cost, open-source replacement for the Quanser Qube-Servo 2, a $5,000 device used in control systems classes. The target was a fully functional prototype delivered by May 2026 at under $1,000 per unit, capable of supporting the same lab experiments, including angular position control with an inertia disc and inverted pendulum balancing, while being self-contained, portable, and compatible with MATLAB/Simulink. 

The design integrates all components into a Nanuk 905 protective case with built-in storage for attachments, cables, and the power supply. The system uses an Arduino microcontroller, a motor driver, and a custom PCB, with two interchangeable magnetic attachments, an inertia disc and a pendulum, mirroring the Qube-Servo 2's functionality. The projected per-unit production cost of $763.68 comes in well under the $1,000 target. 

The device met most implementation requirements. Data transmission, MATLAB/Simulink compatibility, third-party manufacturability, self-contained storage, and transport protection all passed evaluation. Open-source documentation, including CAD files, a bill of materials, and assembly instructions, has been prepared and made publicly available. 

However, the device did not fully replicate the closed-loop control performance of the Qube-Servo 2. Arduino’s data transmission rate is approximately an order of magnitude slower than the proprietary controller used in the Qube-Servo 2, causing significant control loop delays and excessive overshoot during inertia disc testing. As a result, the pendulum balancing test was not attempted. 

The primary recommended next step is to replace the Arduino with a faster microcontroller, retest the system using the same control experiments, and proceed to a small production run once performance requirements are confirmed to be met. 

---

### Circuit Design
### Data Transmission
### Hardware Design
#### Accessory Storage
The accessory storage compartment is made up of two peices of hardboard and one peice of foam with cutouts for the the accessories to fit into. The rear panel of hardboard has magenets glued into it which allow the attachments to be secured in their respective locations during storage. Hardboard was selected for the front and rear panel as it is a light and cheap but sturdy material. Foam was chosen for the middle panel as it is low density and can fill the space between the boards without increasing the weight significantly. The three panels are glued together with super glue and are fixed into top of the Nanuk 905 case using thread forming screws.

#### Device Deck

The main part of the deck of the device is a bent peice of alluminum sheet metal which is screwed into the Nanuk 905 case using thread forming screws and serves as a mounting plate for the rest of the components. This part is bent the way it is in order to create a compartment for storing the power supply and the data transmission cable. The top surface of this panel has two ports. A USB port where the data transmission cable is plugged in and a power port where the device is connected to the power supply. The ports were added to clean up the vissual appearance of the device and revove the need for cables to be plugged directly into the electrical compartment. One the right side of the panel there is a cavity with ushaped bent peice of alluminum sheet metal mounted to the bottom of the panel which is used both as a mounting location for the motor housing and as a barrier between the user and the internal components of the device. The cavity is required to allow the motor housing to fold into the device when it is being stored. The trough contains hole to allow wires to pass through between the motor housing and the electrical compartment and this hole is lined with a silicone gromet which protects the wires passing through the hole from fraying when the motor hosuing is moved between its storage and opperation positions.

#### Motor Housing
The motor housing consists of 3 main structural components which are all alluminum sheet metal. The two motor housing legs give the housing the necessary height required for the use of the pendulum attachment and also serve as the location where the hinge is mounted to the housing which allows the housing to move between operational and storage positions. The hinge consists of a partially threaded socket head screw, nuts, and nylon washers. The partially threaded screw awas chosen to allow for a smooth surface for the hinge to turn on. The nylon washers were used to secure the housing in place during opperation but still allow for roation given ennough force to slightly deform the nylon. The two washers at the end are jammed together to secure the hinge at a fixed tightness calibrated based ensuring the higne can be roated but is still secured during opperation. The third structural component is the motor mounting plate and has several components attached to it as well as a bend that serves as a handle when switching the device between positions. This plate is attached at the top of the two motor legs using bolts and locknuts and has the motor mounted to the underside of it as the name implies using machine screws which are secured directly into threaded holes in the top of the motor. The motor is from maxon and was chosen to replicate the motor of the Qube-Servo 2 as closely as possible given that motor is no longer available. An optical encoder is mounted to the rear axle of the motor using a double sided adhesive which always the encoder to be secured firmly to the rear of the motor without the use of screws as the there are no mounting locations on the rear of the motor. The attachment interface is a small machined aluminum disc that is mounted to the end of the top shaft of the motor using a set screw. This component has magnets on it which allow the attachments to be fixed to the motor shaft during use. The motor place also has a small elbow macaroni shaped component mounted to it near the motor. This component was 3D printed and serves as a stop to ensure that the pendulum doesn't over rotate and damage its wiring. The mounting plate also has a port for a 3.5 mm plug which is used to plug in the encoder from the pendulum attachment while it is being used.


#### Electrical Compartment
The electrical compartment is on placed in between the bend in the top panel of the device deck and the trough. The electrical components including the motor driver, Arduino, and PCB are mounted to the backside of the bend of the panel using nuts and bolts and a peice of acrylic sized to match the PCB with the purpose of insulating the exposed cicuitry of the PCB from the aluminum panel to ensure the panel doesn't cause interference. 



#### Attachments
The two attachements are the inertia disc and the pendulum. The inetia disc is a machined alluminum disc which adds inertia to the motor for use in certain projects. The disc has lines engraved into the top which allow the user of the disc to track the position of the motor during use and confirm their project is successful. The disc also has four magnets of the back which allow the disc to be secured to the attachment interface during used and the attachment storage compartment when stored. The pendulum consists of 3 machined components. The pendulum cylinder is machined out of alluminum and is the mounting location for the pendulum and has magnets on one side for mounting to the attachment interface and storage compartment. The cylinder has two bearings press fit into its internal hole which allow the pendulum axle to freely rotate. The pendulum axle is made out of steel and is inserted through the two bearings in the pendulum cylinder and is secured using two retaining rings which fit into groves in the axle placed in order to be pressed against the bearings. An optical encoder is also attached to the pendulum cylinder using machine screws and is used to track the position of the axle during pendulum projects. The encoder is connected to a 3.5 mm cable which is plugged into a jack in the motor mounting plate and allows the encoder data to be transmitted to the electrical compartment. The pendulum arm is mounted on the far end of the axle from the cylinder using a set screw and acts as a weight to create tipping motion for the pendulum for programing to compensate for in the inverted pendulum project.

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

**Encoder noise at high motor speeds**  
At high voltages, motor vibration can introduce noise on the encoder signal lines, causing occasional miscounts. Adding 10–100nF ceramic capacitors between each encoder signal pin and GND reduces this significantly.

**voltToPWM maps against 24V maximum**  
The `voltToPWM` function in CEWBFunctions maps input voltage against a 24V maximum. Voltages between ±6V — the recommended operating range — use only 25% of the available PWM range. If more motor resolution is needed, change `maxVolt` in `CEWBFunctions.cpp` to match your operating voltage range.

---

## Next Steps
### List of Issues
In order to fix up the fundamental issues of the CEWB and to make improvements to it in the futuree, the following suggestions should be looked into:  

* Research different microcontrollers
    * The Arduino Uno R3 is simply not powerful enough to spin the motor, count the encoder position, and transmit and receive data to and from Simulink in real time accurately. To improve speeds and ease the computational load on one microcontroller, it is recommended to look into:
        - Using one faster microcontroller (Raspberry Pi Pico, STM32, Teensy 4.x, etc.)
        - Using two (or more) microcontrollers
            - Having one control the motor and handle the encoder counting while one only deals wtih the data transmission, having one control the motor, one count the encoder, and one transmit the data, etc.

* Switching the data that is transmitted to and from Simulink from strings to bytes
    * The string transmission format can still be used as a sanity check/user-readible test, but since transmitting bytes is much faster, then the final product should.

* Research different ways to transmit data at a higher rate
    * The Arduino Uno communicates over USB serial at approximately 50Hz, while the Qube 2 can transmit at ~500Hz. Look into how the Qube 2 can do this and figure out how to make the CEWB faster.

* Look into wireless encoders for the inverted pendulum
    * With the current CEWB setup (and with the current inspiration with the Quanser Qubes) the motor hub with the added inverted pendulum attachment can't spin 360° due to the wire that connects to the audio jack port. While this isn't a big deal when trying the just have the pendulum balance at 180°, it can't do it at any other angle without being able to spin constantly around 360°. This can be solved by removing the pendulum's encoder cable and having powered and connected wireless.

* Add a strap to stop the inverted pendulum from keeping the CEWB open
    * With the current setup of the storage area of the CEWB attachements, the inverted pendulum's shaft falls out of the cutout opening that it sits in and props up the lid, keeping it from closing. A simple strap would solve this problem without too much issue or extreme redesigns.

* Look into making more attachments
    * The types of attachments are good, but having inertia disks that are different weights and different sizes would be good to see when teaching/learning about control systems.


### After CEWB Functionality is Reached
Once the CEWB is in a working state, the following steps are planned:  
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
- **Dr. Nathan McNeil** (project sponsor): Nathan.Mcneil@colorado.edu
- **Alyssa Jackson** (project member): alyssajackson436@gmail.com

---

## License

This project is open-source. All CAD files, code, and documentation are made publicly available for reproduction and modification. See the repository for license details.