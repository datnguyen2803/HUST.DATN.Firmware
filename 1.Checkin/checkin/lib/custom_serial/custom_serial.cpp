#include "custom_serial.h"

char Serial_TxBuffer[SERIAL_BUFFER_LEN];
char Serial_RxBuffer[SERIAL_BUFFER_LEN];

void Serial_Rx(char* pbuffer)
{
	memset(pbuffer, 0, SERIAL_BUFFER_LEN);
	byte _buffer_pos = 0;
	while(1)
	{
		if(Serial.available())
		{
			char serial_input = Serial.read();
			//Serial.print(_buffer_pos);
			if(serial_input == '\r' || serial_input == '\n')
			{	
				//_buffer[_buffer_pos] = serial_input;
				break;
			}
			pbuffer[_buffer_pos] = serial_input;
			_buffer_pos++;
		}
		if(_buffer_pos == SERIAL_BUFFER_LEN )
		{
			break;
		}
	}
}

