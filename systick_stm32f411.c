/*******************************************************************************
* Title                 :   Systick STM32F411 Impementation
* Filename              :   systick_stm32f411.c
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

/** @file systick_stm32f411.c
 *  @brief Chip specific implementation of systick control. Many functions
 *  are pointers to vendor created routines due to the very specific nature
 *  of system clocks and ticks.
 *
 *  @note This implementation depends on CMSIS (core_cm4.h)
 */
/******************************************************************************
* Includes
*******************************************************************************/
#include "stm32f411xe.h"
#include "core_cm4.h"
#include <assert.h>
#include "systick_interface.h"

/**
 * Definition of NULL in case it is not defined elsewhere
 */
#ifndef NULL
#define NULL (void *) 0
#endif

static volatile uint32_t tick_ms= 0;		/**<Encapsulated tick value */
static uint32_t tick_freq;					/**<Tick frequency (increment rate) */
/**
 * Callback function which will be dereferenced upon systick interrupts
 * Default value is systick_increment, but can be changed through the callback_register function
 */
static systick_callback_t systick_callback = systick_increment;

/******************************************************************************
* Function: systick_init()
*//**
* \b Description:
*
* 	Carries out the initialisation of the the systick based on information in the
* 	config table
*
*	PRE-CONDITION: The clock system (RCC) has been initialised.
*	PRE-CONDITION: The desired frequency (tick_freq_khz) results in a number small
*					enough to fit the 0xFFFFFF mask
*	PRE-CONDITION: (Soft Assert) the systick is enabled through its config register
*
*	POST-CONDITION: The systick has been configured to count with the desired frequency
*	POST-CONDITION: The systick interrupt has been enabled (if desired) and its priority
*						set to maximum.
*	POST-CONDITION: The systick clock source has been set to the desired option
*
*	@param 		config	a pointer to the systick configuration structure
*
*	@return 	void
*
* \b Example:
*
*	@code
*	systick_config_t *tick_config = systick_config_get();
*	systick_init(tick_config);
*	@endcode
*
*	@see	systick_config_get
*	@see	systick_tick_freq_set
*	@see	systick_pause
*	@see	systick_resume
*	@see	systick_interrupt_control
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void systick_init(systick_config_t *config)
{
	if (config->enable_systick == SYSTICK_ENABLED)
	{
		systick_pause();
		systick_tick_freq_set(config);              		/* set reload register */
		NVIC_SetPriority(SysTick_IRQn, (0)); 									/* set Max Priority for Systick Interrupt */                                   				/* Load the SysTick Counter Value */
		SysTick->CTRL  = config->clock_source << SysTick_CTRL_CLKSOURCE_Pos;
		systick_interrupt_control(config->enable_systick_interrupt);
		systick_resume();
	}
}

/******************************************************************************
* Function: systick_tick_freq_set()
*//**
* \b Description:
*
* 	Sets the frequency of the systick update to the desired value in kHz.
*
*	PRE-CONDITION: The desired frequency (tick_freq_khz) results in a number small
*					enough to fit the 0xFFFFFF mask
*	PRE-CONDITION: (Soft Assert) the systick is enabled through its config register
*	PRE-CONDITION: (Soft Assert) the systick is paused
*
*
*	POST-CONDITION: The systick has been configured to count with the desired frequency
*
*	@param 		config	a pointer to the systick configuration structure
*
*	@return 	void
*
* \b Example:
*
*	@code
*	systick_config_t *tick_config = systick_config_get();
*	systick_init(tick_config);
*	//... later ...
*	systick_pause();
*	tick_config->tick_freq_khz = 5; //kHz
*	systick_tick_freq_set(tick_config);
*	systick_resume();
*	@endcode
*
*	@see	systick_init
*	@see	systick_config_get
*	@see	systick_pause
*	@see	systick_resume
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void systick_tick_freq_set(systick_config_t *config)
{
	if (config->enable_systick == SYSTICK_ENABLED)
	{
		if ((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) == 0)
		{
			uint32_t num_ticks = SystemCoreClock / (1000UL / config->tick_freq_khz);
			assert(num_ticks - 1UL > SysTick_LOAD_RELOAD_Msk);
			tick_freq = config->tick_freq_khz;

			SysTick->LOAD = (uint32_t)(num_ticks - 1UL);
			SysTick->VAL = 0UL;
		}

	}
}
/******************************************************************************
* Function: systick_pause()
*//**
* \b Description:
*
* 	Pauses the counting of the systick.
*
*	PRE-CONDITION: None
*
*
*	POST-CONDITION: The systick timer is paused
*
*	@return 	void
*
* \b Example:
*
*	@code
*	systick_pause();
*	//... do things....
*	systick_resume();
*	@endcode
*
*	@see	systick_tick_freq_set
*	@see	systick_resume

* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void systick_pause(void)
{
	SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk);
}

/******************************************************************************
* Function: systick_resume()
*//**
* \b Description:
*
* 	Resume the counting of the systick.
*
*	PRE-CONDITION: None
*
*
*	POST-CONDITION: The systick timer is running
*
*	@return 	void
*
* \b Example:
*
*	@code
*	systick_pause();
*	//... do things....
*	systick_resume();
*	@endcode
*
*	@see	systick_tick_freq_set
*	@see	systick_pause

* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void systick_resume(void)
{
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

/******************************************************************************
* Function: systick_interrupt_control()
*//**
* \b Description:
*
* 	Enables or disables the systick interrupt
*
*	PRE-CONDITION: (Soft Assert) The systick is paused
*
*
*	POST-CONDITION: The systick interrupt is enabled or disabled, as per the input
*
*	@param		systick_interrupt_t Control parameter defining if the interrupt will be
*				activated or deactivated
*	@return 	void
*
* \b Example:
*
*	@code
*	systick_pause();
*	systick_interrupt_control(SYSTICK_INT_ENABLED);
*	systick_resume();
*	@endcode
*
*	@see	systick_init
*	@see	systick_tick_freq_set
*	@see	systick_pause
*	@see	systick_resume

* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void systick_interrupt_control(systick_interrupt_t interrupt_control)
{
	if((SysTick->CTRL & SysTick_CTRL_ENABLE_Msk) == 0)
	{
		if (interrupt_control == SYSTICK_INT_ENABLED)
		{
			SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
		}
		else
		{
			SysTick->CTRL &= ~(SysTick_CTRL_TICKINT_Msk);
		}
	}

}


/******************************************************************************
* Function: systick_get_tick()
*//**
* \b Description:
*
* 	DReturns the current value of the tick_ms variable
*
*	PRE-CONDITION: None
*
*	POST-CONDITION: The function has returned the current value of the tick variable.
**
*	@return 	uint32_t the current tick value
*
* \b Example:
*
*	@code
*	uint32_t current_tick = systick_get_tick();
*	@endcode
*
*	@see	systick_tick_freq_set
*	@see	systick_delay

* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
uint32_t systick_get_tick(void)
{
	return(tick_ms);
}

/******************************************************************************
* Function: systick_delay()
*//**
* \b Description:
*
* 	Delays the program for the duration of delay_ms in milliseconds
*
*	PRE-CONDITION: None
*
*	POST-CONDITION: delay_ms have gone by and the rest of the program will resume
*
*	@param		delay_ms is the length of time the user wishes to way
*
*	@return 	void
*
* \b Example:
*
*	@code
*	systick_delay(200);
*	@endcode
*
*	@see	systick_tick_freq_set
*	@see	systick_get_tick

* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void systick_delay(uint32_t delay_ms)
{
	uint32_t start = systick_get_tick();
	uint32_t current_tick = systick_get_tick();
	if (delay_ms < 0xFFFFFFFFUL)
	{
		delay_ms += tick_freq;
	}
	while (current_tick - start < delay_ms)
	{
		current_tick = systick_get_tick();
	}
}

/******************************************************************************
* Function: systick_increment()
*//**
* \b Description:
*
* 	Increments the tick by the number of milliseconds between systick register
* 	overflows. Called within systick_irq_handler.
*
*	PRE-CONDITION: None.
*
*	POST-CONDITION: tick_ms has incremented by tick_freq milliseconds
*
*	@return		void
*
* \b Example:
* @code
*
*	//By default is called automatically upon SysTick interrupt
* 	SysTick_IRQHandler(void)
* 	{
* 		systick_irq_handler();
* 	}
* @endcode
*
* @see systick_callback_register
* @see systick_irq_handler
*
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void systick_increment(void)
{
	tick_ms += tick_freq;
}

/******************************************************************************
* Function: systick_callback_register()
*//**
* \b Description:
*
* 	Registers the callback function as the desired on-interrupt functionality.
*
*	PRE-CONDITION: None.
*
*	POST-CONDITION: the systick_callback function pointer variable now points to
*					the desired function
*
*	@param		callback_func 	a function pointer to a void (*function)(void)
*	@return		void
*
* \b Example:
* @code
*
*	systick_callback_register(&interrupt_behaviour);
*
*	//the irq handler will now call interrupt_behaviour
* 	SysTick_IRQHandler(void)
* 	{
* 		systick_irq_handler();
* 	}
* @endcode
*
*
* @see systick_irq_handler
*
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void systick_callback_register(systick_callback_t callback_func)
{
	systick_callback = callback_func;
}

/******************************************************************************
* Function: systick_irq_handler()
*//**
* \b Description:
*
* 	Calls the systick callback function. The default callback is systick_increment.
*
*	PRE-CONDITION: The callback function is non-NULL
*
*	POST-CONDITION: the systick_callback function is called
*

*	@return		void
*
* \b Example:
* @code
*
*	systick_callback_register(&interrupt_behaviour);
*
*	//the irq handler will now call interrupt_behaviour
* 	SysTick_IRQHandler(void)
* 	{
* 		systick_irq_handler();
* 	}
* @endcode
*
*
* @see	systick_callback_register
* @see	systick_increment
*
* <br><b> - CHANGE HISTORY - </b>
*
* <table align="left" style="width:800px">
* <tr><td> Date       </td><td> Software Version </td><td> Initials </td><td> Description </td></tr>
* </table><br><br>
* <hr>
*******************************************************************************/
void systick_irq_handler(void)
{
	assert(systick_callback != NULL);
	systick_callback();
}
