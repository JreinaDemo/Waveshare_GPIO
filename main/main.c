/**************************************************************************************************/
/** @file     main.c
 *  @brief    05_IO_Test example
 *  @details  Use of Waveshare GPIO expander for GPIO demonstration
 *
 *  @author   Justin Reina, Firmware Engineer
 *  @created  3/21/25
 *  @last rev 3/21/25
 *
 *  @section    Pin Description
 * 		DI0 - is the IO0 of CH422G, which corresponds to 0x01
 * 		DI1 - is IO5 for CH422G,    corresponding to 0x20
 * 		DO0 - is the OC0 of CH422G, corresponding to 0x01
 * 		DO1 - is the OC1 of CH422G, corresponding to 0x02
 *
 *  @section    Legal Disclaimer
 *      ©2025 Justin Reina. All rights reserved. All contents of this source file and/or any other
 *      related source files are the explicit property of Justin Reina. Do not distribute.
 *      Do not copy.
 */
/**************************************************************************************************/

//************************************************************************************************//
//                                            INCLUDES                                            //
//************************************************************************************************//

//Standard Lbirary Includes
#include <stdint.h>

//SDK Includes
#include "LCD.h"									/* @todo descr 								  */
#include "CH422G.h"									/* @todo descr 								  */


//************************************************************************************************//
//                                            VARIABLES                                           //
//************************************************************************************************//

uint8_t io_flag = 1; 								/* IO flag									  */


//************************************************************************************************//
//                                          PUBLIC FUNCTIONS                                      //
//************************************************************************************************//

/**************************************************************************************************/
/** @fcn        int app_main(void)
 *  @brief      x
 *  @details    x
 */
/**************************************************************************************************/
int app_main(void) {

	//Locals
    uint8_t io[2]   = {0}; 							/* IO array	 							 	  */
    uint8_t DI_flag = 0; 							/* DI flag	 							 	  */
    uint8_t  num    = 0;							/* detection count	 					 	  */
		
    DEV_Module_Init();                         		/* Initialize the device module				  */
    
    
    for(;;) {
		
        // Output a value to the CH422G GPIO
        CH422G_od_output(0x01);
        DEV_Delay_ms(1);               				/* min delay of 1ms after writing io, else 
        											   data can't be read 						  */

		//Read Pins
        io[0] = CH422G_io_input(0x01); 				/* Read IO pin 0x01							  */
        io[1] = CH422G_io_input(0x20); 				/* Read IO pin 0x20							  */

        // Check if both pins match expected values
        if((io[0] == 0x01) && (io[1] == 0x00)) {
            DI_flag++; 								/* Increment DI flag						  */
        }

        // Output another value to the CH422G GPIO
        CH422G_od_output(0x02);
        DEV_Delay_ms(1);
        
        io[0] = CH422G_io_input(0x01); 				/* Re-read IO pin 0x01						  */
        io[1] = CH422G_io_input(0x20); 				/* Re-read IO pin 0x20						  */

        // Check again if both pins match expected values
        if((io[0] == 0x00) && (io[1] == 0x20)) {
            DI_flag++; 								/* Increment DI flag						  */
        }

        // If both conditions are met, DI & DO are working
        if(DI_flag >= 2) {
			
            printf("DI & DO OK!!!\r\n"); 			/* DI and DO are functioning properly		  */
            
            io_flag = 0;                 			/* Reset IO flag						      */
            
            break;
            
        } else {
			
            num++;        							/* Add 1 to the count						  */
            
            if(num == 3) { 							/* If the test fails three times, we quit	  */

                printf("DI & DO Failure!!!\r\n"); 	/* DI and DO are not functioning			  */
                
                break;
            }
        }
    }

    // Initialize the LCD and get the display object
    lv_disp_t *disp = LCD_init();

    // Run the custom LVGL UI demo
    example_lvgl_demo_ui(disp);

    // Loop to handle LVGL timers
    for(;;) {
		
        lv_timer_handler(); 						/* Handle LVGL tasks						  */
        
        DEV_Delay_ms(10);   						/* Delay to prevent CPU overload			  */
    }
}

