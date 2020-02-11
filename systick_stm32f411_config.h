/*******************************************************************************
* Title                 :   Systick STM32F411 Config
* Filename              :   systick_stm32f411_config.h
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

/** @file systick_stm32f411_config.h
 *  @brief Chip specific header containing all relevant enums and structs
 *  		to configure the systick.
 */
/******************************************************************************
* Includes
*******************************************************************************/
#ifndef _SYSTICK_STM32F411_CONFIG_H
#define _SYSTICK_STM32F411_CONFIG_H

#include <stdint.h>

/**
 * Core clock frequency as defined in system_stm32f4xx.c by STM
 */
extern uint32_t SystemCoreClock;

typedef enum
{
	SYSTICK_1,
	NUM_SYSTICKS
}systick_t;

/**
 * Contains options to enable or disable the systick. Note that a disabled systick will
 * disable timeout features for all communication buses
 */
typedef enum
{
	SYSTICK_DISABLED,
	SYSTICK_ENABLED
}systick_enabled_t;

/**
 * Enables or disables the systick interrupt. The systick should be enabled to allow updating
 * of the source-file scoped timer variable every x ms.
 */
typedef enum
{
	SYSTICK_INT_DISABLED,
	SYSTICK_INT_ENABLED
}systick_interrupt_t;

/**
 * Options for where the systick gets its clock. Internal clock is the default.
 */
typedef enum
{
	SYSTICK_EXTERNAL_CLOCK,
	SYSTICK_INTERNAL_CLOCK
}systick_clock_source_t;

/**
 * Struct containing relevant configuration data to enable the systick
 */
typedef struct
{
	systick_enabled_t enable_systick; /**<Whether or not the systick should be
			enabled. Recommended value is SYSTICK_ENABLED*/
	uint32_t tick_freq_khz;	/**<How quickly the systick should trigger in kHz.
	 	 	Recommended value is 1 */
	systick_interrupt_t enable_systick_interrupt; /**< Whether or not the systick
			interrupt should be enabled. Recommended value si SYSTICK_INT_ENABLED. */
	systick_clock_source_t clock_source; /**< The systick clock source. Recommended
	 	 	value is SYSTICK_INTERNAL_CLOCK */
}systick_config_t;

const systick_config_t *systick_config_get(void);

#endif
