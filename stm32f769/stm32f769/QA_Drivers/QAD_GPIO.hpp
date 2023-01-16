/*
 * QAD_GPIO.hpp
 *
 *  Created on: 12 Jan. 2023
 *      Author: ryan
 */

#ifndef QAD_GPIO_HPP_
#define QAD_GPIO_HPP_

#include "setup.hpp"

class QAD_GPIO_Output {
	enum PinState : uint8_t {PinOff = 0, PinOn};

	GPIO_TypeDef *gpio;
	uint16_t pin;

	PinState state;

public:

	// delete default constructor
  QAD_GPIO_Output() = delete;

  // member initialiser lists more efficient if copying
  // over an object as will not create a duplicate object
  QAD_GPIO_Output(GPIO_TypeDef *gpio, uint16_t pin) :
    gpio(gpio), pin(pin), state(PinOff) {

    GPIO_InitTypeDef GPIO_Init;
    GPIO_Init.Pin = pin;
    GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(gpio, &GPIO_Init);
  }

  ~QAD_GPIO_Output() {
    GPIO_InitTypeDef GPIO_Init;
    GPIO_Init.Pin = pin;
    GPIO_Init.Mode = GPIO_MODE_INPUT;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(gpio, &GPIO_Init);
  }

  void on(void) {
  	HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_SET);
  	state = PinOn;
  }

  void off(void) {
  	HAL_GPIO_WritePin(gpio, pin, GPIO_PIN_RESET);
  	state = PinOff;
  }

  void toggle(void) {
  	if (state == PinOn) {
  		off();
  	} else {
  		on();
  	}
  }

  PinState getState(void) {
  	return state;
  }

};

#endif /* QAD_GPIO_HPP_ */
