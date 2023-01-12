#include "main.hpp"
#include "boot.hpp"

int
main(void)
{
  // System setup
  if (SystemInitialize())
  {
    while (1) {}
  }

  // Initialize GPIOs
  GPIO_InitTypeDef GPIO_Init = {};
  GPIO_Init.Pin = QA_USERLED_RED_GPIO_PIN;
  GPIO_Init.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Init.Pull = GPIO_NOPULL;
  GPIO_Init.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(QA_USERLED_RED_GPIO_PORT, &GPIO_Init);

  GPIO_Init.Pin = QA_USERLED_GREEN_GPIO_PIN;
  HAL_GPIO_Init(QA_USERLED_GREEN_GPIO_PORT, &GPIO_Init);


  // Processing loop
  while (1) {
  	HAL_GPIO_WritePin(QA_USERLED_GREEN_GPIO_PORT, QA_USERLED_GREEN_GPIO_PIN, GPIO_PIN_SET);
  	HAL_Delay(500);
  	HAL_GPIO_WritePin(QA_USERLED_GREEN_GPIO_PORT, QA_USERLED_GREEN_GPIO_PIN, GPIO_PIN_RESET);
  	HAL_Delay(500);
  }

  return 0;
}
