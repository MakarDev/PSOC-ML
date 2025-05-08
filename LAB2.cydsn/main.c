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
#include "lut_8bit.h"  // your generated LUT
#include <project.h>
#include "tft.h"
#include <math.h>

//void LCD_Char_1_PrintHorizontalLine(uint16 length);

uint16 adcResult = 0;

#define LUT_INDEX(a, b) (((uint16_t)(a) << 8) | (uint16_t)(b))

// Accumulator result (optional mirror for debugging)
static uint8_t acc_result = 0;
static uint8_t acc_result_debug = 0;

// Global variables for line drawing
static uint16_t x_head = 0;
static uint16_t y_head = 0;

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
    int i;
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
    uint8 i;
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
        
        // Wait for 2 seconds before next test
        CyDelay(2000);
    }
    
    // Display final accumulated result
    LCD_Char_1_ClearDisplay();
    sprintf(lcd_buffer, "Final Sum: %d", acc_result);
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