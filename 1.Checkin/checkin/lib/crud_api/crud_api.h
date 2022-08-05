#ifndef __CRUD_API_H__
#define __CRUD_API_H__

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Arduino_JSON.h>

#include "crud_api.h"

String HttpGet(const char* url);
String HttpsGet(const char* url);
String APIGet(const char* urlAPI);

#endif

