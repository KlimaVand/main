/*
 * settings.h
 *
 *  Created on: Sep 9, 2011
 *      Author: JOVE
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_
#include <string>
#include <vector>
#include "../base/commonData.h"
using namespace std;

class outputWater{
public:
	double StartDepth;
	double EndDepth;
public:
	double getStartDepth(){return StartDepth;};
	double getEndDepth(){return EndDepth;};
};
class outputNmin{
public:
	double StartDepth;
	double EndDepth;
public:
	double getStartDepth(){return StartDepth;};
	double getEndDepth(){return EndDepth;};
};
class outputPoreWater{
public:
	double Depth;
public:
	double getDepth(){return Depth;};
};
class outputTemperature{
public:
	double Depth;
public:
	double getDepth(){return Depth;};
};
class outputSoilC{
public:
	double StartDepth;
	double EndDepth;
public:
	double getStartDepth(){return StartDepth;};
	double getEndDepth(){return EndDepth;};
};
class outputSoilN{
public:
	double StartDepth;
	double EndDepth;
public:
	double getStartDepth(){return StartDepth;};
	double getEndDepth(){return EndDepth;};
};
class outputRoot{
public:

	double StartDepth;
	double EndDepth;
public:
	double getStartDepth(){return StartDepth;};
	double getEndDepth(){return EndDepth;};
};
class outputDetails{

public:
	outputDetails(){
		PlantDetails=false;
		SoilDetails=false;
		WaterDetails=false;
		CowDetails=false;
		HerdDetails=false;
		PastureDetails=false;
		OperationalDetails=false;
		ManureManageDetails=false;
		DebugSoil=false;
	}
public:
	bool getPlantDetails(){return PlantDetails;};
	bool getSoilDetails(){return SoilDetails;};
	bool getWaterDetails(){return WaterDetails;};
	bool getCowDetails(){return CowDetails;};
	bool getHerdDetails(){return HerdDetails;};
	bool getPastureDetails(){return PastureDetails;};
	bool getOperationalDetails(){return OperationalDetails;};
	bool getManureManageDetails(){return ManureManageDetails;};
	bool getDebugSoil(){return DebugSoil;};
public:
	bool PlantDetails;
	bool SoilDetails;
	bool WaterDetails;
	bool CowDetails;
	bool HerdDetails;
	bool PastureDetails;
	bool OperationalDetails;
	bool ManureManageDetails;
	bool DebugSoil;
};

class settings {
public:
	string outputDir;
	string inputDir;
	int run;
public:
	vector<outputWater> WaterData;
	vector<outputNmin> NminData;
	vector<outputPoreWater> PoreWaterData;
	vector<outputTemperature> TemperatureData;
	vector<outputSoilC> SoilOrgCData;
	vector<outputSoilN> SoilOrgNData;
	vector<outputRoot> RootData;

	outputDetails DetailsData;
	commonData * CropInformation;
	string version;
	settings();
	void readParameter();
	int getCurrentRun(){return run;};
	string getOutputDirectory(){return outputDir;}
	string getInputDirectory(){return inputDir;}
	void setOutputDir(string tmp){outputDir=tmp;}
	void setInputDir(string tmp){inputDir=tmp;}
	virtual ~settings(){
			delete CropInformation;

	};
};
extern settings * globalSettings;
#endif /* SETTINGS_H_ */
