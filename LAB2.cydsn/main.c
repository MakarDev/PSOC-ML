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

//void LCD_Char_1_PrintHorizontalLine(uint16 length);

uint16 adcResult = 0;

#define LUT_INDEX(a, b) (((uint16_t)(a) << 8) | (uint16_t)(b))

// Accumulator result (optional mirror for debugging)
static uint8_t acc_result = 0;
static uint8_t acc_result_debug = 0;
int display_tft()
{
     CyGlobalIntEnable;                  // Enable global interrupts
    SPIM_1_Start();                     // initialize SPIM component
    
    tftStart();                         // initialize the TFT display
    uint16 SC = 20;                     // define a 10x10 square with the top left corner located at pixel (10, 50)
    uint16 EC = 29;
    uint16 SP = 50;
    uint16 EP = 59;
    write8_a0(0x2A);                 	// send Column Address Set command
    write8_a1(0x00);                 // set SC[15:0]
    write8_a1(SC & 0x00FF);
    write8_a1(0x00);                 // set EC[15:0]
    write8_a1(EC & 0x00FF);
    write8_a0(0x2B);                 	// send Page Address Set command
    write8_a1(0x00);                 // set SP[15:0]
    write8_a1(SP & 0x00FF);
    write8_a1(0x00);                 // set EP[15:0]
    write8_a1(EP & 0x00FF);
    write8_a0(0x2C);                    // send Memory Write command
    int i;
    for (i=0; i<100; i++)               // fill the square with the color orange
	{
		write8_a1(0xFF); 			    // 0xFF0F is the color orange
		write8_a1(0x0F); 
	}
    write8_a0(0x00);    
    //for(;;) {}                          // loop

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
    /* Declare all variables at the start of the block */
    unsigned char j = 50;                // milliseconds delay
    uint16 lineLength;

   // LCD_Char_1_Start();                  // initialize lcd
   // LCD_Char_1_ClearDisplay();
   // LCD_Char_1_PrintString("");
    
    ADC_DelSig_1_Start();                // start the ADC_DelSig_1
    ADC_DelSig_1_StartConvert();         // start the ADC_DelSig_1 conversion

    // Initialize and display TFT
    //DC_Write(0xFF); 
    display_tft();
    //display_tft();
    for(;;)
    {
        // set DC line high

        if (0){
        // Test case 1: 5 * 3
        run_mac(5, 3, 0);
        LCD_Char_1_Position(0, 0);
        LCD_Char_1_PrintString("SW:5*3=");
        LCD_Char_1_PrintNumber(acc_result_debug);
        LCD_Char_1_Position(1, 0);
        LCD_Char_1_PrintString("HW:5*3=");
        LCD_Char_1_PrintNumber(acc_result);
        CyDelay(2000);
        
        // Test case 2: 10 * 4
        run_mac(10, 4, 0);
        LCD_Char_1_Position(0, 0);
        LCD_Char_1_PrintString("SW:10*4=");
        LCD_Char_1_PrintNumber(acc_result_debug);
        LCD_Char_1_Position(1, 0);
        LCD_Char_1_PrintString("HW:10*4=");
        LCD_Char_1_PrintNumber(acc_result);
        CyDelay(2000);
        
        // Test case 3: 15 * 2
        run_mac(15, 2, 0);
        LCD_Char_1_Position(0, 0);
        LCD_Char_1_PrintString("SW:15*2=");
        LCD_Char_1_PrintNumber(acc_result_debug);
        LCD_Char_1_Position(1, 0);
        LCD_Char_1_PrintString("HW:15*2=");
        LCD_Char_1_PrintNumber(acc_result);
        CyDelay(2000);
        run_mac(0, 0, 1);
        // Display ADC result
        if( ADC_DelSig_1_IsEndConversion(ADC_DelSig_1_WAIT_FOR_RESULT) )
        {
            LCD_Char_1_Position(0, 6);
            LCD_Char_1_PrintString("     ");            // clean up the previous display
            LCD_Char_1_Position(0, 6);
            adcResult = ADC_DelSig_1_GetResult16();     // read the adc and assign the value adcResult 
            
            if (adcResult & 0x8000)
            {
                adcResult = 0;       // ignore negative ADC results
            }
            else if (adcResult >= 0xfff)
            {
                adcResult = 0xfff;   // ignore high ADC results
            }
            
            /* Display the raw ADC value */
            LCD_Char_1_PrintNumber(adcResult);

            /* Scale ADC result (0-0xFFF) to line length (0-32) */
            lineLength = (adcResult * 32) / 0xFFF;
            
            /* Clear the line area and draw new line */
            LCD_Char_1_Position(1, 0);
            LCD_Char_1_PrintString("                                ");  // Clear the line
            LCD_Char_1_Position(1, 0);
            LCD_Char_1_PrintHorizontalLine(lineLength);

            CyDelay(j);                                 // delay in milliseconds
        }
        }
    }
}

/* [] END OF FILE */