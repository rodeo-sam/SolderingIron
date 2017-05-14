
#include <avr/io.h>

// globally used variables

static uint16_t global_targetTemperature;
static uint16_t global_currentTemperature;
static uint16_t global_showTargetTemperature;

static uint8_t global_timer0_trigger;
