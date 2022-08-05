#include "custom_gpio.h"
#include "stepper_28byj48.h"
#include "custom_serial.h"
#include "time.h"

#define LASER_IN_PIN 4
#define LASER_OUT_PIN 5
#define TRUE_PLATE "68228"

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
uint8_t balanceOfAccount = 100;


customGPIO laserIn(LASER_IN_PIN, INPUT);
customGPIO laserOut(LASER_OUT_PIN, INPUT);
TimeClient tc;
Stepper barrier(stepperPins);

bool CarCome(void);
bool CarLeave(void);
void CameraOn(void);
void CameraOff(void);
String CameraReadPlate(void);
void PostToDB(String plate);
void BarrierOpen(void);
void BarrierClose(void);
bool CheckPlate(String plate);
bool PayParkingFee(void);
bool CheckAppPay(void);
void CallSecurity(void);

void setup()
{
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
	Serial.println("success!");
}

void loop()
{
	int checktime = 0;
	bool checkFlag = 0;
	// put your main code here, to run repeatedly:
	Serial.print("Waiting for cumming command: ");
	while (!CarCome());
	CameraOn();
	String plate;

	Serial.print("Detecting plate...");
	while(checktime < 3)
	{
		checktime++;
		plate = CameraReadPlate();
		if(CheckPlate(plate))
		{
			checkFlag = 1;
			break;
		}
		delay(1000);
	}
	CameraOff();
	if(checkFlag)	Serial.println("success!");

	else
	{
		Serial.println("<ERROR> fail! No plates found in DB. Wait for security for solution.");
		CallSecurity();
		return;
	}

	checkFlag = 0;
	checktime = 0;
	Serial.print("Paying the parking fee...");
	while(checktime < 3)
	{
		checktime++;
		if(PayParkingFee())
		{
			checkFlag = 1;
			break;
		}
		delay(1000);
	}
	if(checkFlag)	Serial.println("success!");
	else
	{
		Serial.println("<ERROR> failed. Not enough money. Please deposit and retry. Thank you!");
		Serial.print("Waiting for paying command: ");
		while (!CheckAppPay());
		//return;
	}

	PostToDB(plate);
	BarrierOpen();
	Serial.print("Waiting for leaving command: ");
	while (!CarLeave());
	BarrierClose();

	Serial.println("====================");
	delay(5000);
}

bool CarCome(void)
{
	memset(Serial_RxBuffer, 0, sizeof(Serial_RxBuffer));
	if (Serial.available())
	{
		Serial_Rx(Serial_RxBuffer);
		// Serial.println(Serial_RxBuffer);
	}
	if (strcmp(Serial_RxBuffer, "c1") == 0)
	{
		Serial.printf("%s. Car came.\n", Serial_RxBuffer);
		return 1;
	}
	return 0;
	// return laserIn.Read();
}

bool CarLeave(void)
{
	memset(Serial_RxBuffer, 0, sizeof(Serial_RxBuffer));
	if (Serial.available())
	{
		Serial_Rx(Serial_RxBuffer);
		// Serial.println(Serial_RxBuffer);
	}
	if (strcmp(Serial_RxBuffer, "c2") == 0)
	{
		Serial.printf("%s. Car left.\n", Serial_RxBuffer);
		return 1;
	}
	return 0;
	// return laserOut.Read();
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

void PostToDB(String plateCar)
{
	tc.Update();
	String _date = tc.GetDateString();
	String _time = tc.GetTimeString();
	//String _plate = CameraReadPlate();
	Serial.printf("Post to DB: %s %s %s\n", _date, _time, plateCar);
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

bool CheckPlate(String plateCar)
{
	//return 0 or 1 randomly
	String _randomPlate = String(68226 + rand() % (68229 + 1 - 68226));
	bool _res = _randomPlate == TRUE_PLATE ? 1 : 0;
	Serial.printf("%d ", _res);
	return _res;
}

/*
automatic pay online via API
*/
bool PayParkingFee(void)
{
	//return 0 or 1 randomly
	int _fee = 1 + rand() % (120 + 1 - 1);
	bool _res = 0;
	if(balanceOfAccount >= _fee)
	{
		balanceOfAccount -= _fee;
		_res = 1;
	}
	Serial.printf("%d ", _res);
	return _res;
}

bool CheckAppPay(void)
{
	memset(Serial_RxBuffer, 0, sizeof(Serial_RxBuffer));
	if (Serial.available())
	{
		Serial_Rx(Serial_RxBuffer);
		// Serial.println(Serial_RxBuffer);
	}
	if (strcmp(Serial_RxBuffer, "paid") == 0)
	{
		Serial.printf("%s. Pay success.\n", Serial_RxBuffer);
		return 1;
	}
	return 0;
	// return laserIn.Read();
}

void CallSecurity(void)
{
	Serial.println("Calling security");
}

