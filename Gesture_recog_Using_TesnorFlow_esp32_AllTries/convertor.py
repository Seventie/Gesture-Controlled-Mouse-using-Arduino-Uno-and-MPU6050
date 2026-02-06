import csv
import numpy as np
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, LSTM, Flatten
from tensorflow.keras.utils import to_categorical
import zlib

# Constants
CSV_FILE = 'gesture_data_1.csv'  # Input CSV file
TFLITE_MODEL_FILE = 'gesture_model_1.tflite'  # Output TFLite model file
HEADER_FILE = 'gesture_model_1.h'  # Output header file

# Function to read data from CSV and prepare training data
def read_csv_data(csv_file):
    all_data = []
    labels = []
    gestures = set()

    with open(csv_file, 'r') as csvfile:
        csv_reader = csv.reader(csvfile)
        next(csv_reader)  # Skip header row

        for row in csv_reader:
            gesture, ax, ay, az, gx, gy, gz = row
            all_data.append([float(ax), float(ay), float(az), float(gx), float(gy), float(gz)])
            labels.append(gesture)
            gestures.add(gesture)

    return np.array(all_data), np.array(labels), gestures

# Read data from CSV
X, y, gestures = read_csv_data(CSV_FILE)

# Convert labels to integers
gesture_to_int = {gesture: index for index, gesture in enumerate(gestures)}
y_int = np.array([gesture_to_int[gesture] for gesture in y])
y_categorical = to_categorical(y_int, num_classes=len(gestures))

# Reshape X for LSTM input (samples, timesteps, features)
X = X.reshape((X.shape[0], 1, X.shape[1]))  # Reshape to (samples, timesteps=1, features=6)

# Define the LSTM model
model = Sequential([
    LSTM(64, input_shape=(1, 6), return_sequences=True),
    Flatten(),
    Dense(32, activation='relu'),
    Dense(len(gestures), activation='softmax')
])

# Compile the model
model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

# Train the model
model.fit(X, y_categorical, epochs=10, batch_size=4)

# Convert the model to TensorFlow Lite
converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.target_spec.supported_ops = [
    tf.lite.OpsSet.TFLITE_BUILTINS,
    tf.lite.OpsSet.SELECT_TF_OPS
]
tflite_model = converter.convert()

# Compress the model using zlib
compressed_model = zlib.compress(tflite_model)

# Save the compressed model to a .tflite file
with open(TFLITE_MODEL_FILE, "wb") as f:
    f.write(compressed_model)

print("Model training complete and converted to TensorFlow Lite.")

# Convert .tflite model to C array for Arduino
def convert_to_c_array(data, output_file):
    try:
        # Create C array in hexadecimal format
        c_array = ", ".join(f"0x{byte:02x}" for byte in data)  # Hexadecimal representation
        data_length = len(data)

        with open(output_file, "w") as f:
            f.write("const uint8_t gesture_model[] = {\n")
            f.write(f"{c_array}\n")
            f.write("};\n")
            f.write(f"const unsigned int gesture_model_len = {data_length};\n")

        print("Successfully converted to C array.")
    except Exception as e:
        print(f"An error occurred while converting to C array: {e}")

# Convert the compressed model to a C header file
convert_to_c_array(compressed_model, HEADER_FILE)
print(f"Model saved as C array in {HEADER_FILE}.")