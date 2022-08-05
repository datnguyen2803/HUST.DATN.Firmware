#include <Arduino.h>
#include "Stepper_28byj48.h"


/*
	@enum BARRIER_STATE
	Value for barrier degree (open/close) 
*/
enum BARRIER_STATE
{
	BARRIER_OPEN	= 90,	//open degree
	BARRIER_CLOSE	= 0		//close degree
};

Stepper Barrier = Stepper();

void setup() 
{
	Serial.begin(115200);
  delay(3000);
	Barrier.SetSpeed(16);
	Barrier.RunToDegree(90, Stepper::STEPPER_CW);
	Serial.println("Open checkin barrier");
	delay(5000);
	Barrier.RunToDegree(0, Stepper::STEPPER_CCW);
	Serial.println("Close checkin barrier");
	delay(5000);
	// Barrier.RunToDegree(180, Stepper::STEPPER_CW);
	// delay(5000);
	// Barrier.RunToDegree(270, Stepper::STEPPER_CCW);
	// delay(5000);
	// Barrier.RunToDegree(360, Stepper::STEPPER_CW);
	// delay(5000);	
	// Serial.println("start run 1 round");
	// Barrier.RunOneRound(Stepper::STEPPER_CCW);
	// delay(3000);
	// Barrier.RunOneRound(Stepper::STEPPER_CCW);
	// delay(3000);
}
 
void loop() 
{
	Barrier.RunOneRound(Stepper::STEPPER_CW);
}
