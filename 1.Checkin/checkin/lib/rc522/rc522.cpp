#include "rc522.h"


MFRC522 RC522;
MFRC522::MIFARE_Key key;
byte RC522_buffer[18];
byte RC522_bufferLen = 0;
bool RC522_PollingTimeoutFlag = 0;


/*
Init module RC522 with default SPI pins of ESP8266
@param void
@return void
*/
void RC522_Init(void)
{
	SPI.begin();        // Init SPI bus
	RC522.PCD_Init(); // Init MFRC522 card

	// Prepare the key (used both as key A)
	// using FFFFFFFFFFFFh which is the default at chip delivery from the factory
	for (byte i = 0; i < 6; i++) {
		key.keyByte[i] = 0xFF;
	}

	Serial.println(F("___Scan a MIFARE Classic PICC to demonstrate read and write___"));
	Serial.print(F("Using key: "));
	for (byte i = 0; i < 6; i++) 
	{
		memset(Serial_TxBuffer, 0, sizeof(Serial_TxBuffer));
		sprintf(Serial_TxBuffer, "%X", key.keyByte[i]);
		Serial.print(Serial_TxBuffer);
	}
	Serial.println();
	Serial.println(F("BEWARE: Data will be written to the PICC, from sector #1"));
}

/*
Polling for new card present, wait 20s
@param void
@return <bool> card present or not
*/
bool RC522_CardPolling()
{
	RC522_Polling_Timer_Init();
	// Reset the loop if no new card present on the sensor/reader.
	// Wait for 20s
	while(!RC522_PollingTimeoutFlag)
	{
		if ( RC522.PICC_IsNewCardPresent()) 
		{
			RC522_PollingTimeoutFlag = 0;
			RC522_Polling_Timer_DeInit();
				// Select one of the cards
			if ( ! RC522.PICC_ReadCardSerial()) 
			{
				return 0;
			}
			Serial.println(F("___Card Detected___"));
			RC522.PICC_DumpDetailsToSerial(&(RC522.uid)); //dump some details about the card
			return 1;
		}
		yield();
	}
	RC522_PollingTimeoutFlag = 0;
	return 0;
}

/*
Read bytes from card
@param blockAddr <byte> address of block
@param pBuffer <byte*> pointer to buffer contain info read from card
@param bufferLen <byte> number of bytes want to read from card
@return void
*/
void RC522_ReadFromCard(byte blockAddr, byte *pBuffer, byte bufferLen)
{
	bufferLen = 18;
	MFRC522::StatusCode status;

	Serial.print("___Authenticating for Reading...");
	status = RC522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddr, &key, &(RC522.uid)); //line 834 of MFRC522.cpp file
	if (status != MFRC522::STATUS_OK) 
	{
		Serial.print(F("...failed: "));
		Serial.println(RC522.GetStatusCodeName(status));
		return;
	}
	Serial.println("Success!");

	status = RC522.MIFARE_Read(blockAddr, pBuffer, &bufferLen);
	if (status != MFRC522::STATUS_OK) 
	{
		Serial.print(F("Reading failed: "));
		Serial.println(RC522.GetStatusCodeName(status));
		return;
	}
	sprintf(Serial_TxBuffer, "Finish reading. Info in block 0x%X: ", blockAddr);
	Serial.print(Serial_TxBuffer);
	for (byte i = 0; i < 16; i++)
	{
		Serial.print(pBuffer[i]);
		Serial.print(" ");
	}
	Serial.println();
}

/*
Write to card
@param blockAddr <byte> address of block
@param pBuffer <byte*> pointer to buffer contain info write to card
@param bufferLen <byte> number of bytes want to write to card
@return void
*/
void RC522_WriteToCard(byte blockAddr, byte *pBuffer, byte bufferLen)
{
	bufferLen = 16;
	MFRC522::StatusCode status;

	Serial.print("___Authenticating for Writing...");
	status = RC522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, blockAddr, &key, &(RC522.uid)); //line 834 of MFRC522.cpp file
	if (status != MFRC522::STATUS_OK) 
	{
		Serial.print(F("failed: "));
		Serial.println(RC522.GetStatusCodeName(status));
		return;
	}
	Serial.println("Success!");

	status = RC522.MIFARE_Write(blockAddr, pBuffer, bufferLen);
	if (status != MFRC522::STATUS_OK) 
	{
		Serial.print(F("Writing failed: "));
		Serial.println(RC522.GetStatusCodeName(status));
		return;
	}
	sprintf(Serial_TxBuffer, "Finish writing into block 0x%X.", blockAddr);
	Serial.println(Serial_TxBuffer);
}

/*
Stop RC522 module after a transaction
@param void
@return void 
*/
void RC522_Stop(void)
{
	RC522.PICC_HaltA();
	RC522.PCD_StopCrypto1();
}

/*
ISR for polling timer1
*/
void IRAM_ATTR RC522_Polling_ISR()
{
  //portENTER_CRITICAL_ISR(&MUX);
	RC522_PollingTimeoutFlag = 1;
  //portEXIT_CRITICAL_ISR(&MUX);
}

/*
Init timer 1 to help RC522_CardPolling(), make a 20s wait
@return void
*/
void RC522_Polling_Timer_Init()
{
	timer1_attachInterrupt(RC522_Polling_ISR);
	timer1_isr_init();
	/*
	312500 -> cứ khi timer tăng đến 312500 tick = 1s thì xảy ra ngắt
	*/
	timer1_write(RC522_POLLING_TIMEOUT * 312500);
	/*
	0x02 -> prescale = 256 -> tần số timer1 = 80MHz/256 = 312.5kHz -> 1 tick = 3.2us
	0 -> edge interrupt, 1 -> level interrupt
	true -> autoreset, false = not autoreset
	*/
	timer1_enable(0x02, 0, false);
}

/*
Deinit timer1
*/
void RC522_Polling_Timer_DeInit(void)
{
	timer1_detachInterrupt();
  	timer1_disable();
}


