#ifndef __CRUD_API_H__
#define __CRUD_API_H__

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Arduino_JSON.h>

#include "crud_api.h"

/*
Difference between PUT and PATCH
- PUT update the object, the fields not included will be null
- PATCH update the object, only change the fields included, the fields not included will not be changed
Ex: {"id": 1, "name": "dat", "age": 23}
-> PUT "name": "chaos" -> new object = {"name": "chaos"}
-> PATCH "name": "chaos" -> new object = {"id": 1, "name": "chaos", "age": 23}

-> in this library, we use PATCH the most, not PUT
*/

String HttpGet(const char* url);
String HttpsGet(const char* url);
String APIGet(const char* urlAPI);

int HttpPost(const char* url, const char* bodyRequest);
int HttpsPost(const char* url, const char* bodyRequest);
int APIPost(const char* urlAPI, const char* bodyRequest);

int HttpPatch(const char* url, int index, const char* bodyRequest);
int HttpsPatch(const char* url, int index, const char* bodyRequest);
int APIPatch(const char* urlAPI, int index, const char* bodyRequest);

int HttpDel(const char* url, int index);
int HttpsDel(const char* url, int index);
int APIDel(const char* urlAPI, int index);

#endif

