#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"
#include "main.h"
#include "serial_data.h"

extern UART_HandleTypeDef huart1;

void NMI_Handler(void)
{

}

void HardFault_Handler(void)
{
 
  while (1)
  {
  }

}

void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{

  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
    
}

extern in_data_t serial_data;

void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  serial_updateTimeout(&serial_data);
}

void USART1_IRQHandler(void)
{
   if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) == SET) {
       fifo_addByte(USART1->DR);
   }
}
