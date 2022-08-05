#include "parking_slot.h"

ParkingSlot::ParkingSlot(uint8_t pinGPIO, AREA_CODE areaSlot, uint8_t rowSlot, uint8_t colSlot) : customGPIO(pinGPIO, INPUT)
{
	mArea = areaSlot;
	mRow = rowSlot;
	mCol = colSlot;
}

ParkingSlot::ParkingSlot(const ParkingSlot &ps) : customGPIO(ps)
{
	mArea = ps.mArea;
	mRow = ps.mRow;
	mCol = ps.mCol;
}

ParkingSlot::~ParkingSlot()
{
	mArea = AREA_A;
	mRow = 0;
	mCol = 0;
}

//info need to put: mArea, mRow, mCol, mState
void ParkingSlot :: PutToDB(void)
{
	Serial.printf("___Info___\nArea: %d, Row: %d, Col: %d, State: %d ", mArea, mRow, mCol, mState);
	Serial.println("Putting to DB... <SUCCESS>");
}

void ParkingSlot :: Update(void)
{
	bool checkState = 0;

	//checkState = this->Read();
	checkState = !mState;
	if(checkState == mState) return;
	delay(3000);

	//checkState = this->Read();
	checkState = !mState;
	if(checkState == mState) return;
	
	mState = checkState;
	PutToDB();
}

