/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Class: period
(c) Bjørn Molt Petersen, Danmarks JordbrugsForskning
E-mail: bpe@pvf.sp.dk
\****************************************************************************/

#ifndef __PERIOD_H
  #define __PERIOD_H

#include "typer.h"

class period
{
	private:
		string name;
      int
         firstmonth,
         firstday,
         lastmonth,
         lastday;

   	period(){}; // Preventing the constructor without arguments from ever beeing used

	public:
		period(char * aName,int firstMonth,int firstDay,int lastMonth,int lastDay);
		period(int firstMonth,int firstDay,int lastMonth,int lastDay);
		 ~period();
		period& operator=(const period& p); // Assignment operator.
		period(const period& p);            // Copy constructor.
      int IsDateInPeriod(int month,int day) const;
		const char * GetName(){return  name.c_str();};
      int GetFirstMonth(){return firstmonth;};
      int GetFirstDay(){return firstday;};
      int GetLastMonth(){return lastmonth;};
      int GetLastDay(){return lastday;};
      int GetLength();
      int FullyContained(const period & per);
      int Overlap(const period & per);
};

#endif

