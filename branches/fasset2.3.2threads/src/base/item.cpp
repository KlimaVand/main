/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "item.h"
#include <string.h>
/****************************************************************************\
  Constructor
\****************************************************************************/
item::item()
{
	strcpy(name,"");
	strcpy(unit,"");

	amount = 0.0;
	nameInt=-4;
   DailyAddIndicator = true;
}

/****************************************************************************\
Constructor with arguments
\****************************************************************************/
 item::item(int intname,const char * aName,const char * aUnit, double aAmount, bool aDailyAddIndicator)
{

	 nameInt=intname;
	strcpy(name,aName);
	strcpy(unit,aUnit);

	amount = aAmount;
   DailyAddIndicator = aDailyAddIndicator;
}

/****************************************************************************\
Destructor
\****************************************************************************/
item::~item()
{
}

item::item(const item& aItem)
{
	 nameInt=aItem.nameInt;
	strcpy(name,aItem.name);
	strcpy(unit,aItem.unit);

	amount = aItem.amount;
  DailyAddIndicator = aItem.DailyAddIndicator;

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
		<< setw(40)
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
         << setw(25)
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




