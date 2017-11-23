/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "sowTech.h"
#include "../products/energy.h"
#include "../products/products.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
sowTech::sowTech(const char * aName, const int aIndex, const base * aOwner)
   : fieldOperationTech(aName, aIndex, aOwner)
{
   prepFillSowing = 0.0;
   fillingTime1Sowing = 0.0;
   fillingTime2Sowing = 0.0;
   fillingAmount1 = 0.0;
   fillingAmount2 = 0.0;
   roadSpeed = 0.0;
   changingTrailer = 0.0;
   loadingTime = 0.0;
   prepLoading = 0.0;
   numOfSeeds=0;
   for (int i=0;i<MaxPlants; i++)
   	theSeeds[i] = NULL;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
sowTech::~sowTech()
{
    for (int i=0;i<MaxPlants; i++)
   	if (theSeeds[i])
         delete theSeeds[i];
}

/****************************************************************************\
\****************************************************************************/
void sowTech::ReadParameters(fstream * file)
{
   fieldOperationTech::ReadParameters(file);
   Setfile(file);
   GetParameter("prepFillSowing",&prepFillSowing);
   GetParameter("fillingTime1Sowing",&fillingTime1Sowing);
   GetParameter("fillingTime2Sowing",&fillingTime2Sowing);
   GetParameter("fillingAmount1",&fillingAmount1);
   GetParameter("fillingAmount2",&fillingAmount2);
   GetParameter("roadSpeed",&roadSpeed);
   GetParameter("changingTrailer",&changingTrailer);
   GetParameter("prepLoading",&prepLoading);
   GetParameter("loadingTime",&loadingTime);
   Setfile(NULL);
}

/****************************************************************************\
\****************************************************************************/
void sowTech::AddSeed(seed * s)
{
   theSeeds[numOfSeeds] = new seed(*s);
   numOfSeeds++;
}

/****************************************************************************\
\****************************************************************************/
void sowTech::ClearTheSeeds()
{
   for (int i=0;i<MaxPlants;i++)
   {
   	if (theSeeds[i])
         delete theSeeds[i];
      theSeeds[i] = NULL;
   }
}


/****************************************************************************\
\****************************************************************************/
void sowTech::CalcGangHours()
{
   double size = 0.0;
   techEquip aMainTechEquip("maintechequip",0,this);
   aMainTechEquip=*(aGang->GetMainTechEquip());
   size = aMainTechEquip.Getsize();

   double carryCapacity=0.0;
   implement aImplement("implement",0,this);
   linkList<techEquip> * aTechEquipList;
   linkList<techEquip>::PS aTechEquipNode;
   aTechEquipList=aGang->GetTechEquipList();
   aTechEquipList->PeekHead(aTechEquipNode);
   while(aTechEquipNode!=NULL)
   {
      if(strcmp(aTechEquipNode->element->GetName(),"implement")==0)
      {
         aImplement= *(implement *)(aTechEquipNode->element);
         if (aImplement.GettechEquipName()!="SeedDrill")
	         carryCapacity+= aImplement.GetCarryCapacity();
      }
      aTechEquipNode=aTechEquipNode->next;
   }

   if(workingSpeed==0)
      theMessage->FatalError("workingSpeed = 0 in operation ",
                            (char*)operationName.c_str());
   if(size==0)
      theMessage->FatalError("size = 0 in operation ",
                            (char*)operationName.c_str());
   if(area==0)
      theMessage->FatalError("area = 0 in operation ",
                            (char*)operationName.c_str());
   if(fillingAmount1==0)
      theMessage->FatalError("fillingAmount1 = 0 in operation ",
                            (char*)operationName.c_str());
   if(roadSpeed==0)
      theMessage->FatalError("roadSpeed = 0 in operation ",
                            (char*)operationName.c_str());
   if(changingTrailer==0)
      theMessage->FatalError("changingTrailer = 0 in operation ",
                            (char*)operationName.c_str());
   if(carryCapacity==0)
      theMessage->FatalError("carryCapacity = 0 in operation ",
                            (char*)operationName.c_str());

   double amount1;
   double amount2 = 0.0;
   double totalAmount = 0.0;
   for (int i=0;i<MaxPlants;i++)
   	if (theSeeds[i]) totalAmount += theSeeds[i]->GetAmount();

   // Warning the calculation is not completely adjusted to more than two different seeds 4/11-98 JB
   amount1 = 0;
   if (theSeeds[0])
      amount1 = theSeeds[0]->GetAmount();
   else
      theMessage->WarningWithDisplay("sowTech::CalcGangHours - no seed defined");
   if(theSeeds[1])
      amount2 = theSeeds[1]->GetAmount();

   double A = ((area*600)/(workingSpeed*size)+
        (turningTime*(sqrt(area*10000/2)-size*6))/size+
         turningHeadland+
        (disturbanceAllowance*area))/area;

   double temp=0.0;
   if(fillingAmount2==0)
      temp=amount1/fillingAmount1;
   else
      temp=max(amount1/fillingAmount1,
               amount2/fillingAmount2);

   double I =  (fillingTime1Sowing*amount1/100)+
        (fillingTime2Sowing*amount2/100)+
        (prepFillSowing*temp);

   double T = ((distance*0.12)/roadSpeed+changingTrailer)*(totalAmount/carryCapacity);

   double L =  (prepLoading+(loadingTime * carryCapacity/100))*(totalAmount/carryCapacity);

   double S = ((A+I+T+L)*(1+relaxationAllowance)*1.1*area) / 60;

   aGang->SetgangHours(S);
}

/****************************************************************************\
\****************************************************************************/
void sowTech::UpdateProducts(bool contractor)
{
   fieldOperationTech::UpdateProducts(contractor);

   for (int i=0;i<MaxPlants;i++)
   	if (theSeeds[i])
		{
      	theSeeds[i]->Setamount(theSeeds[i]->GetAmount()*area);
   		theProducts->SubtractProduct(theSeeds[i]);
      }
}


