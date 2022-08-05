#ifndef __CUSTOM_SERIAL_H__
#define __CUSTOM_SERIAL_H__

#include <Arduino.h>
#include "custom_serial.h"
#include "string.h"

#define SERIAL_BUFFER_LEN	    40
extern char Serial_TxBuffer[SERIAL_BUFFER_LEN];
extern char Serial_RxBuffer[SERIAL_BUFFER_LEN];

//Read from Serial
void Serial_Rx(char* pBuffer);

#endif

