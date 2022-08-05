#include "custom_gpio.h"

bool customGPIOReadItFlag;

/*
Constructor with pin and mode
@param pinGPIO <uint_t> pin
@param modeGPIO <customGPIO::MODE> mode of pin: output/input
*/
customGPIO::customGPIO(uint8_t pinGPIO, uint8_t modeGPIO)
{
	mPin = pinGPIO;
	mMode = modeGPIO;
	mState = 0;
	pinMode(mPin, mMode);
	customGPIOReadItFlag = 0;
}

/*
Copy constructor
*/
customGPIO::customGPIO(const customGPIO &gpio)
{
	mPin = gpio.mPin;
	mMode = gpio.mMode;
	mState = gpio.mState;
}

/*
Destructor
*/
customGPIO::~customGPIO()
{
	mPin = 0;
	mMode = 0;
	mState = 0;
	customGPIOReadItFlag = 0;
}

/*
Write to GPIO output pin
@param stateGPIO <uint8_t> state 1 or 0
@return void
*/
void customGPIO::Write(uint8_t stateGPIO)
{
	mState = stateGPIO;
	digitalWrite(mPin, mState);
	//Serial.printf("mState = %d", mState);
}

/*
Read state of GPIO input pin, using timer1
@param void
@return <uint8_t> state of pin
*/
uint8_t customGPIO::Read(void)
{
	uint8_t readTimes = 0;
	uint8_t readSum = 0;
	//Serial.println("before Timer Init OK");
	customGPIORead_Timer1_Init();
	//Serial.println("after Timer Init OK");

	//read 10 times
	while(readTimes < 10)
	{
		if(!customGPIOReadItFlag)
		{	
			yield();
			continue;
		}
		else
		{
			customGPIOReadItFlag = 0;
			readTimes++;
			readSum += digitalRead(mPin);
			//Serial.printf("Read inside OK time %d\n", readTimes);
		}
	}
	if (readSum == 10)	mState = 1;
	else mState = 0;
	customGPIORead_Timer1_DeInit();
	//Serial.println("Timer deInit OK");
	return mState;
}

/*
Init the timer1 to help customGPIO::Read(), read after each 1ms
*/
void customGPIORead_Timer1_Init(void)
{
	timer1_attachInterrupt(customGPIORead_Timer1_ISR);
	//Serial.println("ok1");
	timer1_isr_init();
	//Serial.println("ok2");
	/*
	5000 -> cứ khi timer tăng đến 5000 tick (5000us = 5ms) thì xảy ra ngắt
	*/
	timer1_write(5000);
	//Serial.println("ok3");
	/*
	read comment ò timer1_enable() for more info
	0x01 -> prescale = 16 -> tần số timer1 = 80MHz/16 = 5MHz ---> 1 tick = 0.2us
	0 -> interrupt at edge, 1 -> interrupt at level
	true -> khi xảy ra ngắt thì khởi động lại timer, false = ko tự động
	*/
	timer1_enable(0x01, 0, true);
	//Serial.println("ok4");
}	

/*
DeInit the timer1
*/
void customGPIORead_Timer1_DeInit(void)
{
	customGPIOReadItFlag = 0;
	timer1_detachInterrupt();
  	timer1_disable();
}

/*
ISR of Timer1
*/
void IRAM_ATTR customGPIORead_Timer1_ISR()
{
	customGPIOReadItFlag = 1;
}

