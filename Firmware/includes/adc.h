/*
 * adc.h
 *
 * Created: Fr 30. Jan 13:42:47 CET 2015
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 */ 

#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	CH_ADC0  = 0b0000,
	CH_ADC1  = 0b0001,
	CH_ADC2  = 0b0010,
	CH_ADC3  = 0b0011,
	CH_ADC4  = 0b0100,
	CH_ADC5  = 0b0101,
	CH_ADC6  = 0b0110,
	CH_ADC7  = 0b0111,
	CH_TEMPERATURE  = 0b1000,
	CH_GND          = 0b1111,
	CH_1V1          = 0b1110,
} adc_channel_t;

typedef enum {
	TRGR_NO_TRIGGER = 0b1000,
	TRGR_MANUEL = 0b1001,
	TRGR_FREERUNNING = 0b000,
	TRGR_ANALOG_COMPARATOR = 0b001,
	TRGR_EXT_INTERRUPT_REQUEST0 = 0b010,
	TRGR_TIMER0_COMPA = 0b011,
	TRGR_TIMER0_OVERFLOW = 0b100,
	TRGR_TIMER1_COMPB = 0b101,
	TRGR_TIMER1_OVERFLOW = 0b110,
	TRGR_TIMER1_CAPTURE_EVENT = 0b111,
} adc_autoTriggerSource_t;

typedef enum {
	REF_VCC = 0b01,
	REF_1V1_NC = 0b11,
	REF_AREF = 0b00
} adc_reference_t;

typedef struct  
{
	adc_channel_t channel;
	adc_autoTriggerSource_t trigger;
	adc_reference_t reference;
} adc_configuration_t;

void adc_init(adc_configuration_t conf);
uint16_t adc_read_blocking();
uint16_t adc_test();
uint16_t adc_average();
uint16_t adc_average_hi(uint8_t r_in_M_ohm);

