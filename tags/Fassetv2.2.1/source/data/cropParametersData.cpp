/*
 * CropParameters.cpp
 *
 *  Created on: Feb 15, 2010
 *      Author: JOVE
 */
#include "cropParametersData.h"
#include "../tools/convert.h"
bool cropParametersData::loadCrop(string value, string name, cropParametersCrop &item)
{
	convert conv;
	if(name.compare("ColonialisationRate")==0)
	{
		item.colonialisationRate=(conv.StringToDouble(value));
		item.colonialisationRateFile=true;
		return true;
	}
	else if(name.compare("PhotoSyntActPart")==0)
	{
		item.PhotoSyntActPart=(conv.StringToDouble(value));
		item.PhotoSyntActPartFile=true;
		return true;
	}
	else if(name.compare("RootpF")==0)
	{
		item.RootpF=(conv.StringToDouble(value));
		item.RootpFFile=true;
		return true;
	}
	else if(name.compare("MinDMtemp")==0)
	{
		item.MinDMtemp=(conv.StringToDouble(value));
		item.MinDMtempFile=true;
		return true;
	}else if(name.compare("MaxDMtemp")==0)
	{
		item.MaxDMtemp=(conv.StringToDouble(value));
		item.MaxDMtempFile=true;
		return true;
	}
	else if(name.compare("MaxRadUseEff")==0)
	{
		item.MaxRadUseEff=(conv.StringToDouble(value));
		item.MaxRadUseEffFile=true;
		return true;
	}
	else if(name.compare("CoeffGreenIdx")==0)
	{
		item.CoeffGreenIdx=(conv.StringToDouble(value));
		item.CoeffGreenIdxFile=true;
		return true;
	}
	else if(name.compare("LAINitrogenRatio")==0)
	{
		item.LAINitrogenRatio=(conv.StringToDouble(value));
		item.LAINitrogenRatioFile=true;
		return true;
	}
	else if(name.compare("LAIDMRatio")==0)
	{
		item.LAIDMRatio=(conv.StringToDouble(value));
		item.LAIDMRatioFile=true;
		return true;
	}else if(name.compare("MinimumSenescense")==0)
	{
		item.MinimumSenescense=(conv.StringToDouble(value));
		item.MinimumSenescenseFile=true;
		return true;
	}else if(name.compare("NCurveParam")==0)
	{
		item.NCurveParam=(conv.StringToDouble(value));
		item.NCurveParamFile=true;
		return true;
	}else if(name.compare("MinN_Store")==0)
	{
		item.MinN_Store=(conv.StringToDouble(value));
		item.MinN_StoreFile=true;
		return true;
	}else if(name.compare("MaxN_Store")==0)
	{
		item.MaxN_Store=(conv.StringToDouble(value));
		item.MaxN_StoreFile=true;
		return true;
	}else if(name.compare("MinN_Root")==0)
	{
		item.MinN_Root=(conv.StringToDouble(value));
		item.MinN_RootFile=true;
		return true;
	}else if(name.compare("MaxN_Root")==0)
	{
		item.MaxN_Root=(conv.StringToDouble(value));
		item.MaxN_RootFile=true;
		return true;
	}else if(name.compare("NPctMax")==0)
	{
		item.NPctMax=(conv.StringToDouble(value));
		item.NPctMaxFile=true;
		return true;
	}else if(name.compare("NPctMin")==0)
	{
		item.NPctMin=(conv.StringToDouble(value));
		item.NPctMinFile=true;
		return true;
	}else if(name.compare("PowDM")==0)
	{
		item.PowDM=(conv.StringToDouble(value));
		item.PowDMFile=true;
		return true;
	}else if(name.compare("Conversfactor")==0)
	{
		item.Conversfactor=(conv.StringToDouble(value));
		item.ConversfactorFile=true;
		return true;
	}else if(name.compare("InitialRootDepth")==0)
	{
		item.InitialRootDepth=(conv.StringToDouble(value));
		item.InitialRootDepthFile=true;
		return true;
	}else if(name.compare("RootPentrRate")==0)
	{
		item.RootPentrRate=(conv.StringToDouble(value));
		item.RootPentrRateFile=true;
		return true;
	}else if(name.compare("RootDistrParm")==0)
	{
		item.RootDistrParm=(conv.StringToDouble(value));
		item.RootDistrParmFile=true;
		return true;
	}else if(name.compare("DecayRoot")==0)
	{
		item.DecayRoot=(conv.StringToDouble(value));
		item.DecayRootFile=true;
		return true;
	}else if(name.compare("TempRootMin")==0)
	{
		item.TempRootMin=(conv.StringToDouble(value));
		item.TempRootMinFile=true;
		return true;
	}else if(name.compare("Conversfactor")==0)
	{
		item.Conversfactor=(conv.StringToDouble(value));
		item.ConversfactorFile=true;
		return true;
	}else if(name.compare("SpecificRootLength")==0)
	{
		item.SpecificRootLength=(conv.StringToDouble(value));
		item.SpecificRootLengthFile=true;
		return true;
	}else if(name.compare("MaxAllocToRoot")==0)
	{
		item.MaxAllocToRoot=(conv.StringToDouble(value));
		item.MaxAllocToRootFile=true;
		return true;
	}else if(name.compare("MinAllocToRoot")==0)
	{
		item.MinAllocToRoot=(conv.StringToDouble(value));
		item.MinAllocToRootFile=true;
		return true;
	}
	else if(name.compare("MaxRootDepthCrop")==0)
	{
		item.MaxRootDepthCrop=(conv.StringToDouble(value));
		item.MaxRootDepthCropFile=true;
		return true;
	}else if(name.compare("WaterDeficitVegGrowth")==0)
	{
		item.WaterDeficitVegGrowth=(conv.StringToDouble(value));
		item.WaterDeficitVegGrowthFile=true;
		return true;
	}else if(name.compare("WaterDeficitLagPhase")==0)
	{
		item.WaterDeficitLagPhase=(conv.StringToDouble(value));
		item.WaterDeficitLagPhaseFile=true;
		return true;
	}else if(name.compare("WaterDeficitGrainFill")==0)
	{
		item.WaterDeficitGrainFill=(conv.StringToDouble(value));
		item.WaterDeficitGrainFillFile=true;
		return true;
	}else if(name.compare("InterceptCoeff")==0)
	{
		item.InterceptCoeff=(conv.StringToDouble(value));
		item.InterceptCoeffFile=true;
		return true;
	}else if(name.compare("NitrateUptakeRate")==0)
	{
		item.NitrateUptakeRate=(conv.StringToDouble(value));
		item.NitrateUptakeRateFile=true;
		return true;
	}else if(name.compare("AmmoniumUptakeRate")==0)
	{
		item.AmmoniumUptakeRate=(conv.StringToDouble(value));
		item.AmmoniumUptakeRateFile=true;
		return true;
	}else if(name.compare("MinimumSoilNitrate")==0)
	{
		item.MinimumSoilNitrate=(conv.StringToDouble(value));
		item.MinimumSoilNitrateFile=true;
		return true;
	}else if(name.compare("MinimumSoilAmmonium")==0)
	{
		item.MinimumSoilAmmonium=(conv.StringToDouble(value));
		item.MinimumSoilAmmoniumFile=true;
		return true;
	}else if(name.compare("MaxPlantHeight")==0)
	{
		item.MaxPlantHeight=(conv.StringToDouble(value));
		item.MaxPlantHeightFile=true;
		return true;
	}else if(name.compare("LAIDistributionPar")==0)
	{
		item.LAIDistributionPar=(conv.StringToDouble(value));
		item.LAIDistributionParFile=true;
		return true;
	}else if(name.compare("StoreForFilling")==0)
	{
		item.StoreForFilling=(conv.StringToDouble(value));
		item.StoreForFillingFile=true;
		return true;
	}else if(name.compare("FillFactor")==0)
	{
		item.FillFactor=(conv.StringToDouble(value));
		item.FillFactorFile=true;
		return true;
	}else if(name.compare("k")==0)
	{
		item.k=(conv.StringToDouble(value));
		item.kFile=true;
		return true;
	}else if(name.compare("FractionNminInRoots")==0)
	{
		item.FractionNminInRoots=(conv.StringToDouble(value));
		item.FractionNminInRootsFile=true;
		return true;
	}else if(name.compare("FractionNminInVegTop")==0)
	{
		item.FractionNminInVegTop=(conv.StringToDouble(value));
		item.FractionNminInVegTopFile=true;
		return true;
	}
	else return false;
}
bool cropParametersData::loadBeetAndPotato(string value, string name, cropParametersBeet &item)
{
	convert conv;
	if(name.compare("TempSumStorage1")==0)
	{
		item.TempSumStorage1=(conv.StringToDouble(value));
		item.TempSumStorage1File=true;
		return true;
	}
	else if(name.compare("TempSumStorage2")==0)
	{
		item.TempSumStorage2=(conv.StringToDouble(value));
		item.TempSumStorage2File=true;
		return true;
	}
	else if(name.compare("MinAllocToStorage")==0)
	{
		item.MinAllocToStorage=(conv.StringToDouble(value));
		item.MinAllocToStorageFile=true;
		return true;
	}
	else if(name.compare("MaxAllocToStorage")==0)
	{
		item.MaxAllocToStorage=(conv.StringToDouble(value));
		item.MaxAllocToStorageFile=true;
		return true;
	}
	else return false;

}
bool cropParametersData::loadRye(string value, string name, cropParametersRyeAndWweat &item)
{
	convert conv;
	if(name.compare("HeightA")==0)
	{
		item.HeightA=(conv.StringToDouble(value));
		item.HeightAFile=true;
		return true;
	}
	else if(name.compare("HeightB")==0)
	{
		item.HeightB=(conv.StringToDouble(value));
		item.HeightBFile=true;
		return true;
	}
	else return false;



}
bool cropParametersData::loadRyegrass(string value, string name, cropParametersRyegrass &item)
{
	convert conv;

	if(name.compare("BulkDensity")==0)
	{
		item.BulkDensity=(conv.StringToDouble(value));
		item.BulkDensityFile=true;
		return true;
	}
	else if(name.compare("RespirationTop")==0)
	{
		item.RespirationTop=(conv.StringToDouble(value));
		item.RespirationTopFile=true;
		return true;
	}else if(name.compare("RespirationRoot")==0)
	{
		item.RespirationRoot=(conv.StringToDouble(value));
		item.RespirationRootFile=true;
		return true;
	}
	else if(name.compare("TransferRate")==0)
	{
		item.TransferRate=(conv.StringToDouble(value));
		item.TransferRateFile=true;
		return true;
	}
	else if(name.compare("RecycleFraction")==0)
	{
		item.RecycleFraction=(conv.StringToDouble(value));
		item.RecycleFractionFile=true;
		return true;
	}
	else return false;

}
bool cropParametersData::loadLucernePea(string value, string name, cropParametersPea &item)
{
	convert conv;
	if(name.compare("MaxFixPerDM")==0)
	{
		item.MaxFixPerDM=(conv.StringToDouble(value));
		item.MaxFixPerDMFile=true;
		return true;
	}
	else if(name.compare("DMCostPerN")==0)
	{
		item.DMCostPerN=(conv.StringToDouble(value));
		item.DMCostPerNFile=true;
		return true;
	}
	else return false;

}
void cropParametersData::loaddata()
{
	if(readFileWithoutExit("CropParameters.dat"))
		{
			for(unsigned int i=0;i<items.size();i++)
			{

			}
		}
}

void cropParametersData::checkData()
{

}
