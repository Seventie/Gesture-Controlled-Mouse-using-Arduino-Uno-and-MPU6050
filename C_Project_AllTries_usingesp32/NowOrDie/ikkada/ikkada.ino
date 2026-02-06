#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <EdgeNeuron.h> // Include the EdgeNeuron library
#include "gesture_model.h" // Include your gesture model header

#define TENSOR_ARENA_SIZE 2048
byte tensorArena[TENSOR_ARENA_SIZE]; // Tensor arena for EdgeNeuron

Adafruit_MPU6050 mpu;

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Initialize MPU6050
    if (!mpu.begin()) {
        Serial.println("MPU6050 initialization failed!");
        while (1);  // Stop here if initialization fails
    }

    // Initialize the model using EdgeNeuron's method
    if (!modelInit(gesture_model, gesture_model_len, tensorArena, TENSOR_ARENA_SIZE)) {
        Serial.println("Model initialization failed!");
        while (1);  // Stop here if initialization fails
    }

    Serial.println("Model initialized successfully!");
}

void loop() {
    // Prepare input data (example: simple static data for testing)
    float inputData[3]; // Adjust based on your input size

    // Read sensor data (replace this with actual sensor reading logic)
    sensors_event_t event;
    mpu.getEvent(&event);
    inputData[0] = event.acceleration.x; // Assuming you want to use acceleration data
    inputData[1] = event.acceleration.y;
    inputData[2] = event.acceleration.z;

    // Set input data using EdgeNeuron's method
    for (int i = 0; i < 3; i++) {
        if (!modelSetInput(inputData[i], i)) {
            Serial.println("Failed to set input!");
            return; // Exit if setting input fails
        }
    }

    // Run inference using EdgeNeuron's method
    if (!modelRunInference()) {
        Serial.println("Inference failed!");
        return; // Exit if inference fails
    }

    // Get output data using EdgeNeuron's method
    float output[6]; // Assuming you have 6 gesture classes
    for (int i = 0; i < 6; i++) {
        output[i] = modelGetOutput(i);
        Serial.print("Output for gesture ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(output[i]);
    }

    delay(1000); // Delay for stability
}