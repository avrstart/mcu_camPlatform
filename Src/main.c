#include "main.h"
#include "init.h"
#include "stm32f1xx_hal.h"
#include "crc8.h"
#include "serial_data.h"

servo_data_t *servo_data;
in_data_t serial_data;

void set_pwm(uint8_t ch, uint16_t pos) 
{
    
    switch(ch) {
        case 1: //lr cam
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_3, pos);  
            break;
        case 2: //ud cam
            if((pos <= 900) && (pos >= 2100))
                return;
            __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_4, pos);  
            break;
        case 3: //stop
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0); //lfw
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); //lb
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0); //rfw
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0); //rb
             break;
        case 5: //turn right
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pos); //lfw
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); //lb
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0); //rfw
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pos); //rb
            break;
        case 6: //turn left
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0); //lfw
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, pos); //lb
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pos); //rfw
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0); //rb
            break;
        case 7:
            //move forward
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pos); //rfw
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 0); //rb
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pos); //lfw
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 0); //lb
            break;
        case 8:
            //move back
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 0); //rfw
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pos); //rb
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 0); //lfw
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, pos); //lb
            break;
                    
        default: break;
    }
}

int main(void)
{   
    init_all();

    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);

    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
    
    set_pwm(1, 1500);
    set_pwm(2, 1500);
       
    set_pwm(3, 0);
   
    while (1)
    {
        if(isFifoNEmpty()) {
            if(serial_parseData(&serial_data, fifo_getByte()) == 1) {
                servo_data = (servo_data_t *)serial_data.data;
                set_pwm(servo_data->cmd, servo_data->data);
            }
        }            
    }
}



