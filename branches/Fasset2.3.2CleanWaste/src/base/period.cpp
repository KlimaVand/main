/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "period.h"


/****************************************************************************\
Constructor with arguments
The period is inclusive both dates
\****************************************************************************/
period::period(char * aName,int firstMonth,int firstDay,int lastMonth,int lastDay)
{
	name=aName;
	firstmonth=firstMonth;
	firstday=firstDay;
   lastmonth=lastMonth;
   lastday=lastDay;
}


/****************************************************************************\
Copy constructor
\****************************************************************************/
period::period(const period& p)
{
   name       = p.name;
	firstmonth = p.firstmonth;
	firstday   = p.firstday;
   lastmonth  = p.lastmonth;
   lastday    = p.lastday;
}

/****************************************************************************\
Destructor
\****************************************************************************/
period::~period()
{
}

/****************************************************************************\
Assignment operator
\****************************************************************************/
period& period::operator=(const period& p)
{
   if (this==&p)
      return *this;
   name       = p.name;
	firstmonth = p.firstmonth;
	firstday   = p.firstday;
   lastmonth  = p.lastmonth;
   lastday    = p.lastday;
   return *this;
}

/****************************************************************************\
\****************************************************************************/
int period::IsDateInPeriod(int month,int day) const
{
   int a=month*31+day;
   int b1=firstmonth*31+firstday;
   int b2=lastmonth*31+lastday;
   int ret=(a>=b1 && a<=b2);
   return ret;
}




