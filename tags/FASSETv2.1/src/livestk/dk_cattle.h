/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __dk_cattle_H  //Required for current class
  #define __dk_cattle_H

#include <clonlist.h>
#include <common.h>
#include <cattle.h>
#include <feed_resource.h>
#include <pmanure.h>
#include <diet.h>
#ifndef _STANDALONE
 #include <field.h>
 #include <stable.h>
#endif
class dk_cattle: public cattle
{
 protected:
//   double rejectionCoefficient;		// controls the rate at which the rejection of dung patches decreases per day (calc here as 0.4 dig units over 14 days)*/
   double potPeakMilkYield;	//target annual milk yield in litres

   //***********************
   double herdMilkYieldConstant;  //bes�tningsydelsen kg EKM  (Ykap)
   double q0, q1;						// coefficients that determine the remobilisation of body tissue to support milk production
   double lactationEnergyFudgeFactor;
   //***********************

	virtual void intake();
   virtual double GetbulkCapacity();
	virtual double calc_energy_maint();
	virtual void Partition();
   double GetPotentialMilkYield(int daysIntoLactation);
   virtual double DoPregnancy(double *avail_ME, double *availableN);
   virtual void DoLactation(double *avail_FE, double *availableN, double *milkN);
   double SucklingCalfMilkIntake(){return 0.0;};
	double GetavailableFodderUnits();
	double CalcFoetusWeight(double timeFromConception);
   void Createfoetus();
	double GetSFUPerKgGain();
	double GetEnergyLimitedGrowth(double *energyForGrowth);

public:
			//Constructor setting attributes to default values
	dk_cattle (const char *aName,const int aIndex,const base * aOwner);
	dk_cattle (const dk_cattle& anAnimal);
#ifdef _STANDALONE
	dk_cattle ();
#endif
	virtual ~dk_cattle(); //Destructor

// Set functions for attributes
	virtual void ReadParameters(fstream * file);
	virtual void ReadParameters(char *filename);
	virtual void DailyUpdate ()  ;

   void MakePregnant(bool isAFemale);
   dk_cattle* CheckBirth();
   //***********************
   double GetSize();
	double GetPotentialGrowth();
	double GetPotentialIntake(double fillPerSFU);
	double GetMilkYieldCapacity();
   double Get_norm_weight();
	double Getfill(feed_resource *aFeed_resource);
	double GetfillperSFU(feed_resource *aFeed_resource);
	double Getweight() const {return weight;};
	double NinEBWchange();
	void 	GiveIndicator(int indicatorType);
	void calculateFill(feedItem *aFeedItem);
	double CalcFoetusProtein(double offset);
	double CalcGravidUterusProtein(double offset);
   double GetFoetusWeight(double offset);
	double GetGravidUterusWeight(double offset);

#ifdef _STANDALONE
	virtual void Initialise(double anAge, int thedaysFromConception, int thelactationNumber);
#endif
	virtual void WriteCowOutput(fstream * afile, bool header);
	bool GetAmountNextFeed(feed_resource * aFeed_resource);
	double CalcFatInEBWchange(double energy, double protein);
	void GrowGravidUterus(double newProtein, double newFat);
   void InitialiseFoetus();
   void CalcHeatProduction(manure *urine);
	void Getintake(double *DM, double *ME, double *FE, double *DM_herb, double *ME_herb, double *FE_herb);
   void DoBirth(double *afterBirthProtein, double *afterBirthFat);
   double GetPalatability(feed_resource *aFeed_resource);
   void ChangeBodyComposition(double weightChange, double *avail_FE, double *availableN);
   double GetPotentialMilkSolidsIntake(int days);
	double GetPotentialAnnualMilkProduction(int durationOfLactation);
   double GetPotentialMilkYield();
   double GetEnergyRequirement();
   double GetSFUsPerKgEKM() {return 0.4;}; //cy
	double GetFoetusEnergyRequirement();
   double GetFoetusPorteinRequirement();
	double GetProteinRequirement();
	double GetPotentialMilkIntake();
	double GetCalfSolidIntakeFraction();
};
#endif
