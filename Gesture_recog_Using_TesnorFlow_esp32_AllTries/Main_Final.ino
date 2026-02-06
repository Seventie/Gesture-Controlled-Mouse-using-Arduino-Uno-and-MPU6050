#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include "gesture_model_2.h"
#include "EdgeNeuron.h"

// Constants
#define SERIAL_BAUD_RATE 115200   // Use a lower baud rate for troubleshooting
#define TENSOR_ARENA_SIZE 2048
const char* GESTURES[] = {"left", "right", "up", "down"};
#define NUM_CLASSES 4
#define INPUT_SHAPE 6

// Object instantiation
Adafruit_MPU6050 mpu;
byte tensorArena[TENSOR_ARENA_SIZE];

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(1000); // Wait a moment to let Serial initialize
    Serial.println("Starting setup...");

    // Initialize I2C communication
    Wire.begin();
    delay(500);

    // Check if the MPU6050 is connected
    Serial.println("Checking MPU6050 connection...");
    if (Wire.requestFrom(0x68, 1) == 0) {
        Serial.println("Error: MPU6050 not responding!");
        while (1);
    }

    // Initialize MPU6050
    Serial.println("Initializing MPU6050...");
    if (!mpu.begin()) {
        Serial.println("Error: MPU6050 initialization failed!");
        while (1);
    }
    Serial.println("MPU6050 initialized successfully.");

    // Load the model
    Serial.println("Loading gesture model...");
    if (!initializeModel(gesture_model_1_tflite, tensorArena, TENSOR_ARENA_SIZE)) {
        Serial.println("Error: Model loading failed!");
        while (1);
    }
    Serial.println("Model loaded successfully.");
}

void loop() {
    float inputData[INPUT_SHAPE] = {0.0};
    sensors_event_t a, g, temp;

    // Read sensor data
    Serial.println("Reading sensor data...");
    mpu.getEvent(&a, &g, &temp);
    inputData[0] = a.acceleration.x;
    inputData[1] = a.acceleration.y;
    inputData[2] = a.acceleration.z;
    inputData[3] = g.gyro.x;
    inputData[4] = g.gyro.y;
    inputData[5] = g.gyro.z;

    // Print input data for verification
    Serial.print("Input Data: ");
    for (int i = 0; i < INPUT_SHAPE; i++) {
        Serial.print(inputData[i]);
        Serial.print(" ");
    }
    Serial.println();

    // Make a prediction
    Serial.println("Running model inference...");
    if (runModelInference()) {
        Serial.println("Model inference successful.");
        int predicted_gesture = -1;
        float max_prob = 0.0;

        // Get model output and print probabilities for each class
        for (int i = 0; i < NUM_CLASSES; i++) {
            float prob = getModelOutput(i);
            Serial.print("Class "); Serial.print(i); Serial.print(" Probability: ");
            Serial.println(prob, 4);
            if (prob > max_prob) {
                max_prob = prob;
                predicted_gesture = i;
            }
        }

        // Print detected gesture if confidence is sufficient
        if (max_prob > 0.5) {
            Serial.print("Detected Gesture: ");
            Serial.print(GESTURES[predicted_gesture]);
            Serial.print(" with confidence: ");
            Serial.println(max_prob, 4);
        } else {
            Serial.println("No gesture detected.");
        }
    } else {
        Serial.println("Error: Model prediction failed.");
    }

    delay(500);  // Delay for readability
}
