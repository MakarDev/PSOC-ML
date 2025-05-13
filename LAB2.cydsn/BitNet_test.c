/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <stdint.h>
#include <stdio.h>

#include "BitNetMCU_model.h"
#include "BitNetMCU_inference.h"
#include "BitNetMCU_MNIST_test_data.h"

/**
 * Performs inference on the MNIST dataset using the BitNetMCU model.
 *
 * @param input The input data for the inference, a 16x16 array of int8_t.
 * @return The predicted digit.
 */

uint32_t BitMnistInference(int8_t*);

uint8_t run_bitnet_test(void) {
    uint32_t output[10];
    uint8_t predicted_label;
    // predicted_label = BitMnistInference(input_data_0);
    // printf("label: %d predicted: %d\n", label_0, predicted_label);
    // predicted_label = BitMnistInference(input_data_1);
    // printf("label: %d predicted: %d\n", label_1, predicted_label);
    // predicted_label = BitMnistInference(input_data_2);
    // printf("label: %d predicted: %d\n", label_2, predicted_label);
    // predicted_label = BitMnistInference(input_data_3);
    // printf("label: %d predicted: %d\n", label_3, predicted_label);
    // predicted_label = BitMnistInference(input_data_4);
    // printf("label: %d predicted: %d\n", label_4, predicted_label);
    // predicted_label = BitMnistInference(input_data_5);
    // printf("label: %d predicted: %d\n", label_5, predicted_label);
    // predicted_label = BitMnistInference(input_data_6);
    // printf("label: %d predicted: %d\n", label_6, predicted_label);
    // predicted_label = BitMnistInference(input_data_7);
    // printf("label: %d predicted: %d\n", label_7, predicted_label);
    // predicted_label = BitMnistInference(input_data_8);
    // printf("label: %d predicted: %d\n", label_8, predicted_label);
    // predicted_label = BitMnistInference(input_data_9);
    // printf("label: %d predicted: %d\n", label_9, predicted_label);
    predicted_label = BitMnistInference(input_data_0);
    //printf("label: %d predicted: %d\n", label_10, predicted_label);
    return predicted_label;
}

/**
 * @brief Performs inference on the BitMnist model.
 *
 * @param input The input data for the inference.
 * @return The result of the inference.
 */
uint32_t BitMnistInference(int8_t *input) {
    int32_t layer_out[256];
    uint8_t layer_in[256];

    processfclayer(input, L3_weights, L3_bitperweight, L3_incoming_weights, L3_outgoing_weights, layer_out);
    ReLUNorm(layer_out, layer_in, L3_outgoing_weights);

    processfclayer(layer_in, L5_weights, L5_bitperweight, L5_incoming_weights,  L5_outgoing_weights, layer_out);
    ReLUNorm(layer_out, layer_in, L5_outgoing_weights);

    processfclayer(layer_in, L7_weights, L7_bitperweight, L7_incoming_weights,  L7_outgoing_weights, layer_out);
    ReLUNorm(layer_out, layer_in, L7_outgoing_weights);

    processfclayer(layer_in, L9_weights, L9_bitperweight, L9_incoming_weights,  L9_outgoing_weights, layer_out);
    return ReLUNorm(layer_out, layer_in, L9_outgoing_weights);
}
/* [] END OF FILE */
