/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
NJH 2001
 ----------------------------------------------------------------------------
	  Log
	  Revised 2002 to include simulation of ventilation
\****************************************************************************/

#ifndef STABLE_H
  #define STABLE_H


#include "building.h"
#include "animSec.h"
#include "instore.h"
#include "pmanure.h"
#include "budget.h"
#include <climclas.h>
class animalStock;

/****************************************************************************\
 Class: stable
 superclass of all stables.Needs to be further derived.
\****************************************************************************/

//!parent class for animal housing
/*!Parent class for animal housing.  Normally derived into specialised animal house
classes.
*/

class stable : public building
{

private:
   int slurrySystem;
	//! default constructor
   stable(){};
protected:
 //! ammonia emission factor for use in static emission model
   double Emissionfactor;
   //!area of one wall, assume housing is cubic
   double wallArea;
   //! plan area of house
   double planArea;
   //!mean height of house
   double meanHeight;
   //!thermal transmissivity of wall
   double thermalTransWall;
   //!thermal transmissivity of roof
   double thermalTransRoof;
   //!true if the housing has controlled ventilation
   int controlledVent;
   //! ventilation rate (cubic metres per second)
   double ventilationRate;
   //!Inside temperature in degrees Celsius
   double insideTemperature;
   //! following params are only for uncontrolled ventilation
   //!width of aperature to outside (m) - uncontrolled ventilation
   double apertureWidth;
   //!height of aperature to outside (m) - uncontrolled ventilation
   double apertureHeight;
   //!absorption coefficient for solar radiation (roof only)
   double absorbCoeff;
   //! emissivity of roof for long wave radiation
   double emissivity;
	//! surface resistence of roof
   double externSurfResis;
   //!Target inside temperature
   double targetTemperature;
   //!min ventilation rate in cubic metres per sec
   double minVentilation;
   //!max ventilation rate in cubic metres per sec
   double maxVentilation;
	//!animal housing sections
	linkList<animalSection> *sections;
	//!Storage within the animal housing.  Usually for manure
	linkList<instore> *inhouseStores;
   //!alternative name
   string Alias;
   //!Set false to skip function calls to housing that is not used
   bool inUse;
   //!File for detailed output (if activated)
   fstream *detailsFile;

   //temporary variables
   double volatAmmonia,
   		waterEvap,
         N2Emission,
         N2OEmission,
         CH4Emission,
         CO2Emission;



public:
	//!the herd of animals living in the stable
	animalStock* Livestock;

   //! Constructor with arguments
   stable(char* aname,const int aIndex, const base* aOwner,
               animalStock* alivestock=NULL, int aVolatModel=0);
   //! Copy constructor
   stable(const stable& aStable);
   //! Destructor
   virtual ~stable();
   // Set functions
   virtual string GetAnimalType() {return "";};
   void SetLivestock(animalStock* aLivestock);

	//! Get functions
   int GetSlurrySystem() {return slurrySystem;};
   string GetAlias() {return Alias;};
   virtual double GetventilationRate() {return ventilationRate;}
//!   bool GetinUse() {return inUse;};
   //! Handles all daily actions of the stable should be called by the manager
   virtual void DailyUpdate();
   //! Reads information from file
   void ReadParameters(fstream * file);
   animalSection * GetanimalSection(const char * name);
   double GetinsideTemperature() {return insideTemperature;}
	double GetTotalN();
	double GetTotalC();
	virtual void CalcLP();
   //! remove todays manure
   virtual void CleanManure();
   virtual double GetManureAmount();
   //! calculate C and N volatilization
   virtual void  Volatize();
   //! check if stable capacity is overflowed
   virtual bool CapacityOverflow();

   virtual void LinkManureStores(manurestore * aManureStore);
   virtual void CheckLinks();
	virtual void EndBudget(bool show);
	virtual double GetNInput();
	virtual double GetNOutput();

   virtual void GiveIndicator(int indicatorType);
	virtual product * GetFromManureStore(double RequestedAmount, product * p);
	virtual void ScaleSize();
   virtual double GetTotalAreaSections();
	virtual void checkArea();
//!used mainly for ventilation
   virtual void calcWallArea();
//!   virtual void calcMinVentilationRate();
//!   virtual void calcMaxVentilationRate();
   virtual void controlled(double heatOP, double outsideAirTemp,double *supplementaryHeat);
	virtual void uncontrolled(double heatOP, double outsideAirTemp, double windspeed, double solarRad);
	void CalcFreeVentilation(double airDensity, double thermalTrans, double surfaceArea, double apertureArea,
			double windspeed, double outsideAirTemp, double heatOP, double q,
         double *ventilationRate, double *deltaTemper);
   virtual double GetGetNrOfAnimals();

   virtual double CalcMinimumVentilationRate(){return 0.0;};
   virtual double CalcMaximumVentilationRate(){return 0.0;};
	double GetDailyElectricity();
   //!virtual int CalcFeedingHours();
	void Initialise(char * InputFilename);
   void SetinUse(bool aVal){inUse=aVal;};
   bool GetinUse(){return inUse;};
   virtual double CalcPropSensible();

   friend ostream& operator <<(ostream& os,stable* s);
	product * EmptyManureFromStore();
   void OpenDetailsFile();
	void CloseDetailsFile();
	void ShowStable(fstream *afile, bool header);

};

/****************************************************************************\
 End of Class stable
\****************************************************************************/



#endif
