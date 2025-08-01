/*******************************************************************************
* File Name: MAC_unit_1_go_to_mac.h  
* Version 1.80
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CONTROL_REG_MAC_unit_1_go_to_mac_H) /* CY_CONTROL_REG_MAC_unit_1_go_to_mac_H */
#define CY_CONTROL_REG_MAC_unit_1_go_to_mac_H

#include "cytypes.h"

    
/***************************************
*     Data Struct Definitions
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 controlState;

} MAC_unit_1_go_to_mac_BACKUP_STRUCT;


/***************************************
*         Function Prototypes 
***************************************/

void    MAC_unit_1_go_to_mac_Write(uint8 control) ;
uint8   MAC_unit_1_go_to_mac_Read(void) ;

void MAC_unit_1_go_to_mac_SaveConfig(void) ;
void MAC_unit_1_go_to_mac_RestoreConfig(void) ;
void MAC_unit_1_go_to_mac_Sleep(void) ; 
void MAC_unit_1_go_to_mac_Wakeup(void) ;


/***************************************
*            Registers        
***************************************/

/* Control Register */
#define MAC_unit_1_go_to_mac_Control        (* (reg8 *) MAC_unit_1_go_to_mac_Sync_ctrl_reg__CONTROL_REG )
#define MAC_unit_1_go_to_mac_Control_PTR    (  (reg8 *) MAC_unit_1_go_to_mac_Sync_ctrl_reg__CONTROL_REG )

#endif /* End CY_CONTROL_REG_MAC_unit_1_go_to_mac_H */


/* [] END OF FILE */
