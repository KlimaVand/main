/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "animalFertTech.h"
#include "../products/energy.h"
#include "technics.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
animalFertTech::animalFertTech(const char * aName, const int aIndex, const base * aOwner)
   : fieldOperationTech(aName, aIndex, aOwner)
{
   prepLoading = 0.0;
   roadSpeed = 0.0;
   spreadingCapacity = 0.0;
   loadingCapacity = 0.0;
   prepForSpreading = 0.0;
   f = 0;
   g = 0;
   j = 0.0;
   theManure = nullptr;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
animalFertTech::~animalFertTech()
{
   if (theManure)
      delete theManure;
}

/****************************************************************************\
\****************************************************************************/
void animalFertTech::ReadParameters(commonData * &file)
{
   fieldOperationTech::ReadParameters(file);

   file->FindItem("prepLoading",&prepLoading);
   file->FindItem("roadSpeed",&roadSpeed);
   file->FindItem("spreadingCapacity",&spreadingCapacity);
   file->FindItem("loadingCapacity",&loadingCapacity);
   file->FindItem("prepForSpreading",&prepForSpreading);
   file->FindItem("j",&j);
   file->FindItem("f",&f);
   file->FindItem("g",&g);

}

/****************************************************************************\
\****************************************************************************/
void animalFertTech::SetTheManure(manure * aManure)
{
   if (aManure)
   {
      theManure = new manure;
      *theManure = *aManure;
   }
}


/****************************************************************************\
\****************************************************************************/
void animalFertTech::ClearTheManure()
{
   if (theManure) {
      delete theManure;
      theManure=nullptr;
   }
}




/****************************************************************************\
\****************************************************************************/
techEquip *animalFertTech::GetApplicationTech()
{
	linkList <techEquip> * techFarmList = theTechnics->GetFarmList();
   linkList <techEquip>::PS aTechEquipInFarmList;
	techFarmList->PeekHead(aTechEquipInFarmList);
	string tech=*techEquipList->ElementAtNumber(0);
   while (aTechEquipInFarmList!=nullptr)
   {
//   cout << aTechEquipInFarmList->element->GettechEquipName() << endl;
      if (aTechEquipInFarmList->element->GettechEquipName().compare(tech)!=0)
      	aTechEquipInFarmList=aTechEquipInFarmList->next;
      else
	      return  aTechEquipInFarmList->element;
   }
   theMessage->WarningWithDisplay("Equipment missing: ",tech.c_str());
	theMessage->FatalError("animalFertTech::GetApplicationTech () - Unable to find manure application equipment amongst farm's equipment");
	return nullptr;
}



