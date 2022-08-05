#include "stepper_28byj48.h"

bool stepperTimerITFlag;
void IRAM_ATTR StepperTimer_ISR()
{
	//  portENTER_CRITICAL_ISR(&MUX);
	stepperTimerITFlag = 1;
	//  portEXIT_CRITICAL_ISR(&MUX);
}

/*
	Init the Stepper with timer1 and default IO, coil1, 2, 3, 4 = D5, D6, D7, D8. default speed = 10rpm
	@return Stepper
*/
Stepper::Stepper()
{
	uint8_t i = 0;
	for (i = 0; i < 4; i++)
	{
		pinMode(mPins[i], OUTPUT);
	}

	timer1_attachInterrupt(StepperTimer_ISR);
	timer1_isr_init();
	/*
	0x01 -> prescale = 16 -> tần số timer0 = 80MHz/16 = 5MHz -> 1 tick = 0.2us
	0 -> interrupt at edge, 1 -> interrupt at level
	true -> auto reset on, false = off
	*/
	timer1_enable(0x01, 0, true);

	/*
	time for 1 step, in milisecond (us)
	param is number of ticks, and tick's duration is calculate on timer1_enable() function
	*/
	int timerThreshold = 60000000 / (mSpeed * STEPS_PER_ROUND * MINISTEPS_PER_STEP);
	timer1_write(timerThreshold * 10 / 2);
	stepperTimerITFlag = 0;

	mCurrentPinState = 0;
	mCurrentPosition = 0;
}

/*
	Init the Stepper IO, speed and timer1
	@param pPinStepper 		<uint8_t*> array of stepper pins
	@return					Stepper
*/
Stepper::Stepper(uint8_t *pPinStepper)
{
	uint8_t i = 0;
	for (i = 0; i < 4; i++)
	{
		mPins[i] = *(pPinStepper + i);
		pinMode(mPins[i], OUTPUT);
	}

	timer1_attachInterrupt(StepperTimer_ISR);
	timer1_isr_init();
	/*
	0x01 -> prescale = 16 -> tần số timer0 = 80MHz/16 = 5MHz -> 1 tick = 0.2us
	0 -> interrupt at edge, 1 -> interrupt at level
	true -> auto reset on, false = off
	*/
	timer1_enable(0x01, 0, true);

	/*
	time for 1 step, in milisecond (us)
	param is number of ticks, and tick's duration is calculate on timer1_enable() function
	*/
	int timerThreshold = 60000000 / (mSpeed * STEPS_PER_ROUND * MINISTEPS_PER_STEP) - 1; // in ms
	timer1_write(timerThreshold * 10 / 2);
	stepperTimerITFlag = 0;

	mCurrentPinState = 0;
	mCurrentPosition = 0;
}

/*
	Init the Stepper IO, speed and timer1
	@param speedStepper		<uint8_t> speed, in rounds per minute
	@param pPinStepper 		<uint8_t*> array of stepper pins
	@return					Stepper
*/
Stepper::Stepper(uint8_t speedStepper, uint8_t *pPinStepper)
{
	uint8_t i = 0;
	for (i = 0; i < 4; i++)
	{
		mPins[i] = *(pPinStepper + i);
		pinMode(mPins[i], OUTPUT);
	}
	mSpeed = speedStepper;

	timer1_attachInterrupt(StepperTimer_ISR);
	timer1_isr_init();
	/*
	0x01 -> prescale = 16 -> tần số timer0 = 80MHz/16 = 5MHz -> 1 tick = 0.2us
	0 -> interrupt at edge, 1 -> interrupt at level
	true -> auto reset on, false = off
	*/
	timer1_enable(0x01, 0, true);

	/*
	time for 1 step, in milisecond (us)
	param is number of ticks, and tick's duration is calculate on timer1_enable() function
	*/
	int timerThreshold = 60000000 / (mSpeed * STEPS_PER_ROUND * MINISTEPS_PER_STEP) - 1; // in ms
	timer1_write(timerThreshold * 10 / 2);
	stepperTimerITFlag = 0;

	mCurrentPinState = 0;
	mCurrentPosition = 0;
}

/*
	Copy constructor, return the Stepper with same attributes
*/
Stepper::Stepper(const Stepper &stp)
{
	mSpeed = stp.mSpeed;
	mCurrentPinState = stp.mCurrentPinState;
	mCurrentPosition = stp.mCurrentPosition;
	uint8_t i = 0;
	for (i = 0; i < 4; i++)
	{
		mPins[i] = stp.mPins[i];
	}
}

/*
	DeInit the Stepper timer1
*/
Stepper::~Stepper()
{
	timer1_detachInterrupt();
	timer1_disable();
	free(mPins);
}

/*
	Run the stepper 1 step with direction CW or CWW, also update the position of the stepper
	@param dirStepper 	<STEPPER_DIRECTION>: CW or CCW
	@return 						<bool> Finish 1 step or not
*/
bool Stepper::RunOneStep(STEPPER_DIRECTION dirStepper)
{
	uint8_t i = 0;
	if (stepperTimerITFlag)
	{
		stepperTimerITFlag = 0;
		/*
		if dirStepper = 1 (CCW) then current state = current state^7
		ex: 2^7 = 0010 ^ 0111 = 0101 = 5
		*/
		mCurrentPinState = dirStepper ? mCurrentPinState ^ 7 : mCurrentPinState;
		for (i = 0; i < 4; i++)
		{
			digitalWrite(mPins[i], PIN_STATE[mCurrentPinState][i]);
		}
		mCurrentPinState = dirStepper ? (mCurrentPinState ^ 7) + 1 : mCurrentPinState + 1;
	}
	else
		yield();

	if (mCurrentPinState == MINISTEPS_PER_STEP)
	{
		mCurrentPinState = 0;

		// if CW -> mCurrentPosition++, if CCW -> mCurrentPosition--
		mCurrentPosition = dirStepper ? mCurrentPosition - 1 : mCurrentPosition + 1;
		if (mCurrentPosition >= STEPS_PER_ROUND)
			mCurrentPosition = 0;
		if (mCurrentPosition < 0)
			mCurrentPosition = STEPS_PER_ROUND - 1;
		// Serial.println("___Finish 1 step___");
		return 1; // finish 1 step
	}
	return 0; // not finish yet
}

/*
	Set speed of stepper by modifier timer1
	@param speedStepper	<uint8_t> speed, in rounds per minute
	@return void
*/
void Stepper::SetSpeed(uint8_t speedStepper)
{
	/*
time for 1 step, in milisecond (ms)
param is number of ticks, and tick's duration is calculate on timer1_enable() function
*/
	mSpeed = speedStepper;
	int timerThreshold = 60000000 / (mSpeed * STEPS_PER_ROUND * MINISTEPS_PER_STEP); // in ms
	timer1_write(timerThreshold * 10 / 2);
}

/*
	Run the stepper 1 round from current position with direction CW or CWW
	@param dirStepper 	<STEPPER_DIRECTION>: CW or CCW
	@return 						void
*/
void Stepper::RunOneRound(STEPPER_DIRECTION dirStepper)
{
	int i = 0;
	for (i = 0; i < STEPS_PER_ROUND; i++)
	{
		// Serial.print("__Steps = "); Serial.print(i); Serial.println("___");
		while (!RunOneStep(dirStepper))
			;
	}
	Serial.println("___Finish 1 round___");
}

/*
	Run the stepper to the desired degree from current position, also update the position of the stepper
	@param degreeStepper	<uint8_t> desired degree, [0; 360]
	@param dirStepper 		<STEPPER_DIRECTION>: CW or CCW
	@return 							void
*/
void Stepper::RunToDegree(int degreeStepper, STEPPER_DIRECTION dirStepper)
{
	Serial.print("___Start position: ");
	Serial.print(mCurrentPosition);
	Serial.println("___");
	// l stands for local
	int _position = degreeStepper * STEPS_PER_ROUND / 360;
	if (_position == STEPS_PER_ROUND)
		_position = 0;
	while (mCurrentPosition != _position)
	{
		while (!RunOneStep(dirStepper))
			;
	}
	Serial.print("___Finish position: ");
	Serial.print(mCurrentPosition);
	Serial.println("___");
}

