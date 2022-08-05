//#include <NTPClient.h>
// change next line to use with another board/shield
//#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
//#include <WiFiUdp.h>


#include "crud_api.h"
#include "time.h"

const char *ssid = "DATCHAOS";
const char *password = "88888888";

TimeClient tc;

void setup()
{
	Serial.begin(115200);
	delay(5000);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		Serial.print(".");
	}
	// timeClient.begin();
}

void loop()
{
	tc.Update();
	tc.PrintDetails();
	delay(10000);
}