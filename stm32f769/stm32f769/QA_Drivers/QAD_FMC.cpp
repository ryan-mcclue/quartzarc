/*
 * QAD_FMC.cpp
 *
 *  Created on: 8 Feb. 2023
 *      Author: ryan
 */

#include "QAD_FMC.hpp"

 QA_Result QAD_FMC::imp_init() {
    if (state) {
    	return QA_OK;
    }

    // configuration order: gpio -> fmc -> sdram

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
    // banks are memory modules that access portion of memory, so might have four banks with 1GB each
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

    // configure the program mode register
    sdram_cmd.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
    sdram_cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
    sdram_cmd.AutoRefreshNumber = 1;
    sdram_cmd.ModeRegisterDefinition = modereg_burstlength_1 | modereg_bursttype_seq | modereg_caslatency_3 |
                                       modereg_opmode_std | modereg_writeburst_single;
    cmd(sdram_cmd);

    // refresh rate
    HAL_SDRAM_ProgramRefreshRate(&handle, refresh);

    state = QA_Initialised;

    return QA_OK;

  }

  QA_Result QAD_FMC::imp_test(void) {

  	// access in 4bytes, hence divide size by 4
    for (uint32_t i = base_addr; i < (base_addr + (size >> 2)); i += 1) {
      QA_FMC_PTR->data[i] = i;
    }

    for (uint32_t i = base_addr; i < (base_addr + (size >> 2)); i += 1) {
      if (QA_FMC_PTR->data[i] != i) {
      	return QA_Fail;
      }
    }
  }

  void QAD_FMC::cmd(FMC_SDRAM_CommandTypeDef &cmd) {
  	HAL_SDRAM_SendCommand(&handle, &cmd, 0);
  }

