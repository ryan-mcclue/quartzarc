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

#define QA_USERBUTTON_GPIO_PORT GPIOA
#define QA_USERBUTTON_GPIO_PIN GPIO_PIN_0

#define QA_UART1_TX_PORT GPIOA
#define QA_UART1_TX_PIN GPIO_PIN_9
#define QA_UART1_TX_AF GPIO_AF7_USART1
#define QA_UART1_RX_PORT GPIOA
#define QA_UART1_RX_PIN GPIO_PIN_10
#define QA_UART1_RX_AF GPIO_AF7_USART1
#define QA_UART1_BAUDRATE 57600
#define QA_UART1_TX_FIFOSIZE 256
#define QA_UART1_RX_FIFOSIZE 256

// higher number, lower priority
#define QA_IRQPRIORITY_UART1 ((uint8_t)0x09)


#endif
