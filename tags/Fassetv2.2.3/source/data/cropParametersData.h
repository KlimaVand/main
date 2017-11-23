/*
 * cropParametersData.h
 *
 *  Created on: Feb 15, 2010
 *      Author: JOVE
 */

#ifndef CROPPARAMETERSDATA_H_
#define CROPPARAMETERSDATA_H_
#include "commonData.h"
class cropParametersCrop{
public:
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
	      WaterDeficitVegGrowth,
	      FractionNminInRoots,
	      FractionNminInVegTop,
	      CO2Effect;

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
	      YellowCropAreaIndex,
	      RootDistrParm;


		bool // PARAMETERS
		      DMCostPerNFile,  //Needed for external crop parameterisation of pea
		      MaxFixPerDMFile, //Needed for external crop parameterisation of pea
		      AmmoniumUptakeRateFile,
		      CoeffGreenIdxFile,
		      colonialisationRateFile,
		      ConversfactorFile,
		      ConversionCoefficientFile,
		      DecayRootFile,
		      FillFactorFile,
		      GerminationConversionFile,
		      InitialCAIFile,
		      InitialRootDepthFile,
		      InterceptCoeffFile,
		      kFile,
		      LAIDistributionParFile,
		      LAIDMRatioFile,
		      LAINitrogenRatioFile,
		      MaxAllocToRootFile,
		      MaxDMtempFile,
		      MaxN_RootFile,
		      MaxN_StoreFile,
		      MaxPlantHeightFile,
		      MaxRadUseEffFile,
		      MaxRootDepthCropFile,
		      MinAllocToRootFile,
		      MinDMtempFile,
		      MinimumRootDensFile,
		      MinimumSenescenseFile,
		      MinimumSoilAmmoniumFile,
		      MinimumSoilNitrateFile,
		      MinN_RootFile,
		      MinN_StoreFile,
		      NCurveParamFile,
		      NitrateUptakeRateFile,
		      NPctMaxFile,
		      NPctMinFile,
		      PhotoSyntActPartFile,
		      PowDMFile,
		      ReducedNInYellowFile,
		      RhizoDepositFractionFile,
		      RootDensAtBottomFile,
		      RootPentrRateFile,
		      RootpFFile,
		      SpecificRootLengthFile,
		      StoreForFillingFile,
		      TempRootMinFile,
		      TopFractionFile,
		      WaterDeficitGrainFillFile,
		      WaterDeficitLagPhaseFile,
		      WaterDeficitVegGrowthFile,
		      FractionNminInRootsFile,
		      FractionNminInVegTopFile,
		      CO2EffectFile;

		bool // STATE VARIABLES
		      AccumulatedDMProductionFile,
		      AccumulatedNFixationFile,
		      AccumulatedRootDepositFile,
		      AccumulatedRootRespirationFile,
		      AccumulatedTopDepositFile,
		      DeltaDMTopFile,
		      DryMatterRootFile,
		      DryMatterStorageFile,
		      DryMatterTotalStandVegTopFile,
		      DryMatterVegTopFile,
		      fNitrogenAfterRipeFile,
		      fractionOfSpaceFile,
		      GreenCropAreaIndexFile,
		      InitialSeedDMFile,
		      InterceptedRadiationFile,
		      maxGLAIFile,
		      MaxRootDepthFile,
		      PlantHeightFile,
		      RelativeDensityFile,                    // Relative to standard sowing density
		      RgFile,
		      RootDensAtSurfFile,
		      RootDepthFile,
		      RootRespirationFile,
		      RootTranslocationFile,
		      SeedDMFile,
		      tempFile,
		      TempSumRootFile,
		      totalLAIStandFile,
		      TotalRootLengthFile,
		      TransferableStorageFile,
		      TransferedDMToStorageFile,
		      TranspirationRatioFile,
		      YellowCropAreaIndexFile,
		      RootDistrParmFile;

};
class cropParametersBare:cropParametersCrop{
	double
	TempSumStorage1,               			// Temperature sum for min allocation to storage
	TempSumStorage2,             				// Temperature sum for max allocation to storage
	MinAllocToStorage,          				// Min allocation to storage organs
	MaxAllocToStorage;        					// Max allocation to storage organs

};

class cropParametersBeet:cropParametersCrop{
public:
	double
	TempSumStorage1,               			// Temperature sum for min allocation to storage
	TempSumStorage2,             				// Temperature sum for max allocation to storage
	MinAllocToStorage,          				// Min allocation to storage organs
	MaxAllocToStorage;        					// Max allocation to storage organs
	bool
	TempSumStorage1File,
	TempSumStorage2File,
	MinAllocToStorageFile,
	MaxAllocToStorageFile;
};
class cropParametersCloverAndLucerne:cropParametersCrop{
public:
	 double
	 MaxFixPerDM,
	 PotNFix,
	 NFixationThisDay,
	 DMCostPerN;
	 bool

	 MaxFixPerDMFile,
	 PotNFixFile,
	 NFixationThisDayFile,
	 DMCostPerNFile;
};
class cropParametersPea:cropParametersCrop{
public:
	 double
	 fNitrogenAtFlowering,
	 MaxFixPerDM,
	 PotNFix,
	 NFixationThisDay,
	 DMCostPerN;
	 bool
	 fNitrogenAtFloweringFile,
	 MaxFixPerDMFile,
	 PotNFixFile,
	 NFixationThisDayFile,
	 DMCostPerNFile;
};
class cropParameterPotato:cropParametersCrop{
	double
	TempSumStorage1,               			// Temperature sum for min allocation to storage
	TempSumStorage2,             				// Temperature sum for max allocation to storage
	MinAllocToStorage,          				// Min allocation to storage organs
	MaxAllocToStorage;        					// Max allocation to storage organs
};
class cropParametersRyeAndWweat:cropParametersCrop{
public:
	double
		HeightA,                // Height constants see Olesen, 2002 for description
	      HeightB,
	      GrainNumber,            // Number of grains per m2
	      LeafNumber,             // Number of appeared leaves
	      CritLeafNo,             // Critical number of leaves
	      FinalLeafNo,            // Final leaf number
	      VernalIndex,            // Vernalization index
	      TSum;                   // Current temperature sum [Cd]
	   bool
	      Sown,                   // Sown
	      Germinated,             // Germinated
	      Emerged,                // Emerged
	      Vernalized,             // Vernalized
	      CommitFlag,             // Committed to flag leaf
	      Extending,              // Extending
	      Anthesis;               // Past anthesis
	   bool
		HeightAFile,
	      HeightBFile,
	      GrainNumberFile,
	      LeafNumberFile,
	      CritLeafNoFile,
	      FinalLeafNoFile,
	      VernalIndexFile,
	      TSumFile;
	   bool
	      SownFile,
	      GerminatedFile,
	      EmergedFile,
	      VernalizedFile,
	      CommitFlagFile,
	      ExtendingFile,
	      AnthesisFile;
};
class cropParametersRyegrass:cropParametersCrop{
public:

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

	bool  HasBeenCut;
	double 	liveOMD,
	   			deadOMD,
	   			grazableDM,
	   			grazableN,
	   			grazedDM[4],
	            uptakeWeight[4],
	            AgeClassTop[4],
	            RateOfRipeReSeed,
	            InitialLAIIncrease,
	            ReSeedDM;
	bool
	  	      MinDayLengthFile,
	  	      MinN_StrawFile,
	  	      BulkDensityFile,
	  	      RespirationTopFile,
	  	      RespirationRootFile,
	  	      TransferRateFile,
	  	      RecycleFractionFile,
	  	      TemperatureSumAfterCutFile,
	  	   	CutDelayFile;

	  	bool  HasBeenCutFile;
	  	bool 	liveOMDFile,
	  	   			deadOMDFile,
	  	   			grazableDMFile,
	  	   			grazableNFile,
	  	   			grazedDMFile[4],
	  	            uptakeWeightFile[4],
	  	            AgeClassTopFile[4],
	  	            RateOfRipeReSeedFile,
	  	            InitialLAIIncreaseFile,
	  	            ReSeedDMFile;



};

class cropParametersData:commonData{
public:


	static cropParametersData* get_instance()
    {
       static cropParametersData* instance=0;
       if(instance==0)
             instance = new cropParametersData();
       return instance;
       }

protected :
	bool loadCrop(string value, string name, cropParametersCrop &item);
	bool loadBeetAndPotato(string value, string name, cropParametersBeet &item);
	bool loadRye(string value, string name, cropParametersRyeAndWweat &item);
	bool loadRyegrass(string value, string name, cropParametersRyegrass &item);
	bool loadLucernePea(string value, string name, cropParametersPea &item);
    void loaddata();
    void checkData();
    static cropParametersData *myReference;
    cropParametersData(){
    	cout<<"Loading farm data"<<endl;
    	loaddata();
    	checkData();
      cout<<"Done with loading and cheking farm data"<<endl;

    }
};
#endif /* CROPPARAMETERSDATA_H_ */
