#include <Arduino.h>
#include "custom_gpio.h"

#define LASER1_TX D1
#define LASER1_RX D2

enum LASER_SIGNAL
{
	TRIGGERED = 0,
	NOT_TRIGGERED
};

customGPIO Laser1_TX(LASER1_TX, OUTPUT);
customGPIO Laser1_RX(LASER1_RX, INPUT);

void setup()
{
	Serial.begin(115200);
	delay(5000);
	Serial.println("All setups done!!!");
	Laser1_TX.Write(0);
}

void loop()
{
	bool laser1Signal = Laser1_RX.Read();
	if (laser1Signal == TRIGGERED) 
	{
		Serial.println("Slot A1: Car on");
		laser1Signal = NOT_TRIGGERED;
	}
	
	delay(5000);
	Serial.println("Slot A1: Car off");
}
