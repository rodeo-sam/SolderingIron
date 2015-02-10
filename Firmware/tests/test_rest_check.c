

#include <util/delay.h>
#include "display.h"
#include "rest_check.h"
#include "config.h"
#include "clock.h"

int main(void)
{
  clock_init();
	display_init();
  rest_check_init();

	while(1)
	{
    rest_check_updater();
    if(get_rest())
      display_number(0);
    else
      display_number(1);
		display_updater();
		_delay_us(800);
	}
}
