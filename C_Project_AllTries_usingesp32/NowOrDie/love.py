import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import LabelEncoder
from sklearn.preprocessing import StandardScaler
import tensorflow as tf
import numpy as np

# Load the data
data = pd.read_csv('gesture_data_1.csv')

# Separate features and labels
X = data[['acceleration.x', 'acceleration.y', 'acceleration.z']]
y = data['gesture']

# Encode labels
label_encoder = LabelEncoder()
y_encoded = label_encoder.fit_transform(y)

# Normalize the feature data
scaler = StandardScaler()
X_scaled = scaler.fit_transform(X)

# Split the data into training and validation sets
X_train, X_val, y_train, y_val = train_test_split(X_scaled, y_encoded, test_size=0.2, random_state=42)

# Build the model
model = tf.keras.models.Sequential([
    tf.keras.layers.Dense(32, activation='relu', input_shape=(X_train.shape[1],)),  # Smaller input layer
    tf.keras.layers.Dense(16, activation='relu'),  # Smaller hidden layer
    tf.keras.layers.Dense(len(label_encoder.classes_), activation='softmax')  # Output layer
])

# Compile the model
model.compile(optimizer='adam',
              loss='sparse_categorical_crossentropy',
              metrics=['accuracy'])

# Train the model
model.fit(X_train, y_train, validation_data=(X_val, y_val), epochs=10, batch_size=32)

# Evaluate the model
loss, accuracy = model.evaluate(X_val, y_val)
print(f"Validation Loss: {loss:.4f}, Validation Accuracy: {accuracy:.4f}")

# Save the trained model
model.save('gesture_model.h5')
print("Model saved as gesture_model.h5")

# Convert the model to TensorFlow Lite format with quantization
converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]  # Apply quantization
quantized_tflite_model = converter.convert()

# Save the quantized model to a new file
with open('gesture_model_quantized.tflite', 'wb') as f:
    f.write(quantized_tflite_model)

print("Quantized model saved as gesture_model_quantized.tflite")

# Convert the quantized model to a C header file
def create_header_file(tflite_model, filename):
    # Convert to a byte array
    byte_array = np.frombuffer(tflite_model, dtype=np.uint8)
    with open(filename, 'w') as f:
        f.write('const unsigned char gesture_model[] = {\n')
        for i in range(0, len(byte_array), 12):  # Print 12 bytes per line
            line = ', '.join(f'0x{byte:02x}' for byte in byte_array[i:i+12])
            f.write(f'    {line},\n')
        f.write('};\n')
        f.write(f'const unsigned int gesture_model_len = {len(byte_array)};\n')

# Create a C header file
create_header_file(quantized_tflite_model, 'gesture_model.h')
print("C header file saved as gesture_model.h")

# Save the label encoder for future use
import joblib
joblib.dump(label_encoder, 'label_encoder.pkl')
print("Label encoder saved as label_encoder.pkl")
