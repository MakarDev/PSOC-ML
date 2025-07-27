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

// Add these declarations after the global variables
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define TARGET_SIZE 16

// Buffer to store drawn pixels (1 bit per pixel)
static uint8_t drawn_pixels[(SCREEN_WIDTH * SCREEN_HEIGHT + 7) / 8] = {0};

// Function to set a pixel in the buffer
void set_pixel(uint16_t x, uint16_t y) {
    if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) return;
    uint32_t index = y * SCREEN_WIDTH + x;
    drawn_pixels[index / 8] |= (1 << (index % 8));
}

// Function to clear the pixel buffer
void clear_pixel_buffer() {
    memset(drawn_pixels, 0, sizeof(drawn_pixels));
}

// Function to downscale the drawing to 16x16
void downscale_to_16x16(int8_t* output) {
    int block_width = SCREEN_WIDTH / TARGET_SIZE;
    int block_height = SCREEN_HEIGHT / TARGET_SIZE;
    uint32_t index;
    int y;
    int x;
    for (y = 0; y < TARGET_SIZE; y++) {
        for (x = 0; x < TARGET_SIZE; x++) {
            int pixel_drawn = 0;
            
            // Check if any pixel in this block is drawn
            int by;
            int bx;
            for (by = 0; by < block_height && !pixel_drawn; by++) {
                for (bx = 0; bx < block_width && !pixel_drawn; bx++) {
                    int screen_x = x * block_width + bx;
                    int screen_y = y * block_height + by;
                    if (screen_x < SCREEN_WIDTH && screen_y < SCREEN_HEIGHT) {
                        index = screen_y * SCREEN_WIDTH + screen_x;
                        if (drawn_pixels[index / 8] & (1 << (index % 8))) {
                            pixel_drawn = 1;
                        }
                    }
                }
            }
            
            // If any pixel in the block is drawn, mark the whole block as drawn
            output[y * TARGET_SIZE + x] = pixel_drawn ? 127 : 0;
        }
    }
}

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
        
        for (i = start_x; i <= end_x; i++) {
            // Draw 7x7 block for each point
            int dy, dx;
            for(dy = -1; dy <= 1; dy++) {
                for(dx = -1; dx <= 1; dx++) {
                    if(i + dx >= 0 && i + dx < SCREEN_WIDTH && 
                       y_in + dy >= 0 && y_in + dy < SCREEN_HEIGHT) {
                        // Set display window for this pixel
                        write8_a0(0x2A);  // Column Address Set
                        write8_a1((i + dx) >> 8);
                        write8_a1((i + dx) & 0x00FF);
                        write8_a1((i + dx) >> 8);
                        write8_a1((i + dx) & 0x00FF);
                        
                        write8_a0(0x2B);  // Page Address Set
                        write8_a1((y_in + dy) >> 8);
                        write8_a1((y_in + dy) & 0x00FF);
                        write8_a1((y_in + dy) >> 8);
                        write8_a1((y_in + dy) & 0x00FF);
                        
                        write8_a0(0x2C);  // Memory Write
                        write8_a1(0xFF);  // Orange color
                        write8_a1(0x0F);
                        set_pixel(i + dx, y_in + dy);  // Store pixel in buffer
                    }
                }
            }
        }
    }

    // Draw vertical line if y coordinates differ
    if (y_in != y_head) {
        uint16_t start_y = (y_in < y_head) ? y_in : y_head;
        uint16_t end_y = (y_in < y_head) ? y_head : y_in;
        uint16_t i;
        
        for (i = start_y; i <= end_y; i++) {
            // Draw 7x7 block for each point
            int dy, dx;
            for(dy = -1; dy <= 1; dy++) {
                for(dx = -1; dx <= 1; dx++) {
                    if(x_in + dx >= 0 && x_in + dx < SCREEN_WIDTH && 
                       i + dy >= 0 && i + dy < SCREEN_HEIGHT) {
                        // Set display window for this pixel
                        write8_a0(0x2A);  // Column Address Set
                        write8_a1((x_in + dx) >> 8);
                        write8_a1((x_in + dx) & 0x00FF);
                        write8_a1((x_in + dx) >> 8);
                        write8_a1((x_in + dx) & 0x00FF);
                        
                        write8_a0(0x2B);  // Page Address Set
                        write8_a1((i + dy) >> 8);
                        write8_a1((i + dy) & 0x00FF);
                        write8_a1((i + dy) >> 8);
                        write8_a1((i + dy) & 0x00FF);
                        
                        write8_a0(0x2C);  // Memory Write
                        write8_a1(0xFF);  // Orange color
                        write8_a1(0x0F);
                        set_pixel(x_in + dx, i + dy);  // Store pixel in buffer
                    }
                }
            }
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
uint8 buffer[512], length;
void main()
{
    CyGlobalIntEnable;                  // Enable global interrupts
    /* Declare all variables at the start of the block */ 
    //USB_Start(0, USB_3V_OPERATION); // Start the USB peripheral
    //while(!USB_GetConfiguration()); // Wait until USB is configured
    //USB_EnableOutEP(2); // Enable our output endpoint (EP2)
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
        CyDelay(20);
    }
    
    // Display final accumulated result
    LCD_Char_1_ClearDisplay();
    sprintf(lcd_buffer, "Final Sum: %d", acc_result);
    LCD_Char_1_PrintString(lcd_buffer);
    LCD_Char_1_Position(1, 0);
    sprintf(lcd_buffer, "Debug=%d", acc_result_debug);
    LCD_Char_1_PrintString(lcd_buffer);
    CyDelay(200);
    
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
    

    // Run BitNet test
    //uint8_t predicted_label = run_bitnet_test(test_image);
    
    LCD_Char_1_ClearDisplay();

    //sprintf(lcd_buffer, "Predicted: %d", predicted_label);
    //LCD_Char_1_PrintString(lcd_buffer);
    //CyDelay(2000);

    // Continue with the rest of your original code...
    //ADC for horizontal movement
    ADC_DelSig_1_Start();                // start the ADC_DelSig_1
    ADC_DelSig_1_StartConvert();         // start the ADC_DelSig_1 conversion

    //ADC for vertical movement
    ADC_SAR_1_Start();            // Initialize and enable ADC
    ADC_SAR_1_StartConvert();     // Start continuous ADC conversion

    // Initialize and display TFT
    display_tft();
    clear_pixel_buffer();  // Clear the pixel buffer
    
    // Test coordinates for line drawing
    uint16_t test_x = 0;
    uint16_t test_y = 120;  // Middle of the screen vertically
    
    // Add a flag to indicate when drawing is complete
    uint8_t drawing_complete = 0;
    
    for(;;)
    {
        //USB_LoadInEP(1, "Check", 5); // Echo the data back into the buffer
        //while(USB_GetEPState(2) == USB_OUT_BUFFER_EMPTY);   // Wait until we have data
        //length = USB_GetEPCount(2);                         // Get the length of received data
        //USB_ReadOutEP(2, buffer, length);                   // Get the data
        //while(USB_GetEPState(1) != USB_IN_BUFFER_EMPTY);    // Wait until our IN EP is empty
        //uint8_t buffer[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        //uint8_t buffer_length = sizeof(buffer);
        //USB_LoadInEP(1, buffer, buffer_length);
        
        if (!drawing_complete) {
            // Check if CLR button is pressed to clear display
            if (CLR_Read() == 0) {
                display_tft();  // Clear TFT display
                clear_pixel_buffer();  // Clear pixel buffer
                x_head = 0;  // Reset head position
                y_head = 0;
                CyDelay(200);  // Debounce delay
            }

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
            
            // Check if a button is pressed to complete drawing
            if (Pin_3_Read() == 0) {  // Pin_3 is connected to SW3
                drawing_complete = 1;
                
                // Downscale the drawing to 16x16
                int8_t digit_drawn[256] = {0};
                downscale_to_16x16(digit_drawn);
                
                // Flip the image horizontally
                //flip_image_horizontal(digit_drawn);

                // Display the downscaled image
                display_downscaled_image(digit_drawn);

                // Run BitNet inference on the drawn digit
                uint8_t predicted_label = run_bitnet_test(digit_drawn);
                
                // Display completion message and prediction
                LCD_Char_1_ClearDisplay();
                sprintf(lcd_buffer, "Predicted: %d", predicted_label);
                LCD_Char_1_PrintString(lcd_buffer);
                CyDelay(2000);
            }
        }
    }
}

// Function to display the downscaled image
void display_downscaled_image(int8_t* downscaled) {
    int scale_factor = SCREEN_WIDTH / TARGET_SIZE;  // 240/16 = 15
    int y, x, sy, sx;
    
    // Display each pixel of the downscaled image
    for (y = 0; y < TARGET_SIZE; y++) {
        for (x = 0; x < TARGET_SIZE; x++) {
            // Calculate block coordinates
            uint16_t start_x = x * scale_factor;
            uint16_t end_x = (x + 1) * scale_factor - 1;
            uint16_t start_y = y * scale_factor;
            uint16_t end_y = (y + 1) * scale_factor - 1;
            
            // Set display window for this block
            write8_a0(0x2A);  // Column Address Set
            write8_a1(start_x >> 8);
            write8_a1(start_x & 0xFF);
            write8_a1(end_x >> 8);
            write8_a1(end_x & 0xFF);
            
            write8_a0(0x2B);  // Page Address Set
            write8_a1(start_y >> 8);
            write8_a1(start_y & 0xFF);
            write8_a1(end_y >> 8);
            write8_a1(end_y & 0xFF);
            
            write8_a0(0x2C);  // Memory Write
            
            // Fill the block with the appropriate color
            for (sy = 0; sy < scale_factor; sy++) {
                for (sx = 0; sx < scale_factor; sx++) {
                    if (downscaled[y * TARGET_SIZE + x] == 127) {
                        write8_a1(0xFF);  // White color
                        write8_a1(0xFF);
                    } else {
                        write8_a1(0x00);  // Black color
                        write8_a1(0x00);
                    }
                }
            }
        }
    }
}

// Function to flip the downscaled image horizontally
void flip_image_horizontal(int8_t* image) {
    int8_t temp;
    int y,x;
    for (y = 0; y < TARGET_SIZE; y++) {
        for (x = 0; x < TARGET_SIZE/2; x++) {
            // Swap pixels from left and right sides
            temp = image[y * TARGET_SIZE + x];
            image[y * TARGET_SIZE + x] = image[y * TARGET_SIZE + (TARGET_SIZE-1-x)];
            image[y * TARGET_SIZE + (TARGET_SIZE-1-x)] = temp;
        }
    }
}

/* [] END OF FILE */