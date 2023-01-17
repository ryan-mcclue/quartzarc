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
public:
	enum PinState : uint8_t {PinOff = 0, PinOn};

	GPIO_TypeDef *gpio;
	uint16_t pin;

	PinState state;

	// delete default constructor
  QAD_GPIO_Output() = delete;

  // member initialiser lists more efficient if copying
  // over an object as will not create a duplicate object
  QAD_GPIO_Output(GPIO_TypeDef *gpio, uint16_t pin);
  ~QAD_GPIO_Output();

  // while GPIO initialisation rarely fails, we could add
  // checks to see if the pin is already in use

  void on(void);
  void off(void);
  void toggle(void);

  PinState getState(void);
};

class QAD_GPIO_Input {
public:
	enum PinState : uint8_t {PinOff = 0, PinOn};

  GPIO_TypeDef *gpio;
  uint16_t pin;
  QAD_GPIO_Input() = delete;
  QAD_GPIO_Input(GPIO_TypeDef *gpio, uint16_t pin) :
  	gpio(gpio), pin(pin) {

    GPIO_InitTypeDef GPIO_Init;
    GPIO_Init.Pin = pin;
    GPIO_Init.Mode = GPIO_MODE_INPUT;
    GPIO_Init.Pull = GPIO_NOPULL;
    GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(gpio, &GPIO_Init);
  }

  ~QAD_GPIO_Input() {
      GPIO_InitTypeDef GPIO_Init;
      GPIO_Init.Pin = pin;
      GPIO_Init.Mode = GPIO_MODE_INPUT;
      GPIO_Init.Pull = GPIO_NOPULL;
      GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;

      HAL_GPIO_Init(gpio, &GPIO_Init);
    }

  PinState getState(void) {
    if (HAL_GPIO_ReadPin(gpio, pin) == GPIO_PIN_SET) {
    	return PinOn;
    } else {
    	return PinOff;
    }
  }
};

#endif /* QAD_GPIO_HPP_ */
