#include "main.hpp"
#include "boot.hpp"

#include "QAD_GPIO.hpp"
#include "QAS_Serial_Dev_UART.hpp"
#include "QAD_FMC.hpp"

// These are global so they can be accessed in fault handlers
QAS_Serial_Dev_UART *Serial_UART;
QAD_GPIO_Output *gpio_user_led_red;
QAD_GPIO_Output *gpio_user_led_green;

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
  gpio_user_led_red = new QAD_GPIO_Output(QA_USERLED_RED_GPIO_PORT, QA_USERLED_RED_GPIO_PIN);
  gpio_user_led_green = new QAD_GPIO_Output(QA_USERLED_GREEN_GPIO_PORT, QA_USERLED_GREEN_GPIO_PIN);

  QAD_GPIO_Input *gpio_user_button = new QAD_GPIO_Input(QA_USERBUTTON_GPIO_PORT, QA_USERBUTTON_GPIO_PIN);

  QAS_Serial_Dev_UART_InitStruct serial_init = {};
  serial_init.uart.uart = QAD_UART1;
  serial_init.uart.baud = QAD_UART1_BAUDRATE;
  serial_init.uart.irq_priority = QAD_IRQPRIORITY_UART1;
  serial_init.uart.tx_gpio = QAD_UART1_TX_PORT;
  serial_init.uart.tx_pin = QAD_UART1_TX_PIN;
  serial_init.uart.tx_af = QAD_UART1_TX_AF;
  serial_init.uart.rx_gpio = QAD_UART1_RX_PORT;
  serial_init.uart.rx_pin = QAD_UART1_RX_PIN;
  serial_init.uart.rx_af = QAD_UART1_RX_AF;
  serial_init.txfifo_size = QAD_UART1_TX_FIFOSIZE;
  serial_init.rxfifo_size = QAD_UART1_RX_FIFOSIZE;
  Serial_UART = new QAS_Serial_Dev_UART(serial_init);

  if (Serial_UART->init(NULL) == QA_Fail) {
  	gpio_user_led_red->on();
    while (1) {}
  }

  if (QAD_FMC::init() == QA_OK) {
    if (QAD_FMC::test() == QA_OK) {
    	Serial_UART->txStringCR("SDRAM: Initialised and tested OK");
    } else {
    	Serial_UART->txStringCR("SDRAM: Initialised, failed test");
    }
  } else {
  	Serial_UART->txStringCR("SDRAM: Initialisation failed");
  }

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
