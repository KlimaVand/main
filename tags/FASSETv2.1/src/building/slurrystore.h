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

#ifndef SLURRY_STORAGE_H
  #define SLURRY_STORAGE_H
//#define STALDDEBUG

#include "manstore.h"
#include "building.h"
#include "pmanure.h"
#include <common.h>
#include <budget.h>
/****************************************************************************\
 Class: slurrystore
 instanciable class used to simulate a slurry-storage.

\****************************************************************************/

class slurrystore: public manurestore, public building
{
private:

//   double AreaPrAnimal;   //surface area per animal - used if manure store is to be scaled to animal numbers present
   double crustResistance;     		//crust resistance to volatization
   double specificResistance;           //building spec. resistance to volatization
   double coverResistance;       //Covers on slurrystore resistance --||--

//   slurrystore() {};

public:
   // Constructor with arguments
   slurrystore(char *aName,const int aIndex,const base* aOwner);
   // Copy constructor
   slurrystore(slurrystore& s);
   // Destructor
   virtual ~slurrystore();
   //initialize from file
//   virtual double GetAreaPrAnimal() {return AreaPrAnimal;};   //surface area per animal - used if manure store is to be scaled to animal numbers present
   virtual void ReadParameters(fstream * file);
	virtual void DailyUpdate();
	virtual void VolatizeSlurry(double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CO2Emission, double *CH4Emission);
//	virtual void VolatizeSlurry();
   virtual void GiveIndicator(int indicatorType);
	virtual void CalcLP();
   virtual void RcvManure(const manure* someManure);
	virtual void Scale(double annualAmount);
   friend ostream& operator <<(ostream& os,slurrystore s);
//	void Volatize();
};

/****************************************************************************\
 End of Class manurestore
\****************************************************************************/



#endif
