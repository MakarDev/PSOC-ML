/*******************************************************************************
* File Name: CLR.h  
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

#if !defined(CY_PINS_CLR_H) /* Pins CLR_H */
#define CY_PINS_CLR_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "CLR_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 CLR__PORT == 15 && ((CLR__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    CLR_Write(uint8 value);
void    CLR_SetDriveMode(uint8 mode);
uint8   CLR_ReadDataReg(void);
uint8   CLR_Read(void);
void    CLR_SetInterruptMode(uint16 position, uint16 mode);
uint8   CLR_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the CLR_SetDriveMode() function.
     *  @{
     */
        #define CLR_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define CLR_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define CLR_DM_RES_UP          PIN_DM_RES_UP
        #define CLR_DM_RES_DWN         PIN_DM_RES_DWN
        #define CLR_DM_OD_LO           PIN_DM_OD_LO
        #define CLR_DM_OD_HI           PIN_DM_OD_HI
        #define CLR_DM_STRONG          PIN_DM_STRONG
        #define CLR_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define CLR_MASK               CLR__MASK
#define CLR_SHIFT              CLR__SHIFT
#define CLR_WIDTH              1u

/* Interrupt constants */
#if defined(CLR__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in CLR_SetInterruptMode() function.
     *  @{
     */
        #define CLR_INTR_NONE      (uint16)(0x0000u)
        #define CLR_INTR_RISING    (uint16)(0x0001u)
        #define CLR_INTR_FALLING   (uint16)(0x0002u)
        #define CLR_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define CLR_INTR_MASK      (0x01u) 
#endif /* (CLR__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define CLR_PS                     (* (reg8 *) CLR__PS)
/* Data Register */
#define CLR_DR                     (* (reg8 *) CLR__DR)
/* Port Number */
#define CLR_PRT_NUM                (* (reg8 *) CLR__PRT) 
/* Connect to Analog Globals */                                                  
#define CLR_AG                     (* (reg8 *) CLR__AG)                       
/* Analog MUX bux enable */
#define CLR_AMUX                   (* (reg8 *) CLR__AMUX) 
/* Bidirectional Enable */                                                        
#define CLR_BIE                    (* (reg8 *) CLR__BIE)
/* Bit-mask for Aliased Register Access */
#define CLR_BIT_MASK               (* (reg8 *) CLR__BIT_MASK)
/* Bypass Enable */
#define CLR_BYP                    (* (reg8 *) CLR__BYP)
/* Port wide control signals */                                                   
#define CLR_CTL                    (* (reg8 *) CLR__CTL)
/* Drive Modes */
#define CLR_DM0                    (* (reg8 *) CLR__DM0) 
#define CLR_DM1                    (* (reg8 *) CLR__DM1)
#define CLR_DM2                    (* (reg8 *) CLR__DM2) 
/* Input Buffer Disable Override */
#define CLR_INP_DIS                (* (reg8 *) CLR__INP_DIS)
/* LCD Common or Segment Drive */
#define CLR_LCD_COM_SEG            (* (reg8 *) CLR__LCD_COM_SEG)
/* Enable Segment LCD */
#define CLR_LCD_EN                 (* (reg8 *) CLR__LCD_EN)
/* Slew Rate Control */
#define CLR_SLW                    (* (reg8 *) CLR__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define CLR_PRTDSI__CAPS_SEL       (* (reg8 *) CLR__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define CLR_PRTDSI__DBL_SYNC_IN    (* (reg8 *) CLR__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define CLR_PRTDSI__OE_SEL0        (* (reg8 *) CLR__PRTDSI__OE_SEL0) 
#define CLR_PRTDSI__OE_SEL1        (* (reg8 *) CLR__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define CLR_PRTDSI__OUT_SEL0       (* (reg8 *) CLR__PRTDSI__OUT_SEL0) 
#define CLR_PRTDSI__OUT_SEL1       (* (reg8 *) CLR__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define CLR_PRTDSI__SYNC_OUT       (* (reg8 *) CLR__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(CLR__SIO_CFG)
    #define CLR_SIO_HYST_EN        (* (reg8 *) CLR__SIO_HYST_EN)
    #define CLR_SIO_REG_HIFREQ     (* (reg8 *) CLR__SIO_REG_HIFREQ)
    #define CLR_SIO_CFG            (* (reg8 *) CLR__SIO_CFG)
    #define CLR_SIO_DIFF           (* (reg8 *) CLR__SIO_DIFF)
#endif /* (CLR__SIO_CFG) */

/* Interrupt Registers */
#if defined(CLR__INTSTAT)
    #define CLR_INTSTAT            (* (reg8 *) CLR__INTSTAT)
    #define CLR_SNAP               (* (reg8 *) CLR__SNAP)
    
	#define CLR_0_INTTYPE_REG 		(* (reg8 *) CLR__0__INTTYPE)
#endif /* (CLR__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_CLR_H */


/* [] END OF FILE */
