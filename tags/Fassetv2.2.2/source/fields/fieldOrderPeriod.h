/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FLDPER
  #define __FLDPER

#include "../base/message.h"
#include "../base/bstime.h"
#include "../base/common.h"
#include "../base/base.h"


class fieldOrderPeriod : public base
{
	protected:
   string * StartDate;
   string * EndDate;
   double TSum;
   double soilMoisture;
   bool   upper;
	fieldOrderPeriod& operator=(const fieldOrderPeriod& f); // Dissable the compilers generation of default assignment operator.

	public:
		fieldOrderPeriod(const char * aName, const int aIndex, const base * aOwner);
      ~fieldOrderPeriod(){};
//		fieldOrderPeriod(const fieldOrderPeriod& f);
//		friend ostream &operator<<(ostream &os,const fieldOrderPeriod &aFieldOrderPeriod);
      string *GetStartDate(){return StartDate;};
      string *GetEndDate(){return EndDate;};
      double GetsoilMoisture(){return soilMoisture;};
      double GetTSum(){return TSum;};
      bool Getupper(){return upper;};
      void ReadPeriod(fstream *aFile);
		bool operator!= (const fieldOrderPeriod& somePeriod) const;
};

#endif

