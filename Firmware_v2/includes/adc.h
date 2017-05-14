/*
 * adc.h
 *
 * Created: Fr 30. Jan 13:42:47 CET 2015
 * Author: Karsten Hinz <k.hinz tu-bs.de>
 */

#pragma once

#include <stdint.h>

void adc_init(void);
uint16_t adc_read(void);
