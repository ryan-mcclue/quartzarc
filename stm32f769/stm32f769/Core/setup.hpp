#ifndef __SETUP_HPP__
#define __SETUP_HPP__

// how are these HAL include paths found?
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

enum QA_Result : uint8_t {QA_OK = 0, QA_Fail};

#define QA_USERLED_RED_GPIO_PORT GPIOJ
#define QA_USERLED_RED_GPIO_PIN GPIO_PIN_13

#define QA_USERLED_GREEN_GPIO_PORT GPIOJ
#define QA_USERLED_GREEN_GPIO_PIN GPIO_PIN_5

#endif
