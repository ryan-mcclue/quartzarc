/*
 * QAS_Serial_Dev_Base.cpp
 *
 *  Created on: 2 Feb. 2023
 *      Author: ryan
 */

#include "QAS_Serial_Dev_Base.hpp"


  QA_Result QAS_Serial_Dev_Base::init(void *p)
  {
  	if (init_state) {
  		return QA_OK;
  	}

  	if (imp_init(p)) {
      return QA_Fail;
  	}

    init_state = QA_Initialised;

    return QA_OK;
  }

  void QAS_Serial_Dev_Base::deinit(void)
  {
  	imp_deinit();

  	init_state = QA_NotInitialised;
  }

  QAS_Serial_Dev_Base::DeviceType QAS_Serial_Dev_Base::getType(void) {
  	return device_type;
  }

  // will be called by specific interrupt handler
  void QAS_Serial_Dev_Base::handler(void *p)
  {
  	imp_handler(p);
  }

  void QAS_Serial_Dev_Base::rxStart(void) {
  	if (rx_state) {
  		return;
  	}

    imp_rxStart();
    rx_state = PeriphActive;
  }

  void QAS_Serial_Dev_Base::rxStop(void) {
  	imp_rxStop();
  	rx_state = PeriphInactive;
  }

  void QAS_Serial_Dev_Base::txString(const char *str) {
  	uint16_t len = strlen(str);
    for (uint16_t i = 0; i < len; i += 1) {
      tx_fifo->push(str[i]);
    }

    imp_txStart();
  }

  // carriage return moves to start of line
  // line feed goes down a line from current cursor position
  // unix treats line feed as newline character, so a slight modification of original typewriter behaviour
  void QAS_Serial_Dev_Base::txStringCR(const char *str) {
  	uint16_t len = strlen(str);
    for (uint16_t i = 0; i < len; i += 1) {
      tx_fifo->push(str[i]);
    }
    tx_fifo->push(13);

    imp_txStart();
  }

  void QAS_Serial_Dev_Base::txCR(void) {
  	tx_fifo->push(13);
  	imp_txStart();
  }

  void QAS_Serial_Dev_Base::txData(uint8_t *data, uint16_t size) {
  	for (uint16_t i = 0; i < size; i += 1) {
  	  tx_fifo->push(data[i]);
  	}
  	imp_txStart();
  }

  QAS_Serial_Dev_Base::DataState QAS_Serial_Dev_Base::rxHasData(void) {
    if (rx_fifo->empty()) return NoData;

    return HasData;
  }

  uint8_t QAS_Serial_Dev_Base::rxPop(void) {
  	return rx_fifo->pop();
  }

  QA_Result QAS_Serial_Dev_Base::rxData(uint8_t *data, uint16_t &size) {
  	size = rx_fifo->pending();
  	if (!size) return QA_Fail;

  	for (uint16_t i = 0; i < size; i += 1) {
      data[i] = rx_fifo->pop();
  	}

  	return QA_OK;
  }


















