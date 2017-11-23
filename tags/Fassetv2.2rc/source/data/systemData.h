/*
 * system.h
 *
 *  Created on: Aug 21, 2009
 *      Author: JOVE
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_


#include "commonData.h"
#include "../base/common.h"
//#include "../base/message.h"
#include <vector>
#include <stdlib.h>


/**
 * This class contains the value for the keywords there are for each run.
 * Each keyword have also a bool *File. This represent if that key word has been read from the file
 */

class systemRunData{
private:
	string InputDirectory;
		string OutputDirectory;
		string ClimateDirectory;
		string ClimateFileName;
		string EnvironmentalIndicator;
		string EconomicIndicatorDate;

		bool InputDirectoryFile;
		bool OutputDirectoryFile;
		bool ClimateDirectoryFile;
		bool ClimateFileNameFile;
		bool EnvironmentalIndicatorFile;
		bool EconomicIndicatorDateFile;


		int ClimateFileType ;
		int EpotCalculationMethod;
		int GenerateFixedPlans;
		int NumberOfFixedYears;
		int NumberOfScenarios;
		int VolatModel;
		int WaterFlowMethodInt;
		string WaterFlowMethodString;
		int fertiliserUnit;
		int UseLPModel;
		int singleField;
		int StartYear;
		int StopYear;

		bool ClimateFileTypeFile;
		bool EpotCalculationMethodFile;
		bool GenerateFixedPlansFile;
		bool NumberOfFixedYearsFile;
		bool NumberOfScenariosFile;
		bool VolatModelFile;
		bool fertiliserUnitFile;
		bool UseLPModelFile;
		bool singleFieldFile;
		bool StartYearFile;
		bool StopYearFile;
		bool WaterFlowMethodFile;

		bool SimpleTemperatureModel;
		bool uniSizeFields;
		bool FieldsFlexSize;
		bool MeasuredSoilTemperature;
		bool AirToSoilTemperature;
		bool WriteSoilTemperature;
		bool ForcedOperations;
		bool ForcedHarvest;
		bool UseReductionParameters;
		bool GrassWeed;
		bool Organic;
		bool SkipRun;


		bool SimpleTemperatureModelFile;
		bool uniSizeFieldsFile;
		bool FieldsFlexSizeFile;
		bool MeasuredSoilTemperatureFile;
		bool AirToSoilTemperatureFile;
		bool WriteSoilTemperatureFile;
		bool ForcedOperationsFile;
		bool ForcedHarvestFile;
		bool UseReductionParametersFile;
		bool GrassWeedFile;
		bool OrganicFile;
		bool SkipRunFile;


		double HarvestWaste;
		double StrawLeft;

		bool HarvestWasteFile;
		bool StrawLeftFile;
	public:
		string getInputDirectory(){return InputDirectory;};
		string getOutputDirectory(){return OutputDirectory;};
		string getClimateDirectory(){return ClimateDirectory;};
		string getClimateFileName(){return ClimateFileName;};
		string getEnvironmentalIndicator(){return EnvironmentalIndicator;};
		string getEconomicIndicatorDate(){return EconomicIndicatorDate;};
		bool getInputDirectoryFile(){return InputDirectoryFile;};
		bool getOutputDirectoryFile(){return OutputDirectoryFile;};
		bool getClimateDirectoryFile(){return ClimateDirectoryFile;};
		bool getClimateFileNameFile(){return ClimateFileNameFile;};
		bool getEnvironmentalIndicatorFile(){return EnvironmentalIndicatorFile;};
		bool getEconomicIndicatorDateFile(){return EnvironmentalIndicatorFile;};
		int getClimateFileType (){return ClimateFileType;};
		int getEpotCalculationMethod(){return EpotCalculationMethod;};
		int getGenerateFixedPlans(){return GenerateFixedPlans;};
		int getNumberOfFixedYears(){return NumberOfFixedYears;};
		int getNumberOfScenarios(){return NumberOfScenarios;};
		int getVolatModel(){return VolatModel;};
		int getWaterFlowMethodInt(){return WaterFlowMethodInt;};
		string getWaterFlowMethodString(){return WaterFlowMethodString;};
		int getfertiliserUnit(){return fertiliserUnit;};
		int getUseLPModel(){return UseLPModel;};
		int getsingleField(){return singleField;};
		int getStartYear(){return StartYear;};
		int getStopYear(){return StopYear;};
		bool getClimateFileTypeFile(){return ClimateFileTypeFile;};
		bool getEpotCalculationMethodFile(){return ClimateFileTypeFile;};
		bool getGenerateFixedPlansFile(){return GenerateFixedPlansFile;};
		bool getNumberOfFixedYearsFile(){return NumberOfFixedYearsFile;};
		bool getNumberOfScenariosFile(){return NumberOfScenariosFile;};
		bool getVolatModelFile(){return VolatModelFile;};
		bool getfertiliserUnitFile(){return fertiliserUnitFile;};
		bool getUseLPModelFile(){return UseLPModelFile;};
		bool getsingleFieldFile(){return singleFieldFile;};
		bool getStartYearFile(){return StartYearFile;};
		bool getStopYearFile(){return StopYearFile;};
		bool getWaterFlowMethodFile(){return WaterFlowMethodFile;};

		bool getSimpleTemperatureModel(){return SimpleTemperatureModel;};
		bool getuniSizeFields(){return uniSizeFields;};
		bool getFieldsFlexSize(){return FieldsFlexSize;};
		bool getMeasuredSoilTemperature(){return MeasuredSoilTemperature;};
		bool getAirToSoilTemperature(){return AirToSoilTemperature;};
		bool getWriteSoilTemperature(){return WriteSoilTemperature;};
		bool getForcedOperations(){return ForcedOperations;};
		bool getForcedHarvest(){return ForcedHarvest;};
		bool getUseReductionParameters(){return UseReductionParameters;};
		bool getGrassWeed(){return GrassWeed;};
		bool getOrganic(){return Organic;};
		bool getSkipRun(){return SkipRun;};
		bool getSimpleTemperatureModelFile(){return SimpleTemperatureModelFile;};
		bool getuniSizeFieldsFile(){return uniSizeFieldsFile;};
		bool getFieldsFlexSizeFile(){return FieldsFlexSizeFile;};
		bool getMeasuredSoilTemperatureFile(){return MeasuredSoilTemperatureFile;};
		bool getAirToSoilTemperatureFile(){return AirToSoilTemperatureFile;};
		bool getWriteSoilTemperatureFile(){return WriteSoilTemperatureFile;};
		bool getForcedOperationsFile(){return ForcedOperationsFile;};
		bool getForcedHarvestFile(){return ForcedHarvestFile;};
		bool getUseReductionParametersFile(){return UseReductionParametersFile;};
		bool getGrassWeedFile(){return GrassWeedFile;};
		bool getOrganicFile(){return OrganicFile;};
		bool getSkipRunFile(){return OrganicFile;};
		double getHarvestWaste(){return HarvestWaste;};
		double getStrawLeft(){return StrawLeft;};

		bool getHarvestWasteFile(){return HarvestWasteFile;};
		bool getStrawLeftFile(){return StrawLeftFile;};

		void setInputDirectory(string aInputDirectory){InputDirectory=aInputDirectory;};
		void setOutputDirectory(string aOutputDirectory){OutputDirectory=aOutputDirectory;};
		void setClimateDirectory(string aClimateDirectory){ClimateDirectory=aClimateDirectory;};
		void setClimateFileName(string aClimateFileName){ClimateFileName=aClimateFileName;};
		void setEnvironmentalIndicator(string aEnvironmentalIndicator){EnvironmentalIndicator=aEnvironmentalIndicator;};
		void setEconomicIndicatorDate(string aEconomicIndicatorDate){EconomicIndicatorDate=aEconomicIndicatorDate;};
		void setInputDirectoryFile(bool aInputDirectoryFile){InputDirectoryFile=aInputDirectoryFile;};
		void setOutputDirectoryFile(bool aOutputDirectoryFile){OutputDirectoryFile=aOutputDirectoryFile;};
		void setClimateDirectoryFile(bool aClimateDirectoryFile){ClimateDirectoryFile=aClimateDirectoryFile;};
		void setClimateFileNameFile(bool aClimateFileNameFile){ClimateFileNameFile=aClimateFileNameFile;};
		void setEnvironmentalIndicatorFile(bool aEnvironmentalIndicatorFile){EnvironmentalIndicatorFile=aEnvironmentalIndicatorFile;};
		void setEconomicIndicatorDateFile(bool aEconomicIndicatorDateFile){EconomicIndicatorDateFile=aEconomicIndicatorDateFile;};

		void setClimateFileType (int aClimateFileType){ClimateFileType=aClimateFileType;};
		void setEpotCalculationMethod(int aEpotCalculationMethod){EpotCalculationMethod=aEpotCalculationMethod;};
		void setGenerateFixedPlans(int aGenerateFixedPlans){GenerateFixedPlans=aGenerateFixedPlans;};
		void setNumberOfFixedYears(int aNumberOfFixedYears){NumberOfFixedYears=aNumberOfFixedYears;};
		void setNumberOfScenarios(int aNumberOfScenarios){NumberOfScenarios=aNumberOfScenarios;};
		void setVolatModel(int aVolatModel){VolatModel=aVolatModel;};
		void setWaterFlowMethodInt(int aWaterFlowMethodInt){WaterFlowMethodInt=aWaterFlowMethodInt;};
		void setWaterFlowMethodString(string aWaterFlowMethodString){WaterFlowMethodString=aWaterFlowMethodString;};
		void setfertiliserUnit(int afertiliserUnit){fertiliserUnit=afertiliserUnit;};
		void setUseLPModel(int aUseLPModel){UseLPModel=aUseLPModel;};
		void setsingleField(int asingleField){singleField=asingleField;};
		void setStartYear(int aStartYear){StartYear=aStartYear;};
		void setStopYear(int aStopYear){StopYear=aStopYear;};
		void setClimateFileTypeFile(bool aClimateFileTypeFile){ClimateFileTypeFile=aClimateFileTypeFile;};
		void setEpotCalculationMethodFile(bool aEpotCalculationMethodFile){EpotCalculationMethodFile=aEpotCalculationMethodFile;};
		void setGenerateFixedPlansFile(bool aGenerateFixedPlansFile){GenerateFixedPlansFile=aGenerateFixedPlansFile;};
		void setNumberOfFixedYearsFile(bool aNumberOfFixedYearsFile){NumberOfFixedYearsFile=aNumberOfFixedYearsFile;};
		void setNumberOfScenariosFile(bool aNumberOfScenariosFile){NumberOfScenariosFile=aNumberOfScenariosFile;};
		void setVolatModelFile(bool aVolatModelFile){VolatModelFile=aVolatModelFile;};
		void setfertiliserUnitFile(bool afertiliserUnitFile){fertiliserUnitFile=afertiliserUnitFile;};
		void setUseLPModelFile(bool aUseLPModelFile){UseLPModelFile=aUseLPModelFile;};
		void setsingleFieldFile(bool asingleFieldFile){singleFieldFile=asingleFieldFile;};
		void setStartYearFile(bool aStartYearFile){StartYearFile=aStartYearFile;};
		void setStopYearFile(bool aStopYearFile){StopYearFile=aStopYearFile;};
		void setWaterFlowMethodFile(bool aWaterFlowMethodFile){WaterFlowMethodFile=aWaterFlowMethodFile;};
		void setSimpleTemperatureModel(bool aSimpleTemperatureModel){SimpleTemperatureModel=aSimpleTemperatureModel;};
		void setuniSizeFields(bool auniSizeFields){uniSizeFields=auniSizeFields;};
		void setFieldsFlexSize(bool aFieldsFlexSize){FieldsFlexSize=aFieldsFlexSize;};
		void setMeasuredSoilTemperature(bool aMeasuredSoilTemperature){MeasuredSoilTemperature=aMeasuredSoilTemperature;};
		void setAirToSoilTemperature(bool aAirToSoilTemperature){AirToSoilTemperature=aAirToSoilTemperature;};
		void setWriteSoilTemperature(bool aWriteSoilTemperature){aWriteSoilTemperature=aWriteSoilTemperature;};
		void setForcedOperations(bool aForcedOperations){ForcedOperations=aForcedOperations;};
		void setForcedHarvest(bool aForcedHarvest){ForcedHarvest=aForcedHarvest;};
		void setUseReductionParameters(bool aUseReductionParameters){UseReductionParameters=aUseReductionParameters;};
		void setGrassWeed(bool aGrassWeed){GrassWeed=aGrassWeed;};
		void setOrganic(bool aOrganic){Organic=aOrganic;};
		void setSkipRun(bool aSkipRun){SkipRun=aSkipRun;};
		void setSimpleTemperatureModelFile(bool aSimpleTemperatureModelFile){SimpleTemperatureModelFile=aSimpleTemperatureModelFile;};
		void setuniSizeFieldsFile(bool auniSizeFieldsFile){uniSizeFieldsFile=auniSizeFieldsFile;};
		void setFieldsFlexSizeFile(bool aFieldsFlexSizeFile){FieldsFlexSizeFile=aFieldsFlexSizeFile;};
		void setMeasuredSoilTemperatureFile(bool aMeasuredSoilTemperatureFile){MeasuredSoilTemperatureFile=aMeasuredSoilTemperatureFile;};
		void setAirToSoilTemperatureFile(bool aAirToSoilTemperatureFile){AirToSoilTemperatureFile=aAirToSoilTemperatureFile;};
		void setWriteSoilTemperatureFile(bool aWriteSoilTemperatureFile){WriteSoilTemperatureFile=aWriteSoilTemperatureFile;};
		void setForcedOperationsFile(bool aForcedOperationsFile){ForcedOperationsFile=aForcedOperationsFile;};
		void setForcedHarvestFile(bool aForcedHarvestFile){ForcedHarvestFile=aForcedHarvestFile;};
		void setUseReductionParametersFile(bool aUseReductionParametersFile){UseReductionParametersFile=aUseReductionParametersFile;};
		void setGrassWeedFile(bool aGrassWeedFile){GrassWeedFile=aGrassWeedFile;};
		void setOrganicFile(bool aOrganicFile){OrganicFile=aOrganicFile;};
		void setSkipRunFile(bool aSkipRunFile){SkipRunFile=aSkipRunFile;};
		void setHarvestWaste(double aHarvestWaste){HarvestWaste=aHarvestWaste;};
		void setStrawLeft(double aStrawLeft){StrawLeft=aStrawLeft;};

		void setHarvestWasteFile(bool aHarvestWasteFile){HarvestWasteFile=aHarvestWasteFile;};
		void setStrawLeftFile(bool aStrawLeftFile){StrawLeftFile=aStrawLeftFile;};
};
/**
 * This class singleton pattern which mean that there can be only one instance of the class and access goes trough get_instance which take cares of initialize if needed
 * It also contains the global keywords for system.dat
 * Each keyword have also a bool *File. This represent if that key word has been read from the file
 */
class systemData:commonData{
private:
    std::vector<systemRunData> runData;
public:
	string getOutputDirectory(){return runData[currentRun].getOutputDirectory();};
	string getInputDirectory(){return runData[currentRun].getInputDirectory();};
	string getClimateDirectory(){return runData[currentRun].getClimateDirectory();};
		string getClimateFileName(){return runData[currentRun].getClimateFileName();};
		string getEnvironmentalIndicator(){return runData[currentRun].getEnvironmentalIndicator();};
		string getEconomicIndicatorDate(){return runData[currentRun].getEconomicIndicatorDate();};
		bool getInputDirectoryFile(){return runData[currentRun].getInputDirectoryFile();};
		bool getOutputDirectoryFile(){return runData[currentRun].getOutputDirectoryFile();};
		bool getClimateDirectoryFile(){return runData[currentRun].getClimateDirectoryFile();};
		bool getClimateFileNameFile(){return runData[currentRun].getClimateFileNameFile();};
		bool getEnvironmentalIndicatorFile(){return runData[currentRun].getEnvironmentalIndicatorFile();};
		bool getEconomicIndicatorDateFile(){return runData[currentRun].getEnvironmentalIndicatorFile();};
		int getClimateFileType (){return runData[currentRun].getClimateFileType();};
		int getEpotCalculationMethod(){return runData[currentRun].getEpotCalculationMethod();};
		int getGenerateFixedPlans(){return runData[currentRun].getGenerateFixedPlans();};
		int getNumberOfFixedYears(){return runData[currentRun].getNumberOfFixedYears();};
		int getNumberOfScenarios(){return runData[currentRun].getNumberOfScenarios();};
		int getVolatModel(){return runData[currentRun].getVolatModel();};
		int getWaterFlowMethodInt(){return runData[currentRun].getWaterFlowMethodInt();};
		string getWaterFlowMethodString(){return runData[currentRun].getWaterFlowMethodString();};
		int getfertiliserUnit(){return runData[currentRun].getfertiliserUnit();};
		int getUseLPModel(){return runData[currentRun].getUseLPModel();};
		int getsingleField(){return runData[currentRun].getsingleField();};
		int getStartYear(){return runData[currentRun].getStartYear();};
		int getStopYear(){return runData[currentRun].getStopYear();};
		bool getClimateFileTypeFile(){return runData[currentRun].getClimateFileTypeFile();};
		bool getEpotCalculationMethodFile(){return runData[currentRun].getClimateFileTypeFile();};
		bool getGenerateFixedPlansFile(){return runData[currentRun].getGenerateFixedPlansFile();};
		bool getNumberOfFixedYearsFile(){return runData[currentRun].getNumberOfFixedYearsFile();};
		bool getNumberOfScenariosFile(){return runData[currentRun].getNumberOfScenariosFile();};
		bool getgetVolatModelFile(){return runData[currentRun].getVolatModelFile();};
		bool getfertiliserUnitFile(){return runData[currentRun].getfertiliserUnitFile();};
		bool getUseLPModelFile(){return runData[currentRun].getUseLPModelFile();};
		bool getsingleFieldFile(){return runData[currentRun].getsingleFieldFile();};
		bool getStartYearFile(){return runData[currentRun].getStartYearFile();};
		bool getStopYearFile(){return runData[currentRun].getStopYearFile();};
		bool getWaterFlowMethodFile(){return runData[currentRun].getWaterFlowMethodFile();};

		bool getSimpleTemperatureModel(){return runData[currentRun].getSimpleTemperatureModel();};
		bool getuniSizeFields(){return runData[currentRun].getuniSizeFields();};
		bool getFieldsFlexSize(){return runData[currentRun].getFieldsFlexSize();};
		bool getMeasuredSoilTemperature(){return runData[currentRun].getMeasuredSoilTemperature();};
		bool getAirToSoilTemperature(){return runData[currentRun].getAirToSoilTemperature();};
		bool getWriteSoilTemperature(){return runData[currentRun].getWriteSoilTemperature();};
		bool getForcedOperations(){return runData[currentRun].getForcedOperations();};
		bool getForcedHarvest(){return runData[currentRun].getForcedHarvest();};
		bool getUseReductionParameters(){return runData[currentRun].getUseReductionParameters();};
		bool getGrassWeed(){return runData[currentRun].getGrassWeed();};
		bool getOrganic(){return runData[currentRun].getOrganic();};
		bool getSkipRun(){return runData[currentRun].getSkipRun();};
		bool getSimpleTemperatureModelFile(){return runData[currentRun].getSimpleTemperatureModelFile();};
		bool getuniSizeFieldsFile(){return runData[currentRun].getuniSizeFieldsFile();};
		bool getFieldsFlexSizeFile(){return runData[currentRun].getFieldsFlexSizeFile();};
		bool getMeasuredSoilTemperatureFile(){return runData[currentRun].getMeasuredSoilTemperatureFile();};
		bool getAirToSoilTemperatureFile(){return runData[currentRun].getAirToSoilTemperatureFile();};
		bool getWriteSoilTemperatureFile(){return runData[currentRun].getWriteSoilTemperatureFile();};
		bool getForcedOperationsFile(){return runData[currentRun].getForcedOperationsFile();};
		bool getForcedHarvestFile(){return runData[currentRun].getForcedHarvestFile();};
		bool getUseReductionParametersFile(){return runData[currentRun].getUseReductionParametersFile();};
		bool getGrassWeedFile(){return runData[currentRun].getGrassWeedFile();};
		bool getOrganicFile(){return runData[currentRun].getOrganicFile();};
		bool getSkipRunFile(){return runData[currentRun].getOrganicFile();};
		double getHarvestWaste(){return runData[currentRun].getHarvestWaste();};
		double getStrawLeft(){return runData[currentRun].getStrawLeft();};

		bool getHarvestWasteFile(){return runData[currentRun].getHarvestWasteFile();};
		bool getStrawLeftFile(){return runData[currentRun].getStrawLeftFile();};

		void setInputDirectory(string aInputDirectory){runData[currentRun].setInputDirectory(aInputDirectory);};
		void setOutputDirectory(string aOutputDirectory){runData[currentRun].setOutputDirectory(aOutputDirectory);};
		void setClimateDirectory(string aClimateDirectory){runData[currentRun].setClimateDirectory(aClimateDirectory);};
		void setClimateFileName(string aClimateFileName){runData[currentRun].setClimateFileName(aClimateFileName);};
		void setEnvironmentalIndicator(string aEnvironmentalIndicator){runData[currentRun].setEnvironmentalIndicator(aEnvironmentalIndicator);};
		void setEconomicIndicatorDate(string aEconomicIndicatorDate){runData[currentRun].setEconomicIndicatorDate(aEconomicIndicatorDate);};
		void setInputDirectoryFile(bool aInputDirectoryFile){runData[currentRun].setInputDirectoryFile(aInputDirectoryFile);};
		void setOutputDirectoryFile(bool aOutputDirectoryFile){runData[currentRun].setOutputDirectoryFile(aOutputDirectoryFile);};
		void setClimateDirectoryFile(bool aClimateDirectoryFile){runData[currentRun].setClimateDirectoryFile(aClimateDirectoryFile);};
		void setClimateFileNameFile(bool aClimateFileNameFile){runData[currentRun].setClimateFileNameFile(aClimateFileNameFile);};
		void setEnvironmentalIndicatorFile(bool aEnvironmentalIndicatorFile){runData[currentRun].setEnvironmentalIndicatorFile(aEnvironmentalIndicatorFile);};
		void setEconomicIndicatorDateFile(bool aEconomicIndicatorDateFile){runData[currentRun].setEconomicIndicatorDateFile(aEconomicIndicatorDateFile);};

		void setClimateFileType (int aClimateFileType){runData[currentRun].setClimateFileType(aClimateFileType);};
		void setEpotCalculationMethod(int aEpotCalculationMethod){runData[currentRun].setEpotCalculationMethod(aEpotCalculationMethod);};
		void setGenerateFixedPlans(int aGenerateFixedPlans){runData[currentRun].setGenerateFixedPlans(aGenerateFixedPlans);};
		void setNumberOfFixedYears(int aNumberOfFixedYears){runData[currentRun].setNumberOfFixedYears(aNumberOfFixedYears);};
		void setNumberOfScenarios(int aNumberOfScenarios){runData[currentRun].setNumberOfScenarios(aNumberOfScenarios);};
		void setVolatModel(int aVolatModel){runData[currentRun].setVolatModel(aVolatModel);};
		void setWaterFlowMethodInt(int aWaterFlowMethodInt){runData[currentRun].setWaterFlowMethodInt(aWaterFlowMethodInt);};
		void setWaterFlowMethodString(string aWaterFlowMethodString){runData[currentRun].setWaterFlowMethodString(aWaterFlowMethodString);};
		void setfertiliserUnit(int afertiliserUnit){runData[currentRun].setfertiliserUnit(afertiliserUnit);};
		void setUseLPModel(int aUseLPModel){runData[currentRun].setUseLPModel(aUseLPModel);};
		void setsingleField(int asingleField){runData[currentRun].setsingleField(asingleField);};
		void setStartYear(int aStartYear){runData[currentRun].setStartYear(aStartYear);};
		void setStopYear(int aStopYear){runData[currentRun].setStopYear(aStopYear);};
		void setClimateFileTypeFile(bool aClimateFileTypeFile){runData[currentRun].setClimateFileTypeFile(aClimateFileTypeFile);};
		void setEpotCalculationMethodFile(bool aEpotCalculationMethodFile){runData[currentRun].setEpotCalculationMethodFile(aEpotCalculationMethodFile);};
		void setGenerateFixedPlansFile(bool aGenerateFixedPlansFile){runData[currentRun].setGenerateFixedPlansFile(aGenerateFixedPlansFile);};
		void setNumberOfFixedYearsFile(bool aNumberOfFixedYearsFile){runData[currentRun].setNumberOfFixedYearsFile(aNumberOfFixedYearsFile);};
		void setNumberOfScenariosFile(bool aNumberOfScenariosFile){runData[currentRun].setNumberOfScenariosFile(aNumberOfScenariosFile);};
		void setVolatModelFile(bool aVolatModelFile){runData[currentRun].setVolatModelFile(aVolatModelFile);};
		void setfertiliserUnitFile(bool afertiliserUnitFile){runData[currentRun].setfertiliserUnitFile(afertiliserUnitFile);};
		void setUseLPModelFile(bool aUseLPModelFile){runData[currentRun].setUseLPModelFile(aUseLPModelFile);};
		void setsingleFieldFile(bool asingleFieldFile){runData[currentRun].setsingleFieldFile(asingleFieldFile);};
		void setStartYearFile(bool aStartYearFile){runData[currentRun].setStartYearFile(aStartYearFile);};
		void setStopYearFile(bool aStopYearFile){runData[currentRun].setStopYearFile(aStopYearFile);};
		void setWaterFlowMethodFile(bool aWaterFlowMethodFile){runData[currentRun].setWaterFlowMethodFile(aWaterFlowMethodFile);};
		void setSimpleTemperatureModel(bool aSimpleTemperatureModel){runData[currentRun].setSimpleTemperatureModel(aSimpleTemperatureModel);};
		void setuniSizeFields(bool auniSizeFields){runData[currentRun].setuniSizeFields(auniSizeFields);};
		void setFieldsFlexSize(bool aFieldsFlexSize){runData[currentRun].setFieldsFlexSize(aFieldsFlexSize);};
		void setMeasuredSoilTemperature(bool aMeasuredSoilTemperature){runData[currentRun].setMeasuredSoilTemperature(aMeasuredSoilTemperature);};
		void setAirToSoilTemperature(bool aAirToSoilTemperature){runData[currentRun].setAirToSoilTemperature(aAirToSoilTemperature);};
		void setWriteSoilTemperature(bool aWriteSoilTemperature){runData[currentRun].setWriteSoilTemperature(aWriteSoilTemperature);};
		void setForcedOperations(bool aForcedOperations){runData[currentRun].setForcedOperations(aForcedOperations);};
		void setForcedHarvest(bool aForcedHarvest){runData[currentRun].setForcedHarvest(aForcedHarvest);};
		void setUseReductionParameters(bool aUseReductionParameters){runData[currentRun].setUseReductionParameters(aUseReductionParameters);};
		void setGrassWeed(bool aGrassWeed){runData[currentRun].setGrassWeed(aGrassWeed);};
		void setOrganic(bool aOrganic){runData[currentRun].setOrganic(aOrganic);};
		void setSkipRun(bool aSkipRun){runData[currentRun].setSkipRun(aSkipRun);};
		void setSimpleTemperatureModelFile(bool aSimpleTemperatureModelFile){runData[currentRun].setSimpleTemperatureModelFile(aSimpleTemperatureModelFile);};
		void setuniSizeFieldsFile(bool auniSizeFieldsFile){runData[currentRun].setuniSizeFieldsFile(auniSizeFieldsFile);};
		void setFieldsFlexSizeFile(bool aFieldsFlexSizeFile){runData[currentRun].setFieldsFlexSizeFile(aFieldsFlexSizeFile);};
		void setMeasuredSoilTemperatureFile(bool aMeasuredSoilTemperatureFile){runData[currentRun].setMeasuredSoilTemperatureFile(aMeasuredSoilTemperatureFile);};
		void setAirToSoilTemperatureFile(bool aAirToSoilTemperatureFile){runData[currentRun].setAirToSoilTemperatureFile(aAirToSoilTemperatureFile);};
		void setWriteSoilTemperatureFile(bool aWriteSoilTemperatureFile){runData[currentRun].setWriteSoilTemperatureFile(aWriteSoilTemperatureFile);};
		void setForcedOperationsFile(bool aForcedOperationsFile){runData[currentRun].setForcedOperationsFile(aForcedOperationsFile);};
		void setForcedHarvestFile(bool aForcedHarvestFile){runData[currentRun].setForcedHarvestFile(aForcedHarvestFile);};
		void setUseReductionParametersFile(bool aUseReductionParametersFile){runData[currentRun].setUseReductionParametersFile(aUseReductionParametersFile);};
		void setGrassWeedFile(bool aGrassWeedFile){runData[currentRun].setGrassWeedFile(aGrassWeedFile);};
		void setOrganicFile(bool aOrganicFile){runData[currentRun].setOrganicFile(aOrganicFile);};
		void setSkipRunFile(bool aSkipRunFile){runData[currentRun].setSkipRunFile(aSkipRunFile);};
		void setHarvestWaste(double aHarvestWaste){runData[currentRun].setHarvestWaste(aHarvestWaste);};
		void setStrawLeft(double aStrawLeft){runData[currentRun].setStrawLeft(aStrawLeft);};

		void setHarvestWasteFile(bool aHarvestWasteFile){runData[currentRun].setHarvestWasteFile(aHarvestWasteFile);};
		void setStrawLeftFile(bool aStrawLeftFile){runData[currentRun].setStrawLeftFile(aStrawLeftFile);};

	private:

    int RunLevel;
    int OperationOffset;
    int JumpStart;
    int JumpStop;

	bool removeOldData;

	bool removeOldDataFile;

    bool RunLevelFile;
    bool OperationOffsetFile;
    bool JumpStartFile;
    bool JumpStopFile;
	int currentRun;
  public:

	int getCurrentRun(){return currentRun;};
	void setCurrentRun(int acurrentRun){
		currentRun=acurrentRun;};



    int getRunLevel(){return RunLevel;};
    int getOperationOffset(){return OperationOffset;};
    int getJumpStart(){return JumpStart;};
    int getJumpStop(){return JumpStop;};

    bool getremoveOldData(){return removeOldData;};

    bool getremoveOldDataFile(){return removeOldDataFile;};

    bool getOperationOffsetFile(){return OperationOffsetFile;};
    bool getgetJumpStartFile(){return JumpStartFile;};
    bool getJumpStopFile(){return JumpStopFile;};
    void problem();

    
    systemData(bool notNitroscape);
    systemRunData getSystemRunData(int dataNumber){return runData[dataNumber];};
    void SetSystemRunData(systemRunData data ,int dataNumber=0){runData[dataNumber]=data ;};	
    protected :
    void addRun();
    vector<string> problems;
    void loaddata();
    void loaddataNITROSCAPE();
    void checkData();
    void addrun();

};
extern systemData * theSystemData;
#endif /* SYSTEM_H_ */
