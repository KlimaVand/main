/*
 * outputData.h
 *
 *  Created on: Sep 9, 2009
 *      Author: JOVE
 */
//#include "../base/message.h"

#include "commonData.h"

#ifndef OUTPUT_H_
#define OUTPUT_H_
class outputWater{
private:
	double StartDepth;
	double EndDepth;

	bool StartDepthFile;
	bool EndDepthFile;
public:
	void setStartDepth(double aStartDepth){StartDepth=aStartDepth;};
	void setEndDepth(double aEndDepth){EndDepth=aEndDepth;};

	void setStartDepthFile(bool aStartDepthFile){StartDepthFile=aStartDepthFile;};
	void setEndDepthFile(bool aEndDepthFile){EndDepthFile=aEndDepthFile;};

	double getStartDepth(){return StartDepth;};
	double getEndDepth(){return EndDepth;};

	bool getStartDepthFile(){return StartDepthFile;};
	bool getEndDepthFile(){return EndDepthFile;};

};

class outputNmin{
private:
	double StartDepth;
	double EndDepth;

	bool StartDepthFile;
	bool EndDepthFile;
public:
	void setStartDepth(double aStartDepth){StartDepth=aStartDepth;};
	void setEndDepth(double aEndDepth){EndDepth=aEndDepth;};

	void setStartDepthFile(bool aStartDepthFile){StartDepthFile=aStartDepthFile;};
	void setEndDepthFile(bool aEndDepthFile){EndDepthFile=aEndDepthFile;};

	double getStartDepth(){return StartDepth;};
	double getEndDepth(){return EndDepth;};

	bool getStartDepthFile(){return StartDepthFile;};
	bool getEndDepthFile(){return EndDepthFile;};

};

class outputPoreWater{
private:
	double Depth;

	bool DepthFile;
public:
	double getDepth(){return Depth;};
	bool getDepthFile(){return DepthFile;};

	void setDepth(double aDepth){Depth=aDepth;};
	void setDepthFile(bool aDepthFile){DepthFile=aDepthFile;};


};

class outputTemperature{
private:
	double Depth;

	bool DepthFile;
public:
	double getDepth(){return Depth;};
	bool getDepthFile(){return DepthFile;};

	void setDepth(double aDepth){Depth=aDepth;};
	void setDepthFile(bool aDepthFile){DepthFile=aDepthFile;};


};

class outputSoilC{
private:
	double StartDepth;
	double EndDepth;

	bool StartDepthFile;
	bool EndDepthFile;
public:
	void setStartDepth(double aStartDepth){StartDepth=aStartDepth;};
	void setEndDepth(double aEndDepth){EndDepth=aEndDepth;};

	void setStartDepthFile(bool aStartDepthFile){StartDepthFile=aStartDepthFile;};
	void setEndDepthFile(bool aEndDepthFile){EndDepthFile=aEndDepthFile;};

	double getStartDepth(){return StartDepth;};
	double getEndDepth(){return EndDepth;};

	bool getStartDepthFile(){return StartDepthFile;};
	bool getEndDepthFile(){return EndDepthFile;};

};
class outputSoilN{
private:
	double StartDepth;
	double EndDepth;

	bool StartDepthFile;
	bool EndDepthFile;
public:
	void setStartDepth(double aStartDepth){StartDepth=aStartDepth;};
	void setEndDepth(double aEndDepth){EndDepth=aEndDepth;};

	void setStartDepthFile(bool aStartDepthFile){StartDepthFile=aStartDepthFile;};
	void setEndDepthFile(bool aEndDepthFile){EndDepthFile=aEndDepthFile;};

	double getStartDepth(){return StartDepth;};
	double getEndDepth(){return EndDepth;};

	bool getStartDepthFile(){return StartDepthFile;};
	bool getEndDepthFile(){return EndDepthFile;};

};

class outputRoot{
private:
	double StartDepth;
	double EndDepth;

	bool StartDepthFile;
	bool EndDepthFile;
public:
	void setStartDepth(double aStartDepth){StartDepth=aStartDepth;};
	void setEndDepth(double aEndDepth){EndDepth=aEndDepth;};

	void setStartDepthFile(bool aStartDepthFile){StartDepthFile=aStartDepthFile;};
	void setEndDepthFile(bool aEndDepthFile){EndDepthFile=aEndDepthFile;};

	double getStartDepth(){return StartDepth;};
	double getEndDepth(){return EndDepth;};

	bool getStartDepthFile(){return StartDepthFile;};
	bool getEndDepthFile(){return EndDepthFile;};

};

class outputDetails{
public:
	bool getPlantDetails(){return PlantDetails;};
	bool getSoilDetails(){return SoilDetails;};
	bool getWaterDetails(){return WaterDetails;};
	bool getCowDetails(){return CowDetails;};
	bool getHerdDetails(){return HerdDetails;};
	bool getPastureDetails(){return PastureDetails;};
	bool getOperationalDetails(){return OperationalDetails;};
	bool getManureManageDetails(){return ManureManageDetails;};

	bool getPlantDetailsFile(){return PlantDetailsFile;};
	bool getSoilDetailsFile(){return SoilDetailsFile;};
	bool getWaterDetailsFile(){return WaterDetailsFile;};
	bool getCowDetailsFile(){return CowDetailsFile;};
	bool getHerdDetailsFile(){return HerdDetailsFile;};
	bool getPastureDetailsFile(){return PastureDetailsFile;};
	bool getOperationalDetailsFile(){return OperationalDetailsFile;};
	bool getManureManageDetailsFile(){return ManureManageDetailsFile;};
public:
	void setPlantDetails(bool aPlantDetails){PlantDetails=aPlantDetails;};
	void setSoilDetails(bool aSoilDetails){SoilDetails=aSoilDetails;};
	void setWaterDetails(bool aWaterDetails){WaterDetails=aWaterDetails;};
	void setCowDetails(bool aCowDetails){CowDetails=aCowDetails;};
	void setHerdDetails(bool aHerdDetails){HerdDetails=aHerdDetails;};
	void setPastureDetails(bool aPastureDetails){PastureDetails=aPastureDetails;};
	void setOperationalDetails(bool aOperationalDetails){OperationalDetails=aOperationalDetails;};
	void setManureManageDetails(bool aManureManageDetails){ManureManageDetails=aManureManageDetails;};

	void setPlantDetailsFile(bool aPlantDetailsFile){PlantDetailsFile=aPlantDetailsFile;};
	void setSoilDetailsFile(bool aSoilDetailsFile){SoilDetailsFile=aSoilDetailsFile;};
	void setWaterDetailsFile(bool aWaterDetailsFile){WaterDetailsFile=aWaterDetailsFile;};
	void setCowDetailsFile(bool aCowDetailsFile){CowDetailsFile=aCowDetailsFile;};
	void setHerdDetailsFile(bool aHerdDetailsFile){HerdDetailsFile=aHerdDetailsFile;};
	void setPastureDetailsFile(bool aPastureDetailsFile){PastureDetailsFile=aPastureDetailsFile;};
	void setOperationalDetailsFile(bool aOperationalDetailsFile){OperationalDetailsFile=aOperationalDetailsFile;};
	void setManureManageDetailsFile(bool aManureManageDetailsFile){ManureManageDetailsFile=aManureManageDetailsFile;};
private:
	bool PlantDetails;
	bool SoilDetails;
	bool WaterDetails;
	bool CowDetails;
	bool HerdDetails;
	bool PastureDetails;
	bool OperationalDetails;
	bool ManureManageDetails;

	bool PlantDetailsFile;
	bool SoilDetailsFile;
	bool WaterDetailsFile;
	bool CowDetailsFile;
	bool HerdDetailsFile;
	bool PastureDetailsFile;
	bool OperationalDetailsFile;
	bool ManureManageDetailsFile;

};


class outputData:commonData{
private:

	int TemperatureLevel;
	int SoilCLevel;
	int SoilNLevel;
	int PoreWaterLevel;
	int NminLevel;
	int WaterLevel;
	int RootLevel;
public:
	vector<outputWater> WaterData;
	vector<outputNmin> NminData;
	vector<outputPoreWater> PoreWaterData;
	vector<outputTemperature> TemperatureData;
	vector<outputSoilC> SoilOrgCData;
	vector<outputSoilN> SoilOrgNData;
	vector<outputRoot> RootData;

	outputDetails DetailsData;

	outputData(){

#ifndef NITROSCAPE
	   loaddata();
	   checkData();
#else
	   loaddataNITROSCAPE();
#endif
	}


protected :
    void loaddata();
    void add();
    void loaddataNITROSCAPE();
    void checkData();


};
extern outputData * theOutputData;
#endif
