# ITPMS

## A lightweight, energy-efficient **Improved TPMS with integrated tire wear assistant (ITPMS)**. Designed for universal compatibility and precision.

## 🔥 Key Features

- **🏋️ Ultra-Compact Design**  
    Minimal weight (<20g) ensures compatibility with 99% of tires without affecting balance.

- **📡 Autonomous Operation**  
    Wakes up via accelerometer motion detection → close to none battery drain when stationary.

- **📊 Fast And Precise**  
    Uses device computing from sensors such as:
    - Pressure sensor
    - Temperature sensor (built into the MCU)
    - 3-axis accelerometer
    - Hall effect sensor

- **🔋 10-Year Battery Life**  
    Has low-powered elements and works only while the vehicle is moving.

- **🎯 Hall Effect Initialization**  
    Quick pairing with magnetic TPMS initializing pen.

---

## 🛠 How It Works

### Installation & Activation

1. **Open** remove the tire from the rim.
2. **Mount** the TPMs on the stem valve inside the tire .
3. **Wake & Pair** the TPMs with the magnetic TPMS initializing pen.
4. **Drive** to see the real time pressure and temperature changes.

### Data Transmission

| Trigger                                   | Action                         |
|-------------------------------------------|--------------------------------|
| > 5% pressure deviation from last read    | Instant RF alert to ECU        |
| > 5% temperature deviation from last read | Instant RF alert to ECU        |
| 10s without any significant changes       | Keep-alive ping                |

---

## 🔍 Code library

ITPMS/  
├── [Assistant/](Assistant/)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# All files for the tire wear assistant.  
│&nbsp;&nbsp;&nbsp;├── [assets/](Assistant/assets/)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Assets for the UI.  
│&nbsp;&nbsp;&nbsp;├── [CSVhandling/](Assistant/CSVhandling/)  
│&nbsp;&nbsp;&nbsp;│&nbsp;&nbsp;&nbsp;&nbsp;|── [CSVmanager.py](Assistant/CSVhandling/CSVmanager.py)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Read and write operations for csv files.  
│&nbsp;&nbsp;&nbsp;│&nbsp;&nbsp;&nbsp;└── [DataGenerator.py](Assistant/CSVhandling/DataGenerator.py)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Generator for (TPMs) tire data.  
│&nbsp;&nbsp;&nbsp;│  
│&nbsp;&nbsp;&nbsp;├── [Data/](Assistant/Data/)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Data files for the different tire types and seasons.  
│&nbsp;&nbsp;&nbsp;├── [Tests/](Assistant/Tests/)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Tests for the functionalities of the TPMs classes.  
│&nbsp;&nbsp;&nbsp;├── [TPMsHandling/](Assistant/TPMsHandling/)  
│&nbsp;&nbsp;&nbsp;│&nbsp;&nbsp;&nbsp;&nbsp;|── [Tire.py](Assistant/TPMsHandling/Tire.py)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Represents the tire (TPMs).  
│&nbsp;&nbsp;&nbsp;│&nbsp;&nbsp;&nbsp;└── [Tires.py](Assistant/TPMsHandling/Tires.py)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Represents the tire complect of the vehicle.  
│&nbsp;&nbsp;&nbsp;│  
│&nbsp;&nbsp;&nbsp;├── [Main.py](Assistant/Main.py)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Runs the needed code and tests.  
│&nbsp;&nbsp;&nbsp;└── [TireAssistantGUI.py](Assistant/TireAssistantGUI.py)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# UI for the assistant. (*Run the file to start it.)  
│  
├── [CAD/](CAD/)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# KiCad models of the pcb.  
├── [Datasheets/](Datasheets/)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Datasheets of all components used in the pcb.  
├── [TPMS&TPMs/](TPMS&TPMs/)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# All files and libraries used by the hardware.  
│&nbsp;&nbsp;&nbsp;├── [Code](TPMS&TPMs/Code/)/  
│&nbsp;&nbsp;&nbsp;│&nbsp;&nbsp;&nbsp;&nbsp;|── [Receiver.ino](TPMS&TPMs/Code/Receiver.ino)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Receiver algorithm.  
│&nbsp;&nbsp;&nbsp;│&nbsp;&nbsp;&nbsp;└── [Transmitter.ino](TPMS&TPMs/Code/Transmitter.ino)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Transmitter algorithm.  
│&nbsp;&nbsp;&nbsp;├── [Libraries/](TPMS&TPMs/Libraries/)  
│&nbsp;&nbsp;&nbsp;│&nbsp;&nbsp;&nbsp;&nbsp;├── [Receiver/](TPMS&TPMs/Libraries/Receiver/)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Receiver libraries.  
│&nbsp;&nbsp;&nbsp;│&nbsp;&nbsp;&nbsp;&nbsp;├── [TPMs/](TPMS&TPMs/Libraries/TPMs/)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Transmitter libraries.  
│&nbsp;&nbsp;&nbsp;│&nbsp;&nbsp;&nbsp;&nbsp;└── [TPMS_structures.h](TPMS&TPMs/Libraries/TPMS_structures.h)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Common configuration.  
│  
├── [LICENSE](LICENSE)  
├── [README.md](README.md)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# You're here!  
└── [requirements.txt](requirements.txt)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;# Python dependencies.  

## 🔑 Setup

### Assistant

Run this command to download the needed dependencies:
```sh
pip install -r .requirements.txt
```
After that you can run the UI with:
```sh
python Assistant/TireAssistantGUI.py
```

### TPMS & TPMs

The software must be flashed on a special TPMs via SPI programmer. From there you have to follow the steps in [Installation & Activation](#installation--activation) an you will be ready to use it.