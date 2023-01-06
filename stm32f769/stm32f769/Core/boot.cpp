/* ========================================================================
  Quartz Arc

  Role: Boot Code
  Date: 06/01/2023
  Created By: Ryan McClue
  Notice: spx license identifier
   ======================================================================== */

#include "boot.hpp"

// System Initialize Function
QA_Result
SystemInitialize(void)
{
  SCB_EnableICache();
  SCB_EnableDCache();

  // adaptive real time accelerator
  // quicker access to in-build flash
  __HAL_FLASH_ART_ENABLE();
  __HAL_FLASH_PREFETCH_BUFFER_ENABLE();

  // so instead of major.minor interrupt priorities,
  // only have single number between 0-15
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  // set systick to a high priority
  HAL_InitTick(TICK_INT_PRIORITY);

  // we want LSE to power RTC?
  // The processor has some sort of 'backup' domain circuitry,
  // i.e. memory and registers that are preserved on reset, code flashing
  // we want to have LSE as part of this 'backup' domain also
  // so it can power RTC?
  // to modify the 'backup' domain, must get access to it
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  // enable power management clock
  __HAL_RCC_PWR_CLK_ENABLE();
  // internal regulator output voltage
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  // for clocks:
  // 1. enable oscillator and PLL so frequency output for system is what we want
  // 2. making sure bus clock and bus clock dividers are correct
  // 3. setup some peripheral clocks (most are driver specific)

  RCC_OscInitTypeDef RCC_OscInit = {0};
  RCC_OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInit.HSEState = RCC_HSE_ON;
  RCC_OscInit.LSEState = RCC_LSE_ON;
  RCC_OscInit.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInit.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInit.PLL.PLLM = 25;
  RCC_OscInit.PLL.PLLN = 432;
  RCC_OscInit.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInit.PLL.PLLQ = 4;

  if (HALL_RCC_OscConfig(&RCC_OscInit) != HAL_OK)
  {
    return QA_Fail;
  }

  // syscfg collection of registers not fitting elsewhere
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  // overdrive mode ensures running smoothly at max. clock value
  // i.e. allow core to run at higher frequency than default
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
  	return QA_Fail;
  }



  return QA_OK;
}
