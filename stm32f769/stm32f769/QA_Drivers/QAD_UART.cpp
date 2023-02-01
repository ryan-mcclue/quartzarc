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
    return QA_Fail;
  }

  state = PeriphActive;
  return QA_OK;
}

void QAD_UART::deinit(void) {
  if (state != PeriphActive) return;

  periphDeinit(DeinitFull);

  state = PeriphInactive;
}

QAD_UART::PeriphState QAD_UART::getState(void) {
  return state;
}

UART_HandleTypeDef& QAD_UART::getHandle(void) {
  return handle;
}

void QAD_UART::start_tx(void) {
	// interrupt when tx register is empty
	// so, will ask when it wants more data
	__HAL_UART_ENABLE_IT(&handle, UART_IT_TXE);
  tx_state = PeriphActive;
}

void QAD_UART::stop_tx(void) {
	__HAL_UART_DISABLE_IT(&handle, UART_IT_TXE);
  tx_state = PeriphInactive;
}

QAD_UART::PeriphState QAD_UART::get_tx_state(void) {
	return tx_state;
}

void QAD_UART::start_rx(void) {
	__HAL_UART_ENABLE_IT(&handle, UART_IT_RXNE);
	rx_state = PeriphActive;
}

void QAD_UART::stop_rx(void) {
	__HAL_UART_DISABLE_IT(&handle, UART_IT_RXNE);
	rx_state = PeriphInactive;
}

QAD_UART::PeriphState QAD_UART::get_rx_state(void) {
	return rx_state;
}

QA_Result QAD_UART::periphInit(void) {
	GPIO_InitTypeDef GPIO_Init = {0};

	GPIO_Init.Pin = tx_pin;
	GPIO_Init.Mode = GPIO_MODE_AF_PP;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_Init.Alternate = tx_af;
	HAL_GPIO_Init(tx_gpio, &GPIO_Init);

	GPIO_Init.Pin = rx_pin;
	GPIO_Init.Mode = GPIO_MODE_AF_PP;
	// prevent possible spurious reads of a start bit resulting from floating state
	GPIO_Init.Pull = GPIO_PULLUP;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_Init.Alternate = rx_af;
	HAL_GPIO_Init(rx_gpio, &GPIO_Init);

	switch (uart) {
	  case (QAD_UART1): {
	    __HAL_RCC_USART1_CLK_ENABLE();
	    __HAL_RCC_USART1_FORCE_RESET();
	    __HAL_RCC_USART1_RELEASE_RESET();
	    irq = USART1_IRQn;
	    handle.Instance = USART1;
	  } break;
	  case (QAD_UART2): {
	    __HAL_RCC_USART2_CLK_ENABLE();
	    __HAL_RCC_USART2_FORCE_RESET();
	    __HAL_RCC_USART2_RELEASE_RESET();
	    irq = USART2_IRQn;
	    handle.Instance = USART2;
	  } break;
	  case (QAD_UART3): {
	    __HAL_RCC_USART3_CLK_ENABLE();
	    __HAL_RCC_USART3_FORCE_RESET();
	    __HAL_RCC_USART3_RELEASE_RESET();
	    irq = USART3_IRQn;
	    handle.Instance = USART3;
	  } break;
	  case (QAD_UART4): {
	    __HAL_RCC_UART4_CLK_ENABLE();
	    __HAL_RCC_UART4_FORCE_RESET();
	    __HAL_RCC_UART4_RELEASE_RESET();
	    irq = UART4_IRQn;
	    handle.Instance = UART4;
	  } break;
	  case (QAD_UART5): {
	    __HAL_RCC_UART5_CLK_ENABLE();
	    __HAL_RCC_UART5_FORCE_RESET();
	    __HAL_RCC_UART5_RELEASE_RESET();
	    irq = UART5_IRQn;
	    handle.Instance = UART5;
	  } break;
	  case (QAD_UART6): {
	    __HAL_RCC_USART6_CLK_ENABLE();
	    __HAL_RCC_USART6_FORCE_RESET();
	    __HAL_RCC_USART6_RELEASE_RESET();
	    irq = USART6_IRQn;
	    handle.Instance = USART6;
	  } break;
	}

	  handle.Init.BaudRate = baud;
	  handle.Init.WordLength = UART_WORDLENGTH_8B;
	  handle.Init.StopBits = UART_STOPBITS_1;
	  handle.Init.Parity = UART_PARITY_NONE;
	  handle.Init.Mode = UART_MODE_TX_RX;
	  handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	  handle.Init.OverSampling = UART_OVERSAMPLING_16;
	  handle.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	  if (HAL_UART_Init(&handle) != HAL_OK) {
	    periphDeinit(DeinitPartial);
	    return QA_Fail;
	  }

	  __HAL_UART_ENABLE(&handle);

	  HAL_NVIC_SetPriority(irq, irq_priority, 0x00);
	  HAL_NVIC_EnableIRQ(irq);

	  return QA_OK;
}

void QAD_UART::periphDeinit(QAD_UART::DeinitMode deinit_mode) {
	GPIO_InitTypeDef GPIO_Init = {0};

	if (deinit_mode == DeinitFull) {
	  stop_tx();
	  stop_rx();
	  HAL_NVIC_DisableIRQ(irq);
	  __HAL_UART_DISABLE(&handle);
	  HAL_UART_DeInit(&handle);
	}

	switch (uart) {
	  case (QAD_UART1): {
	    __HAL_RCC_USART1_CLK_DISABLE();
	  } break;
	  case (QAD_UART2): {
	    __HAL_RCC_USART2_CLK_DISABLE();
	  } break;
	  case (QAD_UART3): {
	    __HAL_RCC_USART3_CLK_DISABLE();
	  } break;
	  case (QAD_UART4): {
	    __HAL_RCC_UART4_CLK_DISABLE();
	  } break;
	  case (QAD_UART5): {
	    __HAL_RCC_UART5_CLK_DISABLE();
	  } break;
	  case (QAD_UART6): {
	    __HAL_RCC_USART6_CLK_DISABLE();
	  } break;
	}

	GPIO_Init.Pin = rx_pin;
	GPIO_Init.Mode = GPIO_MODE_INPUT;
	GPIO_Init.Pull = GPIO_NOPULL;
	GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(rx_gpio, &GPIO_Init);

	GPIO_Init.Pin = tx_pin;
	HAL_GPIO_Init(tx_gpio, &GPIO_Init);

}
