# Gesture-Controlled Mouse using Arduino/ESP32 and MPU6050

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino](https://img.shields.io/badge/Platform-Arduino-blue.svg)](https://www.arduino.cc/)
[![TensorFlow Lite](https://img.shields.io/badge/ML-TensorFlow%20Lite-orange.svg)](https://www.tensorflow.org/lite)

> A gesture recognition system using MPU6050 accelerometer/gyroscope sensor with Arduino Uno/ESP32, powered by TensorFlow Lite for machine learning-based gesture classification.

**Project Year:** 2024

---

## 👥 Contributors

### Project Team

- **[Shaik Abdus Sattar (Seventie)](https://github.com/Seventie)** - Project Lead & Development
- **[Sanhitha A C](https://github.com/sanhithaac)** - Contributor

---

## 🎬 Demo

[📹 Watch Demo Video](./Demo.mp4)

*Click above to watch the gesture-controlled mouse in action!*

---

## 📋 Table of Contents

- [Demo](#-demo)
- [Contributors](#-contributors)
- [Overview](#-overview)
- [Features](#-features)
- [Hardware Requirements](#-hardware-requirements)
- [Software Requirements](#-software-requirements)
- [Project Structure](#-project-structure)
- [Installation](#-installation)
- [Usage](#-usage)
- [How It Works](#-how-it-works)
- [Dataset and Training](#-dataset-and-training)
- [License](#-license)
- [Acknowledgments](#-acknowledgments)
- [Contact](#-contact)
- [Future Enhancements](#-future-enhancements)

---

## 🎯 Overview

This project implements a **gesture-controlled mouse system** using motion sensors and machine learning. The MPU6050 sensor captures 6-axis motion data (accelerometer + gyroscope), which is then processed by a custom-trained TensorFlow Lite neural network model to recognize specific gestures. These gestures can be used to control mouse movements and other computer interactions.

The project was developed in **2024** as an exploration of combining embedded systems with machine learning for intuitive human-computer interaction.

---

## ✨ Features

- **Real-time Gesture Recognition:** Recognizes custom-defined gestures using trained neural network
- **MPU6050 Integration:** Utilizes 6-axis motion tracking (3-axis accelerometer + 3-axis gyroscope)
- **TensorFlow Lite Model:** Lightweight ML model optimized for microcontroller deployment
- **Custom Gesture Dataset:** Record and train on your own gesture data
- **Arduino/ESP32 Compatible:** Works with both Arduino Uno and ESP32 platforms
- **Python Integration:** Data collection, model training, and mouse control scripts
- **Model Conversion Tools:** Convert TensorFlow models to C header files for Arduino

---

## 🔧 Hardware Requirements

- **Microcontroller:** Arduino Uno or ESP32
- **Sensor:** MPU6050 6-axis accelerometer/gyroscope module
- **Connections:**
  - VCC → 5V (Arduino Uno) or 3.3V (ESP32)
  - GND → GND
  - SCL → A5 (Arduino Uno) or GPIO 22 (ESP32)
  - SDA → A4 (Arduino Uno) or GPIO 21 (ESP32)
- **USB Cable** for serial communication
- **Computer** for data collection and training

---

## 💻 Software Requirements

### Arduino/ESP32
- Arduino IDE (1.8.x or later)
- Required Libraries:
  - `Wire.h` (I2C communication)
  - `MPU6050` library
  - `TensorFlowLite_ESP32` (for ESP32 implementations)

### Python (for training and data collection)
- Python 3.7+
- TensorFlow 2.x
- NumPy
- Pandas
- PySerial (for serial communication)
- PyAutoGUI (for mouse control, optional)
- scikit-learn
- joblib

```bash
pip install tensorflow numpy pandas pyserial pyautogui scikit-learn joblib
```

---

## 📁 Project Structure

```
C_Project_AllTries_usingesp32/
├── gesture_data_1.csv              # Recorded gesture dataset
├── gesture_model.h                 # TensorFlow Lite model (C header)
├── gesture_model.tflite            # TensorFlow Lite model file
├── model_details.h                 # Model architecture details
├── normalization_params.h          # Data normalization parameters
├── convert_to_header.py            # Convert .tflite to C header
├── serial_reader.c                 # Serial data reading
├── guesture_recorder/              # Arduino sketch for recording gestures
│   └── guesture_recorder.ino
├── Main/                           # Main implementation
│   ├── Main.ino                    # Main Arduino sketch
│   ├── gesture_model.h
│   ├── model_details.h
│   └── normalization_params.h
├── MAINLAST/                       # Final stable version
│   ├── MAINLAST.ino
│   ├── gesture_model.cpp
│   ├── gesture_model.h
│   ├── model_details.h
│   └── normalization_params.h
├── NowOrDie/                       # Training experiments
│   ├── love.py                     # Main training script
│   ├── new.py                      # Alternative training approach
│   └── ikkada/
│       ├── ikkada.ino
│       ├── love.py
│       └── gesture_model.h
└── output/                         # Multiple gesture datasets
    ├── gesture_data_1.csv
    ├── gesture_data_2.csv
    ├── makeone.py                  # Dataset merger
    └── ...

Gesture_recog_Using_TesnorFlow_esp32_AllTries/
├── gesture_data_1.csv              # Training datasets
├── gesture_model_final_fast.h      # Optimized model
├── gesture_model_quantized.tflite  # Quantized model
├── convertor.py                    # Model conversion script
└── Main_Final.ino                  # Final ESP32 implementation

Main/
├── MainModuleToIntegrateMouseFunctionality.py  # Python mouse control
└── Integrationtoarduino.ino        # Arduino integration
```

---

## 🚀 Installation

### 1. Clone the Repository

```bash
git clone https://github.com/Seventie/Gesture-Controlled-Mouse-using-Arduino-Uno-and-MPU6050.git
cd Gesture-Controlled-Mouse-using-Arduino-Uno-and-MPU6050
```

### 2. Install Arduino Libraries

1. Open Arduino IDE
2. Go to **Sketch → Include Library → Manage Libraries**
3. Search and install:
   - `MPU6050` by Electronic Cats (or Adafruit MPU6050)
   - `TensorFlowLite_ESP32` (for ESP32 only)

### 3. Install Python Dependencies

```bash
pip install tensorflow numpy pandas pyserial pyautogui scikit-learn joblib
```

### 4. Hardware Setup

1. Connect MPU6050 to Arduino/ESP32 as per the wiring diagram above
2. Upload the gesture recorder sketch first to collect data
3. Upload the main gesture recognition sketch after training

---

## 📖 Usage

### Step 1: Record Gesture Data

1. Upload `guesture_recorder/guesture_recorder.ino` to your Arduino
2. Open Serial Monitor (115200 baud)
3. Perform gestures and record sensor data
4. Save the serial output to CSV files (e.g., `gesture_data_1.csv`)

### Step 2: Train the Model

The main training script is located at `C_Project_AllTries_usingesp32/NowOrDie/ikkada/love.py`

```bash
cd C_Project_AllTries_usingesp32/NowOrDie/ikkada
python love.py
```

This script:
- Loads gesture data from CSV
- Normalizes accelerometer data
- Trains a neural network with:
  - Input layer: 3 features (accel_x, accel_y, accel_z)
  - Hidden layer: 32 neurons (ReLU)
  - Hidden layer: 16 neurons (ReLU)
  - Output layer: Softmax for gesture classification
- Applies quantization for optimal microcontroller performance
- Generates:
  - `gesture_model.h5` (Keras model)
  - `gesture_model_quantized.tflite` (TensorFlow Lite model)
  - `gesture_model.h` (C header file for Arduino)
  - `label_encoder.pkl` (Label encoder for predictions)

### Step 3: Upload to Arduino/ESP32

1. Copy `gesture_model.h`, `model_details.h`, and `normalization_params.h` to your sketch folder
2. Upload `Main/Main.ino` or `MAINLAST/MAINLAST.ino` to your board
3. Open Serial Monitor to see gesture predictions in real-time

### Step 4: Mouse Control (Optional)

```bash
# Run Python script for mouse control integration
python Main/MainModuleToIntegrateMouseFunctionality.py
```

---

## 🧠 How It Works

### 1. Data Collection
The MPU6050 sensor continuously reads:
- **Accelerometer data** (X, Y, Z) - linear acceleration in m/s²
- **Gyroscope data** (X, Y, Z) - angular velocity in degrees/sec

Data is sampled at a fixed rate and transmitted over serial to a computer.

### 2. Data Preprocessing
```python
# Normalize features using StandardScaler
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)
```
- Raw sensor data is normalized using mean and standard deviation
- Ensures consistent input range for neural network
- Improves model convergence and accuracy

### 3. Model Training
```python
model = tf.keras.models.Sequential([
    tf.keras.layers.Dense(32, activation='relu', input_shape=(3,)),
    tf.keras.layers.Dense(16, activation='relu'),
    tf.keras.layers.Dense(num_gestures, activation='softmax')
])
```
- **Architecture:** Small feedforward neural network
- **Optimizer:** Adam
- **Loss Function:** Sparse Categorical Crossentropy
- **Training:** 10 epochs with 80/20 train-validation split

### 4. Model Optimization
```python
converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
quantized_tflite_model = converter.convert()
```
- TensorFlow Lite conversion reduces model size
- Quantization converts 32-bit floats to 8-bit integers
- Maintains accuracy while enabling deployment on resource-constrained devices

### 5. Inference on Arduino/ESP32
- The trained model weights are embedded in the Arduino sketch as a C header file
- TensorFlow Lite Micro runtime performs inference on the microcontroller
- Classification results are sent via serial or used to trigger actions
- Real-time prediction with minimal latency

### 6. Mouse Control Integration
- Python script reads gesture predictions from serial port
- PyAutoGUI library translates gestures to mouse movements/clicks
- Mapping examples:
  - Gesture 1 → Move mouse up
  - Gesture 2 → Move mouse down
  - Gesture 3 → Left click
  - etc.

---

## 📊 Dataset and Training

### Dataset Format (CSV)
```csv
acceleration.x,acceleration.y,acceleration.z,gesture
-0.123,0.456,9.789,swipe_right
0.234,0.567,9.823,swipe_right
-0.456,-0.123,9.756,swipe_left
...
```

### Gesture Labels
Define your own gestures (examples):
- `rest` - No movement
- `swipe_right` - Swipe right
- `swipe_left` - Swipe left
- `swipe_up` - Swipe up
- `swipe_down` - Swipe down
- `circle_cw` - Circle clockwise
- `click` - Quick motion for click

### Model Performance
- Achieves high accuracy on custom gesture datasets
- Real-time inference capable on Arduino Uno/ESP32
- Model size: ~2-5 KB (quantized)

### Training Tips
1. **Collect diverse data:** Perform each gesture multiple times with variations
2. **Balance classes:** Record similar amounts of data for each gesture
3. **Include rest state:** Important for distinguishing active gestures from no movement
4. **Experiment with hyperparameters:** Adjust epochs, batch size, and layer sizes

---

## 👥 Contributors

### Project Team

- **[Seventie](https://github.com/Seventie)** - Project Lead & Development
- **[Sanhitha A C](https://github.com/sanhithaac)** - Contributor

### Contributions
We welcome contributions! Please feel free to submit issues, feature requests, or pull requests.

**Ways to contribute:**
- Add new gesture types
- Improve model accuracy
- Optimize for different microcontrollers
- Add support for additional sensors
- Enhance documentation

---

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

- TensorFlow team for TensorFlow Lite for Microcontrollers
- Arduino and ESP32 communities
- MPU6050 library maintainers
- All contributors and testers

---

## 📞 Contact

For questions or collaboration:
- GitHub Issues: [Report an issue](https://github.com/Seventie/Gesture-Controlled-Mouse-using-Arduino-Uno-and-MPU6050/issues)
- Project Link: [https://github.com/Seventie/Gesture-Controlled-Mouse-using-Arduino-Uno-and-MPU6050](https://github.com/Seventie/Gesture-Controlled-Mouse-using-Arduino-Uno-and-MPU6050)

---

## 🔮 Future Enhancements

- [ ] Add support for more complex gesture sequences
- [ ] Implement gesture recording GUI
- [ ] Real-time gesture visualization
- [ ] Support for multiple MPU6050 sensors
- [ ] Mobile app integration
- [ ] Customizable gesture-to-action mapping

---

**Made with ❤️ and ⚡ in 2024**
