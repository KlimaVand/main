/*
 * outputData.cpp
 *
 *  Created on: Sep 9, 2009
 *      Author: JOVE
 */
#include "outputData.h"
#include <string>
#include "../tools/convert.h"
#include "../base/message.h"

void outputData::loaddataNITROSCAPE(){

}
void outputData::add(){
	for(int i=0;i<3;i++)
	{
		outputWater tmpWater;
		tmpWater.setEndDepth(0);
		tmpWater.setEndDepthFile(false);
		tmpWater.setStartDepth(0);
		tmpWater.setStartDepthFile(false);
		WaterData.push_back(tmpWater);

		outputNmin tmpNmin;
		tmpNmin.setEndDepth(0);
		tmpNmin.setEndDepth(false);
		tmpNmin.setStartDepth(0);
		tmpNmin.setStartDepthFile(false);
		NminData.push_back(tmpNmin);

		outputTemperature tmpTemperature;
		tmpTemperature.setDepth(0);
		tmpTemperature.setDepthFile(false);
		TemperatureData.push_back(tmpTemperature);

	  outputRoot tmpRoot;
		tmpRoot.setEndDepth(0);
		tmpRoot.setEndDepthFile(false);
		tmpRoot.setStartDepth(0);
		tmpRoot.setStartDepthFile(false);
		RootData.push_back(tmpRoot);

	}

	outputTemperature tmpTemperature;
	tmpTemperature.setDepth(0);
	tmpTemperature.setDepthFile(false);
	TemperatureData.push_back(tmpTemperature);

	outputPoreWater tmpPoreWater;
	tmpPoreWater.setDepth(0);
	tmpPoreWater.setDepthFile(false);
	PoreWaterData.push_back(tmpPoreWater);

	WaterData[0].setStartDepth( 0.0);
	WaterData[0].setEndDepth(200.0);
	WaterData[1].setStartDepth( 200.0);
	WaterData[1].setEndDepth( 500.0);
	WaterData[2].setStartDepth( 500.0);
	WaterData[2].setEndDepth(1000.0);

	NminData[0].setStartDepth(  0.0);
	NminData[0].setEndDepth( 20.0);
	NminData[1].setStartDepth(  20.0);
	NminData[1].setEndDepth( 200.0);
	NminData[2].setStartDepth(  200.0);
	NminData[2].setEndDepth(1000.0);

	TemperatureData[0].setDepth( 0.0);
	TemperatureData[1].setDepth( 100);
	TemperatureData[2].setDepth( 300);
	TemperatureData[3].setDepth( 500);

	PoreWaterData[0].setDepth(100);
	TemperatureLevel=-1;
	SoilCLevel=-1;
	PoreWaterLevel=-1;
	NminLevel=-1;
	WaterLevel=-1;
	RootLevel=-1;
}

void outputData::checkData()
{
	for(unsigned int i=0;i<WaterData.size();i++)
	{
		if(i!=0)
		{
			if(WaterData[i-1].getEndDepth()>WaterData[i].getStartDepth())
			{
				cout<<"in water section"<<i-1 <<"EndDepth is higher than StartDepth for section "<<i<<" for output.dat"<<endl;
				//theMessage->FatalError();
			}
		}
	}
}

bool loadSection(string sectionName, int &sectionLevel, string nameOfItem, int size)
{
	convert conv;
	if(nameOfItem.substr(1,size).compare(sectionName)==0)
	{
		string substring;
		int times=0;
		int multiplier=1;
		substring= nameOfItem.substr(size+2,1);
		int number=0;
		while(substring.compare(")"))
		{
			number=number*multiplier+conv.StringToInt(substring);
			multiplier=multiplier*10;
			times++;
			substring= nameOfItem.substr(size+2+times,1);

		}
		if(!(sectionLevel+1==number))
		{
			cout<<sectionLevel+1<<" is missing the output.dat for "<<sectionName<<endl;
			exit(1);
		}
		sectionLevel=number;
		return true;
	}
	return false;
}

void outputData::loaddata()
{
	fileAccess hd;
	hd.changeDir(theSystemData->getInputDirectory());
	cout<<hd.changeDir(theSystemData->getInputDirectory())<<" is the the "<<endl;
	cout<<theSystemData->getInputDirectory()<<" is wanted path"<<endl;
	cout<<hd.getCurrentPath()<<" is current path"<<endl;

	readFileWithExit("output.dat");

	add();
	convert conv;

	for(unsigned int i=0;i<items.size();i++)
	{
		string line=items.at(i);
		string nameOfItem=conv.partOfAString(line,1);


		if(loadSection("Water",WaterLevel, nameOfItem))
		{
			if(WaterLevel==0)
			{
				while(!WaterData.empty())
					WaterData.pop_back();
			}

			outputWater tmpWater;
			tmpWater.setEndDepth(0);
			tmpWater.setEndDepthFile(false);
			tmpWater.setStartDepth(0);
			tmpWater.setStartDepthFile(false);
			WaterData.push_back(tmpWater);
			while(true)
			{
				i++;
				line=items.at(i);
				string nameOfItem=conv.partOfAString(line,1);
				string value=conv.partOfAString(line,2);
				if(nameOfItem.compare("EndDepth")==0)
				{

					WaterData[WaterLevel].setEndDepth(conv.StringToDouble(value)*10);
					WaterData[WaterLevel].setEndDepthFile(true);

				}
				else if(nameOfItem.compare("StartDepth")==0)
				{

					WaterData[WaterLevel].setStartDepth(conv.StringToDouble(value)*10);
					WaterData[WaterLevel].setStartDepthFile(true);
				}
            else if(nameOfItem.substr(0,1)=="[" &&nameOfItem.substr(nameOfItem.size()-1,nameOfItem.size())=="]")
				{
            	i--;
				  break;
				}
				else
				{
					string output="for output.dat is there a keyword: "+nameOfItem+"that is not used ";
					//theMessage->Warning(output);

				}


			}

		}
		else if(loadSection("Temperature",TemperatureLevel, nameOfItem))
		{
			if(TemperatureLevel==0)
			{
				while(!TemperatureData.empty())
					TemperatureData.pop_back();
			}
			outputTemperature tmpTemperature;
			tmpTemperature.setDepth(0);
			tmpTemperature.setDepthFile(false);
			TemperatureData.push_back(tmpTemperature);
			while(true)
			{
				i++;
				line=items.at(i);
				string nameOfItem=conv.partOfAString(line,1);
				string value=conv.partOfAString(line,2);
				if(nameOfItem.compare("Depth")==0)
				{

					TemperatureData[TemperatureLevel].setDepth(conv.StringToDouble(value)*10);
					TemperatureData[TemperatureLevel].setDepthFile(true);
				}
            else if(nameOfItem.substr(0,1)=="[" &&nameOfItem.substr(nameOfItem.size()-1,nameOfItem.size())=="]")
				{
            	i--;
				  break;
				}
				else
				{
					string output="for output.dat is there a keyword: "+nameOfItem+"that is not used ";
					//theMessage->Warning(output);

				}
			}
		}
		else if(loadSection("SoilC",SoilCLevel, nameOfItem))
		{
			if(SoilCLevel==0)
			{
				while(!SoilCData.empty())
					SoilCData.pop_back();
			}
			outputSoilC tmpSoilC;
			tmpSoilC.setEndDepth(0);
			tmpSoilC.setEndDepthFile(false);
			tmpSoilC.setStartDepth(0);
			tmpSoilC.setStartDepthFile(false);
			SoilCData.push_back(tmpSoilC);
			while(true)
			{
				i++;
				line=items.at(i);
				string nameOfItem=conv.partOfAString(line,1);
				string value=conv.partOfAString(line,2);
				if(nameOfItem.compare("EndDepth")==0)
				{
					SoilCData[SoilCLevel].setEndDepth(conv.StringToDouble(value)*10);
					SoilCData[SoilCLevel].setEndDepthFile(true);
				}
				else if(nameOfItem.compare("StartDepth")==0)
				{
					SoilCData[SoilCLevel].setStartDepth(conv.StringToDouble(value)*10);
					SoilCData[SoilCLevel].setStartDepthFile(true);
				}
            else if(nameOfItem.substr(0,1)=="[" &&nameOfItem.substr(nameOfItem.size()-1,nameOfItem.size())=="]")
				{
              i--;
				  break;
				}
				else
				{
					string output="for output.dat is there a keyword: "+nameOfItem+"that is not used ";
					//theMessage->Warning(output);

				}
			}
		}
		else if(loadSection("PoreWater",PoreWaterLevel, nameOfItem))
		{
			if(PoreWaterLevel==0)
			{
				while(!PoreWaterData.empty())
					PoreWaterData.pop_back();
			}
			outputPoreWater tmpPoreWater;
			tmpPoreWater.setDepth(0);
			tmpPoreWater.setDepthFile(false);
			PoreWaterData.push_back(tmpPoreWater);
			while(true)
			{
				i++;
				line=items.at(i);
				string nameOfItem=conv.partOfAString(line,1);
				string value=conv.partOfAString(line,2);
				if(nameOfItem.compare("Depth")==0)
				{
					PoreWaterData[PoreWaterLevel].setDepth(conv.StringToDouble(value)*10);
					PoreWaterData[PoreWaterLevel].setDepthFile(true);
				}
            else if(nameOfItem.substr(0,1)=="[" &&nameOfItem.substr(nameOfItem.size()-1,nameOfItem.size())=="]")
				{
            	i--;
				  break;
				}
				else
				{
					string output="for output.dat is there a keyword: "+nameOfItem+"that is not used ";
					//theMessage->Warning(output);

				}
			}

		}
		else if(loadSection("Nmin",NminLevel, nameOfItem ))
		{

			if(NminLevel==0)
			{
				while(!NminData.empty())
					NminData.pop_back();
			}
			outputNmin tmpNmin;
			tmpNmin.setEndDepth(0);
			tmpNmin.setEndDepth(false);
			tmpNmin.setStartDepth(0);
			tmpNmin.setStartDepthFile(false);
			NminData.push_back(tmpNmin);
			while(true)
			{
				i++;
				line=items.at(i);
				string nameOfItem=conv.partOfAString(line,1);
				string value=conv.partOfAString(line,2);
				if(nameOfItem.compare("EndDepth")==0)
				{
					NminData[NminLevel].setEndDepth(conv.StringToDouble(value)*10);
					NminData[NminLevel].setEndDepthFile(true);
				}
				else if(nameOfItem.compare("StartDepth")==0)
				{
					NminData[NminLevel].setStartDepth(conv.StringToDouble(value)*10);
					NminData[NminLevel].setStartDepthFile(true);

				}
            else if(nameOfItem.substr(0,1)=="[" &&nameOfItem.substr(nameOfItem.size()-1,nameOfItem.size())=="]")
				{
            	i--;
				  break;
				}
				else
				{
					string output="for output.dat is there a keyword: "+nameOfItem+"that is not used ";
					//theMessage->Warning(output);

				}

			}

		}
		else if(loadSection("Root",RootLevel, nameOfItem ))
		{

			if(RootLevel==0)
			{
				while(!RootData.empty())
					RootData.pop_back();
			}
			outputRoot tmpRoot;
			tmpRoot.setEndDepth(0);
			tmpRoot.setEndDepthFile(false);
			tmpRoot.setStartDepth(0);
			tmpRoot.setStartDepthFile(false);
			RootData.push_back(tmpRoot);
			while(true)
			{
				i++;
				line=items.at(i);
				string nameOfItem=conv.partOfAString(line,1);
				string value=conv.partOfAString(line,2);
				if(nameOfItem.compare("EndDepth")==0)
				{
					RootData[RootLevel].setEndDepth(conv.StringToDouble(value)*10);
					RootData[RootLevel].setEndDepthFile(true);
				}
				else if(nameOfItem.compare("StartDepth")==0)
				{
					RootData[RootLevel].setStartDepth(conv.StringToDouble(value)*10);
					RootData[RootLevel].setStartDepthFile(true);

				}
            else if(nameOfItem.substr(0,1)=="[" &&nameOfItem.substr(nameOfItem.size()-1,nameOfItem.size())=="]")
				{
            	i--;
				  break;
				}
				else
				{
					string output="for output.dat is there a keyword: "+nameOfItem+"that is not used ";
					//theMessage->Warning(output);

				}

			}

		}
		else if(nameOfItem.compare("[Details]")==0)
		{
			DetailsData.setPlantDetails(false);
			DetailsData.setSoilDetails(false);
			DetailsData.setWaterDetails(false);
			DetailsData.setCowDetails(false);
			DetailsData.setHerdDetails(false);
			DetailsData.setPastureDetails(false);
			DetailsData.setOperationalDetails(false);
			DetailsData.setManureManageDetails(false);

			DetailsData.setPlantDetailsFile(false);
			DetailsData.setSoilDetailsFile(false);
			DetailsData.setWaterDetailsFile(false);
			DetailsData.setCowDetailsFile(false);
			DetailsData.setHerdDetailsFile(false);
			DetailsData.setPastureDetailsFile(false);
			DetailsData.setOperationalDetailsFile(false);
			DetailsData.setManureManageDetailsFile(false);
			while(true)
			{
				i++;
				line=items.at(i);
				string nameOfItem=conv.partOfAString(line,1);
				string value=conv.partOfAString(line,2);

				if(nameOfItem.compare("PlantDetails")==0)
				{
					DetailsData.setPlantDetails(conv.StringToBool(value));
					DetailsData.setPlantDetailsFile(true);

				}
				else if(nameOfItem.compare("SoilDetails")==0)
				{
					DetailsData.setSoilDetails(conv.StringToBool(value));
					DetailsData.setSoilDetailsFile(true);
				}
				else if(nameOfItem.compare("WaterDetails")==0)
				{
					DetailsData.setWaterDetails(conv.StringToBool(value));
					DetailsData.setWaterDetailsFile(true);
				}
				else if(nameOfItem.compare("CowDetails")==0)
				{
					DetailsData.setCowDetails(conv.StringToBool(value));
					DetailsData.setCowDetailsFile(true);
				}
				else if(nameOfItem.compare("HerdDetails")==0)
				{
					DetailsData.setHerdDetails(conv.StringToBool(value));
					DetailsData.setHerdDetailsFile(true);
				}
				else if(nameOfItem.compare("PastureDetails")==0)
				{
					DetailsData.setPastureDetails(conv.StringToBool(value));
					DetailsData.setPastureDetailsFile(true);
				}
				else if(nameOfItem.compare("OperationalDetails")==0)
				{
					DetailsData.setOperationalDetails(conv.StringToBool(value));
					DetailsData.setOperationalDetailsFile(true);
				}
				else if(nameOfItem.compare("ManureManageDetails")==0)
				{
					DetailsData.setManureManageDetails(conv.StringToBool(value));
					DetailsData.setManureManageDetailsFile(true);
				}
            else if(nameOfItem.substr(0,1)=="[" &&nameOfItem.substr(nameOfItem.size()-1,nameOfItem.size())=="]")
				{
            	i--;
				  break;
				}
				else
				{
					string output="for output.dat is there a keyword: "+nameOfItem+"that is not used ";
					//theMessage->Warning(output);

				}
			}

		}
      else
		{	
			string tmp=conv.partOfAString(line,1);
			
			if(tmp.compare("[end]")==0)
				break;
			else
			{
				string output="section "+line +"in product.dat is not a valid section";
				//theMessage->Warning(output);

				line=items.at(i);
				cout<<"last line "<<line<<endl;
			}
		}

	}

	if(TemperatureLevel==-1)
		TemperatureLevel=3;

	if(WaterLevel==-1)
		WaterLevel=2;

	if(NminLevel==-1)
		NminLevel=2;

	if(PoreWaterLevel==-1)
		PoreWaterLevel=0;

//	if(RootLevel==-1)
//		RootLevel=2;
}
