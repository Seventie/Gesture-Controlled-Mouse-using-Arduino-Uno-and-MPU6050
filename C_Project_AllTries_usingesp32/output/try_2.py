import tensorflow as tf
import numpy as np

# Assuming you have already trained your model and it's called 'model'
# and you have your label_encoder from the previous step

# Step 1: Convert the model to TensorFlow Lite format
converter = tf.lite.TFLiteConverter.from_keras_model(model)
tflite_model = converter.convert()

# Save the TensorFlow Lite model to a file
with open('gesture_model.tflite', 'wb') as f:
    f.write(tflite_model)

print("TensorFlow Lite model saved as 'gesture_model.tflite'")

# Step 2: Convert the TensorFlow Lite model to a C array
def convert_tflite_to_c_array(tflite_model, output_file):
    with open(output_file, 'w') as f:
        f.write("const unsigned char model[] = {\n")
        for i, byte in enumerate(tflite_model):
            f.write(f"0x{byte:02x},")
            if (i + 1) % 12 == 0:
                f.write("\n")
        f.write("\n};\n")
        f.write(f"const unsigned int model_len = {len(tflite_model)};\n")

# Convert the TensorFlow Lite model to a C array
convert_tflite_to_c_array(tflite_model, 'gesture_model.h')

print("C array model saved as 'gesture_model.h'")

# Step 3: Save model details
input_shape = model.input_shape[1]  # Assuming input shape is (None, n_features)
num_classes = len(np.unique(y_encoded))

with open('model_details.h', 'w') as f:
    f.write(f"#ifndef MODEL_DETAILS_H\n")
    f.write(f"#define MODEL_DETAILS_H\n\n")
    f.write(f"#define INPUT_SHAPE {input_shape}\n")
    f.write(f"#define NUM_CLASSES {num_classes}\n\n")
    f.write("const char* GESTURES[] = {")
    f.write(", ".join([f'"{gesture}"' for gesture in label_encoder.classes_]))
    f.write("};\n\n")
    f.write("#endif // MODEL_DETAILS_H\n")

print("Model details saved as 'model_details.h'")

# Step 4: Create a header file with normalization parameters
scaler_mean = scaler.mean_
scaler_scale = scaler.scale_

with open('normalization_params.h', 'w') as f:
    f.write("#ifndef NORMALIZATION_PARAMS_H\n")
    f.write("#define NORMALIZATION_PARAMS_H\n\n")
    f.write("const float SCALER_MEAN[] = {")
    f.write(", ".join([f"{mean:.6f}" for mean in scaler_mean]))
    f.write("};\n\n")
    f.write("const float SCALER_SCALE[] = {")
    f.write(", ".join([f"{scale:.6f}" for scale in scaler_scale]))
    f.write("};\n\n")
    f.write("#endif // NORMALIZATION_PARAMS_H\n")

print("Normalization parameters saved as 'normalization_params.h'")