
#include <avr/io.h>
#include <avr/io.h>
#include <stdint.h>


// globally used variables

static uint16_t global_targetTemperature;
static int32_t global_currentTemperature;
static uint16_t global_showTargetTemperature;
static uint8_t global_cycle_counter;
static uint8_t global_pwm_value;

static uint8_t global_display_main_cycle_flag;
static uint8_t led_idx;

static uint8_t global_buttons; // 0x02: PLUS, 0x01: MINUS

static uint16_t global_adc_value;

static uint8_t global_tip_enabled;
