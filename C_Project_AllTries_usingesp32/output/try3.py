import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler, LabelEncoder
from tensorflow import keras
from tensorflow.keras import layers
import tensorflow as tf
import os

# Print current working directory and files
print("Current working directory:", os.getcwd())
print("Files in directory:", os.listdir())

# Step 1: Load and preprocess the data
try:
    # Load the CSV file
    file_name = 'gesture_data_1.csv'  # Replace with your CSV filename
    data = pd.read_csv(file_name)
    print("\nData loaded successfully")
    print("\nFirst few rows of data:")
    print(data.head())
    print("\nColumns in dataset:", data.columns.tolist())

    # Separate features and labels
    X = data[['acceleration.x', 'acceleration.y', 'acceleration.z']].values
    y = data['gesture'].values

    # Encode labels
    label_encoder = LabelEncoder()
    y_encoded = label_encoder.fit_transform(y)
    print("\nGesture classes:", label_encoder.classes_)

    # Normalize features
    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)

    # Split the dataset
    X_train, X_test, y_train, y_test = train_test_split(X_scaled, y_encoded, test_size=0.2, random_state=42)
    print("\nData preprocessing completed")

    # Step 2: Build and train the model
    model = keras.Sequential([
        layers.Dense(64, activation='relu', input_shape=(X_train.shape[1],)),
        layers.Dense(32, activation='relu'),
        layers.Dense(len(np.unique(y_encoded)), activation='softmax')
    ])

    model.compile(optimizer='adam',
                 loss='sparse_categorical_crossentropy',
                 metrics=['accuracy'])

    print("\nTraining the model...")
    history = model.fit(X_train, y_train, epochs=50, batch_size=32, validation_split=0.2, verbose=1)

    # Step 3: Evaluate the model
    test_loss, test_accuracy = model.evaluate(X_test, y_test)
    print(f'\nTest accuracy: {test_accuracy:.4f}')

    # Step 4: Convert to TensorFlow Lite
    print("\nConverting model to TensorFlow Lite...")
    converter = tf.lite.TFLiteConverter.from_keras_model(model)
    tflite_model = converter.convert()

    # Save the TensorFlow Lite model
    with open('gesture_model.tflite', 'wb') as f:
        f.write(tflite_model)
    print("TensorFlow Lite model saved as 'gesture_model.tflite'")

    # Step 5: Create C array for Arduino
    def convert_tflite_to_c_array(tflite_model, output_file):
        with open(output_file, 'w') as f:
            f.write("#ifndef GESTURE_MODEL_H\n")
            f.write("#define GESTURE_MODEL_H\n\n")
            f.write("const unsigned char model[] = {\n")
            for i, byte in enumerate(tflite_model):
                f.write(f"0x{byte:02x},")
                if (i + 1) % 12 == 0:
                    f.write("\n")
            f.write("\n};\n")
            f.write(f"const unsigned int model_len = {len(tflite_model)};\n")
            f.write("\n#endif // GESTURE_MODEL_H\n")

    # Convert to C array
    convert_tflite_to_c_array(tflite_model, 'gesture_model.h')
    print("C array model saved as 'gesture_model.h'")

    # Step 6: Save model details
    with open('model_details.h', 'w') as f:
        f.write("#ifndef MODEL_DETAILS_H\n")
        f.write("#define MODEL_DETAILS_H\n\n")
        f.write(f"#define INPUT_SHAPE {X_train.shape[1]}\n")
        f.write(f"#define NUM_CLASSES {len(np.unique(y_encoded))}\n\n")
        f.write("const char* GESTURES[] = {")
        f.write(", ".join([f'"{gesture}"' for gesture in label_encoder.classes_]))
        f.write("};\n\n")
        f.write("#endif // MODEL_DETAILS_H\n")
    print("Model details saved as 'model_details.h'")

    # Step 7: Save normalization parameters
    with open('normalization_params.h', 'w') as f:
        f.write("#ifndef NORMALIZATION_PARAMS_H\n")
        f.write("#define NORMALIZATION_PARAMS_H\n\n")
        f.write("const float SCALER_MEAN[] = {")
        f.write(", ".join([f"{mean:.6f}" for mean in scaler.mean_]))
        f.write("};\n\n")
        f.write("const float SCALER_SCALE[] = {")
        f.write(", ".join([f"{scale:.6f}" for scale in scaler.scale_]))
        f.write("};\n\n")
        f.write("#endif // NORMALIZATION_PARAMS_H\n")
    print("Normalization parameters saved as 'normalization_params.h'")

    print("\nAll files have been generated successfully!")
    print("\nFiles generated:")
    print("1. gesture_model.tflite - The TensorFlow Lite model")
    print("2. gesture_model.h - The model as a C array")
    print("3. model_details.h - Model parameters and gesture names")
    print("4. normalization_params.h - Normalization parameters")

except FileNotFoundError:
    print(f"Error: The file '{file_name}' was not found.")
    print("Please make sure the file is in the correct directory and the filename is correct.")
except Exception as e:
    print(f"An error occurred: {str(e)}")