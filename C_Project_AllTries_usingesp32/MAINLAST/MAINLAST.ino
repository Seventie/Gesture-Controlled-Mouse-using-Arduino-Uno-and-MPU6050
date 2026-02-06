#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include "gesture_model.h"
#include "normalization_params.h"
#include "model_details.h"
#include "MicroFlow.h"

#define SERIAL_BAUD_RATE 921600

Adafruit_MPU6050 mpu;

void normalizeInput(float* input) {
    for (int i = 0; i < INPUT_SHAPE; i++) {
        input[i] = (input[i] - SCALER_MEAN[i]) / SCALER_SCALE[i];
    }
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Wire.begin();

    if (!mpu.begin()) {
        Serial.println("MPU6050 initialization failed!");
        while (1);
    }
    Serial.println("MPU6050 initialized successfully.");

    // Load the model
    MicroMLP gestureModel(NUM_CLASSES, INPUT_SHAPE, model, model_len);
    if (!gestureModel.begin()) {
        Serial.println("Model loading failed!");
        while (1);
    }
    Serial.println("Model loaded successfully.");
}

void loop() {
    float inputData[INPUT_SHAPE] = {0.0};
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    inputData[0] = a.acceleration.x;
    inputData[1] = a.acceleration.y;
    inputData[2] = a.acceleration.z;

    // Normalize input
    normalizeInput(inputData);

    double output[NUM_CLASSES];
    gestureModel.feedforward(inputData, output);

    int predicted_gesture = -1;
    float max_prob = 0.0;
    for (int i = 0; i < NUM_CLASSES; i++) {
        if (output[i] > max_prob) {
            max_prob = output[i];
            predicted_gesture = i;
        }
    }

    if (max_prob > 0.5) {
        Serial.print("Detected Gesture: ");
        Serial.print(GESTURES[predicted_gesture]);  // Use the gesture label
        Serial.print(" with confidence: ");
        Serial.println(max_prob, 4);
    } else {
        Serial.println("No gesture detected.");
    }

    delay(500);
}
