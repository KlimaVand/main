/*
 * farmData.cpp
 *
 *  Created on: Sep 3, 2009
 *      Author: JOVE
 */

#include "../data/farmData.h"
#include "../data/systemData.h"
#include "../base/message.h"
#include "../tools/convert.h"
#include "../tools/fileAccess.h"

/**
 * This function checks if the necessary data has been read and if it is correct. If something is wrong it either sends a  FatalError or closing fasset
 */
void farmData::checkData()
{
	if(farmLivestockDataFile==false)
	{
		cout<<"could not find [livestock] in farm.dat"<<endl;
      cin.get();
		exit(1);
	}
	if(farmFarmDataFile==false)
	{
		cout<<"could not find [farm] in farm.dat"<<endl;
      cin.get();
		exit(1);
	}
	if ((beeffarm)&&(dairyfarm))
		beeffarm=false;
}

void farmData::loaddataNITROSCAPE()
{
	add();
}
void farmData::add()
{
	CapitalInterest=0.0;
	MinFieldArea=0.0;
	CapitalInterestFile=false;
	MinFieldAreaFile=false;
	Watering=-1;
	daysBetweenIrrigation=0;
	irrigationDelay=0;
	FingerFlow=0;
	ScaleBuildings=0;
	SelltheStrawProducts=0;
	SelltheNonRoughageProducts=0;
	FarmID=0;

	WateringFile=false;
	daysBetweenIrrigationFile=false;
	irrigationDelayFile=false;
	FingerFlowFile=false;
	ScaleBuildingsFile=false;
	SelltheStrawProductsFile=false;
	SelltheNonRoughageProductsFile=false;
	FarmIDFile=false;
	farmFarmDataFile=true;

	pigfarm=false;
	dairyfarm=false;
	beeffarm=false;
	SowsKnowledge=false;
	BaconersKnowledge=false;
	farmLivestockDataFile=false;
   farmLivestockDataFile=false;
	farmFarmDataFile=false;
}
/**
 * This class load the data from farm.dat. Note that farm.dat should be placed in the default path or Fasset will exit
 * If the keywords is not found the default value is used. See farm.xlsx for that information
 */
void farmData::loaddata()
{

   add();
	fileAccess hd;
	hd.changeDir(theSystemData->getInputDirectory());
	readFileWithExit("farm.dat");
	convert conv;
	for(unsigned int i=0;i<items.size();i++)
	{
		string line=items.at(i);
		string nameOfItem=conv.partOfAString(line,1);
		string value=conv.partOfAString(line,2);
		if(nameOfItem.compare("[farm]")==0)
		{
          farmFarmDataFile=true;
		}
		else if(nameOfItem.compare("CapitalInterest")==0)
		{
			CapitalInterest=conv.StringToDouble(value);
			CapitalInterestFile=true;
		}
		else if(nameOfItem.compare("MinFieldArea")==0)
		{
			MinFieldArea=conv.StringToDouble(value);
			MinFieldAreaFile=true;
		}
		else if(nameOfItem.compare("Watering")==0)
		{
			Watering=conv.StringToInt(value);
			WateringFile=true;
		}
		else if(nameOfItem.compare("daysBetweenIrrigation")==0)
		{
			Watering=conv.StringToInt(value);
			daysBetweenIrrigationFile=true;
		}
		else if(nameOfItem.compare("irrigationDelay")==0)
		{
			irrigationDelay=conv.StringToInt(value);
			irrigationDelayFile=true;
		}
		else if(nameOfItem.compare("FingerFlow")==0)
		{
			FingerFlow=conv.StringToInt(value);
			FingerFlowFile=true;
		}
		else if(nameOfItem.compare("ScaleBuildings")==0)
		{
			ScaleBuildings=conv.StringToInt(value);
			ScaleBuildingsFile=true;
		}
		else if(nameOfItem.compare("SelltheStrawProducts")==0)
		{
			SelltheStrawProducts=conv.StringToInt(value);
			SelltheStrawProductsFile=true;
		}
		else if(nameOfItem.compare("SelltheNonRoughageProducts")==0)
		{
			SelltheNonRoughageProducts=conv.StringToInt(value);
			SelltheNonRoughageProductsFile=true;
		}
		else if(nameOfItem.compare("FarmIDFile")==0)
		{
			FarmID=conv.StringToInt(value);
			FarmIDFile=true;
		}
		else if(nameOfItem.compare("[livestock]")==0)
		{
          farmLivestockDataFile=true;
		}
		else if(nameOfItem.compare("pigfarm")==0)
		{
			pigfarm=conv.StringToBool(value);
			pigfarmFile=true;
		}
		else if(nameOfItem.compare("dairyfarm")==0)
		{
			dairyfarm=conv.StringToBool(value);
			dairyfarmFile=true;
		}
		else if(nameOfItem.compare("beeffarm")==0)
		{
			beeffarm=conv.StringToBool(value);
			beeffarmFile=true;
		}
		else if(nameOfItem.compare("SowsKnowledge")==0)
		{
			SowsKnowledge=conv.StringToBool(value);
			SowsKnowledgeFile=true;
		}
		else if(nameOfItem.compare("BaconersKnowledge")==0)
		{
			BaconersKnowledge=conv.StringToBool(value);
			BaconersKnowledgeFile=true;
		}
		else if(nameOfItem.substr(0,1)=="[" &&nameOfItem.substr(nameOfItem.size()-1,nameOfItem.size())=="]")
		{
			break;
		}
		else
		{
			string output="for farm.dat is there a keyword: "+conv.partOfAString(line,1)+"that is not used ";
			//theMessage->Warning(output);
		}

	}


}


