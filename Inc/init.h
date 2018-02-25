#ifndef __INIT_H
#define __INIT_H

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart1;

void init_all(void);

#endif 

