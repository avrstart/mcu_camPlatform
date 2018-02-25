#include "serial_data.h"

/*
1 - AA
2 - 55
3 - data hi size
4 - data lo size
5 - crc
6... data

aa 55 00 04 00 01 00 84 03      900
aa 55 00 04 00 01 00 34 08      2100
aa 55 00 04 00 01 00 DC 05      1500
*/

uint8_t fifo_buf[FIFO_SIZE];
uint16_t get_counter = 0;
uint16_t set_counter = 0;

uint8_t isFifoNEmpty(void)
{
    if(get_counter != set_counter) {
        return 1;
    }
    return 0;
}

uint8_t fifo_getByte(void){
    uint8_t uart_byte = fifo_buf[set_counter++];
    if(set_counter == FIFO_SIZE) set_counter = 0;
    return uart_byte;
}

void fifo_addByte(uint8_t byte){
    fifo_buf[get_counter++] = byte;
    if (get_counter == FIFO_SIZE) get_counter = 0;
}

void serial_updateTimeout(in_data_t *serial_data)
{
    serial_data->timeout_ms++;
    if(serial_data->timeout_ms > 2000) {
        serial_data->status = 0;
    }     
}

uint8_t serial_parseData(in_data_t *serial_data, uint8_t uart_byte)
{
    serial_data->timeout_ms = 0;
    switch(serial_data->status) { 
        case 0:
            if(uart_byte == 0xAA)  {        
                serial_data->status = 1;               
            } 
            break;
        
        case 1:
            if(uart_byte == 0x55)  {        
                serial_data->status = 2;               
            }
            else {
                serial_data->status = 0;
            }       
            break;
        case 2:
            serial_data->size = (uart_byte << 8);      
            serial_data->status = 3;               
            break;
        case 3:
            serial_data->size |= uart_byte;
            if(serial_data->size > 0) {        
                serial_data->status = 4;   
            }
            else {
                serial_data->status = 0;
            }        
            break;
        case 4:
            serial_data->crc = uart_byte;      
            serial_data->status = 5; 
            serial_data->counter = 0;       
            break;
        case 5:
            serial_data->data[serial_data->counter++] = uart_byte;
      
            if(serial_data->counter >= serial_data->size) {
                serial_data->status = 0;
               // if(serial_data->crc == crc8(serial_data->data, serial_data->size)) {                   
                    return 1;
               // }
            }                                       
            break;
        }
    return 0;
}

