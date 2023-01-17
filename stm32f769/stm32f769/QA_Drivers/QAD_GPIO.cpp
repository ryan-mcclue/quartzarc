/*
 * QAD_GPIO.cpp
 *
 *  Created on: 12 Jan. 2023
 *      Author: ryan
 */
#include "QAD_GPIO.hpp"

QAD_GPIO_Output::QAD_GPIO_Output(GPIO_TypeDef *gpio, uint16_t pin) :
    gpio(gpio), pin(pin), state(PinOff) {

    GPIO_InitTypeDef GPIO_Init;
    GPIO_Init.Pin = pin;
    GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(gpio, &GPIO_Init);
  }

  QAD_GPIO_Output::~QAD_GPIO_Output() {
    GPIO_InitTypeDef GPIO_Init;
    GPIO_Init.Pin = pin;
    GPIO_Init.Mode = GPIO_MODE_INPUT;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(gpio, &GPIO_Init);
  }

  void QAD_GPIO_Output::on(void) {
  	HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
  	state = PinOn;
  }

  void QAD_GPIO_Output::off(void) {
  	HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
  	state = PinOff;
  }

  void QAD_GPIO_Output::toggle(void) {
  	if (state == PinOn) {
  		off();
  	} else {
  		on();
  	}
  }

  QAD_GPIO_Output::PinState QAD_GPIO_Output::getState(void) {
  	return state;
  }


