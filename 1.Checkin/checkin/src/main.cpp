#include "custom_gpio.h"
#include "stepper_28byj48.h"
#include "custom_serial.h"
#include "time.h"

#define LASER_IN_PIN  4
#define LASER_OUT_PIN 5

enum STEPPER_PINS
{
  STEPPER_PIN1 = 0,
  STEPPER_PIN2,
  STEPPER_PIN3,
  STEPPER_PIN4
};
uint8_t stepperPins[4] = {STEPPER_PIN1, STEPPER_PIN2, STEPPER_PIN3, STEPPER_PIN4};

const char *ssid = "DATCHAOS";
const char *password = "88888888";

customGPIO laserIn(LASER_IN_PIN, INPUT);
customGPIO laserOut(LASER_OUT_PIN, INPUT);
TimeClient tc;
Stepper barrier(stepperPins);

bool CarCome(void);
bool CarLeave(void);
void CameraOn(void);
void CameraOff(void);
String CameraReadPlate(void);
void PostToDB(void);
void BarrierOpen(void);
void BarrierClose(void);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
	delay(5000);

	WiFi.begin(ssid, password);
	Serial.print("Connecting to WiFi");
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		Serial.print(".");
	}
	Serial.println("Success!");

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Waiting for command: ");
	while(!CarCome());
	CameraOn();
	PostToDB();
	CameraOff();
	BarrierOpen();
	  Serial.print("Waiting for command: ");
	while(!CarLeave());
	BarrierClose();

	Serial.println("====================");
	delay(5000);
}

bool CarCome(void)
{
	if(Serial.available())
	{
		Serial_Rx(Serial_RxBuffer);
		//Serial.println(Serial_RxBuffer);
	}
	if(strcmp(Serial_RxBuffer, "c1") == 0)
	{
		Serial.printf("%s. Car came.\n", Serial_RxBuffer);
		return 1;
	}
	return 0;
	//return laserIn.Read();
}

bool CarLeave(void)
{
	if(Serial.available())
	{
		Serial_Rx(Serial_RxBuffer);
		//Serial.println(Serial_RxBuffer);
	}
	if(strcmp(Serial_RxBuffer, "c2") == 0)
	{
		Serial.printf("%s. Car left.\n", Serial_RxBuffer);
		return 1;
	}
	return 0;
	//return laserOut.Read();
}

void CameraOn(void)
{
	Serial.println("Camera is on.");
}

void CameraOff(void)
{
	Serial.println("Camera is off.");
}

String CameraReadPlate(void)
{
	String _plate = "29T1-68228";
	return _plate;
}

void PostToDB(void)
{
	tc.Update();
	String _date = tc.GetDateString();
	String _time = tc.GetTimeString();
	String _plate = CameraReadPlate();
	Serial.printf("Post to DB: %s %s %s\n", _date, _time, _plate);
}

void BarrierOpen(void)
{
	barrier.RunToDegree(90, Stepper::STEPPER_CW);
	Serial.println("Barrier is opened");
}

void BarrierClose(void)
{
	barrier.RunToDegree(0, Stepper::STEPPER_CCW);
	Serial.println("Barrier is closed");
}

