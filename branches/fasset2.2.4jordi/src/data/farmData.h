/*
 * farmData.h
 *
 *  Created on: Sep 3, 2009
 *      Author: JOVE
 */



#include "commonData.h"
//#include "../base/message.h"
#include <vector>

#ifndef farm_H
   #define farm_H
/**
 * This class singleton pattern which mean that there can be only one instance of the class and access goes trough get_instance which take cares of initialize if needed
 * Each keyword have also a bool *File. This represent if that key word has been read from the file
 */
class farmData:commonData{
	/**
	 * This variables contains the value for the keywords there are under [Livestock] section
	 * Each keyword have also a bool *File. This represent if that key word has been read from the file
	 */
public:
	bool getpigfarm(){return pigfarm;};
	bool getdairyfarm(){return dairyfarm;};
	bool getbeeffarm(){return beeffarm;};
	bool getSowsKnowledge(){return SowsKnowledge;};
	bool getBaconersKnowledge(){return BaconersKnowledge;};

	bool getpigfarmFile(){return pigfarmFile;};
	bool getdairyfarmFile(){return dairyfarmFile;};
	bool getbeeffarmFile(){return beeffarmFile;};
	bool getSowsKnowledgeFile(){return SowsKnowledgeFile;};
	bool getBaconersKnowledgeFile(){return BaconersKnowledgeFile;};
private:

	bool pigfarm;
	bool dairyfarm;
	bool beeffarm;
	bool SowsKnowledge;
	bool BaconersKnowledge;

	bool pigfarmFile;
	bool dairyfarmFile;
	bool beeffarmFile;
	bool SowsKnowledgeFile;
	bool BaconersKnowledgeFile;
	/**
	 * This variables contains the value for the keywords there are under [farm] section
	 * Each keyword have also a bool *File. This represent if that key word has been read from the file
	 */
public:
	double getCapitalInterest(){return CapitalInterest;};
	double getMinFieldArea(){return MinFieldArea ;};

	bool getCapitalInterestFile(){return CapitalInterestFile ;};
	bool getMinFieldAreaFile(){return MinFieldAreaFile ;};

	int getWatering(){return Watering ;};
	int getDaysBetweenIrrigation(){return daysBetweenIrrigation ;};
	int getIrrigationDelay(){return irrigationDelay ;};
	int getFingerFlow(){return FingerFlow ;};
	int getScaleBuildings(){return ScaleBuildings ;};
	int getSelltheStrawProducts(){return SelltheStrawProducts ;};
	int getSelltheNonRoughageProducts(){return SelltheNonRoughageProducts ;};
	int getFarmID(){return FarmID ;};

	bool getWateringFile(){return WateringFile ;};
	bool getDaysBetweenIrrigationFile(){return daysBetweenIrrigationFile ;};
	bool getIrrigationDelayFile(){return irrigationDelayFile ;};
	bool getFingerFlowFile(){return FingerFlowFile ;};
	bool getScaleBuildingsFile(){return ScaleBuildingsFile ;};
	bool getSelltheStrawProductsFile(){return SelltheStrawProductsFile ;};
	bool getSelltheNonRoughageProductsFile(){return SelltheNonRoughageProductsFile ;};
	bool getFarmIDFile(){return FarmIDFile ;};
private:
	double CapitalInterest;
	double MinFieldArea;

	bool CapitalInterestFile;
	bool MinFieldAreaFile;

	int Watering; //-1
	int daysBetweenIrrigation; //0
	int irrigationDelay;//0
	int FingerFlow;//0
	int ScaleBuildings;
	int SelltheStrawProducts;//0
	int SelltheNonRoughageProducts;
	int FarmID;

	bool WateringFile;
	bool daysBetweenIrrigationFile;
	bool irrigationDelayFile;
	bool FingerFlowFile;
	bool ScaleBuildingsFile;
	bool SelltheStrawProductsFile;
	bool SelltheNonRoughageProductsFile;
	bool FarmIDFile;
public:
	bool farmFarmDataFile;
	bool farmLivestockDataFile;
    farmData(){
#ifndef NITROSCAPE
	   loaddata();
	   checkData();
#else
	   loaddataNITROSCAPE();
#endif
	}

protected :
	void loaddataNITROSCAPE();
	void add();
    void loaddata();
    void checkData();

};
extern farmData * theFarmData;
#endif
