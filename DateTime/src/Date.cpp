﻿#include "../include/Date.hpp"

CDate::CDate()
{
	m_lSeconds = 1329235200; // first day to work, 2012-02-15 08:30:00
}

CDate::CDate(unsigned int iYear, unsigned int iMon, unsigned int iDay)
{
	tm DateTime;

	DateTime.tm_year = iYear - 1900;
	DateTime.tm_mon = iMon - 1;
	DateTime.tm_mday = iDay;
	DateTime.tm_hour = 0;
	DateTime.tm_min = 0;
	DateTime.tm_sec = 0;
	DateTime.tm_wday = 0;
	DateTime.tm_yday = 0;
	DateTime.tm_isdst = 0;

	m_lSeconds = mktime(&DateTime);
}

CDate::CDate(const std::string &szDate)
{
	CopyFromDate(szDate);
}

CDate::CDate(time_t lSeconds)
{
	m_lSeconds = lSeconds;
	truncateTime(m_lSeconds);
}

CDate::CDate(const CDateTime &DateTime)
{
	CopyFromDateTime(DateTime);
}

CDate::CDate(const CDate &Other)
{
	m_lSeconds = Other.m_lSeconds;
}

CDate::~CDate()
{
}

void CDate::InitCurrDate(time_t lSecond)
{
	InitCurrDateTime(lSecond);
	truncateTime(m_lSeconds);
}

void CDate::CopyFromDate(const std::string &szDate)
{
	//最少10个字节，"2019-03-23"
	if (szDate.length() < 10)
		return;

	tm DateTime;

	char* pszStr = (char*)szDate.c_str();
	DateTime.tm_year = std::atoi(pszStr);
	DateTime.tm_mon = std::atoi(pszStr + 5);
	DateTime.tm_mday = std::atoi(pszStr + 8);
	DateTime.tm_hour = 0;
	DateTime.tm_min = 0;
	DateTime.tm_sec = 0;

	DateTime.tm_year -= 1900;
	DateTime.tm_mon -= 1;
	DateTime.tm_wday = 0;
	DateTime.tm_yday = 0;
	DateTime.tm_isdst = 0;

	m_lSeconds = mktime(&DateTime);
}

void CDate::CopyFromDateTime(const CDateTime &Other)
{
	m_lSeconds = Other.m_lSeconds;
	truncateTime(m_lSeconds);
}

void CDate::truncateTime(time_t &tDateTime)
{
	struct tm tmTimeTemp;

	LocalTime(m_lSeconds, tmTimeTemp);

	tmTimeTemp.tm_hour = 0;
	tmTimeTemp.tm_min = 0;
	tmTimeTemp.tm_sec = 0;

	tDateTime = mktime(&tmTimeTemp);
}

CDate &CDate::operator=(const CDate &Other)
{
	m_lSeconds = Other.m_lSeconds;
	return *this;
}

CDate &CDate::operator=(const std::string &szDate)
{
	CopyFromDate(szDate);
	return *this;
}

CDate &CDate::operator=(time_t lSeconds)
{
	m_lSeconds = lSeconds;
	truncateTime(m_lSeconds);
	return *this;
}

CDate &CDate::operator=(const CDateTime &DateTime)
{
	CopyFromDateTime(DateTime);
	return *this;
}

CDate::operator std::string() const
{
	char strDate[16] = { '\0' };
	struct tm tmTimeTemp;

	LocalTime(m_lSeconds, tmTimeTemp);
	strftime(strDate, 12, "%Y-%m-%d", &tmTimeTemp);

	return std::move(std::string(strDate));
}

std::ostream& operator<<(std::ostream &out, const CDate& Date)
{
	out << (std::string)Date;
	return out;
}