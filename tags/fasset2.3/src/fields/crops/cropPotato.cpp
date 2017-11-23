/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "../../data/commonData.h"
#include "cropPotato.h"

/****************************************************************************\
\****************************************************************************/
cropPotato::cropPotato(const char * aName, const int aIndex, const base * aOwner,string cropName)
           :crop(aName,aIndex,aOwner,cropName)
{
   // Potato tubers approx. 24% DM ("Fodermiddeltabellen", 2000)
   // Planted approx 2 t/ha wet weight = 0.24 t DM/ha
   // Harvest amounts intermediate early: approx. 50 - 70 t wet weight; late: approx 60 - 80 t wet weight

   // Parameters

   PlantItemName = "POTATO";                 // These names maps to products.dat !
   StrawItemName = "POTATOTOP";              // These names maps to products.dat !
	WinterSeed = false;                       // Is the present crop a wintercrop.
	TempSumStorage1=theCropData->getTempSumStorage1();
	  TempSumStorage2=theCropData->getTempSumStorage2();
	  MinAllocToStorage=theCropData->getMinAllocToStorage();
	  MaxAllocToStorage=theCropData->getMaxAllocToStorage();

}

/****************************************************************************\
\****************************************************************************/
cropPotato::cropPotato(const cropPotato& acrop)
    : crop(acrop)
{

  TempSumStorage1=acrop.TempSumStorage1;
  TempSumStorage2=acrop.TempSumStorage2;
  MinAllocToStorage=acrop.MinAllocToStorage;
  MaxAllocToStorage=acrop.MaxAllocToStorage;
}

/****************************************************************************\
\****************************************************************************/
void cropPotato::Add(crop* aCrop, double fraction)
{
   crop::Add(aCrop,fraction);
   cropPotato * c = (cropPotato*)aCrop;

  	TempSumStorage1=(1-fraction)*TempSumStorage1 + fraction*c->TempSumStorage1;
   TempSumStorage2=(1-fraction)*TempSumStorage2 + fraction*c->TempSumStorage2;
   MinAllocToStorage=(1-fraction)*MinAllocToStorage + fraction*c->MinAllocToStorage;
   MaxAllocToStorage=(1-fraction)*MaxAllocToStorage + fraction*c->MaxAllocToStorage;
}


/****************************************************************************\
\****************************************************************************/
void cropPotato::TransferDryMatterToStorage(double * dDryMatt)
{
   double deltaStorage = 0.0;
   if (TempSumRoot<=TempSumStorage1)
		deltaStorage=*dDryMatt*MinAllocToStorage;
	if ((TempSumRoot>TempSumStorage1) && (TempSumRoot<TempSumStorage2))
		deltaStorage=*dDryMatt*(MinAllocToStorage+(MaxAllocToStorage-MinAllocToStorage)
					 *((TempSumRoot-TempSumStorage1)/(TempSumStorage2-TempSumStorage1)));
	if (TempSumRoot>=TempSumStorage2)
		deltaStorage=*dDryMatt*MaxAllocToStorage;
	*dDryMatt-=deltaStorage;
	DryMatterStorage+=deltaStorage;
}

