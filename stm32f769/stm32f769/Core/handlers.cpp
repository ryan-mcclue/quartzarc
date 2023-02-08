#include "handlers.hpp"

#include "QAS_Serial_Dev_UART.hpp"
#include "QAD_GPIO.hpp"

extern QAS_Serial_Dev_UART *Serial_UART;
extern QAD_GPIO_Output *gpio_user_led_red;
extern QAD_GPIO_Output *gpio_user_led_green;

// these have higher priorities than peripheral interrupt handlers

void
NMI_Handler(void)
{
}

void
HardFault_Handler(void)
{
	gpio_user_led_red->on();
	gpio_user_led_green->on();
	while(1) {}
}

void
MemManage_Handler(void)
{
	gpio_user_led_red->on();
	gpio_user_led_green->on();
	while (1) {}
}

void
BusFault_Handler(void)
{
	gpio_user_led_red->on();
	gpio_user_led_green->on();
	while(1) {}
}

void
UsageFault_Handler(void)
{
	gpio_user_led_red->on();
	gpio_user_led_green->on();
	while(1) {}
}

void
SVC_Handler(void)
{
}

void
DebugMon_Handler(void)
{
}

void
PendSV_Handler(void)
{
}

void
SysTick_Handler(void)
{
	HAL_IncTick();
}

void USART1_IRQHandler(void) {
  Serial_UART->handler(NULL);
}




