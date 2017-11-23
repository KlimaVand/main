#ifndef __uk_cattle_H  //Required for current class
  #define __uk_cattle_H

#include <clonList.h>
#include <common.h>
#include <cattle.h>
#include <stable.h>
#include <feed_resource.h>
#include <pmanure.h>
#include <diet.h>
#ifndef _STANDALONE
   #include <field.h>
#endif
#include <NixCbits.h>

class uk_cattle: public cattle
{
protected:
   int breed;
   double bodyEnergy;
   double birthWeightFactor;					//birth weight as proportion of MBW
   double uterusProtein; 				//track uterus protein (has to be lost at birth) (kg)
   double weightChange;					//current change in empty body weight (kg)
   double rejectionCoefficient;		// controls the rate at which the rejection of dung patches decreases per day (calc here as 0.4 dig units over 14 days)
   int partitionSystem;					//0=DK system, 1= UK energy and DK protein system
   double potPeakMilkYield;	//target annual milk yield in litres
   double lactose;		//g/kg in milk
   double milkfat;     //g/kg in milk
   double a,b,c,Gzero;	//coefficients of lactation curve
   double uterusWeight; 	//track uterus weight (has to be lost at birth) (kg)
   double uterusEnergy; 	//track uterus energy (has to be lost at birth) (kg)
	double fieldOfftakeList[500];
   double gestationPeriod;				//gestation period in days

	double Get_quality_factor( double digestibility);
	double Get_norm_weight();
   double relsiz();
	virtual void intake();
   virtual double GetPotentialIntake();
	virtual double  eff_energy_main();
	virtual double calc_energy_maint();
   virtual double eff_energy_growth();
	virtual double  MEinEBWchange();
   virtual double GetWeightChange(double avail_ME);
	virtual void Partition(double *dungN, double *urineN);
	virtual double NinEBWchange();
	virtual double GetPotentialMilkIntake();
   virtual double GetbodyEnergy() const {return bodyEnergy;};
   double eff_energy_lactation();
   double GetPotentialMilkYield();
   virtual double DoPregnancy(double *avail_ME, double *availableN);
   virtual double DoLactation(double *avail_ME, double *availableN);
	double SucklingCalfMilkIntake();
   void Createfoetus();

public:
   //Constructor setting attributes to default values
	uk_cattle (const char *aName,const int aIndex,const base * aOwner);
	uk_cattle (const uk_cattle& anAnimal);
	virtual ~uk_cattle(); //Destructor

// Set functions for attributes
	virtual void SetBreed(int aBreed){breed = aBreed;}
   virtual void SetbodyEnergy(double anEnergy) {bodyEnergy=anEnergy;};
// void SetisGrazing(bool itIsGrazing) {isGrazing = itIsGrazing;}

	virtual void ReadParameters(fstream * file);
	void ManureProduction(fstream* fileStream);
	void FeedMax(fstream* fileStream);
	void FeedMin(fstream* fileStream);

   void Manure(manure* fluidManure, manure* solidManure);
   void MakePregnant(bool isAFemale);
   uk_cattle* CheckBirth();
	virtual void EndBudget(bool show = false);
   virtual double GetPotentialIntake(double digestibility);
	double CalcFoetusEnergy(double offset);
	double CalcFoetusWeight(double offset);
	double CalcFoetusProtein(double offset);
	double CalcGravidUterusProtein(double offset);
};
#endif
