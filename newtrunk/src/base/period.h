/*
 * * * Class: period
 * (c) Bj�rn Molt Petersen, Danmarks JordbrugsForskning
 * E-mail: bpe@pvf.sp.dk
 * \
 */



#ifndef __PERIOD_H
#define __PERIOD_H

#include "typer.h"

class period {
    private:
        string name;
        int    firstmonth, firstday, lastmonth, lastday;

        period() {
            firstmonth = firstday = lastmonth = lastday = 0;
        }

        // Preventing the constructor without arguments from ever beeing used

    public:
        period(string aName,
               int    firstMonth,
               int    firstDay,
               int    lastMonth,
               int    lastDay);

        period(int firstMonth,
               int firstDay,
               int lastMonth,
               int lastDay);

        ~period();

        period & operator = (const period & p);    // Assignment operator.

        period(const period & p);                  // Copy constructor.

        int IsDateInPeriod(int month,
                           int day) const;

        string GetName() {
            return name;
        }

        int GetFirstMonth() {
            return firstmonth;
        }

        int GetFirstDay() {
            return firstday;
        }

        int GetLastMonth() {
            return lastmonth;
        }

        int GetLastDay() {
            return lastday;
        }

        int GetLength();

        int FullyContained(const period & per);
};
#endif

