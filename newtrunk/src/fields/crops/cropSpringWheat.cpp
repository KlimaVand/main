
#include "../../base/common.h"
#include "cropSpringWheat.h"
#include "../../products/products.h"
#include "../../base/climate.h"
cropSpringWheat::cropSpringWheat(string       aName,
                                 const int    aIndex,
                                 const base * aOwner,
                                 string       cropName):
    cropSbarley(aName,
                aIndex,
                aOwner,
                cropName)
{
    PlantItemName = "SPWHT";         // These names maps to products.dat !
    StrawItemName = "WHEATSTRAW";    // These names maps to products.dat !
	commonData * data = globalSettings -> CropInformation;
	cout << CropName << endl;
	int sectionName=data->FindSection(CropName);
	data->FindItem("SenescenceHeatThres",&SenescenceHeatThres);
	data->FindItem("SenescenceHeatResp",&SenescenceHeatResp);
}
void cropSpringWheat::CalcLeafAreaIndices()
{
   if (GreenCropAreaIndex>maxGLAI)
      maxGLAI=GreenCropAreaIndex;     //state variable that holds the maximum achieved green LAI
   if (Phenology->Emerged())
   {
                             if (Phenology->TempSumForLeaf<=Phenology->LinearLeafPhase)
//                        GreenCropAreaIndex =  InitialCAI*RelativeDensity*(Phenology->TempSumForLeaf/Phenology->LinearLeafPhase);      /Deleted MEL 2010 due to very large LAI at high sowing rates
                             GreenCropAreaIndex =  InitialCAI*(Phenology->TempSumForLeaf/Phenology->LinearLeafPhase);      // value from Porter ....
      else
      {
                               double DS_num = 1.3;
                              if (CropName == "WinterWheat")
                                DS_num =1.0;
                              if (Phenology->DS<DS_num)
         {
                                                                                                                    double reducedGrowth = Phenology->GetFractionToFlagLigule();                 // Correct later
            reducedGrowth = 1.0/(1.0+pow(reducedGrowth,10));
            double expLeafExt = CoeffGreenIdx*GreenCropAreaIndex*max(0.0,temp);
            double expL = expLeafExt*min(1.0,TranspirationRatio/0.6)*reducedGrowth;

            double dmL  = LAIDMRatio*DryMatterVegTop-GreenCropAreaIndex;

                             double NL   = NitrogenInVegTop().n*LAINitrogenRatio-GreenCropAreaIndex;
            double potDGLAI = max(0.0,MaxGLAICrop-GreenCropAreaIndex);
                             double DGLAI = max(0.0,min(expL,min(dmL,min(NL,potDGLAI))));

            GreenCropAreaIndex += DGLAI;
         }
         if ((Phenology->LeafSenesence()>0) || (temp < 0.0))
         {

            double senescense = 1.0/(1.0-MinimumSenescense)*Phenology->LeafSenesence();  // Correct later
            if (temp < 0.0)  //if mean temp is below zero enhanced senescense MEL 2009
                             senescense=1.0/(1.0-MinimumSenescense)*0.005;

double Fheat = 1;
double maximumtemperature=theClimate->tmax;
if (usingPlantTemp == 1)
maximumtemperature = SurfacePlantMax();
if (maximumtemperature>SenescenceHeatThres)
{
	Fheat = 1 + SenescenceHeatResp*(maximumtemperature-SenescenceHeatThres);
}
            double deltaCAIg=-max(0.0,senescense*maxGLAI*(1.0-TranspirationRatio*MinimumSenescense))*Fheat;


            if ((YellowCropAreaIndex>=0.0) || (GreenCropAreaIndex>=0.0))
            {
                             GreenCropAreaIndex+=deltaCAIg;
               if (GreenCropAreaIndex<0.0)
               {
                             deltaCAIg-=GreenCropAreaIndex;
                  GreenCropAreaIndex=0.0;
               }
               YellowCropAreaIndex-=deltaCAIg*Conversfactor;
            }
            else
                             theMessage->WarningWithDisplay("crop::CalcLeafAreaIndices - variables in undefined state");
         }
      }
                             }
}
/*
void cropSpringWheat::ReadParameters(string filename)
{	cropSbarley::ReadParameters(filename);
	commonData data;
   if (data.readFileWithoutExit("CropParameters.dat"))
   {
	   int sectionName=data.FindSection(CropName);
		if (sectionName!=-1)
      {
			data.FindItem("SenescenceHeatThres",sectionName,SenescenceHeatThres);
			data.FindItem("SenescenceHeatResp",sectionName,SenescenceHeatResp);
		}
	}

}
*/