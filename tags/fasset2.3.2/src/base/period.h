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

		 ~period();
		period& operator=(const period& p); // Assignment operator.
		period(const period& p);            // Copy constructor.
      int IsDateInPeriod(int month,int day) const;




};

#endif

