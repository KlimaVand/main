/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef CROPGRSS_H
   #define CROPGRSS_H

#include <crop.h>
#include <pplantit.h>
#include <pfeedit.h>

class cropRyegrass:public crop
{
protected:

	double
      //-----------------------
      MinDayLength,
      MinN_Straw,
      BulkDensity,
      RespirationTop,
      RespirationRoot,
      TransferRate,
      RecycleFraction,
      TemperatureSumAfterCut,
   	CutDelay;

   //Added by MEL 2009
      bool  HasBeenCut;

   //Added by NJH 17.08.00
   /****************/
   double 	liveOMD,
   			deadOMD;
   double   grazableDM,
   			grazableN,
   			grazedDM[4],
            uptakeWeight[4];

   double AgeClassTop[4];
   double StemDryMatter;

   virtual double UpdateAgeClasses(double deltaDryMatt);
   virtual double TopRespiration();
   virtual double DeadDMTop();
   virtual void CalcLeafAreaIndices();
   virtual void TransferDryMatterToStorage(double * deltaDryMatt);
   virtual void RemoveDMFromOldestAgeClass(double transferFromAgeClass);
   virtual void WinterSenescence();

public:
	cropRyegrass(const char * aName, const int aIndex, const base * aOwner);
   cropRyegrass(const cropRyegrass& acrop);
   virtual ~cropRyegrass(){};
   virtual void Add(crop* aCrop, double fraction);
   virtual cropRyegrass * clone();
   virtual double Update(double ActivePar);
   virtual void UpdateHeight();
	virtual bool ReadyForHarvest() {return Phenology->Emerged();}; // BMP corrected Feb. 2007
   virtual void RootDecay(double Translocation);
   virtual double GiveEvapFactor();
   virtual double GiveDryMatterVegTop();
   virtual void ReadParameters(char* fileName);
   virtual double GetBulkDensity(){return BulkDensity;};
   virtual double GiveCropHeight();
	virtual feedItem* GetAvailability(double cutHeight, int animalType);
   virtual double GetAvailableStandingDM(double cutHeight,bool useUptakeWeight=true);
	virtual void SetGrazed(double cutHeight, double DMGrazed);
	virtual double GetgrazableDM() {return grazableDM;};
	virtual double GetgrazableN() {return grazableN;};
	virtual void Cut(plantItem * cutPlantMaterial, double cut_height);
   virtual double GiveTotalDryMatter();
   virtual void Terminate(decomposable* Straw,decomposable* DeadRoot,double *& RootLengthList);
   virtual void GrazeOrCut(decomposable* Hay,double fractionLeft, bool graze);
   virtual void Harvest(decomposable* Storage, decomposable* Straw);
   virtual double GetGrazedDM();
   virtual void ClearTemporaryVariables();
   virtual double Nmax();
};

#endif

