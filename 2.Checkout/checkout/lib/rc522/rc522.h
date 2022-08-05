#ifndef __RC522_H__
#define __RC522_H__

#include <Arduino.h>
#include "MFRC522.h"
#include "string.h"
#include "rc522.h"
#include "custom_serial.h"

#define RC522_POLLING_TIMEOUT	20

extern MFRC522 RC522;
extern MFRC522::MIFARE_Key key;
extern byte RC522_buffer[18];
extern byte RC522_bufferLen;
extern bool RC522_PollingTimeoutFlag;

void RC522_Init(void);
bool RC522_CardPolling(void);
void RC522_ReadFromCard(byte blockAddr, byte *pBuffer, byte bufferLen);
void RC522_WriteToCard(byte blockAddr, byte *pBuffer, byte bufferLen);
void RC522_Stop(void);

//support functions for RC522_CardPolling()
void IRAM_ATTR RC522_Polling_ISR();
void RC522_Polling_Timer_Init(void);
void RC522_Polling_Timer_DeInit(void);

#endif