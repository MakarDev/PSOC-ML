/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * This file is necessary for your project to build.
 * Please do not delete it.
 *
 * ========================================
*/

#include <device.h>
//#include "lut_8bit.h"  // your generated LUT
#include <project.h>
#include "tft.h"
#include <math.h>
#include "ml.c"
#include "tinymaix.h"
#include "mnist_valid_q.h"
#include "BitNet_test.h"
//void LCD_Char_1_PrintHorizontalLine(uint16 length);

uint16 adcResult = 0;

#define LUT_INDEX(a, b) (((uint16_t)(a) << 8) | (uint16_t)(b))

// Accumulator result (optional mirror for debugging)
static uint8_t acc_result = 0;
static uint8_t acc_result_debug = 0;

// Global variables for line drawing
static uint16_t x_head = 0;
static uint16_t y_head = 0;

/*uint8_t mnist_pic[28*28]={
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,116,125,171,255,255,150, 93,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,169,253,253,253,253,253,253,218, 30,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,169,253,253,253,213,142,176,253,253,122,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0, 52,250,253,210, 32, 12,  0,  6,206,253,140,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0, 77,251,210, 25,  0,  0,  0,122,248,253, 65,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0, 31, 18,  0,  0,  0,  0,209,253,253, 65,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,117,247,253,198, 10,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 76,247,253,231, 63,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,128,253,253,144,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,176,246,253,159, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 25,234,253,233, 35,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,198,253,253,141,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0, 78,248,253,189, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0, 19,200,253,253,141,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,134,253,253,173, 12,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253, 25,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253, 43, 20, 20, 20, 20,  5,  0,  5, 20, 20, 37,150,150,150,147, 10,  0,
  0,  0,  0,  0,  0,  0,  0,  0,248,253,253,253,253,253,253,253,168,143,166,253,253,253,253,253,253,253,123,  0,
  0,  0,  0,  0,  0,  0,  0,  0,174,253,253,253,253,253,253,253,253,253,253,253,249,247,247,169,117,117, 57,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,118,123,123,123,166,253,253,253,155,123,123, 41,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
};*/
// uint8_t mnist_pic[28*28]={
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 36, 56,137,201,199, 95, 37,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 45,152,234,254,254,254,254,254,250,211,151,  6,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0, 46,153,240,254,254,227,166,133,251,200,254,229,225,104,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,153,234,254,254,187,142,  8,  0,  0,191, 40,198,246,223,253, 21,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  8,126,253,254,233,128, 11,  0,  0,  0,  0,210, 43, 70,254,254,254, 21,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0, 72,243,254,228, 54,  0,  0,  0,  0,  3, 32,116,225,242,254,255,162,  5,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0, 75,240,254,223,109,138,178,178,169,210,251,231,254,254,254,232, 38,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  9,175,244,253,255,254,254,251,254,254,254,254,254,252,171, 25,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0, 16,136,195,176,146,153,200,254,254,254,254,150, 16,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,162,254,254,241, 99,  3,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,118,250,254,254, 90,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,100,242,254,254,211,  7,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 54,241,254,254,242, 59,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,131,254,254,244, 64,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 13,249,254,254,152,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0, 12,228,254,254,208,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0, 78,255,254,254, 66,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,209,254,254,137,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,227,255,233, 25,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,113,255,108,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
//   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
// };
static tm_err_t layer_cb(tm_mdl_t* mdl, tml_head_t* lh)
{   //dump middle result
    int h = lh->out_dims[1];
    int w = lh->out_dims[2];
    int ch= lh->out_dims[3];
    mtype_t* output = TML_GET_OUTPUT(mdl, lh);
    return TM_OK;
    //TM_PRINTF("Layer %d callback ========\n", mdl->layer_i);
    #if 1
    int y, x, c;
    for(y=0; y<h; y++){
        //TM_PRINTF("[");
        for(x=0; x<w; x++){
          //  TM_PRINTF("[");
            for(c=0; c<ch; c++){
            #if TM_MDL_TYPE == TM_MDL_FP32
                //TM_PRINTF("%.3f,", output[(y*w+x)*ch+c]);
            #else
                //TM_PRINTF("%.3f,", TML_DEQUANT(lh,output[(y*w+x)*ch+c]));
            #endif
            }
            //TM_PRINTF("],");
        }
        //TM_PRINTF("],\n");
    }
    //TM_PRINTF("\n");
    #endif
    return TM_OK;
}

static void parse_output(tm_mat_t* outs)
{
    tm_mat_t out = outs[0];
    float* data  = out.dataf;
    float maxp = 0;
    int maxi = -1;
    int i;
    for(i=0; i<10; i++){
        printf("%d: %.3f\n", i, data[i]);
        if(data[i] > maxp) {
            maxi = i;
            maxp = data[i];
        }
    }
 // Display result
    LCD_Char_1_ClearDisplay();
    char lcd_buffer[16];  // Buffer for LCD display

    sprintf(lcd_buffer, "Predicted: %d", maxi);
    LCD_Char_1_PrintString(lcd_buffer);
    CyDelay(2000);
    return;
}



void draw_continuous_line(uint16_t x_in, uint16_t y_in) {
    // If this is the first point, just update head position
    if (x_head == 0 && y_head == 0) {
        x_head = x_in;
        y_head = y_in;
        return;
    }

    // Draw horizontal line if x coordinates differ
    if (x_in != x_head) {
        uint16_t start_x = (x_in < x_head) ? x_in : x_head;
        uint16_t end_x = (x_in < x_head) ? x_head : x_in;
        uint16_t i;
        
        write8_a0(0x2A);  // Column Address Set
        write8_a1(start_x >> 8);
        write8_a1(start_x & 0x00FF);
        write8_a1(end_x >> 8);
        write8_a1(end_x & 0x00FF);
        
        write8_a0(0x2B);  // Page Address Set
        write8_a1(y_in >> 8);
        write8_a1(y_in & 0x00FF);
        write8_a1(y_in >> 8);
        write8_a1(y_in & 0x00FF);
        
        write8_a0(0x2C);  // Memory Write
        for (i = start_x; i <= end_x; i++) {
            write8_a1(0xFF);  // Orange color
            write8_a1(0x0F);
        }
    }

    // Draw vertical line if y coordinates differ
    if (y_in != y_head) {
        uint16_t start_y = (y_in < y_head) ? y_in : y_head;
        uint16_t end_y = (y_in < y_head) ? y_head : y_in;
        uint16_t i;
        
        write8_a0(0x2A);  // Column Address Set
        write8_a1(x_in >> 8);
        write8_a1(x_in & 0x00FF);
        write8_a1(x_in >> 8);
        write8_a1(x_in & 0x00FF);
        
        write8_a0(0x2B);  // Page Address Set
        write8_a1(start_y >> 8);
        write8_a1(start_y & 0x00FF);
        write8_a1(end_y >> 8);
        write8_a1(end_y & 0x00FF);
        
        write8_a0(0x2C);  // Memory Write
        for (i = start_y; i <= end_y; i++) {
            write8_a1(0xFF);  // Orange color
            write8_a1(0x0F);
        }
    }

    // Update head position
    x_head = x_in;
    y_head = y_in;
}

int display_tft()
{
    SPIM_1_Start();                     // initialize SPIM component
    
    tftStart();                         // initialize the TFT display
    
    // Clear the screen with black color
    write8_a0(0x2A);  // Column Address Set
    write8_a1(0x00);
    write8_a1(0x00);
    write8_a1(0x01);
    write8_a1(0x3F);  // 320 pixels wide
    
    write8_a0(0x2B);  // Page Address Set
    write8_a1(0x00);
    write8_a1(0x00);
    write8_a1(0x00);
    write8_a1(0xEF);  // 240 pixels high
    
    write8_a0(0x2C);  // Memory Write
    int i;  // Declare i before the loop
    for (i = 0; i < 76800; i++) {  // 320 * 240 = 76800 pixels
        write8_a1(0x00);  // Black color
        write8_a1(0x00);
    }
    
    // Reset head position
    x_head = 0;
    y_head = 0;
    
    return 0;
}

void run_mac(uint8_t a, uint8_t b, uint8_t reset)
{
    if (reset) {
        MAC_unit_1_go_to_mac_Write(0x00);  // Reset mode
        acc_result = 0;
        acc_result_debug = 0;
        return;
    }

    // Look up product from Flash LUT
    uint8_t product = mult_LUT[LUT_INDEX(a, b)];

    // Write product to D0 register using proper register access
    MAC_unit_1_Datapath_1_D0_REG = product;
    
    // Set MAC unit control bit to 1 to start accumulation
    MAC_unit_1_go_to_mac_Write(0x01);
    uint8 dsignal = Pin_2_Read();
    // Read the accumulated value from A0
    //for debugging
    acc_result_debug += product;

    // Clear MAC unit control bit to 0 after obtaining sum
    while(dsignal != 1) {
        dsignal = Pin_2_Read();
    }
    acc_result = MAC_unit_1_Datapath_1_A0_REG;

    // Set to accumulate mode
    MAC_unit_1_go_to_mac_Write(0x03);
}

void LCD_Char_1_PrintHorizontalLine(uint16 length)
{
    uint8 i;  // Declare i before the loop
    // Calculate number of underscores to print (half of the length since each underscore takes 2 character spaces)
    uint8 num_underscores = length / 2;
    
    // Print the specified number of underscores
    for(i = 0; i < num_underscores; i++)
    {
        LCD_Char_1_PutChar('_');
    }
}

void main()
{
    CyGlobalIntEnable;                  // Enable global interrupts
    /* Declare all variables at the start of the block */
    unsigned char j = 50;                // milliseconds delay
    uint16 lineLength;
    int16 verticalReading = 0;
    LCD_Char_1_Start();                  // initialize lcd
    LCD_Char_1_ClearDisplay();
    
    // Test cases for multiplication
    uint8_t test_cases[][2] = {
        {5, 6},    // 5 * 6 = 30
        {10, 10},  // 10 * 10 = 100
        {15, 7},   // 15 * 7 = 105
        {8, 12}    // 8 * 12 = 96
    };
    
    char lcd_buffer[16];  // Buffer for LCD display
    int i = 0;
    
    // Reset MAC unit once at the start
    run_mac(0, 0, 1);
    
    // Run multiplication tests with accumulation
    //First multiplication: 5 * 6 = 30
    //Second multiplication: 10 * 10 = 100, sum becomes 130
    //Third multiplication: 15 * 7 = 105, sum becomes 235
    //Fourth multiplication: 8 * 12 = 96, final sum becomes 331
    for(i = 0; i < 4; i++) {
        // Perform multiplication (no reset between operations)
        run_mac(test_cases[i][0], test_cases[i][1], 0);
        
        // Display result on LCD
        LCD_Char_1_ClearDisplay();
        sprintf(lcd_buffer, "Sum %d*%d=%d", 
                test_cases[i][0], 
                test_cases[i][1], 
                acc_result);
        LCD_Char_1_PrintString(lcd_buffer);
        
        // Move to second line and show debug result
        LCD_Char_1_Position(1, 0);
        sprintf(lcd_buffer, "Debug=%d", acc_result_debug);
        LCD_Char_1_PrintString(lcd_buffer);
        
        // Wait for 2 seconds before next test
        CyDelay(2000);
    }
    
    // Display final accumulated result
    LCD_Char_1_ClearDisplay();
    sprintf(lcd_buffer, "Final Sum: %d", acc_result);
    LCD_Char_1_PrintString(lcd_buffer);
    LCD_Char_1_Position(1, 0);
    sprintf(lcd_buffer, "Debug=%d", acc_result_debug);
    LCD_Char_1_PrintString(lcd_buffer);
    CyDelay(2000);
    
    // Test digit recognition
    LCD_Char_1_ClearDisplay();
    LCD_Char_1_PrintString("Testing ML...");
    CyDelay(1000);
    
    // Create a simple test image (28x28 pixels)
    int8_t test_image[784];  // 28*28 = 784
    // Initialize with a simple pattern (you can modify this pattern)
        
    for(i = 0; i < 784; i++) {
        test_image[i] = (i % 28 < 10 || i % 28 > 18) ? 0 : 127;  // Simple vertical line pattern
    }
    
    // Run inference
    //int predicted_digit = run_inference(test_image);
    //  TM_DBGT_INIT();
    // tm_mdl_t mdl;

    // tm_mat_t in_uint8 = {3,28,28,1, {(mtype_t*)mnist_pic}};
    // tm_mat_t in = {3,28,28,1, {NULL}};
    // tm_mat_t outs[1];
    // tm_err_t res;
    // //tm_stat((tm_mdlbin_t*)mdl_data); 
    // res = tm_load(&mdl, mdl_data, NULL, layer_cb, &in);
    // if(res != TM_OK) {
    //     //TM_PRINTF("tm model load err %d\n", res);
    //     return -1;
    // }
    // TM_DBGT_START();
    // res = tm_run(&mdl, &in, outs);
    // TM_DBGT("tm_run");
    // if(res==TM_OK) parse_output(outs);  
    // //else TM_PRINTF("tm run error: %d\n", res);
    // tm_unload(&mdl);                 
   
    // Run BitNet test
    uint8_t predicted_label = run_bitnet_test();
    
    LCD_Char_1_ClearDisplay();

    sprintf(lcd_buffer, "Predicted: %d", predicted_label);
    LCD_Char_1_PrintString(lcd_buffer);
    CyDelay(2000);

    // Continue with the rest of your original code...
    //ADC for horizontal movement
    ADC_DelSig_1_Start();                // start the ADC_DelSig_1
    ADC_DelSig_1_StartConvert();         // start the ADC_DelSig_1 conversion

    //ADC for vertical movement
    ADC_SAR_1_Start();            // Initialize and enable ADC
    ADC_SAR_1_StartConvert();     // Start continuous ADC conversion

    // Initialize and display TFT
    display_tft();
    
    // Test coordinates for line drawing
    uint16_t test_x = 0;
    uint16_t test_y = 120;  // Middle of the screen vertically
    
    for(;;)
    {
        // Display ADC result
        if( ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT) )
        {
            adcResult = ADC_DelSig_1_GetResult16();     // read the adc and assign the value adcResult 
            
            if (adcResult & 0x8000)
            {
                adcResult = 0;       // ignore negative ADC results
            }
            else if (adcResult >= 0xfff)
            {
                adcResult = 0xfff;   // ignore high ADC results
            }
            
            // Map ADC result (0-0xFFF) to screen width (0-320)
            test_x = adcResult*240/0xFFF;
            
            // Draw the line at the new position
            draw_continuous_line(test_x, test_y);
            
            CyDelay(50);  // Small delay to make the movement visible
        }
        if( ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT) )
        {
            adcResult = ADC_SAR_1_GetResult16();     // read the adc and assign the value adcResult 
            
            if (adcResult & 0x8000)
            {
                adcResult = 0;       // ignore negative ADC results
            }
            else if (adcResult >= 0xfff)
            {
                adcResult = 0xfff;   // ignore high ADC results
            }
            
            // Map ADC result (0-0xFFF) to screen width (0-320)
            test_y = adcResult*240/0xFFF;
            
            // Draw the line at the new position
            draw_continuous_line(test_x, test_y);
            
            CyDelay(50);  // Small delay to make the movement visible
        }
    }
}

/* [] END OF FILE */