/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 NJH - 2.7.99
  ----------------------------------------------------------------------------
      Log
\****************************************************************************/

#ifndef FYM_STORAGE_H
  #define FYM_STORAGE_H
//#define STALDDEBUG

#include "manstore.h"
#include "building.h"
#include "../products/manure.h"
#include "../base/common.h"
#include "../base/budget.h"
/****************************************************************************\
 Class: dungheap
 instanciable class used to simulate a slurry-storage.

\****************************************************************************/

class dungheap: public manurestore, public building
{

private:

	bool ventilated;	//true if air can access base of stack
   manurestore * surfaceLayer;
//   dungheap() {};

public:
   // Constructor with arguments
   dungheap(const char *aName,const int aIndex,const base* aOwner);
   // Copy constructor
   dungheap(dungheap& s);
   // Destructor
   virtual ~dungheap();
   //initialize from file
   virtual void ReadParameters(fstream * file);
	void DailyUpdate();
	virtual void GiveIndicator(int indicatorType);

	void VolatizeDung(double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CO2Emission, double *CH4Emission);
	void RcvManure(const manure* someManure);
   friend ostream& operator <<(ostream& os,dungheap s);
	void ReadDefaultParameters();
};

/****************************************************************************\
 End of Class manurestore
\****************************************************************************/



#endif
