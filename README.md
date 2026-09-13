<img width="1280" height="640" alt="git (1)" src="https://github.com/user-attachments/assets/8920b256-2ba8-4988-b824-5351134eb4bd" />



# Kozhikkuttan 🎯


## Basic Details
### Team Name: Informa


### Team Members
- Team Lead: SIDDHARTH K P - SOE, CUSAT
- Member 2:  BHARAT VINOD  - SOE, CUSAT

### Project Description
Kozhikkuttan is a flirt who will high-five women with enthusiasm while flipping off men.

### The Problem (that doesn't exist)
We don't have machines with enough personality.

### The Solution (that nobody asked for)
Kozhikkuttan will high-five women by utlizing the computer vision back-end to detect their gender. If it is a guy, the reply would be middle-finger. He'll ignore
if somebody is not close enough.

## Technical Details
### Technologies/Components Used
For Software:
- C++, Python
- Arduino_ESP32 (Expressif package via Github)
- ESP32_Servo
- Arduino IDE

For Hardware:
- ESP32 DevKit V1, Servo SG90, Webcam
- Dual Core, Servo typical voltage is 5V, Webcam (Built-in one)
- Jumper wires, servo horns

### Implementation
For Software:
# Installation
pip install -r requirements.txt (Make sure to do this inside a virtual env.)

# Run
myenv/bin/python gender.py
(Assuming your virtual environment is named myenv)

# Screenshots (Add at least 3)
<img width="1920" height="1080" alt="gender_detection" src="https://github.com/user-attachments/assets/fe21b1c4-89a7-4cdf-bd28-666a0f654065" />

Screenshot of the viewfinder with bounding boxes around our faces. Gender info overlay is also visible.

<b> We do not have any more screenshots as this is a hardware project. </b>

For Hardware:

# Schematic & Circuit

```
             ESP32 Microcontroller
           +-----------------------+
           |                       |
   USB >---| USB                   |
           |                       |
           |                   VIN |---------------------------+ (5V / VCC)
           |                       |                           |
           |                   GND |-------------------+       |
           |                       |                   |       |
           |                GPIO18 |-----------+       |       |
           |                       |           |       |       |
           +-----------------------+           |       |       |
                                               |       |       |
                                               v       v       v
                                            +---------------------+
                                            | SIG     GND     VCC |
                                            |                     |
                                            |    Servo Motor      |
                                            +---------------------+
```

Schematic Diagram - Electrical schematic detailing the pin-to-pin connections between the ESP32 development board and the servo, illustrating the power (VIN/GND) and data (GPIO 18 to SIG) pathways.


```
                          USB Cable
[ Laptop (Camera) ] ======================[ ESP32 USB Port ]
                                                  |
                                              [ ESP32 ]
                                              /   |   \
                                          VIN/5V GND GPIO18
                                            |     |     |
                           (Red Wire) ----->|     |     |<----- (Orange/Yellow Wire)
                                            |     |     |
                                           VCC   GND   SIG
                                            |     |     |
                                         [ Servo Motor ]
```

Circuit Diagram - The physical wiring layout showing the laptop providing data and power to the ESP32 via USB. The servo motor receives its 5V power from the ESP32's VIN pin, shares a common ground, and receives control signals from GPIO 18.


# Build Photos
<table>
  <tr>
    <td>
      <img width="700" height="700" alt="image" src="https://github.com/user-attachments/assets/d03e7aec-4f5a-4799-a7c1-8f4932887306" />
    </td>
    <td>
      <img width="1000" height="1000" alt="image" src="https://github.com/user-attachments/assets/5cdbb744-7e61-4c07-99ea-6441a7d3fd98" />
    </td>
  </tr>
</table>
<b>Components</b>
- SG90 Servo motor
- ESP32

- We didn't take pictures while the build was ongoing. Sorry :)
- Build steps :
  - Created the gender detection python script.
  - Wired up the Arduino board and the servo motor.
  - Made it throw the returned value over serial connection.
  - Created an Arduino sketch and tested out the servo's functionality.
  - Modified the sketch to pull the values returned by the python script over the serial port and rotate accordingly.

<table>
  <tr>
    <td><img width="3472" height="4624" alt="Abhi_onFire_13092026_172243" src="https://github.com/user-attachments/assets/222e5b81-a7e5-4b99-b59d-e6fa18f2c765" />
    </td>
    <td>
      <img width="3472" height="4624" alt="Abhi_onFire_13092026_172229" src="https://github.com/user-attachments/assets/a669e74e-6d8c-4775-ac03-81f47f827dd6" />
    </td>
  </tr>
</table>
The first image is what a woman would see when she approaches the machine and the second one is what a man would see when he approaches the machine as the thing will rotate.

### Project Demo
# Video
https://www.youtube.com/watch?v=VKf7wQXTU_I


## Team Contributions
- BHARATH   : Setup the computer vision backend
- SIDDHARTH : Hardware setup and sketch program

---
Made with ❤️ at TinkerHub Useless Projects 

![Static Badge](https://img.shields.io/badge/TinkerHub-24?color=%23000000&link=https%3A%2F%2Fwww.tinkerhub.org%2F)
![Static Badge](https://img.shields.io/badge/UselessProjects--26-26?link=https%3A%2F%2Ftinkerhub.org%2Fevents%2F1M8ORET9A1%2Fuseless-projects-3.0)
