/*******************************************************************************
* File Name: Pot_V.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Pot_V_H) /* Pins Pot_V_H */
#define CY_PINS_Pot_V_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pot_V_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pot_V__PORT == 15 && ((Pot_V__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Pot_V_Write(uint8 value);
void    Pot_V_SetDriveMode(uint8 mode);
uint8   Pot_V_ReadDataReg(void);
uint8   Pot_V_Read(void);
void    Pot_V_SetInterruptMode(uint16 position, uint16 mode);
uint8   Pot_V_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Pot_V_SetDriveMode() function.
     *  @{
     */
        #define Pot_V_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Pot_V_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Pot_V_DM_RES_UP          PIN_DM_RES_UP
        #define Pot_V_DM_RES_DWN         PIN_DM_RES_DWN
        #define Pot_V_DM_OD_LO           PIN_DM_OD_LO
        #define Pot_V_DM_OD_HI           PIN_DM_OD_HI
        #define Pot_V_DM_STRONG          PIN_DM_STRONG
        #define Pot_V_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Pot_V_MASK               Pot_V__MASK
#define Pot_V_SHIFT              Pot_V__SHIFT
#define Pot_V_WIDTH              1u

/* Interrupt constants */
#if defined(Pot_V__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Pot_V_SetInterruptMode() function.
     *  @{
     */
        #define Pot_V_INTR_NONE      (uint16)(0x0000u)
        #define Pot_V_INTR_RISING    (uint16)(0x0001u)
        #define Pot_V_INTR_FALLING   (uint16)(0x0002u)
        #define Pot_V_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Pot_V_INTR_MASK      (0x01u) 
#endif /* (Pot_V__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pot_V_PS                     (* (reg8 *) Pot_V__PS)
/* Data Register */
#define Pot_V_DR                     (* (reg8 *) Pot_V__DR)
/* Port Number */
#define Pot_V_PRT_NUM                (* (reg8 *) Pot_V__PRT) 
/* Connect to Analog Globals */                                                  
#define Pot_V_AG                     (* (reg8 *) Pot_V__AG)                       
/* Analog MUX bux enable */
#define Pot_V_AMUX                   (* (reg8 *) Pot_V__AMUX) 
/* Bidirectional Enable */                                                        
#define Pot_V_BIE                    (* (reg8 *) Pot_V__BIE)
/* Bit-mask for Aliased Register Access */
#define Pot_V_BIT_MASK               (* (reg8 *) Pot_V__BIT_MASK)
/* Bypass Enable */
#define Pot_V_BYP                    (* (reg8 *) Pot_V__BYP)
/* Port wide control signals */                                                   
#define Pot_V_CTL                    (* (reg8 *) Pot_V__CTL)
/* Drive Modes */
#define Pot_V_DM0                    (* (reg8 *) Pot_V__DM0) 
#define Pot_V_DM1                    (* (reg8 *) Pot_V__DM1)
#define Pot_V_DM2                    (* (reg8 *) Pot_V__DM2) 
/* Input Buffer Disable Override */
#define Pot_V_INP_DIS                (* (reg8 *) Pot_V__INP_DIS)
/* LCD Common or Segment Drive */
#define Pot_V_LCD_COM_SEG            (* (reg8 *) Pot_V__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pot_V_LCD_EN                 (* (reg8 *) Pot_V__LCD_EN)
/* Slew Rate Control */
#define Pot_V_SLW                    (* (reg8 *) Pot_V__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pot_V_PRTDSI__CAPS_SEL       (* (reg8 *) Pot_V__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pot_V_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pot_V__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pot_V_PRTDSI__OE_SEL0        (* (reg8 *) Pot_V__PRTDSI__OE_SEL0) 
#define Pot_V_PRTDSI__OE_SEL1        (* (reg8 *) Pot_V__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pot_V_PRTDSI__OUT_SEL0       (* (reg8 *) Pot_V__PRTDSI__OUT_SEL0) 
#define Pot_V_PRTDSI__OUT_SEL1       (* (reg8 *) Pot_V__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pot_V_PRTDSI__SYNC_OUT       (* (reg8 *) Pot_V__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Pot_V__SIO_CFG)
    #define Pot_V_SIO_HYST_EN        (* (reg8 *) Pot_V__SIO_HYST_EN)
    #define Pot_V_SIO_REG_HIFREQ     (* (reg8 *) Pot_V__SIO_REG_HIFREQ)
    #define Pot_V_SIO_CFG            (* (reg8 *) Pot_V__SIO_CFG)
    #define Pot_V_SIO_DIFF           (* (reg8 *) Pot_V__SIO_DIFF)
#endif /* (Pot_V__SIO_CFG) */

/* Interrupt Registers */
#if defined(Pot_V__INTSTAT)
    #define Pot_V_INTSTAT            (* (reg8 *) Pot_V__INTSTAT)
    #define Pot_V_SNAP               (* (reg8 *) Pot_V__SNAP)
    
	#define Pot_V_0_INTTYPE_REG 		(* (reg8 *) Pot_V__0__INTTYPE)
#endif /* (Pot_V__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Pot_V_H */


/* [] END OF FILE */
