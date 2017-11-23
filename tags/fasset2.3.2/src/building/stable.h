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
#include "animalSection.h"
#include "instore.h"
#include "../products/manure.h"
#include "../base/budget.h"
#include "../base/climate.h"
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

protected:
  stable(){};
	
   double minTemperature;
   double maxTemperature;
 //! ammonia emission factor for use in static emission model
   double Emissionfactor;
   //!area of all walls, assume housing is cubic
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
   //! following params are only for wind-forced ventilation
   //!max average width of aperture to outside (m) - uncontrolled ventilation
   double apertureWidth;
   //!max average height of aperture to outside (m) - uncontrolled ventilation
   double maxapertureHeight;
   //! assume that the ventilation cannot be choked off completely. This is the minimum closure, as a proportion of the maximum height
   double minPropApertureHeight;
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
   //!optimum air velocity in freely-ventilated housing, in metres per sec
   double optimumAirVelocity;
	//!animal housing sections
	linkList<animalSection> *sections;
	//!standart animal housing sections
	linkList<animalSection> *standartSections;
	//!Storage within the animal housing.  Usually for manure
	linkList<instore> *inhouseStores;
   //!alternative name
   string Alias;
   //!Set false to skip function calls to housing that is not used
   bool inUse;
   //!File for detailed output (if activated)
   fstream *detailsFile;
   //! emission of NH3 (kg per day)
   double NH3Emission;
   //! emission of nitrous oxide (kg per day)
   double N2OEmission;
   //! emission of nitric oxide (kg per day)
   double NOEmission;
   //! emission of dinitrogen (kg per day)
   double N2Emission;
   //! emission of CO2 (kg per day)
   double CO2Emission;
   //! emission of CH4 (kg per day)
   double CH4Emission;
   //temporary variables
   double waterEvap;



public:
	//!the herd of animals living in the stable
	animalStock* Livestock;

   //! Constructor with arguments
   stable(const char* aname,const int aIndex, const base* aOwner,commonData * &runData,commonData * &common);
   //! Copy constructor
   stable(const stable& aStable);
   //! Destructor
   virtual ~stable();
   //!get the type of animal housed (will be overloaded)
   virtual string GetAnimalType() {return "";};
   void SetLivestock(animalStock* aLivestock);

	//! Get functions
   int GetSlurrySystem() {return slurrySystem;};
   //!Returned the alias name of the Aum housing
   string GetAlias() {return Alias;};
   //!Return the ventilation rate of the animal housing (in cubic metres per second)
   virtual double GetventilationRate() {return ventilationRate;}
//!   bool GetinUse() {return inUse;};
   //! Handles all daily actions of the stable; should be called by the manager
   virtual void DailyUpdate();

   //!Return a pointer to the named animal section
   /*!
    * \param name name of the animal section
    */
   animalSection * GetanimalSection(const char * name);
   //!Returns the temperature inside the building in Celsius
   double GetinsideTemperature() {return insideTemperature;}
   //!Return the total amount of nitrogen in the animal housing
	double GetTotalN();
	   //!Return the total amount of Carbon in the animal housing
	double GetTotalC();

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


   //!this function returns a pointer to a manure object that contains the requested amount
   /*!
    * \param RequestedAmount the amount of manure requested (tonnes fresh weight)
    * \param p pointer to the manure object
    */
	virtual product * GetFromManureStore(double RequestedAmount, product * p);
	//!scale the animal house to the number of animals present
	virtual void ScaleSize();
	//!Return the total area
   virtual double GetTotalAreaSections();
   //!//!check to make sure total area of animal sections is less than plan area of building
	virtual void checkArea();
//!Calculates the total wall area of the animal housing (in square metres)
   virtual void calcWallArea();
   //!Calculates the ventilation and inside temperature of animal housing with forced ventilation systems
   /*!
    * \param heatOP output of sensible heat by the animals in the housing (in Watts)
    * \param outsideAirTemp outside air temperature in Celsius
    * \param supplementaryHeat supplementary heating required to maintain the inside temperature at the target level (in Watts)
    */
   virtual void controlled(double heatOP, double outsideAirTemp,double *supplementaryHeat);
   //!Calculates the ventilation and inside temperature of freely-ventilated housing
   /*!
    * \param heatOP output of sensible heat by the animals in the housing (in Watts)
    * \param outsideAirTemp outside air temperature in Celsius
    * \param windspeed outside windspeed (in meters per second)
    * \param solarRad solar radiation in Watts per meter squared
    */
	virtual void uncontrolled(double heatOP, double outsideAirTemp, double windspeed, double solarRad);
	//!Calculates the ventilation rate of freely ventilated housing
	/*!
	 * \param airDensity density of the air in kilograms per metre cubed
	 * \param thermalTrans mean thermal transmissivity of the wall and roof material (in Watts per metre Kelvin)
	 * \param surfaceArea total surface area (wall and roof combined) in square metres
	 * \param apertureArea total area of apertures in the wall or roof (in square metres)
     * \param windspeed outside windspeed (in meters per second)
     * \param outsideAirTemp outside air temperature in Celsius
     * \param heatOP output of sensible heat by the animals in the housing (in Watts)
     * \param q input of heat via solar radiation of the roof (Watts)
     * \param ventilationRate ventilation rate in cubic metres per second
     * \param deltaTemper temperature differential between inside and outside
	 */
	void CalcFreeVentilation(double airDensity, double thermalTrans, double surfaceArea, double apertureHeight,
			double windspeed, double outsideAirTemp, double heatOP, double q,
         double *ventilationRate, double *deltaTemper);
	//!Returns total number of animals in the animal housing
   virtual double GetNrOfAnimals();
   //!Calculates the minimum ventilation rate (will be overloaded)
   virtual double CalcMinimumVentilationRate(){return 0.0;};
   //!Calculates the maximum ventilation rate (will be overloaded)
   virtual double CalcMaximumVentilationRate(){return 0.0;};
   //!Returns the amount of electricity used in the animal housing OBSOLETE
	double GetDailyElectricity();
	//!Initialisation routine OBSOLETE?
	/*!
	 * \param InputFilename name of input file
	 */

	//!Sets whether the animal house will be used
	/*!
	 * \param aVal set to true or false, depending on whether the housing is used or not
	 */
   void SetinUse(bool aVal){inUse=aVal;};
   //!Returns whether the housing is currently in use
   bool GetinUse(){return inUse;};
   //!Calculate the proportion of the heat energy that is in the sensible form
   virtual double CalcPropSensible();

   friend ostream& operator <<(ostream& os,stable* s);
   //!Empty the animal house of manure
	product * EmptyManureFromStore();
	//!Open the file that will store the details of the animal house variables
   void OpenDetailsFile();
	//!Close the file that will store the details of the animal house state variables
	void CloseDetailsFile();
	//!Output the state variables
	/*!
	 * header set to true to print the header details for the state variables
	 */
	void ShowStable(bool header);
	//!Returned the amount of ammonia volatilised (kg N per day)
   double GetNH3Emission(){return NH3Emission;}
	//!Returned the amount of N2O-N emitted (kg N per day)
   double GetN2OEmission(){return N2OEmission;}
	//!Returned the amount of NO-N emission (kg N per day)
   double GetNOEmission(){return NOEmission;}
	//!Returned the amount of N2-N emitted (kg N per day)
   double GetN2Emission(){return N2Emission;}
	//!Returned the amount of CO2-C emitted (kg C per day)
   double GetCO2Emission(){return CO2Emission;}
	//!Returned the amount of CH4-C emitted (kg C per day)
   double GetCH4Emission(){return CH4Emission;}

};

/****************************************************************************\
 End of Class stable
\****************************************************************************/



#endif
