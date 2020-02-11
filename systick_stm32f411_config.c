/*******************************************************************************
* Title                 :   Systick STM32F411 Config Table
* Filename              :   systick_stm32f411_config.c
* Author                :   Marko Galevski
* Origin Date           :   11/02/2020
* Version               :   1.0.0
* Compiler              :   gcc
* Target                :   STM32F411VE (ARM Cortex M4)
* Notes                 :   None
*
*
*******************************************************************************/
/****************************************************************************
* Doxygen C Template
* Copyright (c) 2013 - Jacob Beningo - All Rights Reserved
*
* Feel free to use this Doxygen Code Template at your own risk for your own
* purposes.  The latest license and updates for this Doxygen C template can be
* found at www.beningo.com or by contacting Jacob at jacob@beningo.com.
*
* For updates, free software, training and to stay up to date on the latest
* embedded software techniques sign-up for Jacobs newsletter at
* http://www.beningo.com/814-2/
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Template.
*
*****************************************************************************/

/** @file systick_stm32f411_config.c
 *  @brief Contains the configuration information for the systick
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "systick_stm32f411_config.h"

/**
 * Table containing config information for the configuration of the systick. Populated
 * at first with default values
 */
static const systick_config_t systick_config_table[NUM_SYSTICKS] =
{	//ENABLED			//TICK_FREQ			//INTERRUPT 		//CLOCK
											//ENABLED			//SOURCE
		{SYSTICK_ENABLED,    1,			SYSTICK_INT_ENABLED, 	SYSTICK_INTERNAL_CLOCK}
};

/**
 * Function returning a pointer to the (quite protected) config data
 */
const systick_config_t *systick_config_get(void)
{
	return (systick_config_table);
}
