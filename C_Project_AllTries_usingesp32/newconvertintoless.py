import tensorflow as tf

# Load the existing model
model = tf.keras.models.load_model('"C:\Users\abdus\OneDrive\Desktop\STUDY\SEM3\C_Project\gesture_model.h"')

model.save('gesture_model.h5')

# Convert the model with quantization
converter = tf.lite.TFLiteConverter.from_keras_model(model)
converter.optimizations = [tf.lite.Optimize.DEFAULT]
quantized_tflite_model = converter.convert()

# Save the quantized model to a new file
with open('gesture_model_quantized.tflite', 'wb') as f:
    f.write(quantized_tflite_model)

print("Quantized model saved as gesture_model_quantized.tflite")
