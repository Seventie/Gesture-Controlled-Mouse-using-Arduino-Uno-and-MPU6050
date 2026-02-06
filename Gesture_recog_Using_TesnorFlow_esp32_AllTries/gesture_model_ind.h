#ifndef GESTURE_RECOGNITION_H
#define GESTURE_RECOGNITION_H



// Gesture definitions
#define GESTURE_LEFT 0
#define GESTURE_RIGHT 1
#define GESTURE_UP 2
#define GESTURE_DOWN 3
#define NO_GESTURE -1

// Thresholds for gesture recognition
const float GYRO_THRESHOLD = 1.5; // Adjust this threshold based on testing

int recognizeGesture(float gyroX, float gyroY, float gyroZ) {
    // Recognize gestures based on gyroscope values
    if (gyroX > GYRO_THRESHOLD) {
        return GESTURE_RIGHT;
    } else if (gyroX < -GYRO_THRESHOLD) {
        return GESTURE_LEFT;
    } else if (gyroY > GYRO_THRESHOLD) {
        return GESTURE_UP;
    } else if (gyroY < -GYRO_THRESHOLD) {
        return GESTURE_DOWN;
    }
    
    return NO_GESTURE; // No gesture recognized
}

#endif // GESTURE_RECOGNITION_H
