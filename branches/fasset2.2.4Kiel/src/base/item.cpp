/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "item.h"

/****************************************************************************\
  Constructor
\****************************************************************************/
item::item()
{
	name = "";
	unit = "";
	amount = 0.0;
   DailyAddIndicator = true;
}

/****************************************************************************\
Constructor with arguments
\****************************************************************************/
 item::item(char * aName, char * aUnit, double aAmount, bool aDailyAddIndicator)
{
	name = aName;
	unit = aUnit;
	amount = aAmount;
   DailyAddIndicator = aDailyAddIndicator;
}

/****************************************************************************\
Destructor
\****************************************************************************/
item::~item()
{
}

/****************************************************************************\
\****************************************************************************/
ostream& item::Out(ostream& os, const item& aItem, int year, string sep, string StringMark)
{
   if (year >=0)
      os << year << sep.c_str();
#ifdef JEO_TAB
   os << aItem.name.substr(0,5) << sep;
#endif
	os
		<< setiosflags(ios::left)
     << StringMark
		<< setw(nameLength)
		<< aItem.name
     << StringMark
		<< sep.c_str();
      if (aItem.unit>"")
      {
         os << setiosflags(ios::right)
            << setiosflags(ios::fixed)
		      << setiosflags(ios::showpoint)
		      << setprecision(3)
		      << setw(10)
		      << aItem.amount;
      }
		os << sep.c_str()
         << setiosflags(ios::left)
         << StringMark
         << setw(unitLength)
		   << aItem.unit
         << StringMark
		   << endl;
	return os;
}

/****************************************************************************\
\****************************************************************************/
void item::AddAmount(double aAmount)
{
	amount+=aAmount;
}

/****************************************************************************\
\****************************************************************************/
void item::SetAmount(double aAmount)
{
	amount=aAmount;
}




