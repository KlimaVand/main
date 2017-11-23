/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "../../base/climate.h"
#include "cropSpringWheat.h"
#include "../../products/products.h"
/****************************************************************************\
\****************************************************************************/

cropSpringWheat::cropSpringWheat(const char * aName, const int aIndex, const base * aOwner)
                :cropSbarley(aName,aIndex,aOwner)
{
   CropName = "SpringWheat";
	PlantItemName = "SPWHT";                   // These names maps to products.dat !
   StrawItemName = "WHEATSTRAW";              // These names maps to products.dat !

	WinterSeed=0;                     // Is the present crop a wintercrop.
   MaxRootDepthCrop = 1.0;           // new estimate from KTK (18_3_3)

   // Working on these - but in principle fixed
	MaxRadUseEff=3.2; //0.8*3.33;            // 0.8 to simulate Maricopa remove !!!!!! Maximum radiation use efficiency (g/MJ)

   // Phenology
   // spring wheat estimed 2002 by combining spring barley and winter wheat
   // According to Olesen et al. 2002
   Phenology->TS0          = 125;              // Sowing to emergence (from winter wheat)
   Phenology->TS1          = 445;          	  // Emergence to anthesis (from spring barley)
   Phenology->TS2          = 310;              // Anthesis to end of grainfilling (from spring barley)
   Phenology->TS3          = 218;              // End of grainfilling to ripeness (from MARKVAND)

   //Phenology->DS_Flagligule= 0.6;              // (from spring barley)
   //Phenology->DS_StartFill = 1.2;              // (from spring barley)
   MinimumSenescense       = 0.9*0.35;            // Used to calculate minimum senescense - value as calibrated for winter wheat

	MinN_Store = 0.018;                 // Minimum content of nitrogen i storage organs - estimated
	MaxN_Store = 0.026;                 // Maximum content of nitrogen i storage organs - estimated
	SenescenceHeatThres=0;
	SenescenceHeatResp=0;
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
