
#include "common.h"
#include "item.h"
#include <string.h>

item::item() {
    name              = "";
    unit              = "";
    amount            = 0.0;
    nameInt           = -4;
    DailyAddIndicator = true;
    Dailyamount       = 0.0;
}

item::item(int    intname,
           string aName,
           string aUnit,
           double aAmount,
           bool   aDailyAddIndicator) {
    nameInt           = intname;
    name              = aName;
    unit              = aUnit;
    amount            = aAmount;
    Dailyamount       = aAmount;
    DailyAddIndicator = aDailyAddIndicator;
}

item::~item() {}

ostream & item::Out(ostream &    os,
                    const item & aItem,
                    int          year,
                    string       sep,
                    string       StringMark) {
    if (year >= 0) {
        os << year << sep;
    }

    os << setiosflags(ios::left) << StringMark << setw(40) << aItem.name << StringMark << sep;
    os << setiosflags(ios::right) << setiosflags(ios::fixed) << setiosflags(ios::showpoint) << setprecision(3)
       << setw(10) << aItem.amount;
    os << sep << setiosflags(ios::left) << StringMark << setw(25) << aItem.unit << StringMark << endl;

    return os;
}

void item::AddAmount(double aAmount) {
    amount += aAmount;
}

void item::AddDailyAmount(double aAmount) {
    Dailyamount += aAmount;
}

void item::SetDailyAmount(double aAmount) {
    Dailyamount = aAmount;
}

void item::SetAmount(double aAmount) {
    amount = aAmount;
}
