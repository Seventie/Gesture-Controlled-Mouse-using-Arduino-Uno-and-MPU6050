#include "Wire.h"       
#include "I2Cdev.h"     
#include "MPU6050.h"    

MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

struct MyData {
  byte X;
  byte Y;
};

MyData data;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  mpu.initialize();
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  data.X = map(ax, -17000, 17000, 0, 255);  // X axis data
  data.Y = map(ay, -17000, 17000, 0, 255);  // Y axis data
  delay(500);

  if (data.Y < 80 && (data.X > 100 && data.X < 150)) {  // Gesture: down
    Serial.println("gesture 1");  // Send gesture 1 for "down"
  }
  else if (data.Y > 155 && (data.X > 100 && data.X < 150)) {  // Gesture: up
    Serial.println("gesture 2");  // Send gesture 2 for "up"
  }
  else if (data.X > 160) {  // Gesture: left
    Serial.println("gesture 3");  // Send gesture 3 for "left"
  }
  else if (data.X < 80) {  // Gesture: right
    Serial.println("gesture 4");  // Send gesture 4 for "right"
  }
  else {
    Serial.println("no_gesture");  // Send "no_gesture" if nothing is detected
  }
}
