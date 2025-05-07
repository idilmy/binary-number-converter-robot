# Binary Number Converter Robot

This project implements a hardware and simulation system that converts decimal numbers to 4-bit binary using mechanical disks and LED indicators.  
Designed as part of the DBL Embedded Systems course, it integrates Arduino components and Unity simulation to demonstrate embedded control logic, signal processing, and visualization.

---

## Features

- **Decimal-to-Binary Conversion** using:
  - Color sensors to detect disk positions
  - LEDs to display binary output
- **Mechanical Disk-Based Logic**
  - Disks represent binary states using colored segments
- **Simulation in Unity**
  - Mimics robot behavior in a virtual environment
  - Tests disk rotation, sensor accuracy, and signal interpretation
- **Arduino Integration**
  - Reads sensor data and drives LED outputs
  - Serial communication for testing and calibration

---

## Technologies Used

- Arduino (C/C++)
- Unity (C#)
- RGB Color Sensors
- LEDs & Servos
- 3D Models (.fbx assets)

---

## Project Structure

```
CompleteCode/              # Final Arduino code and logic
color_sensor_calibration/  # RGB sensor tuning
color_sensor_detection/    # Sensor logic and color decoding
decimal_to_binary/         # Decimal-to-binary LED code
simulation_files/          # Unity C# scripts and 3D assets
laser_module/, led_blink/, MotionSensor/, motor_test/, Solenoid/ # Module test code
.github/workflows/         # GitHub Actions config (if applicable)
```

---

## Getting Started

1. Clone the repository:

```bash
git clone https://github.com/idilmy/binary-number-converter-robot.git
```

2. Open Arduino `.ino` files using the Arduino IDE.

3. Open `simulation_files/` in Unity to test the robot simulation.

---

## Credits

Developed by group 13 
Individual contribution and repository maintained by Idil Mısra Yılmaz
Course: DBL Embedded Systems  
Technische Universiteit Eindhoven
