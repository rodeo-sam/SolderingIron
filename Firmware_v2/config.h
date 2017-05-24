
#pragma once


// when the measured temperature exceeds this value, it is assumed that not tip is detected
// measured temperature rises to maximum without a tip because the opamp saturates
#define NO_TIP_TEMP 450

// PID-Controller values
#define PID_P 350
#define PID_I 2
#define PID_D 10

// temp measurement
#define MEASUREMENT_EVERY_N_CYCLES 5

// max temperature values
#define MIN_TEMP 120
#define MAX_TEMP 380

#define START_COUNT 50
#define END_COUNT 250
#define MAX_PWM_VALUE (END_COUNT - START_COUNT)

// temperature is calculated from the adc reading via a linear function: temp = (m * adc + b)/d
#define TEMP_CONVERSION_B 499424L
#define TEMP_CONVERSION_M 4433L
#define TEMP_CONVERSION_D 10000L // solely for fixpoint arithmetics

// UART
#define BAUD 9600

//display
#define TARGET_TEMP_SHOW_TIME 400

#include "pinout.h"
