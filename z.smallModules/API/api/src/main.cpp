#include "crud_api.h"

const char *ssid = "DATCHAOS";
const char *password = "88888888";
//char url[] = "https://catfact.ninja/fact";
// char url[] = "http://jsonplaceholder.typicode.com/users/1";
char url_api_test[] = "https://my-json-server.typicode.com/datnguyen2803/datn_testapi/carIn";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
// unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

String httpBuffer;

void setup()
{
	Serial.begin(115200);
	delay(5000);
	Serial.print("Connecting...");
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(1000);
		Serial.print(".");
	}
	Serial.println();
}

void loop()
{
	// char urlTemp[100];
	// memcpy(urlTemp, url, sizeof(url));
	// char *api_protocol = strtok(urlTemp, ":");

	// Send an HTTP POST request depending on timerDelay
	if ((millis() - lastTime) > timerDelay)
	{
		// Check WiFi connection status
		if (WiFi.status() == WL_CONNECTED)
		{
			//Serial.println("Getting from DB...");
			// httpBuffer = APIGet(url_api_test);
			// JSONVar jsonBuffer = JSON.parse(httpBuffer);
			// int j = 0;
			// int jsonLength = jsonBuffer.length();
			// for(j = 0; j < jsonLength; j++)
			// {
			// 	// JSON.typeof(jsonVar) can be used to get the type of the var
			// 	if (JSON.typeof(jsonBuffer[j]) == "undefined")
			// 	{
			// 		Serial.println("Parsing input failed!");
			// 		lastTime = millis();
			// 		return;
			// 	}
			// 	// myObject.keys() can be used to get an array of all the keys in the object
			// 	JSONVar keys = jsonBuffer[j].keys();

			// 	for (int i = 0; i < keys.length(); i++)
			// 	{
			// 		JSONVar value = jsonBuffer[j][keys[i]];
			// 		Serial.print(keys[i]);
			// 		Serial.print(" = ");
			// 		Serial.println(value);
			// 	}
			// 	Serial.println("--------------------");
			// }

			// Serial.println("Posting to DB...");
			// int id = 4;
			// char* plate = "45654";
			// char object [100];
			// sprintf(object, "{\"id\":%d,\"plate\":%s}", id, plate);
			// Serial.print("HTTP Response code: ");
			// Serial.println(APIPost(url_api_test, object));
			// Serial.println("--------------------");


			// Serial.println("Patching to DB...");
			// int id = 2;
			// char* plate = "45654";
			// char body[100];
			// sprintf(body, "{\"plate\":\"%s\"}", plate);
			// Serial.printf("HTTP Response code: %d\n", APIPatch(url_api_test, id, body));
			// Serial.println("--------------------");

			Serial.println("Deleting in DB...");
			Serial.printf("HTTP Response code: %d\n", APIDel(url_api_test, 2));
			Serial.println("--------------------");

		}
		else
		{
			Serial.println("WiFi Disconnected");
		}
		lastTime = millis();
	}
}
