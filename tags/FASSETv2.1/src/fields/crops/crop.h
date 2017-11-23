/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef CROP_H
   #define CROP_H

#include <message.h>
#include <soil.h>
#include <budget.h>
#include "phenology.h"
#include <nitrogen.h>
#include <pplantit.h>
#include <pfeedit.h>

class crop : public base
{
private:
	crop& operator=(const crop& c); // Dissable the compilers generation of default assignment operator.
   crop();

protected:
	enum {MaxPhases=9,MaxMeasuredPoints = 50};
   phenology* Phenology;

   bool
      terminated,
		WinterSeed,
      FillFlag,
      CutOrHarvested;
	int
      NumOfLayers;

   nitrogen
   	Nitrogen,
   	AccumulatedRootNDeposit,
      AccumulatedTopNDeposit,
   	AccumulatedNProduction;

   /*
      OBSOLETE VARIABLES:
      maxCropAreaFraction,
		TempSumR1,
		TempSumR2,
      fNitrogenAtFlowering,
      CurrentNPct,
		FractionInStorage,
      DryMatterVegTopLinear,
   */

	double // PARAMETERS
      DMCostPerN,  //Needed for external crop parameterisation of pea
      MaxFixPerDM, //Needed for external crop parameterisation of pea
      AmmoniumUptakeRate,
      CoeffGreenIdx,
      colonialisationRate,
      Conversfactor,
      ConversionCoefficient,
      DecayRoot,
      FillFactor,
      GerminationConversion,
      InitialCAI,
      InitialRootDepth,
      InterceptCoeff,
      k,
      LAIDistributionPar,
      LAIDMRatio,
      LAINitrogenRatio,
      MaxAllocToRoot,
      MaxDMtemp,
      MaxN_Root,
      MaxN_Store,
      MaxPlantHeight,
      MaxRadUseEff,
      MaxRootDepthCrop,
      MinAllocToRoot,
      MinDMtemp,
      MinimumRootDens,
      MinimumSenescense,
      MinimumSoilAmmonium,
      MinimumSoilNitrate,
      MinN_Root,
      MinN_Store,
      NCurveParam,
      NitrateUptakeRate,
      NPctMax,
      NPctMin,
      PhotoSyntActPart,
      PowDM,
      ReducedNInYellow,
      RhizoDepositFraction,
      RootDensAtBottom,
      RootPentrRate,
      RootpF,
      SpecificRootLength,
      StoreForFilling,
      TempRootMin,
      TopFraction,
      WaterDeficitGrainFill,
      WaterDeficitLagPhase,
      WaterDeficitVegGrowth;

	double // STATE VARIABLES
      AccumulatedDMProduction,
      AccumulatedNFixation,
      AccumulatedRootDeposit,
      AccumulatedRootRespiration,
      AccumulatedTopDeposit,
      DeltaDMTop,
      DryMatterRoot,
      DryMatterStorage,
      DryMatterTotalStandVegTop,
      DryMatterVegTop,
      fNitrogenAfterRipe,
      fractionOfSpace,
      GreenCropAreaIndex,
      InitialSeedDM,
      InterceptedRadiation,
      maxGLAI,
      MaxRootDepth,
      PlantHeight,
      RelativeDensity,                    // Relative to standard sowing density
      Rg,
      RootDensAtSurf,
      RootDepth,
      RootRespiration,
      RootTranslocation,
      SeedDM,
      temp,
      TempSumRoot,
      totalLAIStand,
      TotalRootLength,
      TransferableStorage,
      TransferedDMToStorage,
      TranspirationRatio,
      YellowCropAreaIndex;

   decomposable * rootMatter;
   decomposable * topMatter;
	long double RootDistrParm;
   int MeasuredNumber;
   double DateMeasured[MaxMeasuredPoints];
   double DMmeasured[MaxMeasuredPoints],GLAIMeasured[MaxMeasuredPoints],NTopMeasured[MaxMeasuredPoints];   // used for forcing simulation to observations
   double DMmeasuredDeriv[MaxMeasuredPoints],GLAIMeasuredDeriv[MaxMeasuredPoints],NTopMeasuredDeriv[MaxMeasuredPoints];
	double RootLengthList[MaxSoilLayers];
	double LayerThickness[MaxSoilLayers];
   bool UnderSown;                                  // added by JB to ensure that undersown crops are not deleted
   bool GrowthContinue;										 // growthContinue indicates if crop can continue growing the next season
   budget Nbudget;                                  // budget for N
   budget N15budget;                                // budget for N15
   budget DMbudget;                                 // budget for DM
   soil * aSoil;

   virtual double TemperatureEffect(double t);
	virtual double TargetFunction(double az,double RootDepth,double TotalRootLength,double RootDensAtBottom);
   virtual double RootPenetrationReduction();
	virtual void   CalcFormParameter(long double * az,double RootDepth,double TotalRootLength,double RootDensAtBottom);
	virtual double CalcRootDensAtSurf(long double az,double RootDepth,double RootDensAtBottom);
	virtual double RootLengthInInterval(double startdepth,double thickness);
	virtual void   CalcRootGrowth();
	virtual double DeltaDryMatter();
   virtual double OptimalDeltaDryMatter();
   virtual double FractionToRoot();
	virtual void   TransferDryMatterToRoot(double * deltaDryMatt);
   virtual double MaxTransferable();
	virtual void   TransferDryMatterToStorage(double * deltaDryMatt);
	virtual void   CalcLeafAreaIndices();
   virtual double Nmax();
   virtual double Nmin();
   virtual double NpctMaxVegTop();
   virtual double NpctMinVegTop();
   virtual double NitrogenDemand();
   virtual void   RootDecay(double RootTranslocation);
   virtual void   CalcRootDistribution();
   virtual int    RootDistrIsLinear();


public:
	crop(const char * aName, const int aIndex, const base * aOwner);
   crop(const crop& acrop);
   virtual ~crop();

   string CropName;
   string PlantItemName;
   string StrawItemName;

	virtual crop* clone(){return new crop(*this);};
   virtual double GiveEvapFactor();
   virtual double GetAndFlushTransferredNitrogen(){return 0;};
   virtual double GetAndFlushTransferredCarbon(){return 0;};
	virtual void AssignRootParameters(soil* CropSoil);
	virtual int IrrigationDemand();
	virtual void SetExpectedYieldFactor(double factor);
	virtual void Sow(double SeedDensDryMatt,double RelativeDensity,double NitrogenInSeed);
	//! harvests all aboveground dry matter. Is used for harvesting non-forage crops
	/*!
   \param storage pointer to an object containing the maincrop product (e.g. grain)
   \param  Straw pointer to an object containing the remaining crop product (e.g. Straw)
   */
   virtual void Harvest(decomposable* Storage, decomposable* Straw);
   //!kills the crop
   /*!terminator crop and partitions the dry matter
   \param  straw contains the aboveground dry matter
   \param  DeadRoot  the belowground dry matter
   \param  RootLengthList not sure
   */
   virtual void Terminate(decomposable* Straw,decomposable* DeadRoot,double *& RootLengthList);
   //!harvest vegetation above a certain height.  Is used for forage crops. Not currently used
   /*!
   \param cutPlantMaterial pointer to an object containing material harvested. Amount is in grams per metre squared.
   \param  cut_height cut height in centimetres
   */
   virtual void Cut(plantItem * cutPlantMaterial, double cut_height);
	virtual void Graze(plantItem * cutPlantMaterial, double cut_height);
   virtual void KillRootsBelow(double depth, decomposable *DeadRoot, double* RootList);
	virtual void StreamKeyData(fstream& os, bool header);

   // N functions
	virtual nitrogen NitrogenInStorage();
	virtual nitrogen NitrogenInVegTop();
	virtual nitrogen NitrogenInRoot();
	virtual nitrogen GiveTotalNitrogen();

	virtual double GiveLeafAreaIndex();
   virtual double LeafAreaFraction(double height) const;
	virtual double TotalLeafAreaIndex(double height,double thickness) const;
	virtual double GreenLeafAreaIndex(double height,double thickness);
   virtual double YellowLeafAreaIndex(double height,double thickness);
	virtual double GiveYellowLeafAreaIndex();
	virtual double InterceptionCapacity();
	virtual double GiveDryMatterStorage();
	virtual double GiveDryMatterVegTop();
	virtual double GiveDryMatterRoot();
   virtual double GiveDryMatterRootInInterval(double startDep, double thick);
   virtual double GiveNRootInInterval(double startDep, double thick);
   virtual double GiveExtCoeff() const;
   virtual double GiveCropHeight() {return PlantHeight;};
   virtual double GetDailyDMProduction() {return DeltaDMTop;};
	virtual double * GiveRootLengthList();
   virtual double fTW();
   virtual double fTW(double temperature);
   virtual double fNitrogen();
   virtual double fNitrogenCurve();
	virtual void GetStatus (double * aRootRadius,
									double * aRootpF,
									double * aNitrogenDemand,
									double * aNitrateUptakeRate,
									double * aAmmoniumUptakeRate,
                           double * aMinimumSoilNitrate,
                           double * aMinimumSoilAmmonium);
	virtual void NitrogenAndTranspiration(nitrogen aNitrogenUptake,double aTranspirationRatio);
   virtual double Update(double ActivePar);
   virtual void UpdateHeight();
   virtual double GetNFixationThisDay(){return 0.0;};
   virtual decomposable* GetRootMatter(){return rootMatter;};
   virtual decomposable* GetTopMatter(){return topMatter;};
   virtual double getLastNUptake() {return 0.0;};

   virtual string GetCropName() {return CropName;};
   virtual bool GetTerminated() {return terminated;};
   virtual bool ReadyForRemoval() {return terminated && (!rootMatter>0.0) && (!topMatter>0.0);};
   virtual void SetUndersown(bool isPermanent) {UnderSown = isPermanent;}
   virtual void SetDryMatterVegTopTotalStand(double DMVT) {DryMatterTotalStandVegTop = DMVT;}
   virtual void SetTotalLAIStand(double TLS) {totalLAIStand=TLS;};
//   virtual void SetMaxCropAreaFraction(double CAF) {maxCropAreaFraction = CAF;}
   virtual bool IsUndersown() {return UnderSown;};
	virtual bool ContinueGrowth() {return GrowthContinue;};
   virtual bool IsBareCrop() {return false;};
   virtual bool IsMaize() {return false;};
   virtual void Add(crop* aCrop, double fraction);

   // Phenology
   virtual bool ReadyForHarvest() {return Phenology->Ripe();};
   virtual bool Sown() {return Phenology->Sown();};

   virtual void SetSoilPointer(soil * sp){aSoil=sp;};
   virtual void SetRUEfactor(double f);
   virtual void SetMinDMtemp(double aTemp) {MinDMtemp = aTemp;};
   virtual void SetMaxPlantHeight(double aHeight) {MaxPlantHeight = aHeight;};
//   virtual void CutPlant(double CuttingHeight,double& DM, double& N, double& digest) {};
   virtual void ReadParameters(char* filename);
   virtual void MeasuredData();
   virtual double GetBulkDensity(){return 1400;}; // Guesstimate. This funtion should only be called for grazable crops

   // Budget functions
   virtual void StartBudget();
	virtual bool EndBudget(double * NRemain, double * DMRemain);
   virtual budget* GetNbudget() {return &Nbudget;};
   virtual budget* GetDMbudget() {return &DMbudget;};

   virtual feedItem *  GetAvailability(double cutHeight, int animalType) {return NULL;}
	virtual double GiveStandingDryMatter() {return (GiveDryMatterStorage()+GiveDryMatterVegTop());};
	virtual void SetGrazed(double grazing_height, double DMGrazed) {};
	virtual double GetgrazableDM() {return 0.0;};
	virtual double GetgrazableN() {return 0.0;};
	virtual double GetgrazableC() {return 0.0;};
	virtual double GetAvailableStandingDM(double cutHeight,bool useUptakeWeight=true){return 0.0;};
	virtual double PotentialDryMatter(double radiation, double temperature);
   virtual void ClearTemporaryVariables(){};
   virtual double GetGrazedDM() {return 0.0;};

   virtual void SetCropName(char * aName){CropName = aName;};
};

#endif

