#pragma once
#include <stdbool.h>




void rest_check_init(void);
static void rest_check_start_conversion(void);
void rest_check_conversion_complete_callback(uint16_t raw);
bool get_rest(void);
uint16_t get_rest_raw(void);
void rest_check_updater(void);
