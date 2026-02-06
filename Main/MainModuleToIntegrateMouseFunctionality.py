import serial
import pyautogui
import time
import tkinter as tk
from tkinter import font

# Open the serial port (replace 'COM11' with your Arduino port)
ser = serial.Serial('COM11', 115200, timeout=1)
time.sleep(2)  # Give some time for the serial connection to establish

# Function to perform mouse action based on gesture
def perform_mouse_action(gesture):
    distance = 40  # Movement distance
    steps = 1  # Steps to complete movement

    global last_gesture, movement_speed
    if gesture == "gesture 1":
        pyautogui.move(distance, 0)  # Gesture down
        
        last_gesture = "Down"
        movement_speed = distance / 0.01
    elif gesture == "gesture 2":
        pyautogui.move(-distance, 0)  # Gesture up
        
        last_gesture = "Up"
        movement_speed = distance / 0.01
    elif gesture == "gesture 3":
        pyautogui.move(0, -distance)  # Gesture left
        
        last_gesture = "Left"
        movement_speed = distance / 0.01
    elif gesture == "gesture 4":
        pyautogui.move(0, distance)  # Gesture right
        
        last_gesture = "Right"
        movement_speed = distance / 0.01

# Function to update the UI
def update_ui():
    if ser.in_waiting > 0:
        gesture = ser.readline().decode('utf-8').strip()
        gesture_label.config(text=f"Current Gesture: {gesture}")
        if gesture != "no_gesture":
            perform_mouse_action(gesture)

    # Update movement display
    speed_label.config(text=f"Movement Speed: {movement_speed:.2f} px/s")
    direction_label.config(text=f"Direction: {last_gesture}")
    
    # Schedule the next update
    root.after(100, update_ui)

# Function to change cursor appearance on enter
def on_enter(event):
    canvas.itemconfig(cursor, fill="black")  # Change cursor color to black
    canvas.coords(cursor, 50, 50, 150, 150)  # Increase cursor size
    cursor_size_label.config(text="Cursor Size: Large")
    canvas.configure(bg="lightgrey")  # Change background color on hover

# Function to reset cursor appearance on leave
def on_leave(event):
    canvas.itemconfig(cursor, fill="blue")  # Reset cursor color
    canvas.coords(cursor, 85, 85, 115, 115)  # Reset cursor size
    cursor_size_label.config(text="Cursor Size: Small")
    canvas.configure(bg="white")  # Reset background color

# Create the main window
root = tk.Tk()
root.title("Gesture Control UI")
root.geometry("500x400")
root.configure(bg='white')

# Load a custom font (if you have it, or use default)
custom_font = font.Font(family="Helvetica", size=14, weight="bold")

# Create a label to show the current gesture
gesture_label = tk.Label(root, text="Current Gesture: None", font=custom_font, bg='white')
gesture_label.pack(pady=20)

# Create a label to show the movement speed
speed_label = tk.Label(root, text="Movement Speed: 0 px/s", font=custom_font, bg='white')
speed_label.pack(pady=10)

# Create a label to show the movement direction
direction_label = tk.Label(root, text="Direction: None", font=custom_font, bg='white')
direction_label.pack(pady=10)

# Create a label to show the cursor size
cursor_size_label = tk.Label(root, text="Cursor Size: Small", font=custom_font, bg='white')
cursor_size_label.pack(pady=10)

# Create a large canvas to represent the cursor
canvas = tk.Canvas(root, width=800, height=800, bg="white", highlightthickness=0)
canvas.pack(pady=20)

# Draw a large cursor
cursor = canvas.create_oval(85, 85, 115, 115, fill="blue", outline="")

# Bind enter and leave events for cursor effects
canvas.bind("<Enter>", on_enter)
canvas.bind("<Leave>", on_leave)

# Initialize global variables
last_gesture = "None"
movement_speed = 0

# Start updating the UI
update_ui()

# Run the Tkinter main loop
try:
    root.mainloop()
except KeyboardInterrupt:
    print("Program interrupted")
finally:
    ser.close()
