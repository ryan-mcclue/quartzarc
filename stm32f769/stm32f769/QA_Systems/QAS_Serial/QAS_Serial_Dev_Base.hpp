/*
 * QAS_Serial_Dev_Base.hpp
 *
 *  Created on: 2 Feb. 2023
 *      Author: ryan
 */

#ifndef QAS_SERIAL_QAS_SERIAL_DEV_BASE_HPP_
#define QAS_SERIAL_QAS_SERIAL_DEV_BASE_HPP_

#include "setup.hpp"

#include <memory>

#include "QAT_FIFO.hpp"

class QAS_Serial_Dev_Base {
public:
	enum PeriphState : uint8_t {PeriphActive = 0, PeriphInactive};
	// echoing a state machine perhaps?
	enum DataState : uint8_t {NoData = 0, HasData};
	// this is to have say a file/bluetooth/tcp connection all use a serial interface
	enum DeviceType : uint8_t {DT_UART = 0, DT_File = 1, DT_Unknown};

  std::unique_ptr<QAT_FIFOBuffer> tx_fifo;
  std::unique_ptr<QAT_FIFOBuffer> rx_fifo;

  PeriphState tx_state;
  PeriphState rx_state;

  QA_InitState init_state;

  DeviceType device_type;

  QAS_Serial_Dev_Base() = delete;

  // members actually initialised in the order of their declaration, not init-list
  // so will get warning of this
  QAS_Serial_Dev_Base(uint16_t tx_fifo_size, uint16_t rx_fifo_size, DeviceType device_type) :
    tx_fifo(std::make_unique<QAT_FIFOBuffer>(tx_fifo_size)),
    rx_fifo(std::make_unique<QAT_FIFOBuffer>(rx_fifo_size)),
    tx_state(PeriphInactive),
    rx_state(PeriphInactive),
    init_state(QA_NotInitialised),
    device_type(device_type) {}

  QA_Result init(void *p);
  void deinit(void);

  DeviceType getType(void);

  // will be called by specific interrupt handler
  void handler(void *p);

  void rxStart(void);
  void rxStop(void);

  void txString(const char *str);
  void txStringCR(const char *str);
  void txCR(void);
  void txData(uint8_t *data, uint16_t size);

  DataState rxHasData(void);

  uint8_t rxPop(void);
  QA_Result rxData(uint8_t *data, uint16_t &size);

  // interface means class needs to implement
  // virtual specifically relates to inheritance/subclasses
  virtual QA_Result imp_init(void *p) = 0;
  virtual void imp_deinit(void) = 0;

  virtual void imp_handler(void *p) = 0;

  virtual void imp_txStart(void) = 0;
  virtual void imp_txStop(void) = 0;
  virtual void imp_rxStart(void) = 0;
  virtual void imp_rxStop(void) = 0;
};


#endif /* QAS_SERIAL_QAS_SERIAL_DEV_BASE_HPP_ */
