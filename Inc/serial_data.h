#ifndef _SERIAL_DATA_H_
#define _SERIAL_DATA_H_

#include <stdint.h>
#include "crc8.h"

#define INDATA_SIZE  64
#define FIFO_SIZE    512

typedef struct
{
    uint16_t cmd;
    uint16_t data;
}servo_data_t;

typedef struct
{
    uint8_t status;
    uint16_t size;
    uint8_t crc;
    uint16_t counter;
    uint8_t data[INDATA_SIZE];
    uint32_t timeout_ms;    
}in_data_t;

uint8_t serial_parseData(in_data_t *serial_data, uint8_t uart_byte);
void fifo_addByte(uint8_t byte);
uint8_t isFifoNEmpty(void);
uint8_t fifo_getByte(void);
void serial_updateTimeout(in_data_t *serial_data);

#endif 
