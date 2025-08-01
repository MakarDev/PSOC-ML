/*******************************************************************************
* File Name: MAC_unit_1_input_a_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "MAC_unit_1_input_a.h"

/* Check for removal by optimization */
#if !defined(MAC_unit_1_input_a_Sync_ctrl_reg__REMOVED)

static MAC_unit_1_input_a_BACKUP_STRUCT  MAC_unit_1_input_a_backup = {0u};

    
/*******************************************************************************
* Function Name: MAC_unit_1_input_a_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void MAC_unit_1_input_a_SaveConfig(void) 
{
    MAC_unit_1_input_a_backup.controlState = MAC_unit_1_input_a_Control;
}


/*******************************************************************************
* Function Name: MAC_unit_1_input_a_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void MAC_unit_1_input_a_RestoreConfig(void) 
{
     MAC_unit_1_input_a_Control = MAC_unit_1_input_a_backup.controlState;
}


/*******************************************************************************
* Function Name: MAC_unit_1_input_a_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void MAC_unit_1_input_a_Sleep(void) 
{
    MAC_unit_1_input_a_SaveConfig();
}


/*******************************************************************************
* Function Name: MAC_unit_1_input_a_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void MAC_unit_1_input_a_Wakeup(void)  
{
    MAC_unit_1_input_a_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
