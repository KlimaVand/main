
#include "common.h"
#include "period.h"

const int monthLength[12] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

/*
 * Constructor with arguments
 * The period is inclusive both dates
 */
period::period(string aName,
               int    firstMonth,
               int    firstDay,
               int    lastMonth,
               int    lastDay) {
    name       = aName;
    firstmonth = firstMonth;
    firstday   = firstDay;
    lastmonth  = lastMonth;
    lastday    = lastDay;
}

period::period(int firstMonth,
               int firstDay,
               int lastMonth,
               int lastDay) {
    name       = "";
    firstmonth = firstMonth;
    firstday   = firstDay;
    lastmonth  = lastMonth;
    lastday    = lastDay;
}

period::period(const period & p) {
    name       = p.name;
    firstmonth = p.firstmonth;
    firstday   = p.firstday;
    lastmonth  = p.lastmonth;
    lastday    = p.lastday;
}

period::~period() {}

period & period::operator = (const period & p) {
    if (this == &p) {
        return *this;
    }

    name       = p.name;
    firstmonth = p.firstmonth;
    firstday   = p.firstday;
    lastmonth  = p.lastmonth;
    lastday    = p.lastday;

    return *this;
}

int period::IsDateInPeriod(int month,
                           int day) const {
    int a   = month * 31 + day;
    int b1  = firstmonth * 31 + firstday;
    int b2  = lastmonth * 31 + lastday;
    int ret = ((a >= b1) && (a <= b2));

    return ret;
}
