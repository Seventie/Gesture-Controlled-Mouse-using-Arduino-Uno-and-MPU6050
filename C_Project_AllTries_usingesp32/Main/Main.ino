#include <Wire.h>
#include <MPU6050.h>
#include "gesture_model.h"
#include "model_details.h"
#include "normalization_params.h"

// Create MPU6050 instance
MPU6050 mpu;

// Macros definitions
#define SERIAL_BAUD_RATE  115200
#define SENSOR_SAMPLES 256
#define AXIS 3

// Buffer for storing sensor data
static float sensor_buffer[SENSOR_SAMPLES * AXIS] = {0.0};
static uint16_t buffer_index = 0;

// Variables for gesture recognition
float probabilities[6]; // For 6 gestures
int predicted_gesture = 0;

// TensorFlow Lite globals
static tflite::MicroInterpreter* interpreter = nullptr;
constexpr int kTensorArenaSize = 8000;
uint8_t tensor_arena[kTensorArenaSize];

void setup() {
  // Initialize Serial communication
  Serial.begin(SERIAL_BAUD_RATE);
  while (!Serial);

  // Initialize I2C
  Wire.begin();

  // Initialize MPU6050
  Serial.println("Initializing MPU6050...");
  mpu.initialize();

  // Verify MPU6050 connection
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }
  Serial.println("MPU6050 connection successful!");

  // Initialize TensorFlow Lite model
  static tflite::MicroErrorReporter micro_error_reporter;
  static tflite::MicroMutableOpResolver<4> micro_op_resolver;

  micro_op_resolver.AddFullyConnected();
  micro_op_resolver.AddRelu();
  micro_op_resolver.AddSoftmax();
  micro_op_resolver.AddReshape();

  static tflite::MicroInterpreter static_interpreter(
      model, micro_op_resolver, tensor_arena, kTensorArenaSize,
      &micro_error_reporter);
  interpreter = &static_interpreter;

  // Allocate memory for TensorFlow Lite
  if (interpreter->AllocateTensors() != kTfLiteOk) {
    Serial.println("AllocateTensors() failed");
    return;
  }

  Serial.println("Setup complete!");
  delay(1000);
}

void loop() {
  // Collect sensor data
  while (buffer_index < SENSOR_SAMPLES) {
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);
    
    // Normalize the data using the parameters from normalization_params.h
    sensor_buffer[buffer_index * AXIS] = (ax - SCALER_MEAN[0]) / SCALER_SCALE[0];
    sensor_buffer[buffer_index * AXIS + 1] = (ay - SCALER_MEAN[1]) / SCALER_SCALE[1];
    sensor_buffer[buffer_index * AXIS + 2] = (az - SCALER_MEAN[2]) / SCALER_SCALE[2];
    
    buffer_index++;
  }

  // Reset buffer index
  buffer_index = 0;

  // Copy data to model input tensor
  float* input = interpreter->input(0)->data.f;
  memcpy(input, sensor_buffer, SENSOR_SAMPLES * AXIS * sizeof(float));

  // Run inference
  if (interpreter->Invoke() != kTfLiteOk) {
    Serial.println("Invoke failed!");
    return;
  }

  // Get the prediction
  float* output = interpreter->output(0)->data.f;
  predicted_gesture = 0;
  float max_prob = output[0];
  
  for (int i = 1; i < 6; i++) {
    if (output[i] > max_prob) {
      max_prob = output[i];
      predicted_gesture = i;
    }
  }

  // Output predicted gesture
  Serial.print("Predicted Gesture: ");
  Serial.println(predicted_gesture);

  // Perform actions based on predicted gesture
  switch(predicted_gesture) {
    case 0: // Tap
      Serial.println("Tap detected");
      break;
      
    case 1: // Double Tap
      Serial.println("Double Tap detected");
      break;
      
    case 2: // Swipe Right
      Serial.println("Swipe Right detected");
      break;
      
    case 3: // Swipe Left
      Serial.println("Swipe Left detected");
      break;
      
    case 4: // Swipe Up
      Serial.println("Swipe Up detected");
      break;
      
    case 5: // Swipe Down
      Serial.println("Swipe Down detected");
      break;
  }

  // Add a small delay to prevent too rapid gesture detection
  delay(100);

  // Clear the buffer
  memset(sensor_buffer, 0, SENSOR_SAMPLES * AXIS * sizeof(float));
}

// Function to normalize sensor data
float normalize_sensor_data(float value, int axis) {
  return (value - SCALER_MEAN[axis]) / SCALER_SCALE[axis];
}
