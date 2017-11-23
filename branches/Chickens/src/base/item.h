/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Class: item
(c) Bjørn Molt Petersen, Danmarks JordbrugsForskning
E-mail: bpe@pvf.sp.dk
\****************************************************************************/

#ifndef __ITEM_H
  #define __ITEM_H

#include "typer.h"


class item
{
	private:
		string name;
		string unit;
		double amount;
      bool DailyAddIndicator;           // true if amount should be added to the indicator (added by JB)

		item& operator=(const item& i); // Dissable the compilers generation of default assignment operator.
		item(const item& i);            // Dissable the compilers generation of default copy constructor.

	public:
		item();
		item(char * aName, char * aUnit, double aAmount, bool AaddIndicator);
		 ~item();
		ostream& Out(ostream& os, const item& aItem, int year=-1, string sep=" ", string StringMark="");
		void AddAmount(double aAmount);
      void SetAmount(double aAmount);
      void SetDailyAddIndicator(bool add) {DailyAddIndicator = add;};
      bool GetDailyAddIndicator() {return DailyAddIndicator;};
      double GetAmount(){return amount;};
		const char * GetName(){return name.c_str();};
};

#endif

