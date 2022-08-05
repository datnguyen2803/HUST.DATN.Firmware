#include "crud_api.h"

#define URL_INVALID	-9

/**
 * GET method via HTTP
 * @param url const char* the url needed GET
 * @return <String> payload
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
		// Serial.print("HTTP Response code: ");
		// Serial.println(httpResponseCode);
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

/**
 * GET method via HTTPs
 * @param url const char* the url needed GET
 * @return <String> payload
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
		// Serial.print("HTTP Response code: ");
		// Serial.println(httpResponseCode);
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

/**
 * RECOMMEND
 * GET method, don't care the protocol
 * @param url const char* the url needed GET
 * @return <String> payload or "Protocol not support"
*/
String APIGet(const char* urlAPI)
{
	char _urlTemp[200];
	strcpy(_urlTemp, urlAPI);
	char *_protocolAPI = strtok(_urlTemp, ":");

	if (!strcmp(_protocolAPI, "https"))
	{
		return HttpsGet(urlAPI);
	}
	if (!strcmp(_protocolAPI, "http"))
	{
		return HttpGet(urlAPI);
	}
	return "URL_INVALID";
}

/**
 * POST method via HTTP
 * @param url const char* the url needed POST
 * @param bodyRequest const char* the object to post
 * @return httpResponse to POST
*/
int HttpPost(const char* url, const char* bodyRequest)
{
	WiFiClient wifiClient;
	HTTPClient httpClient;
	// Your IP address with path or Domain name with URL path
	httpClient.begin(wifiClient, url);

	httpClient.addHeader("Content-Type", "application/json");
	int httpResponseCode = httpClient.POST(bodyRequest);

	// Free resources
	httpClient.end();

	return httpResponseCode;
}

/**
 * POST method via HTTPs
 * @param url const char* the url needed POST
 * @param bodyRequest const char* the object to post
 * @return httpResponse to POST
*/
int HttpsPost(const char* url, const char* bodyRequest)
{

	std::unique_ptr<BearSSL::WiFiClientSecure> wifiClient(new BearSSL::WiFiClientSecure);
	HTTPClient httpClient;

	wifiClient->setInsecure();
	// Your IP address with path or Domain name with URL path
	httpClient.begin(*wifiClient, url);

	httpClient.addHeader("Content-Type", "application/json");
	int httpResponseCode = httpClient.POST(bodyRequest);

	// Free resources
	httpClient.end();

	return httpResponseCode;	
}

/**
 * RECOMMEND
 * POST method, don't care the protocol
 * @param url const char* the url needed GET
 * @param bodyRequest const char* the object to post
 * @return httpResponse to POST or URL_INVALID (= -9)
*/
int APIPost(const char* urlAPI, const char* bodyRequest)
{
	char _urlTemp[200];
	strcpy(_urlTemp, urlAPI);
	char *_protocolAPI = strtok(_urlTemp, ":");

	if (!strcmp(_protocolAPI, "https"))
	{
		return HttpsPost(urlAPI, bodyRequest);
	}
	if (!strcmp(_protocolAPI, "http"))
	{
		return HttpPost(urlAPI, bodyRequest);
	}
	return URL_INVALID;
}

/**
 * PATCH method via HTTP
 * @param url const char* the url needed PATCH
 * @param index int order of object in DB
 * @param bodyRequest const char* body of the PATCH request
 * @return httpResponse to PATCH
*/
int HttpPatch(const char* url, int index, const char* bodyRequest)
{
	char _urlTemp[200];
	char _charIndex[10];
	sprintf(_charIndex, "/%d", index);
	strcpy(_urlTemp, url);
	strcat(_urlTemp, _charIndex);
	//Serial.println(_urlTemp);

	WiFiClient wifiClient;
	HTTPClient httpClient;
	// Your IP address with path or Domain name with URL path
	httpClient.begin(wifiClient, _urlTemp);


	httpClient.addHeader("Content-Type", "application/json");
	int httpResponseCode = httpClient.PATCH(bodyRequest);

	// Free resources
	httpClient.end();

	return httpResponseCode;
}

/**
* PATCH method via HTTPs
* @param url const char* the url needed PATCH
* @param index int order of object in DB
* @param bodyRequest const char* body of the PATCH request
* @return httpResponse to PATCH
*/
int HttpsPatch(const char* url, int index, const char* bodyRequest)
{
	char _urlTemp[200];
	char _charIndex[10];
	sprintf(_charIndex, "/%d", index);
	strcpy(_urlTemp, url);
	strcat(_urlTemp, _charIndex);
	//Serial.println(_urlTemp);

	std::unique_ptr<BearSSL::WiFiClientSecure> wifiClient(new BearSSL::WiFiClientSecure);
	HTTPClient httpClient;

	wifiClient->setInsecure();
	// Your IP address with path or Domain name with URL path
	httpClient.begin(*wifiClient, _urlTemp);

	httpClient.addHeader("Content-Type", "application/json");
	int httpResponseCode = httpClient.PATCH(bodyRequest);

	// Free resources
	httpClient.end();

	return httpResponseCode;
}

/**
 * RECOMMEND
 * PATCH method, don't care protocol
 * @param url const char* the url needed PATCH
 * @param index int order of object in DB
 * @param bodyRequest const char* body of the PATCH request
 * @return httpResponse to PATCH or URL_INVALID (= -9)
*/
int APIPatch(const char* urlAPI, int index, const char* bodyRequest)
{
	char _urlTemp[200];
	strcpy(_urlTemp, urlAPI);
	char* _protocolAPI = strtok(_urlTemp, ":");

	if (!strcmp(_protocolAPI, "https"))
	{
		return HttpsPatch(urlAPI, index, bodyRequest);
	}
	if (!strcmp(_protocolAPI, "http"))
	{
		return HttpPatch(urlAPI, index, bodyRequest);
	}
	return URL_INVALID;
}

/**
* DELETE method via HTTP
* @param url const char* the url
* @param index int order of object in DB
* @return httpResponse to DELETE
*/
int HttpDel(const char* url, int index)
{
	char _urlTemp[200];
	char _charIndex[10];
	sprintf(_charIndex, "/%d", index);
	strcpy(_urlTemp, url);
	strcat(_urlTemp, _charIndex);
	//Serial.println(_urlTemp);

	WiFiClient wifiClient;
	HTTPClient httpClient;
	// Your IP address with path or Domain name with URL path
	httpClient.begin(wifiClient, _urlTemp);

	int httpResponseCode = httpClient.sendRequest("DELETE");
	return httpResponseCode;
}

/**
* DELETE method via HTTPs
* @param url const char* the url
* @param index int order of object in DB
* @return httpResponse to DELETE
*/
int HttpsDel(const char* url, int index)
{
	char _urlTemp[200];
	char _charIndex[10];
	sprintf(_charIndex, "/%d", index);
	strcpy(_urlTemp, url);
	strcat(_urlTemp, _charIndex);
	//Serial.println(_urlTemp);

	std::unique_ptr<BearSSL::WiFiClientSecure> wifiClient(new BearSSL::WiFiClientSecure);
	HTTPClient httpClient;

	wifiClient->setInsecure();
	// Your IP address with path or Domain name with URL path
	httpClient.begin(*wifiClient, _urlTemp);

	int httpResponseCode = httpClient.sendRequest("DELETE");
	return httpResponseCode;
}

/**
* RECOMMEND 
* DELETE method, don't care the protocol
* @param url const char* the url
* @param index int order of object in DB
* @return httpResponse to DELETE or URL_INVALID (= -9)
*/
int APIDel(const char* urlAPI, int index)
{
	char _urlTemp[200];
	strcpy(_urlTemp, urlAPI);
	char* _protocolAPI = strtok(_urlTemp, ":");

	if (!strcmp(_protocolAPI, "https"))
	{
		return HttpsDel(urlAPI, index);
	}
	if (!strcmp(_protocolAPI, "http"))
	{
		return HttpDel(urlAPI, index);
	}
	return URL_INVALID;
}


