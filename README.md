# C.E.W.B.: The Open Source Teaching Platform for Control Systems
**ECEN4620/MCEN4085: Senior Design Project - University of Colorado Boulder**  
**Sponsors:** Talles Santos  
&emsp;&emsp;&emsp;&emsp;&emsp; Nathan McNeil  
**Students:** [Alyssa Jackson](https://github.com/aJax-EXE) (Electrical and Computer Engineering)  
&emsp;&emsp;&emsp;&emsp;&emsp;Eric Pollert (Mechanical Engineering)  
&emsp;&emsp;&emsp;&emsp;&emsp;[Isaiah Knox](https://github.com/iknox001) (Mechanical Engineering)  

---

[The CEWB](/Images/IMG_6527.jpg)  
*The CEWB, fully built*  
&emsp;  


## Introduction
C.E.W.B. (name pending) is a low-cost, open-source electromechanical teaching platform designed for teaching and applying control systems in a real-world platform. It was developed as a direct functional replacement for the Quanser Qube-Servo 2, a $5,000 proprietary device that is now outdated and needs an old version of MATLAB to even use.

The device supports the same core laboratory experiments as the Qube-Servo 2, including angular position control with an inertia disc and inverted pendulum balancing. It is designed to be self-contained, portable, and fully compatible with MATLAB and Simulink.

All design files, code, and documentation are publicly available in this repository so that the device can be reproduced by any institution without reliance on proprietary hardware or software.

## Project Overview
This project set out to design a low-cost, open-source replacement for the Quanser Qube-Servo 2, a $5,000 device used in control systems classes. The target was a fully functional prototype delivered by May 2026 at under $1,000 per unit, capable of supporting the same lab experiments, including angular position control with an inertia disc and inverted pendulum balancing, while being self-contained, portable, and compatible with MATLAB/Simulink. 

The design integrates all components into a Nanuk 905 protective case with built-in storage for attachments, cables, and the power supply. The system uses an Arduino microcontroller, a motor driver, and a custom PCB, with two interchangeable magnetic attachments, an inertia disc and a pendulum, mirroring the Qube-Servo 2's functionality. The projected per-unit production cost of $763.68 comes in well under the $1,000 target. 

The device met most implementation requirements. Data transmission, MATLAB/Simulink compatibility, third-party manufacturability, self-contained storage, and transport protection all passed evaluation. Open-source documentation, including CAD files, a bill of materials, and assembly instructions, has been prepared and made publicly available. 

However, the device did not fully replicate the closed-loop control performance of the Qube-Servo 2. Arduino’s data transmission rate is approximately an order of magnitude slower than the proprietary controller used in the Qube-Servo 2, causing significant control loop delays and excessive overshoot during inertia disc testing. As a result, the pendulum balancing test was not attempted. 

The primary recommended next step is to replace the Arduino with a faster microcontroller, retest the system using the same control experiments, and proceed to a small production run once performance requirements are confirmed to be met. 

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

## Next Steps/List of Potential Solutions
In order to fix up the fundamental issues of the CEWB and to make 
