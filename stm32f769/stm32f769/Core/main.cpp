#include "main.hpp"
#include "boot.hpp"

#include "QAD_GPIO.hpp"

int
main(void)
{
	uint32_t ticks = 0;
	uint32_t old_tick = 0;
	uint32_t new_tick = 0;
	uint32_t cur_tick = 0;

	uint32_t heartbeat_ticks = 0;
	const uint32_t heartbeat_tick_threshold = 500;

  // System setup
  if (SystemInitialize())
  {
    while (1) {}
  }

  // User LEDs
  QAD_GPIO_Output *gpio_user_led_red = new QAD_GPIO_Output(QA_USERLED_RED_GPIO_PORT, QA_USERLED_RED_GPIO_PIN);
  QAD_GPIO_Output *gpio_user_led_green = new QAD_GPIO_Output(QA_USERLED_GREEN_GPIO_PORT, QA_USERLED_GREEN_GPIO_PIN);

  QAD_GPIO_Input *gpio_user_button = new QAD_GPIO_Input(QA_USERBUTTON_GPIO_PORT, QA_USERBUTTON_GPIO_PIN);

  // Processing loop
	new_tick = HAL_GetTick();
	old_tick = new_tick;

  while (1) {
  	cur_tick = HAL_GetTick();
  	// check more than 1ms elapsed since previous loop iteration
  	if (cur_tick != new_tick) {
  	  old_tick = new_tick;
  	  new_tick = cur_tick;

  	  // as cumulative ticks, if running longer than 52days, 32bit overflow will occur
  	  if (new_tick < old_tick) {
  	    ticks = new_tick + (0xffffffff - old_tick);
  	  } else {
  	    ticks = new_tick - old_tick;
  	  }
  	} else {
  	  // ticks holds the amount of ms since last loop
  	  ticks = 0;
  	}



  	// update heartbeat led
  	heartbeat_ticks += ticks;
  	if (heartbeat_ticks >= heartbeat_tick_threshold) {
      gpio_user_led_green->toggle();
      heartbeat_ticks -= heartbeat_tick_threshold;
  	}

  }

  return 0;
}
