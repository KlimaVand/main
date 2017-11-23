/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 NJH - 25.2.00
  ----------------------------------------------------------------------------
      Log
\****************************************************************************/

#ifndef INMANURE_STORAGE_H
  #define INMANURE_STORAGE_H
//#define STALDDEBUG

#include "manstore.h"
#include "building.h"
/****************************************************************************\
 Class: instore
 instanciable class used to simulate inhouse manure storage.

\****************************************************************************/

class instore: public base, public manurestore, public GenAsset
{

private:
  // instore() {};

protected:
  double velocityConstant;	//used to convert ventilation rate to air velocity - see Aarninck 1997
//  bool independent;
public:
   // Constructor with arguments
   instore(const char *aName,const int aIndex,const base* aOwner);
   // Copy constructor
   //manurestore(manurestore& s);
   // Destructor
   virtual ~instore();
   //initialize from file
   virtual void ReadParameters(fstream * file);
   // Volatize the slurry in the store
   virtual void Volatize(double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CH4Emission,
		double *CO2Emission);
	virtual void floorVolatize(double airVelocity, double humidity, double temperature, double areaSoiled, double duration,
		double *volatAmmonia, double *waterEvap, double *N2Emission, double *N2OEmission, double *CH4Emission, double *CO2Emission);
	virtual void inhouseVolatize(double airVelocity, double humidity, double temperature, double duration,
		double *volatAmmonia, double *waterEvap, double *N2Emission, double *N2OEmission, double *CH4Emission, double *CO2Emission);
	virtual double VolatizeDung(bool ventilated);
	virtual void DailyUpdate();
   virtual void GiveIndicator(int indicatorType);
	virtual void ScaleInstore(double numberOfAnimals);
//   virtual void SetIndependent(bool aValue) {independent = aValue;}
   friend ostream& operator <<(ostream& os,const instore& s);
};

/****************************************************************************\
 End of Class instore
\****************************************************************************/



#endif
