
#pragma once


// when the measured temperature exceeds this value, it is assumed that not tip is detected
// measured temperature rises to maximum without a tip because the opamp saturates
#define NO_TIP_TEMP 450

// PID-Controller values
#define PID_P 350
#define PID_I 2
#define PID_D 10

// max temperature values
#define MIN_TEMP 120
#define MAX_TEMP 380

// temperature is calculated from the adc reading via a linear function: temp = m * adc + b
#define TEMP_CONVERSION_B 49.9424k
#define TEMP_CONVERSION_M 0.4433k

// UART
#define BAUD 38400

#include "pinout.h"
