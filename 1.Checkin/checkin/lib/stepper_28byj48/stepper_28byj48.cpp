#include "stepper_28byj48.h"

bool stepperTimerITFlag;
void IRAM_ATTR StepperTimer_ISR()
{
//  portENTER_CRITICAL_ISR(&MUX);
	stepperTimerITFlag = 1;
//  portEXIT_CRITICAL_ISR(&MUX);
}

/*
	Init the Stepper with timer1 and default IO
	@return Stepper
*/
Stepper::Stepper()
{
	timer1_attachInterrupt(StepperTimer_ISR);
	timer1_isr_init();
	/*
	80 -> prescale = 80 -> tần số timer0 = 80MHz/80 = 1MHz -> 1 tick = 1us
	0 -> interrupt at edge, 1 -> interrupt at level
	true -> đếm tăng lên, false = đếm giảm xuống
	*/
	timer1_enable(80, 0, true);

	/*
	time for 1 step, in milisecond (ms)
	param is number of ticks, and tick's duration is calculate on timer1_enable() function
	*/
	int timerThreshold = 60000/(mSpeed * STEPS_PER_ROUND * MINISTEPS_PER_STEP) - 1;
	timer1_write(timerThreshold * 1000);
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
  for(i = 0; i < 4; i++)
  {
	mPins[i] = *(pPinStepper+i);
    pinMode(mPins[i], OUTPUT);
  }
	timer1_attachInterrupt(StepperTimer_ISR);
	timer1_isr_init();
	/*
	80 -> prescale = 80 -> tần số timer0 = 80MHz/80 = 1MHz -> 1 tick = 1us
	0 -> interrupt at edge, 1 -> interrupt at level
	true -> đếm tăng lên, false = đếm giảm xuống
	*/
	timer1_enable(80, 0, true);
	
	/*
	time for 1 step, in milisecond (ms)
	param is number of ticks, and tick's duration is calculate on timer1_enable() function
	*/
	int timerThreshold = 60000/(mSpeed * STEPS_PER_ROUND * MINISTEPS_PER_STEP) - 1; //in ms
	timer1_write(timerThreshold * 1000);
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
  for(i = 0; i < 4; i++)
  {
	mPins[i] = *(pPinStepper+i);
    pinMode(mPins[i], OUTPUT);
  }
	timer1_attachInterrupt(StepperTimer_ISR);
	timer1_isr_init();
	/*
	80 -> prescale = 80 -> tần số timer0 = 80MHz/80 = 1MHz -> 1 tick = 1us
	0 -> interrupt at edge, 1 -> interrupt at level
	true -> đếm tăng lên, false = đếm giảm xuống
	*/
	timer1_enable(80, 0, true);
	
	/*
	time for 1 step, in milisecond (ms)
	param is number of ticks, and tick's duration is calculate on timer1_enable() function
	*/
	int timerThreshold = 60000/(speedStepper * STEPS_PER_ROUND * MINISTEPS_PER_STEP) - 1; //in ms
	timer1_write(timerThreshold * 1000);
	stepperTimerITFlag = 0;

	mSpeed = speedStepper;
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
	for(i = 0; i < 4; i++)
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
	if(stepperTimerITFlag)
	{
		/*
		if _dir = 1 (CCW) then current state = current state^7
		ex: 2^7 = 0010 ^ 0111 = 0101 = 5
		*/
		mCurrentPinState = dirStepper ? mCurrentPinState^7 : mCurrentPinState;
		for(i = 0; i < 4; i++)
		{
			digitalWrite(mPins[i], PIN_STATE[mCurrentPinState][i]);
		}
		mCurrentPinState = dirStepper ? (mCurrentPinState^7) + 1 : mCurrentPinState + 1;
		stepperTimerITFlag = 0;
	}

	if(mCurrentPinState == MINISTEPS_PER_STEP)	
	{
		mCurrentPosition = dirStepper ? mCurrentPosition - 1 : mCurrentPosition + 1;
		if(mCurrentPosition >= STEPS_PER_ROUND)	mCurrentPosition = 0;
		if(mCurrentPosition < 0)	mCurrentPosition = STEPS_PER_ROUND - 1;

		mCurrentPinState = 0;
		//Serial.println("___Finish 1 step___");
		return 1;	//finish 1 step
	}
	return 0;	//not finish yet
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
	int timerThreshold = 60000/(speedStepper * STEPS_PER_ROUND * MINISTEPS_PER_STEP) - 1; //in ms
	timer1_write(timerThreshold * 1000);
}


/*
	Run the stepper 1 round from current position with direction CW or CWW
	@param dirStepper 	<STEPPER_DIRECTION>: CW or CCW
	@return 						void
*/
void Stepper::RunOneRound(STEPPER_DIRECTION dirStepper)
{
	uint8_t i = 0;
	for( i = 0; i < STEPS_PER_ROUND; i++)
	{
		while(!RunOneStep(dirStepper));
		//Serial.print("__Steps = "); Serial.print(i); Serial.println("___");
		
	}
	Serial.println("___Finish 1 round___");
}

/*
	Run the stepper to the desired degree from current position, also update the position of the stepper
	@param degreeStepper	<uint8_t> desired degree, [0; 360]
	@param dirStepper 		<STEPPER_DIRECTION>: CW or CCW
	@return 							void
*/
void Stepper::RunToDegree(uint8_t degreeStepper, STEPPER_DIRECTION dirStepper)
{
	Serial.print("___Start position: "); Serial.println(mCurrentPosition);
	//l stands for local
	uint8_t l_position = degreeStepper * 64 / 360;
	if(l_position == 64)	l_position = 0;
	while(mCurrentPosition != l_position)
	{
		while(!RunOneStep(dirStepper));
	}
	Serial.print("___Finish position: "); Serial.println(mCurrentPosition);
}


