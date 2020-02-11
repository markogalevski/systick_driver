/*******************************************************************************
* Title                 :   Systick Interface
* Filename              :   systick_interface.h
* Author                :   Marko Galevski
* Origin Date           :   11/02/2020
* Version               :   1.0.0
* Compiler              :   None
* Target                :   None
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

/** @file systick_interface.h
 *  @brief General interface covering user accesses to initialise the system
 *  		wide tick for timeout/timekeeping operations
 */
/******************************************************************************
* Includes
*******************************************************************************/
#ifndef _SYSTICK_H
#define _SYSTICK_H

#include "systick_stm32f411_config.h"

typedef void (*systick_callback_t) (void);

void systick_init(systick_config_t *config);
void systick_tick_freq_set(systick_config_t *config);
void systick_interrupt_control(systick_interrupt_t interrupt_control);
void systick_pause(void);
void systick_resume(void);

uint32_t systick_get_tick(void);
void systick_delay(uint32_t delay_ms);

void systick_increment(void);
void systick_callback_register(systick_callback_t callback_func);
void systick_irq_handler(void);






















#endif
