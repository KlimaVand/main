
#ifndef BSTIME_H
#define BSTIME_H

#include "common.h"
#include "../tools/convert.h"

enum monthSymbol {
    January, February, March, April, May, June, July, August, September, October, November, December
};

class bsTime {
    private:
        int     day;
        int     month;
        int     year;
        int     dayInYear;
        int     dayInWeek;
        long    longTime;
        int     tabDaysPerMonth[2][12];
        long    AccumulatedDaysSince1900[500];
        convert conv;

        void InitDaysPerMonth(void);

    public:
        bsTime();

        bsTime(int d,
               int m,
               int y);

        bsTime(const bsTime & f);

        void SetTime(int d,
                     int m,
                     int y);

        void SetTime(string day,
                     string month,
                     string year);

        void SetTime(string aTimeString);

        void SetTime(int inityear,
                     int JulianDay);
        void AddYear(int extraYear)
        {
        	year=year+extraYear;
        }
        int GetYear() {
            return year;
        }

        int GetMonth() {
            return month;
        }

        int GetDay() {
            return day;
        }

        int GetDayInYear() {
            return dayInYear;
        }

        void AddOneDay(void);

        void AddOneYear(void);

        string GetString2();

        void CalculateDateValue();

        long GetDateValue() {
            return longTime;
        }

        bool IsLeapYear(int year);

        int daysInYear(int offset);

        bool operator ==(bsTime time2);

        bool operator <(bsTime time2);

        bool operator <=(bsTime time2);

        bool operator >(bsTime time2);

        bool operator >=(bsTime time2);

        int operator -(bsTime time2);

        friend ostream & operator <<(ostream &      os,
                                     const bsTime & aTime);

        void AddDays(int nrOfDays);

        int GetJulianDay(int aday,
                         int amonth,
                         int ayear);

        int GetJulianDay();

        bsTime * clone() const;
};


extern bsTime theTime;
#endif

