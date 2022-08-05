#include "crud_api.h"

/*
GET method via HTTP
@param url const char* the url needed GET
@return <String> payload
*/
String HttpGet(const char* url)
{
	WiFiClient wifiClient;
	HTTPClient httpClient;
	// Your IP address with path or Domain name with URL path
	httpClient.begin(wifiClient, url);

	// Send HTTP POST request
	int httpResponseCode = httpClient.GET();

	String payload = "{}";

	if (httpResponseCode > 0)
	{
		Serial.print("HTTP Response code: ");
		Serial.println(httpResponseCode);
		payload = httpClient.getString();
	}
	else
	{
		Serial.print("Error code: ");
		Serial.println(httpResponseCode);
	}
	// Free resources
	httpClient.end();
	return payload;
}

/*
GET method via HTTPs
@param url const char* the url needed GET
@return <String> payload
*/
String HttpsGet(const char* url)
{
	std::unique_ptr<BearSSL::WiFiClientSecure> wifiClient(new BearSSL::WiFiClientSecure);
	HTTPClient httpClient;

	wifiClient->setInsecure();

	// Your IP address with path or Domain name with URL path
	httpClient.begin(*wifiClient, url);

	// Send HTTP POST request
	int httpResponseCode = httpClient.GET();

	String payload = "{}";

	if (httpResponseCode > 0)
	{
		Serial.print("HTTP Response code: ");
		Serial.println(httpResponseCode);
		payload = httpClient.getString();
	}
	else
	{
		Serial.print("Error code: ");
		Serial.println(httpResponseCode);
	}
	// Free resources
	httpClient.end();
	return payload;
}

/*
GET method, don't care the protocol
@param url const char* the url needed GET
@return <String> payload or "Protocol not support"
*/
String APIGet(const char* urlAPI)
{
	char urlTemp[100];
	strcpy(urlTemp, urlAPI);
	char *_protocolAPI = strtok(urlTemp, ":");

	if (!strcmp(_protocolAPI, "https"))
	{
		return HttpsGet(urlAPI);
	}
	if (!strcmp(_protocolAPI, "http"))
	{
		return HttpGet(urlAPI);
	}
	return "Protocol not support";
}