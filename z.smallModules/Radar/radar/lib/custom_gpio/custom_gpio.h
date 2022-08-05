#ifndef __CUSTOM_GPIO_H__
#define __CUSTOM_GPIO_H__

#include <Arduino.h>
#include "custom_gpio.h"

class customGPIO
{
private:
	uint8_t mPin, mMode;//, mState;
	bool mItFlag;
protected:
	uint8_t mState;

public:
	// customGPIO();
	customGPIO(uint8_t pinGPIO, uint8_t modeGPIO);
	customGPIO(const customGPIO &gpio);
	~customGPIO();
	void Write(uint8_t stateGPIO);
	uint8_t Read(void);
};

// support function for GPIO::Read()
extern bool customGPIOReadItFlag;
void customGPIORead_Timer1_Init(void);
void customGPIORead_Timer1_DeInit(void);
void IRAM_ATTR customGPIORead_Timer1_ISR();

#endif
