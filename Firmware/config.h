#pragma once

// #define HW_VERSION 1
#define HW_VERSION 12 // current hardware version

// Uncomment to use rotary encoder instead of button for temperature setting
//#define ROTARY_ENCODER 1

// the temperature the system is supposed to not exceed
#define TEMP_MAX 380

// temperature that should be held when resting
#define TEMP_REST 150

// lowest temperature allowed
#define TEMP_MIN 0 

// default power on temperature
#define DEFAULT_TEMP 300
#define SAVED_TEMP0 350
#define SAVED_TEMP1 250

// when the measured temperate exceeds this value, it is assumed that not tip is detected
// measured temperature rises to maximum without a tip because the opamp saturates
#define NO_TIP_TEMP 450

// minimum observed temperature rise per second to assume that the tip is connected correctly
// it is possible to connect the tip only half way which leads to wrong temperature readings
// this must be avoided as it can harm the tip
#define MIN_TEMP_PER_SECOND 5

// maximum realistic temperature change per second
//if the temperature changes too fast, something hardware wise must be wrong
#define MAX_TEMP_PER_SECOND 20

//PID-Controller values
#define PID_P 350
#define PID_I 2
#define PID_D 10

// threshold at which the hall sensor is supposed to trigger
// if set to 0 rest detection will be disabled
#define REST_HALL_THRESHOLD 15

// temperature is calculated from the adc reading via a linear function: temp = m * adc + b
#define TEMP_CONVERSION_B 49.9424k
#define TEMP_CONVERSION_M 0.4433k

// temperature scale
// if defined the display will work in fahrenheit
//#define TEMP_KELVIN
#define TEMP_CELSIUS
//#define TEMP_FAHRENHEIT
//#define TEMP_RANKINE
//#define TEMP_DELISLE
//#define TEMP_REAMUR
//#define TEMP_NEWTON
//#define TEMP_ROMER

#include "pinout.h"
