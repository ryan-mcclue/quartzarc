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
  // systick is a timer built into ARM, so is consistent unlike peripheral timers
  // also only counts up and no capture modes
  // IMPORTANT: If setting systick to say 1ms, NVIC might dump some interrupts as too many
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
  // 1. enable oscillators and PLL so frequency output for system is what we want
  // 2. making sure bus clock and bus clock dividers are correct
  // 3. setup some peripheral clocks (most are driver specific)
  // 4. setup GPIO clocks and DMA clocks

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

  if (HAL_RCC_OscConfig(&RCC_OscInit) != HAL_OK)
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

  // enable cpu and bus clocks
  RCC_ClkInitTypeDef RCC_ClkInit = {0};
  RCC_ClkInit.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
  		                    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInit.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInit.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInit.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInit.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInit, FLASH_LATENCY_7) != HAL_OK)
  {
    return QA_Fail;
  }

  // setup peripheral clocks
  // RTC is considered a peripheral?
  RCC_PeriphCLKInitTypeDef RCC_PeriphClkInit = {0};
  RCC_PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_LTDC | RCC_PERIPHCLK_USART1;

  // LTDC uses SAI PLL
  RCC_PeriphClkInit.PLLSAI.PLLSAIN = 384;
  RCC_PeriphClkInit.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  RCC_PeriphClkInit.PLLSAI.PLLSAIR = 7;
  RCC_PeriphClkInit.PLLSAI.PLLSAIQ = 8;
  RCC_PeriphClkInit.PLLSAIDivQ = 1;
  RCC_PeriphClkInit.PLLSAIDivR = RCC_PLLSAIDIVR_2;

  RCC_PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  RCC_PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;

  if (HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphClkInit) != HAL_OK)
  {
    return QA_Fail;
  }

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOK_CLK_ENABLE();

  __HAL_RCC_DMA1_CLK_ENABLE();
  __HAL_RCC_DMA2_CLK_ENABLE();


  return QA_OK;
}
