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

#ifndef MANURE_STORAGE_H
  #define MANURE_STORAGE_H
//#define STALDDEBUG

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

   char   Alias[MAX_TXT];
//   char * Alias;
public:
   manurestore() {};

protected:
   base * aBase;
   double NH3Emissionfactor;
   double N2Emissionfactor;
   double N2OEmissionfactor;
   double CO2Emissionfactor;
   double CH4Emissionfactor;
	double CMinfactor;
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
   double CH4Emission,
   			CO2Emission,
            volatAmmonia,
            N2Emission,
            N2OEmission;


public:
   // Constructor with arguments
   manurestore(const char *aName,const int aIndex,const base* aOwner);
   // Copy constructor
   manurestore(manurestore& s);
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
   virtual const char * GetAlias() {return Alias;};
   virtual int GetManureToIndex(){return manureToIndex;};
   virtual string GetManureToName() {return manureToName;};
   virtual double GetpostCleanDepth() {return postCleanDepth;}
   virtual double GetsurfaceArea() {return surfaceArea;};
   virtual double Getdepth() {return depth;}
   virtual void ReadParameters(fstream * file);
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
   virtual void GiveIndicator(int indicatorType) {};
	virtual void CalcLP();
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
   virtual double GetvolatAmmonia(){return volatAmmonia;}
};

/****************************************************************************\
 End of Class manurestore
\****************************************************************************/



#endif
