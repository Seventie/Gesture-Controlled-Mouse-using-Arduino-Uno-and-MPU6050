#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include "gesture_model.h"
#include "model_details.h"
#include "normalization_params.h"

#define SERIAL_BAUD_RATE  115200
#define SENSOR_SAMPLES 64  // Sample size as required for model input
#define AXIS 3

MPU6050 mpu(Wire);
static int16_t neai_buffer[SENSOR_SAMPLES * AXIS];

// Function to initialize MPU6050
void initializeMPU() {
    mpu.begin();
    if (!mpu.testConnection()) {
        Serial.println("MPU6050 connection failed");
        while (1); // Halt if initialization fails
    }
    Serial.println("MPU6050 initialized");
}

// Function to collect data and normalize it based on params
void collectSensorData() {
    for (uint16_t i = 0; i < SENSOR_SAMPLES; i++) {
        mpu.update();
        neai_buffer[AXIS * i] = mpu.getAccX();
        neai_buffer[(AXIS * i) + 1] = mpu.getAccY();
        neai_buffer[(AXIS * i) + 2] = mpu.getAccZ();
        delay(10);  // Adjust delay based on sampling rate needed
    }
}

// Function to process collected data and classify gestures
String classifyGesture() {
    // Normalize and prepare data for model prediction
    normalize_data(neai_buffer, SENSOR_SAMPLES); // Assuming this function normalizes data as per your model

    // Run the gesture recognition model
    int gesture_id = predict_gesture(neai_buffer); // Assuming `predict_gesture` returns an ID corresponding to the gesture

    // Map gesture ID to gesture name
    switch (gesture_id) {
        case 0: return "Tap";
        case 1: return "Double Tap";
        case 2: return "Swipe Right";
        case 3: return "Swipe Left";
        case 4: return "Swipe Up";
        case 5: return "Swipe Down";
        default: return "Unknown Gesture";
    }
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    initializeMPU();
}

void loop() {
    collectSensorData(); // Collect data from MPU6050

    // Classify the gesture based on the collected data
    String gesture = classifyGesture();
    
    // Print the recognized gesture
    Serial.println(gesture);

    delay(500); // Delay before next sample, adjust based on your application's needs
}

// Main function for educational purposes
int main() {
    setup();
    while (true) {
        loop();
    }
    return 0;
}
