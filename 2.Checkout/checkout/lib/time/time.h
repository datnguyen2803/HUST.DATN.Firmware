#ifndef __TIME_H__
#define __TIME_H__

#include "time.h"
#include "crud_api.h"

class TimeClient
{
private:
	// URL API to get time from
	const char *API_URL = "http://worldtimeapi.org/api/timezone/Asia/Ho_Chi_Minh";
	unsigned int mYear, mMonth, mDate, mDay, mHour, mMinute, mSecond;
	String mGMT, mDateString, mTimeString;

public:
	enum INFO_CODE
	{
		YEAR = 0,
		MONTH = 1,
		DATE,
		DAY,
		HOUR,
		MINUTE,
		SECOND
	};
	enum DAY_OF_WEEK
	{
		SUNDAY = 0,
		MONDAY = 1,
		TUESDAY,
		WEDNESDAY,
		THURSDAY,
		FRIDAY,
		SATURDAY
	};

public:
	TimeClient();
	TimeClient(const TimeClient &tc);
	~TimeClient();
	bool Update();
	unsigned int GetInfo(uint8_t infoCodeTime);
	String GetGMT(void);
	String GetDateString(void);
	String GetTimeString(void);
	void PrintDetails(void);
};


#endif
