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
		int VolatModel;
		int fertiliserUnit;
		int singleField;
		int StartYear;
		int StopYear;

		bool ClimateFileTypeFile;
		bool EpotCalculationMethodFile;
		bool VolatModelFile;
		bool fertiliserUnitFile;
		bool singleFieldFile;
		bool StartYearFile;
		bool StopYearFile;

		bool SimpleTemperatureModel;
		bool uniSizeFields;
		bool FieldsFlexSize;
		bool MeasuredSoilTemperature;
		bool AirToSoilTemperature;
		bool WriteSoilTemperature;
		bool ForcedOperations;
		bool ForcedHarvest;

		bool SimpleTemperatureModelFile;
		bool uniSizeFieldsFile;
		bool FieldsFlexSizeFile;
		bool MeasuredSoilTemperatureFile;
		bool AirToSoilTemperatureFile;
		bool WriteSoilTemperatureFile;
		bool ForcedOperationsFile;
		bool ForcedHarvestFile;


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
		int getVolatModel(){return VolatModel;};
		int getfertiliserUnit(){return fertiliserUnit;};
		int getsingleField(){return singleField;};
		int getStartYear(){return StartYear;};
		int getStopYear(){return StopYear;};
		bool getClimateFileTypeFile(){return ClimateFileTypeFile;};
		bool getEpotCalculationMethodFile(){return ClimateFileTypeFile;};
		bool getVolatModelFile(){return VolatModelFile;};
		bool getfertiliserUnitFile(){return fertiliserUnitFile;};
		bool getsingleFieldFile(){return singleFieldFile;};
		bool getStartYearFile(){return StartYearFile;};
		bool getStopYearFile(){return StopYearFile;};

		bool getSimpleTemperatureModel(){return SimpleTemperatureModel;};
		bool getuniSizeFields(){return uniSizeFields;};
		bool getFieldsFlexSize(){return FieldsFlexSize;};
		bool getMeasuredSoilTemperature(){return MeasuredSoilTemperature;};
		bool getAirToSoilTemperature(){return AirToSoilTemperature;};
		bool getWriteSoilTemperature(){return WriteSoilTemperature;};
		bool getForcedOperations(){return ForcedOperations;};
		bool getForcedHarvest(){return ForcedHarvest;};

		bool getSimpleTemperatureModelFile(){return SimpleTemperatureModelFile;};
		bool getuniSizeFieldsFile(){return uniSizeFieldsFile;};
		bool getFieldsFlexSizeFile(){return FieldsFlexSizeFile;};
		bool getMeasuredSoilTemperatureFile(){return MeasuredSoilTemperatureFile;};
		bool getAirToSoilTemperatureFile(){return AirToSoilTemperatureFile;};
		bool getWriteSoilTemperatureFile(){return WriteSoilTemperatureFile;};
		bool getForcedOperationsFile(){return ForcedOperationsFile;};
		bool getForcedHarvestFile(){return ForcedHarvestFile;};

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
		void setVolatModel(int aVolatModel){VolatModel=aVolatModel;};
		void setfertiliserUnit(int afertiliserUnit){fertiliserUnit=afertiliserUnit;};
		void setsingleField(int asingleField){singleField=asingleField;};
		void setStartYear(int aStartYear){StartYear=aStartYear;};
		void setStopYear(int aStopYear){StopYear=aStopYear;};
		void setClimateFileTypeFile(bool aClimateFileTypeFile){ClimateFileTypeFile=aClimateFileTypeFile;};
		void setEpotCalculationMethodFile(bool aEpotCalculationMethodFile){EpotCalculationMethodFile=aEpotCalculationMethodFile;};
		void setVolatModelFile(bool aVolatModelFile){VolatModelFile=aVolatModelFile;};
		void setfertiliserUnitFile(bool afertiliserUnitFile){fertiliserUnitFile=afertiliserUnitFile;};
		void setsingleFieldFile(bool asingleFieldFile){singleFieldFile=asingleFieldFile;};
		void setStartYearFile(bool aStartYearFile){StartYearFile=aStartYearFile;};
		void setStopYearFile(bool aStopYearFile){StopYearFile=aStopYearFile;};
		void setSimpleTemperatureModel(bool aSimpleTemperatureModel){SimpleTemperatureModel=aSimpleTemperatureModel;};
		void setuniSizeFields(bool auniSizeFields){uniSizeFields=auniSizeFields;};
		void setFieldsFlexSize(bool aFieldsFlexSize){FieldsFlexSize=aFieldsFlexSize;};
		void setMeasuredSoilTemperature(bool aMeasuredSoilTemperature){MeasuredSoilTemperature=aMeasuredSoilTemperature;};
		void setAirToSoilTemperature(bool aAirToSoilTemperature){AirToSoilTemperature=aAirToSoilTemperature;};
		void setWriteSoilTemperature(bool aWriteSoilTemperature){aWriteSoilTemperature=aWriteSoilTemperature;};
		void setForcedOperations(bool aForcedOperations){ForcedOperations=aForcedOperations;};
		void setForcedHarvest(bool aForcedHarvest){ForcedHarvest=aForcedHarvest;};

		void setSimpleTemperatureModelFile(bool aSimpleTemperatureModelFile){SimpleTemperatureModelFile=aSimpleTemperatureModelFile;};
		void setuniSizeFieldsFile(bool auniSizeFieldsFile){uniSizeFieldsFile=auniSizeFieldsFile;};
		void setFieldsFlexSizeFile(bool aFieldsFlexSizeFile){FieldsFlexSizeFile=aFieldsFlexSizeFile;};
		void setMeasuredSoilTemperatureFile(bool aMeasuredSoilTemperatureFile){MeasuredSoilTemperatureFile=aMeasuredSoilTemperatureFile;};
		void setAirToSoilTemperatureFile(bool aAirToSoilTemperatureFile){AirToSoilTemperatureFile=aAirToSoilTemperatureFile;};
		void setWriteSoilTemperatureFile(bool aWriteSoilTemperatureFile){WriteSoilTemperatureFile=aWriteSoilTemperatureFile;};
		void setForcedOperationsFile(bool aForcedOperationsFile){ForcedOperationsFile=aForcedOperationsFile;};
		void setForcedHarvestFile(bool aForcedHarvestFile){ForcedHarvestFile=aForcedHarvestFile;};

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
    string orgPath;

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
		int getVolatModel(){return runData[currentRun].getVolatModel();};
		int getfertiliserUnit(){return runData[currentRun].getfertiliserUnit();};
		int getsingleField(){return runData[currentRun].getsingleField();};
		int getStartYear(){return runData[currentRun].getStartYear();};
		int getStopYear(){return runData[currentRun].getStopYear();};
		bool getClimateFileTypeFile(){return runData[currentRun].getClimateFileTypeFile();};
		bool getEpotCalculationMethodFile(){return runData[currentRun].getClimateFileTypeFile();};
		bool getgetVolatModelFile(){return runData[currentRun].getVolatModelFile();};
		bool getfertiliserUnitFile(){return runData[currentRun].getfertiliserUnitFile();};
		bool getsingleFieldFile(){return runData[currentRun].getsingleFieldFile();};
		bool getStartYearFile(){return runData[currentRun].getStartYearFile();};
		bool getStopYearFile(){return runData[currentRun].getStopYearFile();};

		bool getSimpleTemperatureModel(){return runData[currentRun].getSimpleTemperatureModel();};
		bool getuniSizeFields(){return runData[currentRun].getuniSizeFields();};
		bool getFieldsFlexSize(){return runData[currentRun].getFieldsFlexSize();};
		bool getMeasuredSoilTemperature(){return runData[currentRun].getMeasuredSoilTemperature();};
		bool getAirToSoilTemperature(){return runData[currentRun].getAirToSoilTemperature();};
		bool getWriteSoilTemperature(){return runData[currentRun].getWriteSoilTemperature();};
		bool getForcedOperations(){return runData[currentRun].getForcedOperations();};
		bool getForcedHarvest(){return runData[currentRun].getForcedHarvest();};


		bool getSimpleTemperatureModelFile(){return runData[currentRun].getSimpleTemperatureModelFile();};
		bool getuniSizeFieldsFile(){return runData[currentRun].getuniSizeFieldsFile();};
		bool getFieldsFlexSizeFile(){return runData[currentRun].getFieldsFlexSizeFile();};
		bool getMeasuredSoilTemperatureFile(){return runData[currentRun].getMeasuredSoilTemperatureFile();};
		bool getAirToSoilTemperatureFile(){return runData[currentRun].getAirToSoilTemperatureFile();};
		bool getWriteSoilTemperatureFile(){return runData[currentRun].getWriteSoilTemperatureFile();};
		bool getForcedOperationsFile(){return runData[currentRun].getForcedOperationsFile();};
		bool getForcedHarvestFile(){return runData[currentRun].getForcedHarvestFile();};


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
		void setVolatModel(int aVolatModel){runData[currentRun].setVolatModel(aVolatModel);};
		void setfertiliserUnit(int afertiliserUnit){runData[currentRun].setfertiliserUnit(afertiliserUnit);};
		void setsingleField(int asingleField){runData[currentRun].setsingleField(asingleField);};
		void setStartYear(int aStartYear){runData[currentRun].setStartYear(aStartYear);};
		void setStopYear(int aStopYear){runData[currentRun].setStopYear(aStopYear);};
		void setClimateFileTypeFile(bool aClimateFileTypeFile){runData[currentRun].setClimateFileTypeFile(aClimateFileTypeFile);};
		void setEpotCalculationMethodFile(bool aEpotCalculationMethodFile){runData[currentRun].setEpotCalculationMethodFile(aEpotCalculationMethodFile);};
		void setVolatModelFile(bool aVolatModelFile){runData[currentRun].setVolatModelFile(aVolatModelFile);};
		void setfertiliserUnitFile(bool afertiliserUnitFile){runData[currentRun].setfertiliserUnitFile(afertiliserUnitFile);};
		void setsingleFieldFile(bool asingleFieldFile){runData[currentRun].setsingleFieldFile(asingleFieldFile);};
		void setStartYearFile(bool aStartYearFile){runData[currentRun].setStartYearFile(aStartYearFile);};
		void setStopYearFile(bool aStopYearFile){runData[currentRun].setStopYearFile(aStopYearFile);};
		void setSimpleTemperatureModel(bool aSimpleTemperatureModel){runData[currentRun].setSimpleTemperatureModel(aSimpleTemperatureModel);};
		void setuniSizeFields(bool auniSizeFields){runData[currentRun].setuniSizeFields(auniSizeFields);};
		void setFieldsFlexSize(bool aFieldsFlexSize){runData[currentRun].setFieldsFlexSize(aFieldsFlexSize);};
		void setMeasuredSoilTemperature(bool aMeasuredSoilTemperature){runData[currentRun].setMeasuredSoilTemperature(aMeasuredSoilTemperature);};
		void setAirToSoilTemperature(bool aAirToSoilTemperature){runData[currentRun].setAirToSoilTemperature(aAirToSoilTemperature);};
		void setWriteSoilTemperature(bool aWriteSoilTemperature){runData[currentRun].setWriteSoilTemperature(aWriteSoilTemperature);};
		void setForcedOperations(bool aForcedOperations){runData[currentRun].setForcedOperations(aForcedOperations);};
		void setForcedHarvest(bool aForcedHarvest){runData[currentRun].setForcedHarvest(aForcedHarvest);};

		void setSimpleTemperatureModelFile(bool aSimpleTemperatureModelFile){runData[currentRun].setSimpleTemperatureModelFile(aSimpleTemperatureModelFile);};
		void setuniSizeFieldsFile(bool auniSizeFieldsFile){runData[currentRun].setuniSizeFieldsFile(auniSizeFieldsFile);};
		void setFieldsFlexSizeFile(bool aFieldsFlexSizeFile){runData[currentRun].setFieldsFlexSizeFile(aFieldsFlexSizeFile);};
		void setMeasuredSoilTemperatureFile(bool aMeasuredSoilTemperatureFile){runData[currentRun].setMeasuredSoilTemperatureFile(aMeasuredSoilTemperatureFile);};
		void setAirToSoilTemperatureFile(bool aAirToSoilTemperatureFile){runData[currentRun].setAirToSoilTemperatureFile(aAirToSoilTemperatureFile);};
		void setWriteSoilTemperatureFile(bool aWriteSoilTemperatureFile){runData[currentRun].setWriteSoilTemperatureFile(aWriteSoilTemperatureFile);};
		void setForcedOperationsFile(bool aForcedOperationsFile){runData[currentRun].setForcedOperationsFile(aForcedOperationsFile);};
		void setForcedHarvestFile(bool aForcedHarvestFile){runData[currentRun].setForcedHarvestFile(aForcedHarvestFile);};

		void setHarvestWaste(double aHarvestWaste){runData[currentRun].setHarvestWaste(aHarvestWaste);};
		void setStrawLeft(double aStrawLeft){runData[currentRun].setStrawLeft(aStrawLeft);};

		void setHarvestWasteFile(bool aHarvestWasteFile){runData[currentRun].setHarvestWasteFile(aHarvestWasteFile);};
		void setStrawLeftFile(bool aStrawLeftFile){runData[currentRun].setStrawLeftFile(aStrawLeftFile);};

	private:

    int RunLevel;

    int JumpStart;
    int JumpStop;

	bool removeOldData;
	bool removeOldDataFile;
    bool RunLevelFile;
    bool JumpStartFile;
    bool JumpStopFile;
	int currentRun;
  public:
	string getOrgPath(){return orgPath;};
    void reload(string orgPath);
	int getCurrentRun(){return currentRun;};
	void setCurrentRun(int acurrentRun){
		currentRun=acurrentRun;};



    int getRunLevel(){return RunLevel;};
    int getJumpStart(){return JumpStart;};
    int getJumpStop(){return JumpStop;};

    bool getremoveOldData(){return removeOldData;};

    bool getremoveOldDataFile(){return removeOldDataFile;};
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
