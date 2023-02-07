/*
 * QAD_FMC.hpp
 *
 *  Created on: 8 Feb. 2023
 *      Author: ryan
 */

#ifndef QAD_FMC_HPP_
#define QAD_FMC_HPP_

#include "setup.hpp"

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

  static uint32_t getBaseAddr(void) {
  	return base_addr;
  }

  static uint32_t getSize(void) {
  	return size;
  }

private:

  QA_Result imp_init() {
    if (state) {
    	return QA_OK;
    }

    GPIO_InitTypeDef gpio_init = {};
    FMC_SDRAM_TimingTypeDef sdram_timing = {};
    FMC_SDRAM_CommandTypeDef sdram_cmd = {};

    // We keep pin definitions local here as FMC controller has a lot
    // of pin connections, e.g. data, address, control signals etc.
    gpio_init.Mode = GPIO_MODE_AF_PP;
    gpio_init.Pull = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_init.Alternate = GPIO_AF12_FMC;

    // gpio ports or banks
    gpio_init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &gpio_init);

    gpio_init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &gpio_init);

    gpio_init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOF, &gpio_init);

    gpio_init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9;
    HAL_GPIO_Init(GPIOG, &gpio_init);

    gpio_init.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOH, &gpio_init);

    gpio_init.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOI, &gpio_init);

    __HAL_RCC_FMC_CLK_ENABLE();

    handle.Instance = FMC_SDRAM_DEVICE;
    handle.Init.SDBank = FMC_SDRAM_BANK1;
    handle.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
    handle.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
    handle.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;
    handle.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
    // how many clock cycles to give column address to sdram chip
    handle.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
    handle.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
    handle.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
    handle.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
    handle.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;

    sdram_timing.LoadToActiveDelay = 2;
    sdram_timing.ExitSelfRefreshDelay = 7;
    sdram_timing.SelfRefreshTime = 4;
    sdram_timing.RowCycleDelay = 7;
    sdram_timing.WriteRecoveryTime = 2;
    sdram_timing.RPDelay = 2;
    sdram_timing.RCDDelay = 2;

    if (HAL_SDRAM_Init(&handle, &sdram_timing) != HAL_OK) {
      return QA_Fail;
    }

    // enable clock
    sdram_cmd.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
    sdram_cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
    sdram_cmd.AutoRefreshNumber = 1;
    sdram_cmd.ModeRegisterDefinition = 0;
    cmd(sdram_cmd);

    HAL_Delay(1);

    // precharge banks
    sdram_cmd.CommandMode = FMC_SDRAM_CMD_PALL;
    sdram_cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
    sdram_cmd.AutoRefreshNumber = 1;
    sdram_cmd.ModeRegisterDefinition = 0;
    cmd(sdram_cmd);

    // auto refresh
    sdram_cmd.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    sdram_cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
    sdram_cmd.AutoRefreshNumber = 8;
    sdram_cmd.ModeRegisterDefinition = 0;
    cmd(sdram_cmd);

    // program mode register
    sdram_cmd.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
    sdram_cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
    sdram_cmd.AutoRefreshNumber = 1;
    sdram_cmd.ModeRegisterDefinition = modereg_burstlength_1 | modereg_bursttype_seq | modereg_caslatency_3 |
                                       modereg_opmode_std | modereg_writeburst_single;
    cmd(sdram_cmd);


    state = QA_Initialised;

    return QA_OK;

  }

  void cmd(FMC_SDRAM_CommandTypeDef &cmd) {
  	HAL_SDRAM_SendCommand(&handle, &cmd, 0);
  }

};

#endif /* QAD_FMC_HPP_ */
