# Shake, Rattle, and Roll – Wearable Tremor & Dyskinesia Detection

> **Final project for Embedded Systems (NYU New York, Spring 2025)**  
> Developed collaboratively with [Matthew Ponon](https://github.com/mop9047) and [Gaelle Mechleb](https://github.com/GMechleb).  
> This fork documents my contributions and provides a detailed project overview.

---

## 🎯 Project Overview
This wearable device detects tremors and dyskinesia in Parkinson’s patients using the onboard accelerometer of the **Adafruit Circuit Playground Classic**.  
By applying **FFT-based signal analysis**, the system identifies movement patterns and displays results using a NeoPixel LED ring with dynamic brightness mapping.

---

## 🛠️ Tech Stack
- **Hardware:** Adafruit Circuit Playground Classic
- **Firmware:** C++ with Arduino framework via PlatformIO
- **Libraries:** ArduinoFFT, Adafruit NeoPixel
- **Tools:** PlatformIO, VS Code
- **Sensors:** LIS3DH accelerometer (onboard)

---

## 🚀 Features
- **3-second motion capture** windows for accurate frequency analysis
- **FFT signal processing** to identify tremor (3–5 Hz) and dyskinesia (5–7 Hz) ranges
- **Dynamic LED feedback** indicating movement type and intensity
- **Smoothing filters** to reduce false positives
- Modular code structure (`led_logic`, `tremor_detection`) for maintainability

---

## ⚙ How It Works
1. Captures 3 seconds of motion data from the onboard accelerometer.
2. Runs an **FFT** to transform the time-domain signal into frequency-domain data.
3. Classifies movement as:
   - Tremor (3–5 Hz)
   - Dyskinesia (5–7 Hz)
   - Normal movement
4. Maps classification results to **NeoPixel LEDs**:
   - Color indicates movement type
   - Brightness indicates intensity

---

## 📂 Project Structure
```
/
├── .vscode/               # VS Code settings
├── include/               # Header files
├── lib/                   # External libraries
├── src/                   # Source code
│   ├── led_logic/          # LED control logic
│   │   ├── led.cpp
│   │   └── led.h
│   ├── tremor_detection/   # Tremor & dyskinesia detection logic
│   │   ├── tremor.cpp
│   │   └── tremor.h
│   └── main.cpp            # Main application logic
├── test/                   # Unit tests (if any)
├── platformio.ini          # PlatformIO configuration
└── README.md               # Project documentation
```

---

## 🖥️ Getting Started

### 1️⃣ Prerequisites
- Install [PlatformIO](https://platformio.org/) in VS Code.
- Have an **Adafruit Circuit Playground Classic** connected via USB.

### 2️⃣ Clone the repository
```bash
git clone https://github.com/YOUR_USERNAME/parkinsons_classification.git
cd parkinsons_classification
```

### 3️⃣ Build and upload to device
```bash
pio run --target upload
```

### 4️⃣ Monitor serial output
```bash
pio device monitor
```

---

## 📸 Demo
[🎥 Watch Demo Video](assets/demo.mp4)

---

## 📌 My Contributions
- Implemented FFT-based signal analysis using **ArduinoFFT**
- Designed LED indication logic with dynamic brightness mapping
- Developed smoothing filters to improve classification stability
- Integrated motion capture and analysis loop in `main.cpp`

---

## 📜 License
MIT – For educational purposes only.

