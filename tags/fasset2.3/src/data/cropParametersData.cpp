/*
 * cropParametersData2.cpp
 *
 *  Created on: Apr 8, 2011
 *      Author: JOVE
 */
#include "cropParametersData.h"
#include "../tools/convert.h"
#include <vector>
beetData::beetData()
{
	TempSumStorage1=-1;              			// Temperature sum for min allocation to storage
	TempSumStorage2=-1;              				// Temperature sum for max allocation to storage
	MinAllocToStorage=-1;           				// Min allocation to storage organs
	MaxAllocToStorage=-1;

}
ryeGrassData::ryeGrassData()
{
	MinDayLength=-1;
	BulkDensity=-1;
	RespirationTop=-1;
	RespirationRoot=-1;
	TransferRate=-1;
	RecycleFraction=-1;
	InitialLAIIncrease=-1;
	liveOMD=-1;
	deadOMD=-1;
	RateOfRipeReSeed=-1;

}
cloverData::cloverData()
{

	NFixationThisDay=-1;
};
ryeData::ryeData()
{
	HeightA=-1;                  // See Olesen, 2002 for description. Value taken from winter wheat
	HeightB=-1;
};
generalData::generalData()
{
	DMCostPerN=-1,  //Needed for external crop parameterisation of pea
    MaxFixPerDM=-1, //Needed for external crop parameterisation of pea
    AmmoniumUptakeRate=-1,
	CoeffGreenIdx=-1,
	ColonialisationRate=-1,
	Conversfactor=-1,
	ConversionCoefficient=-1,
	DecayRoot=-1,
	FillFactor=-1,
	GerminationConversion=-1,
	InitialCAI=-1,
	InitialRootDepth=-1,
	InterceptCoeff=-1,
	   k=-1,
		      LAIDistributionPar=-1,
		      LAIDMRatio=-1,
		      LAINitrogenRatio=-1,
		      MaxGLAICrop=-1,
		      MaxAllocToRoot=-1,
		      MaxDMtemp=-1,
		      MaxN_Root=-1,
		      MaxN_Store=-1,
		      MaxPlantHeight=-1,
		      MaxRadUseEff=-1,
		      MinAllocToRoot=-1,
		      MinDMtemp=-1,
		      MinimumRootDens=-1,
		      MinimumSenescense=-1,
		      MinimumSoilAmmonium=-1,
		      MinimumSoilNitrate=-1,
		      MinN_Root=-1,
		      MinN_Store=-1,
		      NCurveParam=0,
		      NitrateUptakeRate=-1,
		      NPctMax=-1,
		      NPctMin=-1,
		      PhotoSyntActPart=-1,
		      PowDM=0,
		      ReducedNInYellow=-1,
		      RhizoDepositFraction=-1,
		      RootDensAtBottom=-1,
		      RootPentrRate=-1,
		      RootpF=-1,
		      SpecificRootLength=-1,
		      StoreForFilling=-1,
		      TempRootMin=-1,
		      TopFraction=-1,
		      WaterDeficitGrainFill=-1,
		      WaterDeficitLagPhase=-1,
		      WaterDeficitVegGrowth=-1,
		      FractionNminInRoots=-1,
		      FractionNminInVegTop=-1,
		      CO2Effect=-1;

		      MaxRootDepth=-1,
		      RelativeDensity=-1;                    // Relative to standard sowing density
		      RootDistrParm=-1;
		      MaxN_Root=-1;


}
void cropParametersData::loaddata()
{



	fileAccess hd;
	hd.changeDir(theSystemData->getInputDirectory());

	if(readFileWithExit("StandardCrop.dat"))
	{
		processing(true);

	}
	if(readFileWithoutExit("RunParameters.dat"))
	{
		processing(false);
	}

}
void cropParametersData::setCrop(string CropName)
{

	placeInBeetVector=-1;
	placeInRyeGrassVector=-1;
	placeInCcoverVector=-1;
	placeInRyeVector=-1;
	placeInBaseVector=-1;
	for(unsigned int i=0; i<data.size();i++)
	{
		if(data[i].cropName.compare(CropName)==0)
		{
			placeInBaseVector=i;

		}
	}
	for(unsigned i=0; i<beet.size();i++)
	{
		if(beet[i].cropName.compare(CropName)==0)
		{
			placeInBeetVector=i;
		}
	}
	for(unsigned i=0; i<ryeGrass.size();i++)
	{

		if(ryeGrass[i].cropName.compare(CropName)==0)
		{
			placeInRyeGrassVector=i;

		}
	}
	for(unsigned i=0; i<clover.size();i++)
	{
		if(clover[i].cropName.compare(CropName)==0)
		{
			placeInCcoverVector=i;

		}
	}
	for(unsigned i=0; i<rye.size();i++)
	{

		if(rye[i].cropName.compare(CropName)==0)
		{
			placeInRyeVector=i;

		}
	}
	if(placeInBaseVector==-1)
	{

		string output="cropParametersData: cropName "+CropName+ " not supported";
		theMessage->FatalError( output.c_str());
	}
}

void cropParametersData::processing(bool primaryFile)
{
	convert conv;
	vector<string> AlreayUsed;
	for(unsigned int i=0;i<items.size();i++)
	{

		string line=items.at(i);
		string tmp=conv.partOfAString(line,1);
		string cropName=tmp.substr(1,tmp.size()-2);
		if(tmp[0]=='['&&tmp[tmp.size()-1]==']')
		{
			if(tmp.compare("[end]")==0)
			{

				break;
			}

			beetData beetDatas;
			bool usedBeet =false;
			ryeGrassData ryeGrassDatas;
			bool usedRyeGrass =false;
			cloverData cloverDatas;
			bool usedClover =false;
			ryeData ryeDatas;
			bool usedRye =false;
			string name=tmp.substr(1,tmp.size()-2);
			generalData newData;
			newData.cropName=name;
			for(unsigned int k=0;k<AlreayUsed.size();k++)
			{
				if(AlreayUsed[k].compare(name)==0)
					theMessage->FatalError("already read crop with name "+name);
			}
			for(unsigned int j=0;j<data.size();j++)
			{
				if(data.at(j).cropName.compare(name)==0)
				{

					newData=data.at(j);
					data.erase(data.begin()+j);
					break;

				}
			}
			for(unsigned int j=0;j<beet.size();j++)
						{
							if(beet.at(j).cropName.compare(name)==0)
							{

								beetDatas=beet.at(j);
								beet.erase(beet.begin()+j);
								usedBeet=true;

							}
						}
			for(unsigned int j=0;j<ryeGrass.size();j++)
						{
							if(ryeGrass.at(j).cropName.compare(name)==0)
							{

									ryeGrassDatas=ryeGrass.at(j);
									ryeGrass.erase(ryeGrass.begin()+j);
									usedRyeGrass=true;

							}
						}
			for(unsigned int j=0;j<clover.size();j++)
						{
							if(clover.at(j).cropName.compare(name)==0)
							{

										cloverDatas=clover.at(j);
										clover.erase(clover.begin()+j);
										usedClover=true;

							}
						}
			for(unsigned int j=0;j<rye.size();j++)
						{
							if(rye.at(j).cropName.compare(name)==0)
							{

								ryeDatas=rye.at(j);
								rye.erase(rye.begin()+j);
								usedRye=true;

							}
						}
			i++;
			AlreayUsed.push_back(name);
			line=items.at(i);
			tmp=conv.partOfAString(line,1);
			while(!(tmp[0]=='['&&tmp[tmp.size()-1]==']'))
			{


				string Data=conv.partOfAString(line,2);

				if(tmp.compare("DMCostPerN")==0)
					newData.DMCostPerN=conv.StringToDouble(Data);
				else if(tmp.compare("MaxFixPerDM")==0)
					newData.MaxFixPerDM=conv.StringToDouble(Data);
				else if(tmp.compare("AmmoniumUptakeRate")==0)
					newData.AmmoniumUptakeRate=conv.StringToDouble(Data);
				else if(tmp.compare("CoeffGreenIdx")==0)
					newData.CoeffGreenIdx=conv.StringToDouble(Data);
				else if(tmp.compare("ColonialisationRate")==0)
					newData.ColonialisationRate=conv.StringToDouble(Data);
				else if(tmp.compare("Conversfactor")==0)
					newData.Conversfactor=conv.StringToDouble(Data);
				else if(tmp.compare("ConversionCoefficient")==0)
					newData.ConversionCoefficient=conv.StringToDouble(Data);
				else if(tmp.compare("DecayRoot")==0)
					newData.DecayRoot=conv.StringToDouble(Data);
				else if(tmp.compare("FillFactor")==0)
					newData.FillFactor=conv.StringToDouble(Data);
				else if(tmp.compare("GerminationConversion")==0)
					newData.GerminationConversion=conv.StringToDouble(Data);
				else if(tmp.compare("InitialCAI")==0)
					newData.InitialCAI=conv.StringToDouble(Data);
				else if(tmp.compare("InitialRootDepth")==0)
					newData.InitialRootDepth=conv.StringToDouble(Data);
				else if(tmp.compare("InterceptCoeff")==0)
					newData.InterceptCoeff=conv.StringToDouble(Data);
				else if(tmp.compare("k")==0)
					newData.k=conv.StringToDouble(Data);
				else if(tmp.compare("LAIDistributionPar")==0)
					newData.LAIDistributionPar=conv.StringToDouble(Data);
				else if(tmp.compare("LAIDMRatio")==0)
					newData.LAIDMRatio=conv.StringToDouble(Data);
				else if(tmp.compare("LAINitrogenRatio")==0)
					newData.LAINitrogenRatio=conv.StringToDouble(Data);
				else if(tmp.compare("MaxGLAICrop")==0)
					newData.MaxGLAICrop=conv.StringToDouble(Data);
				else if(tmp.compare("MaxAllocToRoot")==0)
					newData.MaxAllocToRoot=conv.StringToDouble(Data);
				else if(tmp.compare("MaxDMtemp")==0)
					newData.MaxDMtemp=conv.StringToDouble(Data);
				else if(tmp.compare("MaxPlantHeight")==0)
					newData.MaxPlantHeight=conv.StringToDouble(Data);
				else if(tmp.compare("MaxRadUseEff")==0)
					newData.MaxRadUseEff=conv.StringToDouble(Data);
				else if(tmp.compare("MinAllocToRoot")==0)
					newData.MinAllocToRoot=conv.StringToDouble(Data);
				else if(tmp.compare("MinDMtemp")==0)
					newData.MinDMtemp=conv.StringToDouble(Data);
				else if(tmp.compare("MinimumRootDens")==0)
					newData.MinimumRootDens=conv.StringToDouble(Data);
				else if(tmp.compare("MinimumSenescense")==0)
					newData.MinimumSenescense=conv.StringToDouble(Data);
				else if(tmp.compare("MinimumSoilAmmonium")==0)
					newData.MinimumSoilAmmonium=conv.StringToDouble(Data);
				else if(tmp.compare("MinimumSoilNitrate")==0)
					newData.MinimumSoilNitrate=conv.StringToDouble(Data);
				else if(tmp.compare("MinN_Root")==0)
					newData.MinN_Root=conv.StringToDouble(Data);
				else if(tmp.compare("MinN_Store")==0)
					newData.MinN_Store=conv.StringToDouble(Data);
				else if(tmp.compare("NCurveParam")==0)
					newData.NCurveParam=conv.StringToDouble(Data);
				else if(tmp.compare("NitrateUptakeRate")==0)
					newData.NitrateUptakeRate=conv.StringToDouble(Data);
				else if(tmp.compare("NPctMax")==0)
					newData.NPctMax=conv.StringToDouble(Data);
				else if(tmp.compare("NPctMin")==0)
					newData.NPctMin=conv.StringToDouble(Data);
				else if(tmp.compare("PhotoSyntActPart")==0)
					newData.PhotoSyntActPart=conv.StringToDouble(Data);
				else if(tmp.compare("PowDM")==0)
					newData.PowDM=conv.StringToDouble(Data);
				else if(tmp.compare("ReducedNInYellow")==0)
					newData.ReducedNInYellow=conv.StringToDouble(Data);
				else if(tmp.compare("RhizoDepositFraction")==0)
					newData.RhizoDepositFraction=conv.StringToDouble(Data);
				else if(tmp.compare("RootDensAtBottom")==0)
					newData.RootDensAtBottom=conv.StringToDouble(Data);
				else if(tmp.compare("RootPentrRate")==0)
					newData.RootPentrRate=conv.StringToDouble(Data);
				else if(tmp.compare("RootpF")==0)
					newData.RootpF=conv.StringToDouble(Data);
				else if(tmp.compare("SpecificRootLength")==0)
					newData.SpecificRootLength=conv.StringToDouble(Data);
				else if(tmp.compare("StoreForFilling")==0)
					newData.StoreForFilling=conv.StringToDouble(Data);
				else if(tmp.compare("TempRootMin")==0)
					newData.TempRootMin=conv.StringToDouble(Data);
				else if(tmp.compare("TopFraction")==0)
					newData.TopFraction=conv.StringToDouble(Data);
				else if(tmp.compare("WaterDeficitGrainFill")==0)
					newData.WaterDeficitGrainFill=conv.StringToDouble(Data);
				else if(tmp.compare("WaterDeficitLagPhase")==0)
					newData.WaterDeficitLagPhase=conv.StringToDouble(Data);
				else if(tmp.compare("WaterDeficitVegGrowth")==0)
					newData.WaterDeficitVegGrowth=conv.StringToDouble(Data);
				else if(tmp.compare("FractionNminInRoots")==0)
					newData.FractionNminInRoots=conv.StringToDouble(Data);
				else if(tmp.compare("FractionNminInVegTop")==0)
					newData.FractionNminInVegTop=conv.StringToDouble(Data);
				else if(tmp.compare("CO2Effect")==0)
					newData.CO2Effect=conv.StringToDouble(Data);
				else if(tmp.compare("MaxRootDepth")==0)
					newData.MaxRootDepth=conv.StringToDouble(Data);
				else if(tmp.compare("RelativeDensity")==0)
					newData.RelativeDensity=conv.StringToDouble(Data);
				else if(tmp.compare("MaxN_Store")==0)
					newData.MaxN_Store=conv.StringToDouble(Data);
				else if(tmp.compare("DS_Flagligule")==0)
					newData.DS_Flagligule=conv.StringToDouble(Data);
				else if(tmp.compare("DS_StartFill")==0)
					newData.DS_StartFill=conv.StringToDouble(Data);
				else if(tmp.compare("TS0")==0)
					newData.TS0=conv.StringToDouble(Data);
				else if(tmp.compare("TS1")==0)
					newData.TS1=conv.StringToDouble(Data);
				else if(tmp.compare("TS2")==0)
					newData.TS2=conv.StringToDouble(Data);
				else if(tmp.compare("TS3")==0)
					newData.TS3=conv.StringToDouble(Data);
				else if(tmp.compare("TB0")==0)
					newData.TB0=conv.StringToDouble(Data);
				else if(tmp.compare("TB1")==0)
					newData.TB1=conv.StringToDouble(Data);
				else if(tmp.compare("TB2")==0)
					newData.TB2=conv.StringToDouble(Data);
				else if(tmp.compare("TB3")==0)
					newData.TB3=conv.StringToDouble(Data);
				else if(tmp.compare("DB")==0)
					newData.DB=conv.StringToDouble(Data);
				else if(tmp.compare("DMAX")==0)
					newData.DMAX=conv.StringToDouble(Data);
				else if(tmp.compare("LinearLeafPhase")==0)
					newData.LinearLeafPhase=conv.StringToDouble(Data);
				else if(tmp.compare("RootDistrParm")==0)
					newData.RootDistrParm=conv.StringToDouble(Data);
				else if(tmp.compare("MaxN_Root")==0)
					newData.MaxN_Root=conv.StringToDouble(Data);
				else if(tmp.compare("MinAllocToStorage")==0)
				{
					usedBeet=true;
					beetDatas.MinAllocToStorage=conv.StringToDouble(Data);
				}
				else if(tmp.compare("MaxAllocToStorage")==0)
				{
					usedBeet=true;
					beetDatas.MaxAllocToStorage=conv.StringToDouble(Data);
				}
				else if(tmp.compare("TempSumStorage1")==0)
				{
					usedBeet=true;
					beetDatas.TempSumStorage1=conv.StringToDouble(Data);
				}
				else if(tmp.compare("TempSumStorage2")==0)
				{
					usedBeet=true;
					beetDatas.TempSumStorage2=conv.StringToDouble(Data);
				}
				else if(tmp.compare("MinDayLength")==0)
				{
					usedRyeGrass=true;
					ryeGrassDatas.MinDayLength=conv.StringToDouble(Data);
				}
				else if(tmp.compare("BulkDensity")==0)
				{
					usedRyeGrass=true;
					ryeGrassDatas.BulkDensity=conv.StringToDouble(Data);
				}
				else if(tmp.compare("RespirationTop")==0)
				{
					usedRyeGrass=true;
					ryeGrassDatas.RespirationTop=conv.StringToDouble(Data);
				}
				else if(tmp.compare("RespirationRoot")==0)
				{
					usedRyeGrass=true;
					ryeGrassDatas.RespirationRoot=conv.StringToDouble(Data);
				}
				else if(tmp.compare("TransferRate")==0)
				{
					usedRyeGrass=true;
					ryeGrassDatas.TransferRate=conv.StringToDouble(Data);
				}
				else if(tmp.compare("RecycleFraction")==0)
				{
					usedRyeGrass=true;
					ryeGrassDatas.RecycleFraction=conv.StringToDouble(Data);
				}
				else if(tmp.compare("InitialLAIIncrease")==0)
				{
					usedRyeGrass=true;
					ryeGrassDatas.InitialLAIIncrease=conv.StringToDouble(Data);
				}
				else if(tmp.compare("liveOMD")==0)
				{
					usedRyeGrass=true;
					ryeGrassDatas.liveOMD=conv.StringToDouble(Data);
				}
				else if(tmp.compare("deadOMD")==0)
				{
					usedRyeGrass=true;
					ryeGrassDatas.deadOMD=conv.StringToDouble(Data);
				}
				else if(tmp.compare("RateOfRipeReSeed")==0)
				{
					usedRyeGrass=true;
					ryeGrassDatas.RateOfRipeReSeed=conv.StringToDouble(Data);
				}
				else if(tmp.compare("HeightB")==0)
				{

					usedRye=true;
					ryeDatas.HeightB=conv.StringToDouble(Data);
				}
				else if(tmp.compare("HeightA")==0)
				{
					usedRye=true;
					ryeDatas.HeightA=conv.StringToDouble(Data);
				}
				else if(tmp.compare("NFixationThisDay")==0)
				{
					usedClover=true;
					cloverDatas.NFixationThisDay=conv.StringToDouble(Data);
				}
				else
				{


					string output="Fasset does not support "+tmp+" in CropParameters for "+newData.cropName;

					theMessage->Warning(output.c_str());
				}

				i++;
				if(items.size()==i)
					theMessage->FatalError("CropParametersData::Prosseing Something wrong with file. Forgot [end] ?");
				line=items.at(i);
				tmp=conv.partOfAString(line,1);

			}
			data.push_back(newData);
			if(usedBeet)
			{
				beetDatas.cropName=name;
				beet.push_back(beetDatas);
			}
			if(usedRyeGrass)
			{
				ryeGrassDatas.cropName=name;
				ryeGrass.push_back(ryeGrassDatas);
			}
			if(usedClover)
			{
				cloverDatas.cropName=name;
				clover.push_back(cloverDatas);
			}
			if(usedRye)
			{
				ryeDatas.cropName=name;
				rye.push_back(ryeDatas);
			}
			i--;
		}
		else
			theMessage->FatalError("problem with reading crop");
	}
}
void cropParametersData::checkData()
{


    for(unsigned int i=0;i<beet.size();i++)
    {
    	beetData tmp=beet.at(i);
    	if(tmp.MaxAllocToStorage<0)
    		theMessage->FatalError("cropParametersData:MaxAllocToStorage is not found in file for "+tmp.cropName);
    	else if(tmp.MinAllocToStorage<0)
    		theMessage->FatalError("cropParametersData:MinAllocToStorage is not found in file");
    	else if(tmp.TempSumStorage1<0)
    	   theMessage->FatalError("cropParametersData:TempSumStorage1 is not found in file");
    	else if(tmp.TempSumStorage2<0)
    	   theMessage->FatalError("cropParametersData:TempSumStorage2 is not found in file");
    }
    for(unsigned int i=0;i<ryeGrass.size();i++)
    {
    	ryeGrassData tmp=ryeGrass.at(i);
    	if(tmp.BulkDensity<0)
    	    theMessage->FatalError("cropParametersData:BulkDensity is not found in file for " +tmp.cropName);
    	else if(tmp.InitialLAIIncrease<0)
    		theMessage->FatalError("cropParametersData:InitialLAIIncrease is not found in file for " +tmp.cropName);
    	else if(tmp.MinDayLength<0)
    	   theMessage->FatalError("cropParametersData:MinDayLength is not found in file for " +tmp.cropName);
    	else if(tmp.RateOfRipeReSeed<0)
    	   theMessage->FatalError("cropParametersData:RateOfRipeReSeed is not found in file for " +tmp.cropName);
    	else if(tmp.RecycleFraction<0)
    	    theMessage->FatalError("cropParametersData:RecycleFraction is not found in file for " +tmp.cropName);
    	else if(tmp.RespirationTop<0)
    	    theMessage->FatalError("cropParametersData:RespirationTop is not found in file for " +tmp.cropName);
    	else if(tmp.TransferRate<0)
    	    theMessage->FatalError("cropParametersData:TransferRate is not found in file for " +tmp.cropName);
    	else if(tmp.liveOMD<0)
    		theMessage->FatalError("cropParametersData:liveOMD is not found in file for " +tmp.cropName);
    	else if(tmp.deadOMD<0)
    	    theMessage->FatalError("cropParametersData:deadOMD is not found in file for " +tmp.cropName);
    }
    for(unsigned int i=0;i<clover.size();i++)
    {
    	cloverData tmp=clover.at(i);
    	if(tmp.NFixationThisDay<0)
    		theMessage->FatalError("cropParametersData:NFixationThisDay is not found in file for " +tmp.cropName);

    }
    for(unsigned int i=0;i<rye.size();i++)
    {
    	ryeData tmp=rye.at(i);
    	if(tmp.HeightA<0)
    	{

    		theMessage->FatalError("cropParametersData:HeightA is not found in file for " +tmp.cropName);
    	}
    	if(tmp.HeightB<0)
    	{

    	   theMessage->FatalError("cropParametersData:HeightB is not found in file for " +tmp.cropName);
    	}
     }
	for(unsigned int i=0;i<data.size();i++)
	{
		generalData newData=data.at(i);

		if(newData.DMCostPerN<0)
			theMessage->FatalError("cropParametersData:DMCostPerN is not found in file for "+newData.cropName);
		else if(newData.MaxFixPerDM<0)
			theMessage->FatalError("cropParametersData:MaxFixPerDM is not found in file for "+newData.cropName);
		else if(newData.AmmoniumUptakeRate<0)
			theMessage->FatalError("cropParametersData:AmmoniumUptakeRate is not found in file for "+newData.cropName);
		else if(newData.CoeffGreenIdx<0)
			theMessage->FatalError("cropParametersData:CoeffGreenIdx is not found in file for "+newData.cropName);
		else if(newData.ColonialisationRate<0)
			theMessage->FatalError("cropParametersData:ColonialisationRate is not found in file for "+newData.cropName);
		else if(newData.Conversfactor<0)
			theMessage->FatalError("cropParametersData:Conversfactor is not found in file for "+newData.cropName);
		else if(newData.ConversionCoefficient<0)
			theMessage->FatalError("cropParametersData:ConversionCoefficient is not found in file for "+newData.cropName);
		else if(newData.DecayRoot<0)
			theMessage->FatalError("cropParametersData:DecayRoot is not found in file for "+newData.cropName);
		else if(newData.FillFactor<0)
			theMessage->FatalError("cropParametersData:FillFactor is not found in file for "+newData.cropName);
		else if(newData.GerminationConversion<0)
			theMessage->FatalError("cropParametersData:GerminationConversion is not found in file for "+newData.cropName);
		else if(newData.InitialCAI<0)
			theMessage->FatalError("cropParametersData:InitialCAI is not found in file for "+newData.cropName);
		else if(newData.InitialRootDepth<0)
			theMessage->FatalError("cropParametersData:InitialRootDepth is not found in file for "+newData.cropName);
		else if(newData.InterceptCoeff<0)
			theMessage->FatalError("cropParametersData:InterceptCoeff is not found in file for "+newData.cropName);
		else if(newData.k<0)
			theMessage->FatalError("cropParametersData:k is not found in file for "+newData.cropName);
		else if(newData.LAIDistributionPar<0)
			theMessage->FatalError("cropParametersData:LAIDistributionPar is not found in file for "+newData.cropName);
		else if(newData.LAIDMRatio<0)
			theMessage->FatalError("cropParametersData:LAIDMRatio is not found in file for "+newData.cropName);
		else if(newData.LAINitrogenRatio<0)
			theMessage->FatalError("cropParametersData:LAINitrogenRatio is not found in file for "+newData.cropName);
		else if(newData.MaxGLAICrop<0)
			theMessage->FatalError("cropParametersData:MaxGLAICrop is not found in file for "+newData.cropName);
		else if(newData.MaxAllocToRoot<0)
			theMessage->FatalError("cropParametersData:MaxAllocToRoot is not found in file for "+newData.cropName);
		else if(newData.MaxDMtemp<0)
			theMessage->FatalError("cropParametersData:MaxDMtemp is not found in file for "+newData.cropName);
		else if(newData.MaxPlantHeight<0)
			theMessage->FatalError("cropParametersData:MaxPlantHeight is not found in file for "+newData.cropName);
		else if(newData.MaxRadUseEff<0)
			theMessage->FatalError("cropParametersData:MaxRadUseEff is not found in file for "+newData.cropName);
		else if(newData.MinAllocToRoot<0)
			theMessage->FatalError("cropParametersData:MinAllocToRoot is not found in file for "+newData.cropName);
		else if(newData.MinDMtemp<0)
		{
			string output="cropParametersData:MinDMtemp is not found in file "+newData.cropName;
			theMessage->FatalError(output.c_str());
		}
		else if(newData.MinimumRootDens<0)
			theMessage->FatalError("cropParametersData:MinimumRootDens is not found in file for "+newData.cropName);
		else if(newData.MinimumSenescense<0)
			theMessage->FatalError("cropParametersData:MinimumSenescense is not found in file for "+newData.cropName);
		else if(newData.MinimumSoilAmmonium<0)
			theMessage->FatalError("cropParametersData:MinimumSoilAmmonium is not found in file for "+newData.cropName);
		else if(newData.MinimumSoilNitrate<0)
			theMessage->FatalError("cropParametersData:MinimumSoilNitrate is not found in file for "+newData.cropName);
		else if(newData.MinN_Root<0)
			theMessage->FatalError("cropParametersData:MinN_Root is not found in file for "+newData.cropName);
		else if(newData.MinN_Store<0)
			theMessage->FatalError("cropParametersData:MinN_Store is not found in file for "+newData.cropName);
		else if(newData.NCurveParam>=0)
			theMessage->FatalError("cropParametersData:NCurveParam is not found in file for "+newData.cropName);
		else if(newData.NitrateUptakeRate<0)
			theMessage->FatalError("cropParametersData:NitrateUptakeRate is not found in file for "+newData.cropName);
		else if(newData.NPctMax<0)
			theMessage->FatalError("cropParametersData:NPctMax is not found in file for "+newData.cropName);
		else if(newData.NPctMin<0)
			theMessage->FatalError("cropParametersData:NPctMin is not found in file for "+newData.cropName);
		else if(newData.PhotoSyntActPart<0)
			theMessage->FatalError("cropParametersData:PhotoSyntActPart is not found in file for "+newData.cropName);
		else if(newData.PowDM>=0)
			theMessage->FatalError("cropParametersData:PowDM is not found in file for "+newData.cropName);
		else if(newData.ReducedNInYellow<0)
			theMessage->FatalError("cropParametersData:ReducedNInYellow is not found in file for "+newData.cropName);
		else if(newData.RhizoDepositFraction<0)
			theMessage->FatalError("cropParametersData:RhizoDepositFraction is not found in file for "+newData.cropName);
		else if(newData.RootDensAtBottom<0)
			theMessage->FatalError("cropParametersData:RootDensAtBottom is not found in file for "+newData.cropName);
		else if(newData.RootPentrRate<0)
			theMessage->FatalError("cropParametersData:RootPentrRate is not found in file for "+newData.cropName);
		else if(newData.RootpF<0)
			theMessage->FatalError("cropParametersData:RootpF is not found in file for "+newData.cropName);
		else if(newData.SpecificRootLength<0)
			theMessage->FatalError("cropParametersData:SpecificRootLength is not found in file for "+newData.cropName);
		else if(newData.StoreForFilling<0)
			theMessage->FatalError("cropParametersData:StoreForFilling is not found in file for "+newData.cropName);
		else if(newData.TempRootMin<0)
			theMessage->FatalError("cropParametersData:TempRootMin is not found in file for "+newData.cropName);
		else if(newData.TopFraction<0)
			theMessage->FatalError("cropParametersData:TopFraction is not found in file for "+newData.cropName);
		else if(newData.WaterDeficitGrainFill<0)
			theMessage->FatalError("cropParametersData:WaterDeficitGrainFill is not found in file for "+newData.cropName);
		else if(newData.WaterDeficitLagPhase<0)
			theMessage->FatalError("cropParametersData:WaterDeficitLagPhase is not found in file for "+newData.cropName);
		else if(newData.WaterDeficitVegGrowth<0)
			theMessage->FatalError("cropParametersData:WaterDeficitVegGrowth is not found in file for "+newData.cropName);
		else if(newData.FractionNminInRoots<0)
			theMessage->FatalError("cropParametersData:FractionNminInRoots is not found in file for "+newData.cropName);
		else if(newData.FractionNminInVegTop<0)
			theMessage->FatalError("cropParametersData:FractionNminInVegTop is not found in file for "+newData.cropName);
		else if(newData.CO2Effect<0)
			theMessage->FatalError("cropParametersData:CO2Effect is not found in file for "+newData.cropName);
		else if(newData.MaxRootDepth<0)
			theMessage->FatalError("cropParametersData:MaxRootDepth is not found in file for "+newData.cropName);
		else if(newData.RelativeDensity<0)
			theMessage->FatalError("cropParametersData:RelativeDensity is not found in file for "+newData.cropName);
		else if(newData.MaxN_Store<0)
			theMessage->FatalError("cropParametersData:MaxN_Store is not found in file for "+newData.cropName);
		else if(newData.DS_Flagligule<0)
			theMessage->FatalError("cropParametersData:DS_Flagligule is not found in file for "+newData.cropName);
		else if(newData.DS_StartFill<0)
			theMessage->FatalError("cropParametersData:DS_StartFill is not found in file for "+newData.cropName);
		else if(newData.TS0<0)
			theMessage->FatalError("cropParametersData:TS0 is not found in file for "+newData.cropName);
		else if(newData.TS1<0)
			theMessage->FatalError("cropParametersData:TS1 is not found in file for "+newData.cropName);
		else if(newData.TS2<0)
			theMessage->FatalError("cropParametersData:TS2 is not found in file for "+newData.cropName);
		else if(newData.TS3<0)
			theMessage->FatalError("cropParametersData:TS3 is not found in file for "+newData.cropName);
		else if(newData.TB0<0)
			theMessage->FatalError("cropParametersData:TB0 is not found in file for "+newData.cropName);
		else if(newData.TB1<0)
			theMessage->FatalError("cropParametersData:TB1 is not found in file for "+newData.cropName);
		else if(newData.TB2<0)
			theMessage->FatalError("cropParametersData:TB2 is not found in file for "+newData.cropName);
		else if(newData.TB3<0)
			theMessage->FatalError("cropParametersData:TB3 is not found in file for "+newData.cropName);
		else if(newData.DB<0)
			theMessage->FatalError("cropParametersData:DB is not found in file for "+newData.cropName);
		else if(newData.DMAX<0)
			theMessage->FatalError("cropParametersData:DMAX is not found in file for "+newData.cropName);
		else if(newData.LinearLeafPhase<0)
			theMessage->FatalError("cropParametersData:LinearLeafPhase is not found in file for "+newData.cropName);
		else if(newData.MaxAllocToRoot<newData.MinAllocToRoot)
			theMessage->FatalError("cropParametersData:MaxAllocToRoot and MinAllocToRoot wrong for " +newData.cropName);
		else if(newData.MaxDMtemp<newData.MinDMtemp)
			theMessage->FatalError("cropParametersData:MaxDMtemp and MinDMtemp wrong for " +newData.cropName);
		else if(newData.MaxN_Root<newData.MinN_Root)
			theMessage->FatalError("cropParametersData:MaxN_Root and MinN_Root wrong for " +newData.cropName);
		else if(newData.MaxN_Store<newData.MinN_Store)
		{
			cout<<newData.MaxN_Store<<endl;
			cout<<newData.MinN_Store<<endl;
			theMessage->FatalError("cropParametersData:MaxN_Store and MinN_Store wrong for " +newData.cropName);
		}

	}
   }
