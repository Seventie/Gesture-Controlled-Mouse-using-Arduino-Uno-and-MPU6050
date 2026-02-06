#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <errno.h>

#define SERIAL_PORT "COM9"           // Set the correct port
#define BAUDRATE CBR_115200          // Adjust baud rate as needed
#define RECORD_DURATION 30000        // Duration to record each gesture (30 seconds)
#define THRESHOLD_DURATION 2000      // Threshold before starting recording (2 seconds)
#define DELAY_BETWEEN_GESTURES 2000  // Delay between gestures (2 seconds)

int main() {
    // Open the serial port
    HANDLE serial_fd = CreateFile(SERIAL_PORT,
                                  GENERIC_READ | GENERIC_WRITE,
                                  0,          // No sharing
                                  NULL,       // Default security attributes
                                  OPEN_EXISTING,
                                  0,          // No special flags
                                  NULL);      // No template file
    if (serial_fd == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Unable to open serial port. Error code: %d\n", GetLastError());
        return 1;
    }
    printf("Serial port %s opened successfully.\n", SERIAL_PORT);

    // Set up the DCB structure for serial communication
    DCB dcb;
    memset(&dcb, 0, sizeof(dcb));
    dcb.DCBlength = sizeof(DCB);
    if (!GetCommState(serial_fd, &dcb)) {
        fprintf(stderr, "Error getting state: %d\n", GetLastError());
        CloseHandle(serial_fd);
        return 1;
    }

    // Configure the DCB structure for communication
    dcb.BaudRate = BAUDRATE;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;
    if (!SetCommState(serial_fd, &dcb)) {
        fprintf(stderr, "Error setting serial port state: %d\n", GetLastError());
        CloseHandle(serial_fd);
        return 1;
    }

    // Open the output file for writing
    FILE *file = fopen("output.csv", "w");
    if (file == NULL) {
        fprintf(stderr, "Unable to open output file: %s\n", strerror(errno));
        CloseHandle(serial_fd);
        return 1;
    }

    // Write the header to the CSV file
    fprintf(file, "Timestamp,SensorData\n");

    // Initial threshold before recording the first gesture
    printf("Waiting for %d seconds before starting the first gesture...\n", THRESHOLD_DURATION / 1000);
    Sleep(THRESHOLD_DURATION);

    // Loop to record gestures
    for (int gesture = 0; gesture < 6; gesture++) { // Using 6 as a constant for loop, you can modify as needed
        printf("Recording Gesture %d...\n", gesture + 1);
        // Record for the specified duration
        DWORD startTime = GetTickCount(); // Get current time
        while (GetTickCount() - startTime < RECORD_DURATION) {
            char buffer[256];
            DWORD bytesRead;
            if (ReadFile(serial_fd, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {
                buffer[bytesRead] = '\0';  // Null-terminate the string
                if (bytesRead > 0) {
                    // Write data to CSV file without gesture label
                    if (fprintf(file, "%lu,%s\n", GetTickCount(), buffer) < 0) {
                        fprintf(stderr, "Failed to write to CSV.\n");
                    }
                    fflush(file);  // Ensure data is written to file immediately
                    printf("Data written to CSV: %s\n", buffer);  // Print data to console for verification
                } else {
                    printf("No data read during this cycle.\n");
                }
            } else {
                fprintf(stderr, "Read error: %d\n", GetLastError());
                break;
            }
        }

        // Wait before the next gesture
        if (gesture < 5) { // Adjusted loop to wait before the next gesture
            printf("Waiting for %d seconds before the next gesture...\n", DELAY_BETWEEN_GESTURES / 1000);
            Sleep(DELAY_BETWEEN_GESTURES);
        }
    }

    printf("Recording complete. Closing resources...\n");
    fclose(file);  // Close file after recording all gestures
    CloseHandle(serial_fd);
    printf("Resources cleaned up. Exiting program.\n");
    return 0;
}
