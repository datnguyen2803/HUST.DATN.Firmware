#ifndef __STEPPER_28BYJ48_H__
#define __STEPPER_28BYJ48_H__

#include <Arduino.h>
#include "stepper_28byj48.h"

void IRAM_ATTR StepperTimer_ISR();

class Stepper
{
private:
	//define some specific of stepper 28byj-48
	const uint8_t MINISTEPS_PER_STEP = 8;  //= 4, in full-step mode, = 8 in half-step mode
	const int STEPS_PER_ROUND = 512;  //= 512 
	//const uint8_t ROTATION_PER_ROUND	= 64; // = 64
	const bool PIN_STATE[8][4] =
	{
		{1, 0, 0, 0},
		{1, 1, 0, 0},
		{0, 1, 0, 0},
		{0, 1, 1, 0},
		{0, 0, 1, 0},
		{0, 0, 1, 1},
		{0, 0, 0, 1},
		{1, 0, 0, 1}
	};

	/*
	@enum STEPPER_PIN
	Value for stepper coil pin of 28byj-48
	*/
	enum STEPPER_PIN
	{
		STEPPER_COIL1 = D5,  //blue wire
		STEPPER_COIL2 = D6,  //pink wire
		STEPPER_COIL3 = D7,  //yellow wire
		STEPPER_COIL4 = D8   //orange wire
	};


	
	//m stand for private attribute

	uint8_t mPins[4] = {STEPPER_COIL1, STEPPER_COIL2, STEPPER_COIL3, STEPPER_COIL4};	//pins of stepper
	uint8_t mSpeed = 10; //speed of stepper, rounds per minute	
	int8_t mCurrentPinState; //current pin state, use in RunOneStep()
	int mCurrentPosition; //current position, in range [0; STEPS_PER_ROUND - 1]
public:
	/*
	@enum STEPPER_DIRECTION
	Value for stepper direction 
	*/
	enum STEPPER_DIRECTION
	{
		STEPPER_CW	= 0,		//clockwise
		STEPPER_CCW	= 1			//counter clockwise
	};

private:
	bool RunOneStep(STEPPER_DIRECTION dirStepper);
public:
	Stepper();
	Stepper(uint8_t *pPinStepper);
	Stepper(uint8_t speedStepper, uint8_t *pPinStepper);
	Stepper(const Stepper &stp);
	~Stepper();
	void SetSpeed(uint8_t speedStepper);
	void RunOneRound(STEPPER_DIRECTION dirStepper);
	void RunToDegree(int degreeStepper, STEPPER_DIRECTION dirStepper);
};


#endif