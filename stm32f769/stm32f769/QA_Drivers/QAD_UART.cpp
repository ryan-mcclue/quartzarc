/*
 * QAD_UART.cpp
 *
 *  Created on: 17 Jan. 2023
 *      Author: ryan
 */
#include "QAD_UART.hpp"

QA_Result QAD_UART::init(void) {
  if (state == PeriphActive) return QA_Fail;

  if (periphInit()) {

  }
}

void QAD_UART::deinit(void) {

}

QAD_UART::PeriphState QAD_UART::getState(void) {

}

UART_HandleTypeDef& QAD_UART::getHandle(void) {

}

void QAD_UART::start_tx(void);
void QAD_UART::stop_tx(void);
QAD_UART::PeriphState QAD_UART::get_tx_state(void);

void QAD_UART::start_rx(void);
void QAD_UART::stop_rx(void);
QAD_UART::PeriphState QAD_UART::get_rx_state(void);

QA_Result QAD_UART::periphInit(void);
void QAD_UART::periphDeinit(QAD_UART::DeinitMode deinit_mode);
