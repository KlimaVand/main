/*
 * settings.cpp
 *
 *  Created on: Sep 12, 2011
 *      Author: JOVE
 */

#include "settings.h"
#include "../base/commonData.h"
settings::settings()
{
	CropInformation=new commonData();
	version= "Farm ASSEsment Toolt version 2.4.1 ";

}
void settings::readParameter()
{

	fileAccess hd;

	int error=hd.changeDir(inputDir);
	if(error==-1)
	{
		cout<<"cannot open dir "<<inputDir<<endl;
		int i;
		cin>>i;
		exit(99);
	}
	CropInformation =new commonData();
	CropInformation->setCritical(true);

	CropInformation->readFile("StandardCrop.dat");

	CropInformation->readSecondaryFile("RunParameters.dat");




	commonData data;
	data.setCritical(true);
	data.readFile("output.dat");
	data.setCritical(false);
	int small,large;
	if(data.getSectionsNumbers("Water",&small,&large))
	{
		for(int i=small;i<=large;i++)
		{
			data.FindSection("Water",i);
			outputWater tmpWater;
			tmpWater.EndDepth=0;
			tmpWater.StartDepth=0;
			data.FindItem("EndDepth",&tmpWater.EndDepth);
			data.FindItem("StartDepth",&tmpWater.StartDepth);
			tmpWater.EndDepth*=10;
			tmpWater.StartDepth*=10;
			WaterData.push_back(tmpWater);
		}
	}
	else
	{
		for(int i=0;i<3;i++)
		{
			outputWater tmpWater;
			WaterData.push_back(tmpWater);
		}
		WaterData[0].StartDepth= 0.0;
		WaterData[0].EndDepth=200.0;
		WaterData[1].StartDepth= 200.0;
		WaterData[1].EndDepth= 500.0;
		WaterData[2].StartDepth= 500.0;
		WaterData[2].EndDepth=1000.0;
	}
	if(data.getSectionsNumbers("Temperature",&small,&large))
	{
		for(int i=small;i<=large;i++)
		{
			data.FindSection("Temperature",i);
			outputTemperature tmpTemperature;
			tmpTemperature.Depth=0;

			data.FindItem("Depth",&tmpTemperature.Depth);
			tmpTemperature.Depth*=10;
			TemperatureData.push_back(tmpTemperature);
		}
	}
	else
	{
		for(int i=0;i<4;i++)
		{
			outputTemperature tmp;
			TemperatureData.push_back(tmp);
		}
		TemperatureData[0].Depth= 0.0;
		TemperatureData[1].Depth= 100;
		TemperatureData[2].Depth= 300;
		TemperatureData[3].Depth= 500;
	}
	if(data.getSectionsNumbers("SoilCLevel",&small,&large))
	{
		for(int i=small;i<=large;i++)
		{
			data.FindSection("SoilCLevel",i);
			outputSoilC tmpSoilC;
			tmpSoilC.StartDepth=0;
			tmpSoilC.EndDepth=0;
			data.FindItem("StartDepth",&tmpSoilC.StartDepth);
			data.FindItem("EndDepth",&tmpSoilC.EndDepth);
			tmpSoilC.StartDepth*=10;
			tmpSoilC.EndDepth*=10;
			SoilOrgCData.push_back(tmpSoilC);
		}
	}
	else
	{
		for(int i=0;i<3;i++)
		{
			outputSoilC tmpSoilOrgC;
			SoilOrgCData.push_back(tmpSoilOrgC);
		}
		SoilOrgCData[0].StartDepth= 0.0;
		SoilOrgCData[0].EndDepth=250.0;
		SoilOrgCData[1].StartDepth= 250.0;
		SoilOrgCData[1].EndDepth= 500.0;
		SoilOrgCData[2].StartDepth= 500.0;
		SoilOrgCData[2].EndDepth=1000.0;
	}
	if(data.getSectionsNumbers("SoilNLevel",&small,&large))
	{
		for(int i=small;i<=large;i++)
		{
			data.FindSection("SoilNLevel",i);
			outputSoilN tmpSoilN;
			tmpSoilN.StartDepth=0;
			tmpSoilN.EndDepth=0;
			data.FindItem("StartDepth",&tmpSoilN.StartDepth);
			data.FindItem("EndDepth",&tmpSoilN.EndDepth);
			tmpSoilN.StartDepth*=10;
			tmpSoilN.EndDepth*=10;
			SoilOrgNData.push_back(tmpSoilN);
		}
	}
	else
	{
		for(int i=0;i<3;i++)
		{
			outputSoilN tmpSoilN;
			SoilOrgNData.push_back(tmpSoilN);
		}
		SoilOrgNData[0].StartDepth= 0.0;
		SoilOrgNData[0].EndDepth=250.0;
		SoilOrgNData[1].StartDepth= 250.0;
		SoilOrgNData[1].EndDepth= 500.0;
		SoilOrgNData[2].StartDepth= 500.0;
		SoilOrgNData[2].EndDepth=1000.0;
	}
	if(data.getSectionsNumbers("PoreWater",&small,&large))
	{
		for(int i=small;i<=large;i++)
		{
			data.FindSection("PoreWater",i);
			outputPoreWater tmpPoreWater;
			tmpPoreWater.Depth=0;

			data.FindItem("Depth",&tmpPoreWater.Depth);

			tmpPoreWater.Depth*=10;

			PoreWaterData.push_back(tmpPoreWater);
		}
	}
	else
	{
		outputPoreWater tmpPoreWater;
		PoreWaterData.push_back(tmpPoreWater);
		PoreWaterData[0].Depth= 0.0;
	}
	if(data.getSectionsNumbers("Nmin",&small,&large))
	{
		for(int i=small;i<=large;i++)
		{
			outputNmin tmpNmin;
			tmpNmin.StartDepth=0;
			tmpNmin.EndDepth=0;
			data.FindSection("Nmin",i);
			data.FindItem("StartDepth",&tmpNmin.StartDepth);
			data.FindItem("EndDepth",&tmpNmin.EndDepth);
	

			tmpNmin.StartDepth*=10;
			tmpNmin.EndDepth*=10;
			NminData.push_back(tmpNmin);
		}
	}
	else
	{
		for(int i=0;i<3;i++)
		{
			outputNmin tmpNmin;
			NminData.push_back(tmpNmin);
		}
		NminData[0].StartDepth= 0.0;
		NminData[0].EndDepth=20.0;
		NminData[1].StartDepth= 20.0;
		NminData[1].EndDepth= 200.0;
		NminData[2].StartDepth= 200.0;
		NminData[2].EndDepth=1000.0;
	}
	if(data.getSectionsNumbers("Root",&small,&large))
	{
		for(int i=small;i<=large;i++)
		{
			data.FindSection("Root",i);
			outputRoot tmpRoot;
			tmpRoot.StartDepth=0;
			tmpRoot.EndDepth=0;
			data.FindItem("StartDepth",&tmpRoot.StartDepth);
			data.FindItem("EndDepth",&tmpRoot.EndDepth);
			tmpRoot.StartDepth*=10;
			tmpRoot.EndDepth*=10;
			RootData.push_back(tmpRoot);
		}
	}
	else
	{
		for(int i=0;i<3;i++)
		{
			outputRoot tmpRoot;
			RootData.push_back(tmpRoot);
		}
		RootData[0].StartDepth= 0.0;
		RootData[0].EndDepth=0.0;
		RootData[1].StartDepth= 0.0;
		RootData[1].EndDepth= 0.0;
		RootData[2].StartDepth= 00.0;
		RootData[2].EndDepth=0.0;
	}
	if(data.FindSection("Details"))
	{
		data.FindItem("PlantDetails",&DetailsData.PlantDetails);
		data.FindItem("SoilDetails",&DetailsData.SoilDetails);
		data.FindItem("WaterDetails",&DetailsData.WaterDetails);
		data.FindItem("CowDetails",&DetailsData.CowDetails);
		data.FindItem("HerdDetails",&DetailsData.HerdDetails);
		data.FindItem("PastureDetails",&DetailsData.PastureDetails);
		data.FindItem("OperationalDetails",&DetailsData.OperationalDetails);
		data.FindItem("ManureManageDetails",&DetailsData.ManureManageDetails);
		data.FindItem("DebugSoil",&DetailsData.DebugSoil);
	}

}


