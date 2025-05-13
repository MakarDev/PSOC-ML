#include <stdint.h>
#include "lut_8bit.h"  // your generated LUT
#include "W1.h"
#include "B1.h"
#include "W2.h"
#include "B2.h"

#define INPUT_SIZE 784
#define HIDDEN_SIZE 64
#define OUTPUT_SIZE 10

#define LUT_INDEX(a, b) (((uint16_t)(a) << 8) | (uint16_t)(b))


// Weights stored in Flash
// const int8_t W1[HIDDEN_SIZE][INPUT_SIZE] = { ... }; // (64 × 784)
// const int8_t B1[HIDDEN_SIZE] = { ... };

// const int8_t W2[OUTPUT_SIZE][HIDDEN_SIZE] = { ... }; // (10 × 64)
// const int8_t B2[OUTPUT_SIZE] = { ... };

// Intermediate buffers
int8_t input[INPUT_SIZE];
int8_t hidden[HIDDEN_SIZE];
int8_t output[OUTPUT_SIZE];

void relu(int8_t* vec, int len) {
    int i;
    for (i = 0; i < len; i++) {
        if (vec[i] < 0) vec[i] = 0;
    }
}

void dense_layer(
    int8_t* input, int input_len,
    const int8_t* weights, const int8_t* bias,
    int8_t* output, int output_len
) { 
    int i;
    for (i = 0; i < output_len; i++) {
        int32_t sum = 0;
        int j;
        for (j = 0; j < input_len; j++) {
            uint16_t prod = mult_LUT[LUT_INDEX(input[j], weights[i * input_len + j])];
            sum += prod;
        }
        sum += bias[i];
        output[i] = sum > 127 ? 127 : (sum < -128 ? -128 : (int8_t)sum); // clamp
    }
}
int run_inference(int8_t* input_image) {
    // Input → Hidden
    dense_layer(input_image, INPUT_SIZE, W1, B1, hidden, HIDDEN_SIZE);
    relu(hidden, HIDDEN_SIZE);

    // Hidden → Output
    dense_layer(hidden, HIDDEN_SIZE, W2, B2, output, OUTPUT_SIZE);

    // Argmax
    int max_val = -128;
    int max_idx = -1;
    int i;
    for (i = 0; i < OUTPUT_SIZE; i++) {
        if (output[i] > max_val) {
            max_val = output[i];
            max_idx = i;
        }
    }

    return max_idx;  // Predicted digit
}
