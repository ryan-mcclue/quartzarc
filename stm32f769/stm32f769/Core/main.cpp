#include "main.hpp"
#include "boot.hpp"

#include "QAD_GPIO.hpp"

int
main(void)
{
  // System setup
  if (SystemInitialize())
  {
    while (1) {}
  }

  // User LEDs
  QAD_GPIO_Output *gpio_user_led_red = new QAD_GPIO_Output(QA_USERLED_RED_GPIO_PORT, QA_USERLED_RED_GPIO_PIN);
  QAD_GPIO_Output *gpio_user_led_green = new QAD_GPIO_Output(QA_USERLED_GREEN_GPIO_PORT, QA_USERLED_GREEN_GPIO_PIN);

  // Processing loop
  while (1) {
    gpio_user_led_green->on();
    HAL_Delay(500);
    gpio_user_led_green->off();
    HAL_Delay(500);
  }

  return 0;
}
