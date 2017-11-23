
#ifndef INMANURE_STORAGE_H
#define INMANURE_STORAGE_H
//#define STALDDEBUG

#include "manstore.h"
#include "building.h"
#include "../base/base.h"
/****************************************************************************\
 Class: instore
 instanciable class used to simulate inhouse manure storage.

\****************************************************************************/

class instore: public base, public manurestore
{

private:

protected:
	 bool  scalable;
	double velocityConstant;	//used to convert ventilation rate to air velocity - see Aarninck 1997
	//!  storage capacity number of days manure production
	double StorageCapacity;
	//  bool independent;

public:
	void SetScalable(bool scale){scalable=scale;};
	instore(const char *aName,const int aIndex,const base* aOwner);
	instore(const char *aName,const int aIndex,const base* aOwner,string manureType);

	// Constructor with arguments
	instore(const char *aName,const int aIndex,const base* aOwner,commonData * &runData,commonData * &common);
	// Copy constructor
	instore(const instore& aInstore);
	// Destructor
	virtual ~instore();
	friend ostream& operator <<(ostream& os,const instore& s);

	void SetvelocityConstant(double aVal){velocityConstant = aVal;}
	// Volatize the slurry in the store
	/*!
	 * \param volatAmmonia points to amount of NH3-N that have emitted (kg/time period)
	 * \param N2Emission points to amount of N2-N that have emitted (kg/time period)
	 * \param N2OEmission points to amount of N2O-N that have emitted (kg/time period)
	 * \param CO2Emission points to amount of CO2 that have emitted (kg/time period)
	 * \param CH4Emission points to amount of CH4 that have emitted (kg/time period)
	 */
	virtual void Volatize(double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CH4Emission,
			double *CO2Emission);
	/*!

	 * \param volatAmmonia points to amount of NH3-N that have emitted (kg/time period)
	 * \param N2Emission points to amount of N2-N that have emitted (kg/time period)
	 * \param N2OEmission points to amount of N2O-N that have emitted (kg/time period)
	 * \param CO2Emission points to amount of CO2 that have emitted (kg/time period)
	 * \param CH4Emission points to amount of CH4 that have emitted (kg/time period)
	 */
	virtual void floorVolatize(double airVelocity, double tempWaterEvap, double temperature, double areaSoiled, double duration,
			double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CH4Emission, double *CO2Emission);
	/*!
	\param airVelocity air velocity in the animal section (metres per second)
	 * \param tempwaterEvap evaporation of water (kg/ time period)
	\param temperature temperature of the surface the floor (Celsius)
	\param areaSoiled area of the floor soil by excreta (square metres)
	\param duration duration of the emission (hours)
	 * \param volatAmmonia points to amount of NH3-N that have emitted (kg/time period)
	 * \param N2Emission points to amount of N2-N that have emitted (kg/time period)
	 * \param N2OEmission points to amount of N2O-N that have emitted (kg/time period)
	 * \param CO2Emission points to amount of CO2 that have emitted (kg/time period)
	 * \param CH4Emission points to amount of CH4 that have emitted (kg/time period)
	 */
	virtual void inhouseVolatize(double airVelocity, double tempWaterEvap, double temperature, double duration,
			double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CH4Emission, double *CO2Emission);

	virtual void DailyUpdate();
	virtual double isInstore(){return true;}
	virtual void Scale(double annualManure);
};

/****************************************************************************\
 End of Class instore
\****************************************************************************/



#endif