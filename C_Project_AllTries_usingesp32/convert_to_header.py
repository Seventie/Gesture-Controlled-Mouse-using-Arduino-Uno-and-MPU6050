# Python script to convert .tflite model to .h file
import sys

# Check if the correct argument is provided
if len(sys.argv) != 3:
    print("Usage: python convert_to_header.py <model.tflite> <output.h>")
    sys.exit(1)

# File paths
tflite_model_path = sys.argv[1]
header_file_path = sys.argv[2]

# Read the TFLite model file
with open(tflite_model_path, 'rb') as f:
    model_data = f.read()

# Create the header file
with open(header_file_path, 'w') as f:
    f.write("// Converted TensorFlow Lite model\n")
    f.write(f"const unsigned char {header_file_path.split('.')[0]}[] = {{\n")

    # Write model data as a comma-separated array of bytes
    for i, byte in enumerate(model_data):
        if i % 12 == 0:
            f.write("\n  ")
        f.write(f"0x{byte:02x}, ")
    f.write("\n};\n")
    f.write(f"const int {header_file_path.split('.')[0]}_len = {len(model_data)};\n")

print(f"{header_file_path} created successfully!")
