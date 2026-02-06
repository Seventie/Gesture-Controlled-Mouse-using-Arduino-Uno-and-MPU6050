#include <Wire.h>
#include <MPU6050.h>

#define SERIAL_BAUD_RATE  115200
#define SENSOR_SAMPLES 64  // Reduced sample size
#define AXIS 3

MPU6050 mpu;
static int16_t neai_buffer[SENSOR_SAMPLES * AXIS];

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }
  Serial.println("MPU6050 initialized");
}

void loop() {
  for (uint16_t neai_ptr = 0; neai_ptr < SENSOR_SAMPLES; neai_ptr++) {
    int16_t ax, ay, az;
    mpu.getMotion6(&ax, &ay, &az, NULL, NULL, NULL);
    neai_buffer[AXIS * neai_ptr] = ax;
    neai_buffer[(AXIS * neai_ptr) + 1] = ay;
    neai_buffer[(AXIS * neai_ptr) + 2] = az;
  }

  float x = neai_buffer[AXIS * (SENSOR_SAMPLES - 1)] / 16384.0;
  float y = neai_buffer[(AXIS * (SENSOR_SAMPLES - 1)) + 1] / 16384.0;
  float z = neai_buffer[(AXIS * (SENSOR_SAMPLES - 1)) + 2] / 16384.0;

  // Gesture detection logic
  uint16_t id_class = 0;
  if (x > 1.5) id_class = 1;  // Scroll down
  else if (x < -1.5) id_class = 2;  // Scroll up
  else if (y > 1.5) id_class = 3;  // Move right
  else if (y < -1.5) id_class = 4;  // Move left
  else if (z > 1.5) id_class = 5;  // Move up
  else if (z < -1.5) id_class = 6;  // Move down

  // Print action based on id_class
  switch (id_class) {
    case 1: Serial.println("Scroll Down"); break;
    case 2: Serial.println("Scroll Up"); break;
    case 3: Serial.println("Move Mouse Right"); break;
    case 4: Serial.println("Move Mouse Left"); break;
    case 5: Serial.println("Move Mouse Up"); break;
    case 6: Serial.println("Move Mouse Down"); break;
    default: Serial.println("No gesture detected"); break;
  }

  delay(100);
}
