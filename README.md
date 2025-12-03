#  Gesture-Controlled Mouse (Arduino + MPU6050 + TensorFlow Lite Micro)

##  Overview
This project implements a **gesture-controlled mouse** using an Arduino Uno, an MPU-6050 motion sensor, and a TensorFlow Lite Micro model.  
I recorded my own gesture data (left, right, up, down, click), trained a lightweight ML model, converted it into a `.h` file, and deployed it on the Arduino for real-time classification.  
The Arduino reads IMU values, predicts the gesture, and sends it to a Python script that moves the mouse on the computer.

##  Features
- Real-time gesture recognition using MPU6050  
- TensorFlow Lite Micro model running directly on Arduino  
- Predicts Left, Right, Up, Down, and Click gestures  
- Python script converts predictions into mouse movement  
- Fully custom dataset recorded and trained by me  

##  Workflow
1. Record IMU gesture samples  
2. Train TensorFlow model  
3. Convert to TFLite → `.h`  
4. Upload Arduino sketch  
5. Run Python script for mouse control  

##  Summary
A complete end-to-end ML + embedded systems project that turns hand motions into actual mouse movements using Arduino, MPU6050, and TensorFlow Lite Micro.
