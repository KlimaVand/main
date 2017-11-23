/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "plantCareTech.h"
#include "../products/energy.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
plantCareTech::plantCareTech(const char * aName, const int aIndex, const base * aOwner)
   : fieldOperationTech(aName, aIndex, aOwner)
{
   roadSpeed = 0.0;
   specificArea = 0.0;
   fillingTime = 0.0;
   prepFillingTank = 0.0;


   theWater = nullptr;
   theChemical1 = nullptr;
   theChemical2 = nullptr;
   theChemical3 = nullptr;
}


/****************************************************************************\
  Destructor
\****************************************************************************/
plantCareTech::~plantCareTech()
{
   if(theWater)
      delete theWater;
   if(theChemical1)
      delete theChemical1;
   if(theChemical2)
      delete theChemical2;
   if(theChemical3)
      delete theChemical3;
}

/****************************************************************************\
\****************************************************************************/
void plantCareTech::ReadParameters(commonData * &file)
{

   fieldOperationTech::ReadParameters(file);

   file->FindItem("roadSpeed",&roadSpeed);
   file->FindItem("specificArea",&specificArea);
   file->FindItem("prepFillingTank",&prepFillingTank);
   file->FindItem("fillingTime",&fillingTime);

}

/****************************************************************************\
\****************************************************************************/
void plantCareTech::SetTheWaterAndChemicals(water * aWater, chemical * aChemical1, chemical * aChemical2, chemical * aChemical3)
{
   if(aWater)
   {
      theWater = new water;
      {*theWater = *aWater;}
   }

   if(aChemical1)
   {
      theChemical1 = new chemical;
      *theChemical1 = *aChemical1;
   }

   if(aChemical2)
   {
      theChemical2 = new chemical;
      *theChemical2 = *aChemical2;
   }

   if(aChemical3)
   {
      theChemical3 = new chemical;
      *theChemical3 = *aChemical3;
   }
}

/****************************************************************************\
\****************************************************************************/
void plantCareTech::ClearTheWaterAndChemicals()
{
   if(theWater) {
      delete theWater;
      theWater=nullptr;
   }
   if(theChemical1) {
      delete theChemical1;
      theChemical1=nullptr;
   }
   if(theChemical2) {
      delete theChemical2;
      theChemical2=nullptr;
   }
   if(theChemical3) {
      delete theChemical3;
      theChemical3=nullptr;
   }
}



/****************************************************************************\
\****************************************************************************/
void plantCareTech::UpdateProducts(bool contractor)
{
   fieldOperationTech::UpdateProducts(contractor);

   UpdateChemical(theChemical1);
   if (theChemical2) UpdateChemical(theChemical2);
   if (theChemical3) UpdateChemical(theChemical3);
}

/****************************************************************************\
\****************************************************************************/
void plantCareTech::UpdateChemical(chemical* aChemical)
{
	(*aChemical)*area;
   theProducts->SubtractProduct(aChemical);

   double aAmount = aChemical->GetAmount();

   int flag=aChemical->Gettype();
   switch (flag) {
		case herbicide:
	      theOutput->AddIndicator(8001,"80.01 Herbicide consumption","TFI",aAmount);
			break;
      case fungicide:
      	theOutput->AddIndicator(8002,"80.02 Fungicide consumption","TFI",aAmount);
			break;
      case insecticide:
      	theOutput->AddIndicator(8003,"80.03 Insecticide consumption","TFI",aAmount);
         break;
		default:
			break;
   }
}

