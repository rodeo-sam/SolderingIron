#pragma once

#define HW_VERSION 1

// the temperature the system is supposed to not exceed
#define TEMP_MAX 420

// lowest temperature allowed
#define TEMP_MIN 0 

// default power on temperature
#define DEFAULT_TEMP 300

//PID-Controller values
#define PID_P 170 
#define PID_I 4 
#define PID_D 100 

// threshold at which the hall sensor is supposed to trigger
// if set to 0 rest detection will be disabled
#define REST_HALL_THRESHOLD 15

