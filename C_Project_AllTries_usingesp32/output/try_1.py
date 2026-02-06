import pandas as pd
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler, LabelEncoder
from tensorflow import keras
from tensorflow.keras import layers
import os

# Print current working directory
print("Current working directory:", os.getcwd())

# List files in the current directory
print("Files in the current directory:", os.listdir())

# Step 1: Load and preprocess the data
# Replace 'your_actual_file_name.csv' with the name of your CSV file
file_name = 'gesture_data_1.csv'

try:
    # Load the CSV file
    data = pd.read_csv(file_name)
    print("Data loaded successfully")
    
    # Display the first few rows of the data
    print(data.head())
    print("Columns in the dataset:", data.columns.tolist())

    # Separate features and labels
    # Adjust these column names to match your actual CSV file
    X = data[['acceleration.x', 'acceleration.y', 'acceleration.z']].values
    y = data['gesture'].values  # Assuming the column is named 'gesture'

    # Encode labels (e.g., gesture_1 -> 0, gesture_2 -> 1, etc.)
    label_encoder = LabelEncoder()
    y_encoded = label_encoder.fit_transform(y)

    # Normalize the features
    scaler = StandardScaler()
    X_scaled = scaler.fit_transform(X)

    # Split the dataset into training and testing sets
    X_train, X_test, y_train, y_test = train_test_split(X_scaled, y_encoded, test_size=0.2, random_state=42)

    # Step 2: Build the TensorFlow model
    model = keras.Sequential([
        layers.Dense(64, activation='relu', input_shape=(X_train.shape[1],)),
        layers.Dense(32, activation='relu'),
        layers.Dense(len(np.unique(y_encoded)), activation='softmax')
    ])

    # Compile the model
    model.compile(optimizer='adam',
                  loss='sparse_categorical_crossentropy',
                  metrics=['accuracy'])

    # Step 3: Train the model
    model.fit(X_train, y_train, epochs=50, batch_size=32, validation_split=0.2)

    # Evaluate the model on the test set
    test_loss, test_accuracy = model.evaluate(X_test, y_test)
    print(f'Test accuracy: {test_accuracy:.4f}')

    # Step 4: Make predictions
    predictions = model.predict(X_test)
    predicted_classes = np.argmax(predictions, axis=1)

    # Decode the predicted labels back to original gesture names
    predicted_gestures = label_encoder.inverse_transform(predicted_classes)
    print(predicted_gestures)

except FileNotFoundError:
    print(f"Error: The file '{file_name}' was not found.")
    print("Please make sure the file is in the correct directory and the filename is correct.")
except Exception as e:
    print(f"An error occurred: {str(e)}")