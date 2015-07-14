/*
* @file: rotary_encoder.c
*
* Reads rotary encoder every 2ms and calls plus/minus/button-callbacks
*
* Author: Keno Garlichs, <k.garlichs@tu-braunschweig.de>              
*/
#include "rotary_encoder.h"

// Encoder was rotated clockwise
void (*plus_callback)(void);
// Encoder was rotated counter clockwise
void (*minus_callback)(void);
// Buttons was pressed
void (*third_callback)(void);
// Buttons was held
void (*third_long_callback)(void);

uint8_t state, prevstate = 0;
uint8_t nextEncoderState[4] = {2, 0, 3, 1};
uint8_t prevEncoderState[4] = {1, 3, 0, 2};

next_time_t rotary_check_timer; 

/*
 * @brief	Initializes required port pins,
 * 			sets the poll timer and assigns
 * 			callback functions for the different events
 *
 * @param	*p_callback		Pointer to the function to be called upon CW rotation
 * @param	*m_callback		Pointer to the function to be called upon CCW rotation
 * @param	*t_callback		Pointer to the function to be called upon short button press
 * @param	*tl_callback	Pointer to the function to be called upon long button press
 *
 * @return void
 */
void encode_init(void (*p_callback)(void), void (*m_callback)(void), void (*t_callback)(void), void (*tl_callback)(void))
{
	// Configure pins as inputs
	ROTARY_A_DDR &= ~(1 << ROTARY_A_PIN_NR);
	ROTARY_B_DDR &= ~(1 << ROTARY_B_PIN_NR);
	ROTARY_BUTTON_DDR &= ~(1 << ROTARY_BUTTON_PIN_NR);

	// Enable internal pullups
	ROTARY_A_PORT |= (1 << ROTARY_A_PIN_NR);
	ROTARY_B_PORT |= (1 << ROTARY_B_PIN_NR);
	ROTARY_BUTTON_PORT |= (1 << ROTARY_BUTTON_PIN_NR);
   	
	// Initialize poll timer
   	timer_init(&rotary_check_timer, 0, 0, 1); 	// ~2ms
   	timer_prepare();
   	timer_set(&rotary_check_timer);

	// Set up functions to be called upon respective actions
	plus_callback = p_callback;
	minus_callback = m_callback;
	third_callback = t_callback;
	third_long_callback = tl_callback;
}

/*
 * @brief	Gets called by timer every x ms, evaluates
 * rotary encoder and it's button. Then calls repective
 * callback functions, defined during initialization
 *
 * @param void
 * @return void
 */
void rotary_callback(void) {
	static uint8_t skip_count = 0;
	static uint16_t long_press_count = 0;

	state = ROTARY_A_STATE | ROTARY_B_STATE;
	if((state != prevstate) ) {
		/* The rotary encoder we use gives four pulses per step
		 * so we only count every fourth step */
		if(skip_count++ == 3) {			
			if (state == nextEncoderState[prevstate]) {
				if(minus_callback != NULL) {
					minus_callback();
				}
			}
			else if (state == prevEncoderState[prevstate]) {
				if(plus_callback != NULL) {
					plus_callback();
				}
			}
			skip_count = 0;
		}

		prevstate = state;
  	}

	// Now check the button
	if(ROTARY_BUTTON_PRESSED) {
		// Button is pressed at the moment
		long_press_count++;
	}
	else {
		/* Button was released, now check if it was pressed in the last cycle
		 * and if yes, call functions with respect to to time it was held */
		if((long_press_count < 250) && (long_press_count > 0)) {
			// Button was pressed for less than a second
			if(third_callback != NULL) {
				third_callback();
			}	
		}
		else if((long_press_count >= 250) && (long_press_count > 0)) {
			// Button was pressed for more than a second
			if(third_long_callback != NULL) {
				third_long_callback();
			}
		}
		long_press_count = 0;
	}
}

/*
 * @brief Gets called by main() every loop cycle
 * 		  Checks, whether timer has expired. If yes, it calls
 * 		  rotary_callback and restarts the timer
 *
 * @param void
 * @return void
 */
void check_rotary_timer(void) {
	if(timer_past(&rotary_check_timer)) {
		timer_set(&rotary_check_timer);
		rotary_callback();
	}
}
