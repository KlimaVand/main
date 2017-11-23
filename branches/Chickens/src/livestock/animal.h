/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/


// Class: animal                  //Borland C++ 5.02 - FASSET style
#ifndef __animal_H  //Required for current class
  #define __animal_H

#include "../base/cloneList.h"
#include "../base/base.h" //Required for all FASSET classes
#ifdef _STANDALONE
	#include "../base/OutputControl.h"
  	#include "../base/message.h"
#else
	#include "../building/animalSection.h"
#endif
 #include "../products/feedItem.h"//Required for include files
 #include "../products/manure.h" //Required for include files

#include "../products/animalProduct.h"
#include "../base/budget.h"


class animal: public base
{
 protected:
 double amountSolid; 					//FW faeces kg per day
 double amountFluid; 					//FW urine kg per day
 double heatProduction; 				//heat production per animal (W)

 double N_growth;                   // N partitioned to tissue growth
 double C_growth;                   // C partitioned to tissue growth
 double P_growth;                   // P partitioned to tissue growth
 double K_growth;                   // K partitioned to tissue growth
 double FE_need;                    // Scandinavian feed units required for growth

 double Number;                     // number of animals represented by this instance
 double LivestockUnits;             // livestock units (are read in from file)
 double variableCosts;
#ifndef _STANDALONE
 animalSection * StableSection;     //pointer to the section of the animal house where the animal is located when housed
#endif
 feedItem* currentfeed;             //pointer to the actual diet eatten
 manure* manurePrDay;					//pointer to the total manure production in a day (dung and urine combined)
 manure* fluidManurePrDay;				//pointer to the urine production in a day
 manure* solidManurePrDay;				//pointer to the dung production in a day
 product* vetExpensesPrDay;
 animalProduct* thisProduct;  		//used in slaughterSwine
 budget Nbudget;            // budgets
 budget Cbudget;            // budgets
 string Alias;

#ifndef _STANDALONE
  animal () {};
#endif

public:
			//Constructor setting attributes to default values
  animal(const char *aName,const int aIndex,const base * aOwner);
      // Copy Constructor
  animal(const animal& anAnimal);
#ifdef _STANDALONE
  animal ();// :animal();
#endif
  virtual	animal* clone() const;
  virtual ~ animal ( ); //Destructor
	void Initialise();


   #ifndef _STANDALONE
   virtual void SetStableSection (animalSection* aStableSection){ StableSection = aStableSection;}
   virtual animalSection* GetStableSection () { return StableSection;}
   #endif

   //user created functions
   virtual   void DailyUpdate ()  ;
   virtual void SetN_growth(double aN_growth) {N_growth = aN_growth;}
   virtual double GetN_growth() {return N_growth;}
   virtual void SetC_growth(double aC_growth) {C_growth = aC_growth;}
   virtual double GetC_growth() {return C_growth;}
   virtual void SetthisProduct (animalProduct* ananimalProduct) {thisProduct = ananimalProduct;};
   virtual animalProduct* GetthisProduct() {return thisProduct;};
   virtual void SetNumber(double aNumber) {Number = aNumber;};
   virtual double GetNumber() {return Number;};
   virtual void SetAlias(string anAlias) {Alias = anAlias;};
   virtual void SetDE(double aVal) {LivestockUnits=aVal;};
   virtual double GetDE() {return Number * LivestockUnits;};
   virtual string GetAlias() {return Alias;}

   //void ReceivePlan ( char* fileName)  ;
   virtual void ReadParameters (fstream * file);
   virtual void checkBalances(bool show = false);
   virtual feedItem* Getcurrentfeed() {return currentfeed;};

   virtual double GetamountSolid() {return amountSolid;}
   virtual double GetamountFluid() {return amountFluid;}
	virtual double GetCarbon(double fat, double protein);
};
#endif
