#include "rc522.h"

#define RC522_ID_BLOCK_ADDR	12

void setup() 
{
  // put your setup code here, to run once:
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, 1);
	Serial.begin(115200);
	delay(5000);
	digitalWrite(LED_BUILTIN, 0);
	RC522_Init();
	delay(1000);
}

void loop() 
{
	//rx buffer is not empty
	if(Serial.available())
	{
		Serial_Rx(Serial_RxBuffer);
		Serial.println(Serial_RxBuffer);
		//Serial.println(strcmp(Serial_RxBuffer, "rfid read"));
	}
	
	if(strcmp(Serial_RxBuffer, "rfid read") == 0)
	{
		Serial.println("RFID read command confirm!");

		if(!RC522_CardPolling())
		{
			Serial.println("___No card detect___");
		}
		else
		{
			RC522_ReadFromCard(RC522_ID_BLOCK_ADDR, RC522_buffer, RC522_bufferLen);
			RC522_Stop();
		}
	}
}
