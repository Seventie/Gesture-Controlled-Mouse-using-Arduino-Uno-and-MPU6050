#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include "gesture_model.h"
#include "normalization_params.h"
#include "model_details.h"
#include "EdgeNeuron.h"

#define SERIAL_BAUD_RATE 921600
#define TENSOR_ARENA_SIZE 2048  // Adjust size as necessary
#define NUM_CLASSES 3  // Update based on your model
#define INPUT_SHAPE 3   // Assuming 3 input features (x, y, z)

Adafruit_MPU6050 mpu;
byte tensorArena[TENSOR_ARENA_SIZE];

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.println("Initializing MPU6050...");
    Wire.begin();

    if (!mpu.begin()) {
        Serial.println("MPU6050 initialization failed!");
        while (1);
    }
    Serial.println("MPU6050 initialized successfully.");

    // Load the model
    if (!initializeModel(model_data, tensorArena, TENSOR_ARENA_SIZE)) {
        Serial.println("Model loading failed!");
        while (1);
    }
    Serial.println("Model loaded successfully.");
}

void loop() {
    float inputData[INPUT_SHAPE] = {0.0};
    sensors_event_t a, g, temp;
    
    Serial.println("Reading sensor data...");
    mpu.getEvent(&a, &g, &temp);
    inputData[0] = a.acceleration.x;
    inputData[1] = a.acceleration.y;
    inputData[2] = a.acceleration.z;

    // Normalize input
    normalizeInput(inputData);
    Serial.print("Normalized Input: ");
    Serial.print(inputData[0]);
    Serial.print(", ");
    Serial.print(inputData[1]);
    Serial.print(", ");
    Serial.println(inputData[2]);

    // Make a prediction
    if (runModelInference()) {
        Serial.println("Model inference successful.");
        int predicted_gesture = -1;
        float max_prob = 0.0;

        for (int i = 0; i < NUM_CLASSES; i++) {
            float prob = getModelOutput(i);
            if (prob > max_prob) {
                max_prob = prob;
                predicted_gesture = i;
            }
        }

        if (max_prob > 0.5) {
            Serial.print("Detected Gesture: ");
            Serial.print(GESTURES[predicted_gesture]);
            Serial.print(" with confidence: ");
            Serial.println(max_prob, 4);
        } else {
            Serial.println("No gesture detected.");
        }
    } else {
        Serial.println("Prediction failed.");
    }

    delay(500);
}

void normalizeInput(float* input) {
    for (int i = 0; i < INPUT_SHAPE; i++) {
        input[i] = (input[i] - SCALER_MEAN[i]) / SCALER_SCALE[i];
    }
}
