/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "staticItalianRyeGrass.h"

// WARNING - All date's and amounts are wrong !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

/****************************************************************************\
\****************************************************************************/
staticItalianRyeGrass::staticItalianRyeGrass()
{
	strcpy(cropName,"ITALRYEG");
}

/****************************************************************************\
\****************************************************************************/
void staticItalianRyeGrass::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
   if (PP->GetNumOfHarvestDates()>1)
      theMessage->FatalError("static_italryeg::SpecifiedPlan - more than one harvest date specified");
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * staticItalianRyeGrass::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
	aPlantItem1=new plantItem;
  	aPlantItem1->Setname(cropName);
	aPlantItem2=new plantItem;
  	aPlantItem2->Setname("ITALRYEGSTRAW");
	char crop_id[3];      // TEMPORARY
	strcpy(crop_id,"IR");
   ResetOpList();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,crop_id);
   else
   { // Automatic plan
      theMessage->FatalError("static_italryeg::MakeOpList - automatic plan not implemented");
   }
   delete aPlantItem1;
   delete aPlantItem2;
 	return LP;
}



