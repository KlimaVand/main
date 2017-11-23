
#include "../../base/common.h"
#include "cropWwheat.h"
#include "../../base/climate.h"
#include "../../base/bstime.h"

cropWwheat::cropWwheat(string       aName,
                       const int    aIndex,
                       const base * aOwner,
                       string       cropName):
    crop(aName,
         aIndex,
         aOwner,
         cropName)
{
    Sown          = Vernalized = Emerged = Extending = false;
    GrainNumber   = FinalLeafNo = CritLeafNo = 0;
    LeafNumber    = VernalIndex = 0;
    TSum          = 0.0;
    LeafNumber    = Germinated = Anthesis = CommitFlag = false;
    PlantItemName = "WNWHT";         // These names maps to products.dat !
    StrawItemName = "WHEATSTRAW";    // These names maps to products.dat !
    WinterSeed    = true;

    commonData * data = globalSettings -> CropInformation;    // See Olesen, 2002 for description
		int sectionName=data->FindSection(CropName);
    data -> FindItem("HeightA", &HeightA);
    data -> FindItem("HeightB", &HeightB);


	data->FindItem("SenescenceHeatThres",&SenescenceHeatThres);
	data->FindItem("SenescenceHeatResp",&SenescenceHeatResp);
}

cropWwheat::~cropWwheat() {}

cropWwheat::cropWwheat(const cropWwheat & acrop):
    crop(acrop)
{
    Germinated          = false;
    Sown                = Vernalized = Emerged = Extending = Anthesis = CommitFlag = false;
    GrainNumber         = FinalLeafNo = 0.0;
    HeightA             = HeightB = CritLeafNo = LeafNumber = VernalIndex = 0;
    TSum                = 0.0;
    TransferableStorage = acrop.TransferableStorage;
    StoreForFilling     = acrop.StoreForFilling;
}
void cropWwheat::CalcLeafAreaIndices()
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
maximumtemperature =SurfacePlantMax();

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
void cropWwheat::Sow(double SeedDensDryMatt,
                     double aRelativeDensity,
                     double NitrogenInSeed) {
    crop::Sow(SeedDensDryMatt, aRelativeDensity, NitrogenInSeed);
    Phenology -> Sow();
}

void cropWwheat::Harvest(decomposable * Storage,
                         decomposable * Straw) {
    crop::Harvest(Storage, Straw);
}

double cropWwheat::fNitrogen() {
    if (Phenology -> Ripe()) {
        return fNitrogenAfterRipe;    // ensures that no N translocation takes place after ripeness
    } else {
        return crop::fNitrogen();
    }
}

void cropWwheat::UpdateHeight() {
    double DS_Converted = Phenology -> DS / Phenology -> DS_Flagligule;

    MaxPlantHeight = 1.0 / (1 / HeightA - log(2.0) / (HeightB * 2));

    if (Phenology -> Emerged()) {
        PlantHeight = 1.0 / (1 / HeightA - log(DS_Converted) / (HeightB * DS_Converted));
    }

    PlantHeight = min(PlantHeight, MaxPlantHeight);
}
