/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef CONTRPARM_H
   #define CONTRPARM_H

class controlParameters
{
   private:
      int
         waterFlowMethod,
         operationOffset,
         volatModel,
         fertiliserUnit,
         singleField;
      bool
         writeTuples,
         uniSizeFields,
         flexSizeFields,
         simpleTemperatureModel,
         AirToSoilTemperature,
         measuredSoilTemperature,
         WriteSoilTemperature,
         writeDetails,
         reductionParm,
         ForcedOperations,
         ForcedHarvest,
         OrganicFarm,
         GrassWeed,
         simTupleOpened,
         obsTupleOpened;
      double
         harvestWaste,
         StrawLeft;
      string
         farmID;

   public:
      controlParameters()
      {
		  uniSizeFields			 = false;
		  flexSizeFields		 = false;
         waterFlowMethod         = 1;// 0: Darcy; 1: Tipping Bucket; 2: Richards; 3: Combination, #1 followed by #0
         simpleTemperatureModel  = false;
         AirToSoilTemperature    = false;
         reductionParm           = false;
         harvestWaste            = 0.0;
         StrawLeft               = 0.25;
         GrassWeed               = false;
         OrganicFarm             = false;
         measuredSoilTemperature = false;
         WriteSoilTemperature    = false;
         ForcedHarvest           = false;
         ForcedOperations        = false;
         volatModel					= 0;    //0: emission factor, 1: dynamic modelling
         singleField             =-1;    // All fields used by default
         fertiliserUnit          = 0;    // 0: fertiliser per ha; 1: fertiliser total amount; higher values ad hoc mode(s), see fieldOrder
#ifdef TUPLE
         simTupleOpened          = false;
         obsTupleOpened          = false;
         farmID                  = "";
         writeTuples             = false;
#endif
      };
      void SetWaterFlowMethod(int w){waterFlowMethod=w;};
      void SetSimpleTemperatureModel(bool s){simpleTemperatureModel=s;};
      void SetAirToSoilTemperature(bool s){AirToSoilTemperature=s;};
      void SetMeasuredSoilTemperature(bool s) {measuredSoilTemperature=s;};
      void SetWriteSoilTemperature(bool s) {WriteSoilTemperature=s;};
      void SetHarvestWaste(double w){harvestWaste=w;};
      void SetStrawLeft(double w){StrawLeft=w;};
      void SetUseReductionParameters(bool u){reductionParm=u;};
      void SetGrassWeed(bool gw){GrassWeed=gw;};
      void SetOrganicFarm(bool organic){OrganicFarm=organic;};
      void SetForcedOperations(bool fo){ForcedOperations=fo;};
		void SetForcedHarvest(bool fo){ForcedHarvest=fo;};
      void SetoperationOffset(int numberOfYears){operationOffset=numberOfYears;}
      void SetFieldsFlexSize(bool flexSize){flexSizeFields=flexSize;};
      void SetFieldsUniSize(bool uniSize){uniSizeFields=uniSize;};
      void SetvolatModel(int value) {volatModel=value;};
      void SetSingleField(int nr) {singleField=nr;};
      void SetFertiliserUnit(int fu) {fertiliserUnit=fu;};
#ifdef TUPLE
      void SetSimTupleHasBeenOpened(bool t){simTupleOpened=t;};
      void SetObsTupleHasBeenOpened(bool t){obsTupleOpened=t;};
      void SetFarmID(string id){farmID=id;};
      void SetWriteTuples(bool wt){writeTuples=wt;};

      bool GetWriteTuples(){return writeTuples;};
      string GetFarmID(){return farmID;};
      bool GetObsTupleHasBeenOpened(){return obsTupleOpened;};
      bool GetSimTupleHasBeenOpened(){return simTupleOpened;};
#endif
      int  GetWaterFlowMethod(){return waterFlowMethod;};
      bool GetSimpleTemperatureModel(){return simpleTemperatureModel;};
      bool GetAirToSoilTemperature(){return AirToSoilTemperature;};
      bool GetMeasuredSoilTemperature() {return measuredSoilTemperature;};
      bool GetWriteSoilTemperature() {return WriteSoilTemperature;};
      double GetHarvestWaste(){return harvestWaste;};
      double GetStrawLeft(){return StrawLeft;};
      bool GetUseReductionParameters(){return reductionParm;};
      bool GetGrassWeed(){return GrassWeed;};
      bool GetOrganicFarm(){return OrganicFarm;};
      bool GetForcedOperations(){return ForcedOperations;};
      bool GetForcedHarvest(){return ForcedHarvest;};
      bool GetUniSizeFields(){return uniSizeFields;};
      bool GetFlexSizeFields() {return flexSizeFields;};
      int GetoperationOffset() {return operationOffset;};
      int GetvolatModel() {return volatModel;};
      int GetSingleField() {return singleField;};
      int GetFertiliserUnit() {return fertiliserUnit;};
};

extern controlParameters * theControlParameters;

#endif

