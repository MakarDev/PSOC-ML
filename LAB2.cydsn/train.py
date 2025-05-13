import numpy as np
import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Flatten
from tensorflow.keras.datasets import mnist
from tensorflow.keras.utils import to_categorical

# Load MNIST
(x_train, y_train), (x_test, y_test) = mnist.load_data()
x_train = x_train / 255.0
x_test = x_test / 255.0

# Convert labels to one-hot
y_train = to_categorical(y_train, 10)
y_test = to_categorical(y_test, 10)

# Build MLP Model: 784 → 64 → 10
model = Sequential([
    Flatten(input_shape=(28, 28)),
    Dense(64, activation='relu'),
    Dense(10, activation='linear')  # use linear to match fixed-point output
])

model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

print("Training model...")
model.fit(x_train, y_train, epochs=5, batch_size=64, validation_split=0.1)

# Evaluate
loss, acc = model.evaluate(x_test, y_test)
print(f"Test Accuracy: {acc:.4f}")

# Get weights
W1, B1 = model.layers[1].get_weights()
W2, B2 = model.layers[2].get_weights()

# Quantize to int8
def quantize_array(arr, scale=127.0):
    return np.clip(np.round(arr * scale), -128, 127).astype(np.int8)

W1_q = quantize_array(W1)
B1_q = quantize_array(B1)
W2_q = quantize_array(W2)
B2_q = quantize_array(B2)

# Export as .h files
def to_c_array(name, arr):
    flat = arr.flatten()
    shape = ','.join(str(d) for d in arr.shape)
    c = f"// shape: {arr.shape}\n"
    c += f"const int8_t {name}[] = {{\n    "
    c += ', '.join(str(x) for x in flat)
    c += "\n};\n"
    return c

with open("W1.h", "w") as f: f.write(to_c_array("W1", W1_q))
with open("B1.h", "w") as f: f.write(to_c_array("B1", B1_q))
with open("W2.h", "w") as f: f.write(to_c_array("W2", W2_q))
with open("B2.h", "w") as f: f.write(to_c_array("B2", B2_q))

print("Exported W1.h, B1.h, W2.h, B2.h.")
