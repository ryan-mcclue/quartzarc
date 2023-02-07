/*
 * QAD_UART.hpp
 *
 *  Created on: 17 Jan. 2023
 *      Author: ryan
 */

#ifndef QAD_UART_HPP_
#define QAD_UART_HPP_

#include "setup.hpp"

// number of UART peripherals
enum QAD_UART_Periph : uint8_t {QAD_UART1 = 0, QAD_UART2, QAD_UART3, QAD_UART4, QAD_UART5, QAD_UART6};

typedef struct {
	QAD_UART_Periph uart;
	uint32_t baud;
	uint8_t irq_priority;

	GPIO_TypeDef *tx_gpio;
	uint16_t tx_pin;
	uint8_t tx_af;

	GPIO_TypeDef *rx_gpio;
	uint16_t rx_pin;
	uint8_t rx_af;
} QAD_UART_InitStruct;

class QAD_UART {
public:
	// uart more complex than GPIO, so should check for errors
	enum PeriphState : uint8_t {PeriphInactive = 0, PeriphActive};

  PeriphState state;
  QAD_UART_Periph uart;
  uint32_t baud;
  uint8_t irq_priority;

  GPIO_TypeDef *tx_gpio;
  uint16_t tx_pin;
  uint8_t tx_af;

  GPIO_TypeDef *rx_gpio;
  uint16_t rx_pin;
  uint8_t rx_af;

  IRQn_Type irq;
  UART_HandleTypeDef handle;

  PeriphState tx_state;
  PeriphState rx_state;

  enum DeinitMode : uint8_t {DeinitPartial = 0, DeinitFull};

  QA_Result periphInit(void);
  void periphDeinit(DeinitMode deinit_mode);


  QAD_UART() = delete;

  // reference is just a name, cannot be NULL
  // it cannot be reassigned to something else, i.e. reseated
  QAD_UART(QAD_UART_InitStruct& init) :
    state(PeriphInactive),
		uart(init.uart),
    baud(init.baud),
		irq_priority(init.irq_priority),
		tx_gpio(init.tx_gpio),
		tx_pin(init.tx_pin),
    tx_af(init.tx_af),
		rx_gpio(init.rx_gpio),
		rx_pin(init.rx_pin),
    rx_af(init.rx_af),
		irq(USART1_IRQn),
		handle({0}),
		tx_state(PeriphInactive),
		rx_state(PeriphInactive) {}

  ~QAD_UART() {

  }

  QA_Result init(void);
  void deinit(void);

  PeriphState getState(void);
  UART_HandleTypeDef& getHandle(void);

  void start_tx(void);
  void stop_tx(void);
  PeriphState get_tx_state(void);

  void start_rx(void);
  void stop_rx(void);
  PeriphState get_rx_state(void);

  void data_tx(uint8_t data);
  uint8_t data_rx(void);
};

// use buffers to offload work for the CPU?

#endif /* QAD_UART_HPP_ */
