/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Class: item
(c) Bj�rn Molt Petersen, Danmarks JordbrugsForskning
E-mail: bpe@pvf.sp.dk
\****************************************************************************/

#ifndef __ITEM_H
  #define __ITEM_H

#include "typer.h"


class item
{
	private:
		char  name[250];
		int nameInt;
		char  unit[250];
		double amount;

      bool DailyAddIndicator;           // true if amount should be added to the indicator (added by JB)

		item& operator=(const item& i); // Dissable the compilers generation of default assignment operator.
		item(const item& i);            // Dissable the compilers generation of default copy constructor.

	public:
		item();
		item(int intname,const char * aName,const char * aUnit, double aAmount, bool AaddIndicator);
		 ~item();

		ostream& Out(ostream& os, const item& aItem, int year=-1, string sep=" ", string StringMark="");
		void AddAmount(double aAmount);
      void SetAmount(double aAmount);
      void SetDailyAddIndicator(bool add) {DailyAddIndicator = add;};
      bool GetDailyAddIndicator() {return DailyAddIndicator;};
      double GetAmount(){return amount;};
		const char * GetName(){
			return name;
		};

};

#endif

