#include "time.h"

/*
Constructor of a TimeClient
*/
TimeClient::TimeClient()
{
	mYear = 0;
	mMonth = 0;
	mDate = 0;
	mDay = 0;
	mHour = 0;
	mMinute = 0;
	mSecond = 0;
	mGMT = "0";
}

/*
Copy constructor of a TimeClient
*/
TimeClient::TimeClient(const TimeClient &tc)
{
	mYear = tc.mYear;
	mMonth = tc.mMonth;
	mDate = tc.mDate;
	mDay = tc.mDay;
	mHour = tc.mHour;
	mMinute = tc.mMinute;
	mSecond = tc.mSecond;
	mGMT = tc.mGMT;
}

/*
Destructor of a Time Client
*/
TimeClient::~TimeClient()
{
	mYear = 0;
	mMonth = 0;
	mDate = 0;
	mDay = 0;
	mHour = 0;
	mMinute = 0;
	mSecond = 0;
	mGMT = "0";
}

/*
Update time from API URL
@param none
@return <bool> success or not
*/
bool TimeClient::Update()
{

	//do method GET to API
	String httpBuffer = APIGet(API_URL);
	JSONVar jsonBuffer = JSON.parse(httpBuffer);

	//retry 100000 times if fail
	int timeTry = 0;
	while(JSON.typeof(jsonBuffer) == "undefined")
	{
		timeTry++;
		//do method GET to API
		httpBuffer = APIGet(API_URL);
		jsonBuffer = JSON.parse(httpBuffer);
		if(timeTry >= 100000){
			Serial.println("ERROR: Fail to update time.");
			return 0;
		}
	}

	// myObject.keys() can be used to get an array of all the keys in the object
	JSONVar keys = jsonBuffer.keys();

	for (int i = 0; i < keys.length(); i++)
	{
		//get the GMT
		if (JSON.stringify(keys[i]) == "\"abbreviation\"")
		{
			JSONVar value = jsonBuffer[keys[i]];
			String tempValue = JSON.stringify(value);
			String valueString = tempValue.substring(1, tempValue.length() - 1);

			mGMT = valueString;
		}

		//get the date, time
		if (JSON.stringify(keys[i]) == "\"datetime\"")
		{
			JSONVar value = jsonBuffer[keys[i]];
			String tempValue = JSON.stringify(value);
			String valueString = tempValue.substring(1, tempValue.length() - 1);

			int specPos1 = valueString.indexOf('T');
			int specPos2 = valueString.indexOf('.');
			String dateString = valueString.substring(0, specPos1);
			mDateString = dateString;
			String timeString = valueString.substring(specPos1 + 1, specPos2);
			mTimeString = timeString;

			mYear = dateString.substring(0, dateString.indexOf('-')).toInt();
			mMonth = dateString.substring(dateString.indexOf('-') + 1, dateString.lastIndexOf('-')).toInt();
			mDate = dateString.substring(dateString.lastIndexOf('-') + 1, dateString.length()).toInt();

			mHour = timeString.substring(0, timeString.indexOf(':')).toInt();
			mMinute = timeString.substring(timeString.indexOf(':') + 1, timeString.lastIndexOf(':')).toInt();
			mSecond = timeString.substring(timeString.lastIndexOf(':') + 1, timeString.length()).toInt();
		}

		//get the day of week
		if (JSON.stringify(keys[i]) == "\"day_of_week\"")
		{
			JSONVar value = jsonBuffer[keys[i]];
			String tempValue = JSON.stringify(value);
			String valueString = tempValue.substring(1, tempValue.length() - 1);

			mDay = valueString.toInt();
		}
	}
	return 1;
}


/*
Get info of current time, including YEAR, MONTH, DATE, DAY, OUR, MINUTE, SECOND
@param infoCodeTime <uint8_t> the code, show in TimeClient::INFO_CODE
@return <unsigned int> time info
*/
unsigned int TimeClient::GetInfo(uint8_t infoCodeTime)
{
	switch (infoCodeTime)
	{
	case YEAR:
		return mYear;
		break;
	case MONTH:
		return mMonth;
		break;
	case DATE:
		return mDate;
		break;
	case DAY:
		return mDay;
		break;
	case HOUR:
		return mHour;
		break;
	case MINUTE:
		return mMinute;
		break;
	case SECOND:
		return mSecond;
		break;
	default:
		return -1;
	}
}

/*
Get info GMT
@param void
@return <String> GMT
*/
String TimeClient::GetGMT()
{
	return mGMT;
}

/*
Get string of full Date info
@param void
@return <String> Date string
*/
String TimeClient::GetDateString()
{
	return mDateString;
}

/*
Get string of full Time info
@param void
@return <String> Time string
*/
String TimeClient::GetTimeString()
{
	return mTimeString;
}

/*
Print all info of the time
@param void
@return void
*/
void TimeClient::PrintDetails(void)
{
	Serial.printf("Current time: %s %s GMT%s\n", mDateString, mTimeString, mGMT);
}

