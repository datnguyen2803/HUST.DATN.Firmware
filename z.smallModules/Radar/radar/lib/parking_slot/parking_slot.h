#ifndef __PARKING_SLOT_H__
#define __PARKING_SLOT_H__

#include <Arduino.h>
#include "custom_gpio.h"
#include "parking_slot.h"

class ParkingSlot : public customGPIO
{
private:
	enum SLOT_STATE
	{
		STATE_EMPTY = 0,
		STATE_NOT_EMPTY = 1
	};
	uint8_t mArea;
	uint8_t mRow;
	uint8_t mCol;
public:
	enum AREA_CODE
	{
		AREA_A = 0,
		AREA_B = 1,
		AREA_C = 2, 
		AREA_D = 3
	};

private:
	void PutToDB(void);
public:
	//ParkingSlot();
	ParkingSlot(uint8_t pinGPIO, AREA_CODE areaSlot, uint8_t rowSlot, uint8_t colSlot);
	ParkingSlot(const ParkingSlot &ps);
	~ParkingSlot();
	void Update(void);

};

#endif
