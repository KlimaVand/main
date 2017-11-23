/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "harvestTech.h"
#include "../products/energy.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
harvestTech::harvestTech(const char * aName, const int aIndex, const base * aOwner)
   : fieldOperationTech(aName, aIndex, aOwner)
{
   loadClampCapacity = 0.0;
   reloadingCapacity = 0.0;
   unloadCapacity = 0.0;
   unloadRoothouseCapacity = 0.0;
   depositCapacity = 0.0;
   depositRoothouseCapacity = 0.0;
   strawBalerCapacity = 0.0;
   prepUnloadingStorage = 0.0;
   prepUnloadingRoothouse = 0.0;
   prepReloading = 0.0;
   prepReloadingHarvester = 0.0;
   prepClamp = 0.0;
   uncoverClamp = 0.0;
   stops = 0;
   roadSpeed = 0.0;
   balesLost = 0.0;
   balesLoad = 0.0;
   bindTime = 0.0;
   operationID = 0;

   loadCapacity = 0.0;
	reloading = 0.0;
	moweCarriage = 0.0;
   for (int i=0;i<MaxPlants;i++)
   	MainProduct[i] = StrawProduct[i] = ForageProduct[i] = nullptr;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
harvestTech::~harvestTech()
{
}


/****************************************************************************\
\****************************************************************************/
void harvestTech::ReadParameters(commonData * &file)
{
   fieldOperationTech::ReadParameters(file);


   file->FindItem("roadSpeed",&roadSpeed);
   file->setCritical(false);
   file->FindItem("unloadCapacity",&unloadCapacity);
   file->FindItem("prepUnloadingStorage",&prepUnloadingStorage);
   file->FindItem("prepReloadingHarvester",&prepReloadingHarvester);
   file->FindItem("prepReloading",&prepReloading);
   file->FindItem("moweCarriage",&moweCarriage);
   file->FindItem("reloading",&reloading);
   file->FindItem("strawBalerCapacity",&strawBalerCapacity);        // new parameters added by JB
   file->FindItem("loadCapacity",&loadCapacity);                    // new parameters added by JB

   file->FindItem("bindTime",&bindTime);
   file->FindItem("balesLoad",&balesLoad);
   file->FindItem("balesLost",&balesLost);
   file->FindItem("stops",&stops);
   file->FindItem("uncoverClamp",&uncoverClamp);
   file->FindItem("prepClamp",&prepClamp);
   file->FindItem("prepUnloadingRoothouse",&prepUnloadingRoothouse);
   file->FindItem("depositCapacity",&depositCapacity);
   file->FindItem("loadClampCapacity",&loadClampCapacity);
   file->FindItem("reloadingCapacity",&reloadingCapacity);
   file->FindItem("unloadRoothouseCapacity",&unloadRoothouseCapacity);
   file->FindItem("depositRoothouseCapacity",&depositRoothouseCapacity);

}




/****************************************************************************\
Sends products to storage via Products.cpp
This code is a compromise; technics handles products as an array whilst Products handles them as a linklist
\****************************************************************************/
void harvestTech::UpdateProducts(bool contractor)
{
   fieldOperationTech::UpdateProducts(contractor);

   if ((operationID!=CutSetAside)&&(operationID!=Mowing)&&(operationID!=BeetTopChopping))
   {
	   linkList<plantItem>* aList = new linkList<plantItem>;  //why is this copying necessary??
      for (int i=0;i<MaxPlants;i++)
      {
			if (MainProduct[i] && MainProduct[i]->GetAmount()>0) //pack MainProducts into a linklist and send to Products for transport to storage
			   aList->InsertLast(MainProduct[i]);
      }
      if (aList->NumOfNodes()>0)
      {
	      theProducts->AddHarvestGrainProducts(aList);
      	aList->ForgetAll();
      }
      for (int i=0;i<MaxPlants;i++)
      {
         if (StrawProduct[i] && StrawProduct[i]->GetAmount()>0)
			   aList->InsertLast(StrawProduct[i]);
      }
      if (aList->NumOfNodes()>0)
      {
	      theProducts->AddHarvestStrawProducts(aList);
   	   aList->ForgetAll();
      }
      for (int i=0;i<MaxPlants;i++)
      {
         if (ForageProduct[i] && ForageProduct[i]->GetAmount()>0)
			   aList->InsertLast(ForageProduct[i]);
      }
      if (aList->NumOfNodes()>0)
      {
	      theProducts->AddForageProducts(aList);
   	   aList->ForgetAll();
      }
      delete aList;
   }
}

