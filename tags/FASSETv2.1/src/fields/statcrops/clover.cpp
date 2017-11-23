/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include <common.h>
#include "clover.h"

/****************************************************************************\
\****************************************************************************/
static_clover::static_clover()
{
	strcpy(cropName,"CLOVER");
}

/****************************************************************************\
\****************************************************************************/
void static_clover::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * static_clover::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
	aPlantItem1=new plantItem;
   aPlantItem1->Setname("CLOVER");
	char crop_id[3];      // TEMPORARY
	strcpy(crop_id,"K1");
	ResetOpList();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,crop_id);
   else
   { // Automatic plan
      { // Ploughing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(Ploughing,crop_id,cropName,PP->GetStartYear(),9,1,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      { // Seedbed harrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(SeedbedHarrowing,crop_id,cropName,PP->GetStartYear(),9,2,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      { // Sowing
         sowFields * FO=new sowFields;
         seed * aSeed=new seed;
         aSeed->Setname("SEEDCLOVER");
         theProducts->GiveProductInformation(aSeed);
         aSeed->Setamount(aSeed->GetStandardDensity());     // 25
         FO->DefineSowFieldsOp(Sowing,crop_id,cropName,PP->GetStartYear(),9,3,PP->GetArea(),0,PP->GetDistance(),PP->GetPVStrat(),reduction,aSeed);
         AddOpList(FO);
         delete aSeed;
      }
   }
   delete aPlantItem1;
	return LP;
}

/****************************************************************************\
\****************************************************************************/
int static_clover::NumOfProtectStrat_()
{
	return 2;
}

/****************************************************************************\
\****************************************************************************/
int static_clover::NitrogenLevels_()
{
	return Nitrogen_levels;
}

/****************************************************************************\
\****************************************************************************/
double static_clover::NitrogenAmount(int JB_NO,double free_N,int N_level)
{
	return 0;
}

/****************************************************************************\
\****************************************************************************/
double static_clover::ExpectedYield_(int JB_NO,int N_level,int PV_strat)
{
	double factor=PVstratFactor(PV_strat);
	if (JB_NO<=4) // Sandy soil
	{
		if (irrigation)
         return factor*3.5;
		else
         return factor*2.0;
	}
   return factor*4.5;                                  // FORKERTE VÆRDIER RET!!!
}

/****************************************************************************\
\****************************************************************************/
double static_clover::PhosphorAmount_()
{
	return 25;
}

/****************************************************************************\
\****************************************************************************/
double static_clover::PotassiumAmount_()
{
	return 50;
}

/****************************************************************************\
\****************************************************************************/
double static_clover::ExtraNRelease_()
{
	return 35;
}

