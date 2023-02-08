/*
 * QAD_FMC.hpp
 *
 *  Created on: 8 Feb. 2023
 *      Author: ryan
 */

#ifndef QAD_FMC_HPP_
#define QAD_FMC_HPP_

#include "setup.hpp"

// for added index security when working with memory regions
#define QA_FMC_32BITWORD_COUNT ((uint32_t)4194304)
typedef struct {
	__IO uint32_t data[QA_FMC_32BITWORD_COUNT];
} QA_FMC_32;
#define QA_FMC_PTR ((QA_FMC_32*)0xC0000000)

// slower FMC than QSPI as flash/ram and seamless LCD usage?
class QAD_FMC {
private:
	// how many bytes per read (multiple bytes is burstmode)
	uint32_t modereg_burstlength_1 = 0x0;
	uint32_t modereg_burstlength_2 = 0x1;
	uint32_t modereg_burstlength_3 = 0x2;
	uint32_t modereg_burstlength_4 = 0x4;

	uint32_t modereg_bursttype_seq = 0x0;
	uint32_t modereg_bursttype_int = 0x8;

	uint32_t modereg_caslatency_2 = 0x20;
	uint32_t modereg_caslatency_3 = 0x30;

	uint32_t modereg_opmode_std = 0x0;

	uint32_t modereg_writeburst_prog = 0x0;
	uint32_t modereg_writeburst_single = 0x200;

	uint32_t timeout = 0xffff;
	// how often FMC should refresh RAM cells
	uint32_t refresh = 0x0603;

	// FMC documentation specifies this base address
	// ISO C++ mandates const for static members
	static const uint32_t base_addr = 0xC0000000;
	static const uint32_t size = 0x01000000;

	SDRAM_HandleTypeDef handle;

  QA_InitState state;

	QAD_FMC() :
	  state(QA_NotInitialised) {}

public:
	// copy constructor
  QAD_FMC(const QAD_FMC& other) = delete;
  // seems references and consts used for operator overloading or default constructor methods
  QAD_FMC& operator=(const QAD_FMC& other) = delete;

  // to create a singleton
  static QAD_FMC& get(void) {
  	// this will call default constructor
    static QAD_FMC instance;

    return instance;
  }

  static QA_InitState getState(void) {
    return get().state;
  }

  static QA_Result init(void) {
  	return get().imp_init();
  }

  static QA_Result test(void) {
  	return get().imp_test();
  }

  static uint32_t getBaseAddr(void) {
  	return base_addr;
  }

  static uint32_t getSize(void) {
  	return size;
  }

private:

  QA_Result imp_init();

  QA_Result imp_test(void);

  void cmd(FMC_SDRAM_CommandTypeDef &cmd);
};

#endif /* QAD_FMC_HPP_ */
