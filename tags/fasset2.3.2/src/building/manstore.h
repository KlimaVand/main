#ifndef MANURE_STORAGE_H
#define MANURE_STORAGE_H

#include "../building/storage.h"
#include "../products/manure.h"
#include "../products/plantItem.h"
#include "../base/budget.h"
/****************************************************************************\
 Class: manurestore
 instanciable class used to simulate a slurry-storage.

\****************************************************************************/

class manurestore
{

private:
  	enum {MAX_TXT=81};

public:
   manurestore() {};

public:
   string   Alias;
   base * aBase;
   double NH3Emissionfactor;
   double N2Emissionfactor;
   double N2OEmissionfactor;
   double CO2Emissionfactor;
   double CH4Emissionfactor;

   double NMinfactor;
   double OMdegRateConst;   //maximum degradation rate of organic matter
   double postCleanDepth;  ////Depth of manure remaining after cleaning (m)
   storage* theStore;    //Whatever is being stored
   double surfaceArea;       //in square metres
   double depth;				// depth of store in metres
   manurestore * nextStore;
   string manureToName;
   int manureToIndex;
   budget Nbudget;            // budgets
   budget Cbudget;            // budgets
   double cumvolat;
   bool external;
   double AreaPrAnimal;  //always set in decendent class
   string ALFAMtype;			//pig, cattle
   int locked; 			//true if manure cannot be removed other than by a request for cleaning to occur
   fstream *detailsFile;

   //temporary variables
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
   //! unique identifier number
   int manureStoreID;
	//!ID number for manurestore receiving manure from this floor (optional)
	int ManureToID;

public:
   // Constructor with arguments
   manurestore(const char *aName,const int aIndex,const base* aOwner,commonData * &runData,commonData * &common);
   // Copy constructor
   manurestore(const manurestore& s);
   // Destructor
   virtual ~manurestore();
   //initialize from file
   virtual manurestore * GetnextStore() {return nextStore;};
   virtual void SetnextStore(manurestore * aManureStore) {nextStore = aManureStore;};
   virtual void SetManureToIndex(int amanureToIndex){manureToIndex=amanureToIndex;};
   virtual void SetManureToName(string amanureToName) {manureToName=amanureToName;};
   virtual void Setexternal(bool isItExternal) {external = isItExternal;}
   virtual void SetpostCleanDepth(double apostCleanDepth) {postCleanDepth=apostCleanDepth;};
   virtual void SetsurfaceArea(double area) {surfaceArea = area;};
   virtual void Setdepth(double aDepth) {depth = aDepth;}
   void SetOMdegRateConst(double aValue){OMdegRateConst=aValue;};
   virtual const char* GetName() {return aBase->GetName();};
   virtual const int GetIndex() {return aBase->GetIndex();};
   virtual const char * GetAlias() {return Alias.c_str();};
   virtual int GetManureToIndex(){return manureToIndex;};
   virtual string GetManureToName() {return manureToName;};
   virtual double GetpostCleanDepth() {return postCleanDepth;}
   virtual double GetsurfaceArea() {return surfaceArea;};
   virtual double Getdepth() {return depth;}

   virtual double GetAmountManure();
   virtual double GetTotalNStored();
   virtual double GetTotalCStored();
   virtual double GetTotalDMStored();
   // Handles all daily actions of the manurestore, should be called by the manager
   virtual void DailyUpdate(){};
   virtual void DailyUpdate(double volatAmmonia, double N2Emission, double N2OEmission, double CO2Emission, double CH4Emission);
   // Receive some manure from a stable
   virtual void RcvManure(const manure* someManure);
   virtual void SendManure(const double amount, manure *aManure);
   virtual bool CleanStore(manure * aManure);
   virtual void FullyCleanStore(manure * aManure);
   virtual product * GetFromManureStore(double RequestedAmount, product * p, bool checkAccess = false);
   // Volatize the slurry in the store
   //   virtual double Volatize();
   virtual void Volatize(double Ameantemp, double relHumidity, double precipitation, double surfaceArea, double resistance,
		   double *volatAmmonia, double *waterEvap, double *N2Emission, double *N2OEmission, double *CH4Emission, double *CO2Emission);
   virtual double GetAvailableAmountOf(const product * p);
   virtual double GetCapacity();
   virtual void SetManure(manure *aManure);
   virtual manure * GetManure(){return (manure *) theStore->GetProductPointer();};
   virtual void EndBudget(bool show=false);
   virtual double GetNInput() {return Nbudget.GetInput();}
   virtual double GetNOutput() {return Nbudget.GetOutput();}
   virtual void SetNOutput(double aVal) {Nbudget.SetOutput(aVal);}


   virtual void ReceiveBedding(plantItem *bedding);
   friend ostream& operator <<(ostream& os,const manurestore& s);
   virtual void Scale(double annualAmount);
   virtual void SetCapacity(double aValue) {theStore->SetCapacity(aValue);}
   virtual double GetAmountInManureStore();
   virtual void Volatize(double *Nlost, double *Clost,double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CH4Emission,
		   double *CO2Emission);
   virtual product *EmptyManureStore();
   virtual void OpenDetailsFile();
   virtual void CloseDetailsFile();
   virtual void ShowManureStore(fstream * afile, bool header);
   //!Returned the amount of ammonia volatilised (kg N per day)
   virtual double GetNH3Emission(){return NH3Emission;}
   //!Returned the amount of N2O-N emitted (kg N per day)
   virtual double GetN2OEmission(){return N2OEmission;}
   //!Returned the amount of NO-N emission (kg N per day)
   virtual double GetNOEmission(){return NOEmission;}
   //!Returned the amount of N2-N emitted (kg N per day)
   virtual double GetN2Emission(){return N2Emission;}
   //!Returned the amount of CO2-C emitted (kg C per day)
   virtual double GetCO2Emission(){return CO2Emission;}
   //!Returned the amount of CH4-C emitted (kg C per day)
   virtual double GetCH4Emission(){return CH4Emission;}

   virtual base * getBase(){return aBase;}
   virtual storage* gettheStore(){return theStore;}
   virtual string getManureName(){return theStore->GiveNameStoredProduct();}
   virtual int getmanureStoreID(){return manureStoreID;}
	int getManureToID(){return ManureToID;}
	void setManureToID(int aVal){ManureToID=aVal;}
};

/****************************************************************************\
 End of Class manurestore
\****************************************************************************/



#endif
