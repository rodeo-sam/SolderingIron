/*
 * rest_check.c
 *
 * Created: Tue 10. Feb 13:19:47 CET 2015
 * Author: Johannes van Balen <j.van-balen tu-bs.de>
 */

#include "adc.h"
#include "rest_check.h"
#include "../config.h"
#include "timing.h"
#include <avr/interrupt.h>
#include <avr/io.h>

volatile bool rest_check = false;
volatile uint16_t rest_check_raw = 14;
next_time_t rest_check_timer;


void rest_check_init(void) {
  timer_init(&rest_check_timer,0,0,20); // 100ms
  timer_prepare();
  timer_set(&rest_check_timer);
}


static void rest_check_start_conversion(void) {
  #if REST_HALL_THRESHOLD != 0

    // wait if adc is busy
    while(adc_is_busy());

    // disable interrupts
    uint8_t sreg = SREG;
    cli();

    // if a conversion was started though, rest check aborts
    if (adc_is_busy()) {
      SREG = sreg;
      return;
    }

    adc_configuration_t conf = {
      .channel = CH_ADC1,
      .reference = REF_VCC,
      .trigger = TRGR_MANUEL,
    };
    adc_init(conf);
    adc_set_conversion_complete_callback(rest_check_conversion_complete_callback);
    adc_trigger();

    SREG = sreg;
  #endif
  return;
}


void rest_check_conversion_complete_callback(uint16_t raw) {
  if (raw < 511-REST_HALL_THRESHOLD || raw > 511+REST_HALL_THRESHOLD) {
    rest_check = true;
  } else {
    rest_check = false;
  }
  rest_check_raw = raw;
}


bool get_rest(void) {
  return rest_check;
}

uint16_t get_rest_raw(void) {
  return rest_check_raw;
}


void rest_check_updater(void) {
  if(timer_past(&rest_check_timer)){
    timer_set(&rest_check_timer);
    rest_check_start_conversion();
  }
}
