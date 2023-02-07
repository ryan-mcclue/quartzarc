/*
 * QAS_Serial_Dev_UART.hpp
 *
 *  Created on: 7 Feb. 2023
 *      Author: ryan
 */

#ifndef QAS_SERIAL_QAS_SERIAL_DEV_UART_HPP_
#define QAS_SERIAL_QAS_SERIAL_DEV_UART_HPP_

#include "setup.hpp"
#include <memory>
#include <string.h>

#include "QAT_FIFO.hpp"
#include "QAS_Serial_Dev_Base.hpp"
#include "QAD_UART.hpp"

struct QAS_Serial_Dev_UART_InitStruct {
	QAD_UART_InitStruct uart;

	uint16_t txfifo_size;
	uint16_t rxfifo_size;
};

// without 'public' defaults to private class inheritance
class QAS_Serial_Dev_UART : public QAS_Serial_Dev_Base {
public:
	QAD_UART_Periph periph;

	std::unique_ptr<QAD_UART> uart;

	QAS_Serial_Dev_UART() = delete;
	QAS_Serial_Dev_UART(QAS_Serial_Dev_UART_InitStruct &init) :
		QAS_Serial_Dev_Base(init.txfifo_size, init.rxfifo_size, DT_UART),
    periph(init.uart.uart),
		uart(std::make_unique<QAD_UART>(init.uart)) {}

  QA_Result imp_init(void *p) override;
  void imp_deinit(void) override;
  void imp_handler(void *p) override;
  void imp_txStart(void) override;
  void imp_txStop(void) override;
  void imp_rxStart(void) override;
  void imp_rxStop(void) override;
};


#endif /* QAS_SERIAL_QAS_SERIAL_DEV_UART_HPP_ */
