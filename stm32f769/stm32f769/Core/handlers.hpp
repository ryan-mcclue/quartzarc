#ifndef __HANDLERS_HPP__
#define __HANDLERS_HPP__


#include "setup.hpp"

extern "C" {

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

// determine this name from the startup.s
void USART1_IRQHandler(void);

}

#endif
