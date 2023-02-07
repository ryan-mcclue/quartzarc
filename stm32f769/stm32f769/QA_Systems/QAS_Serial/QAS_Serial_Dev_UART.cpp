/*
 * QAS_Serial_Dev_UART.cpp
 *
 *  Created on: 7 Feb. 2023
 *      Author: ryan
 */

#include "QAS_Serial_Dev_UART.hpp"

QA_Result QAS_Serial_Dev_UART::imp_init(void *p) {
  return uart->init();
}

void QAS_Serial_Dev_UART::imp_deinit(void) {
  uart->deinit();
}

void QAS_Serial_Dev_UART::imp_handler(void *p) {
  UART_HandleTypeDef *handle = &uart->getHandle();

  if (__HAL_UART_GET_FLAG(handle, UART_FLAG_RXNE)) {
    uint8_t data = uart->data_rx();
    if (rx_state) {
    	rx_fifo->push(data);
    	// clear this to ensure it doesn't continually call the interrupt handler again
    	__HAL_UART_CLEAR_FLAG(handle, UART_FLAG_RXNE);
    }
  }

  if (__HAL_UART_GET_FLAG(handle, UART_FLAG_TXE)) {
  	if (!tx_fifo->empty()) {
      uart->data_tx(tx_fifo->pop());
  	} else {
  		uart->stop_tx();
  		tx_state = PeriphInactive;
  	}

  	__HAL_UART_CLEAR_FLAG(handle, UART_FLAG_TXE);

  }
}

void QAS_Serial_Dev_UART::imp_rxStart(void) {
  uart->start_rx();
}

void QAS_Serial_Dev_UART::imp_rxStop(void) {
	uart->stop_rx();
}

void QAS_Serial_Dev_UART::imp_txStart(void) {
	uart->start_tx();
}

void QAS_Serial_Dev_UART::imp_txStop(void) {
	uart->stop_tx();
}
