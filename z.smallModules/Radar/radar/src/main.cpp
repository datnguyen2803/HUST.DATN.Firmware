#include <Arduino.h>
#include "parking_slot.h"

#define SLOT_A1 LED_BUILTIN

ParkingSlot slotA1(SLOT_A1, ParkingSlot::AREA_A, 2, 4);

void setup() {
  Serial.begin(115200);
	delay(2000);
}	

void loop() {

	slotA1.Update();
	delay(3000);
}



