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
   theManure = NULL;
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
void animalFertTech::ReadParameters(fstream * file)
{
   fieldOperationTech::ReadParameters(file);
   Setfile(file);
   GetParameter("prepLoading",&prepLoading);
   GetParameter("roadSpeed",&roadSpeed);
   GetParameter("spreadingCapacity",&spreadingCapacity);
   GetParameter("loadingCapacity",&loadingCapacity);
   GetParameter("prepForSpreading",&prepForSpreading);
   GetParameter("j",&j);
   GetParameter("f",&f);
   GetParameter("g",&g);
   Setfile(NULL);
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
      theManure=NULL;
   }
}



/****************************************************************************\
\****************************************************************************/
void animalFertTech::CalcGangHours()
{
   linkList<techEquip> * aTechEquipList;
   aTechEquipList=aGang->GetTechEquipList();
   linkList<techEquip>::PS aTechEquipNode;
   double amount = theManure->GetAmount();

   double size = 0.0;
   techEquip aMainTechEquip("maintechequip",0,this);
   aMainTechEquip=*(aGang->GetMainTechEquip());
   size = aMainTechEquip.Getsize();
   double carryCapacity = aMainTechEquip.GetCarryCapacity();

   double pumpSize=0.0;
   aTechEquipList->PeekHead(aTechEquipNode);
   while(aTechEquipNode!=NULL)
   {
      if(aTechEquipNode->element->GettechEquipName()=="SlurryPump")
         break;
      aTechEquipNode=aTechEquipNode->next;
   }
   if(aTechEquipNode!=NULL)
      pumpSize=aTechEquipNode->element->Getsize();
/*
   if(pumpSize==0)
      theMessage->FatalError("animalFertTech::CalcGangHours - pumpSize = 0 in operation ",
                            (char*)operationName.c_str());
*/
   if(spreadingCapacity==0)
      theMessage->FatalError("animalFertTech::CalcGangHours - spreadingCapacity = 0 in operation ",
                            (char*)operationName.c_str());
   if(size==0)
      theMessage->FatalError("animalFertTech::CalcGangHours - size = 0 in operation ",
                            (char*)operationName.c_str());
   if(carryCapacity==0)
      theMessage->FatalError("animalFertTech::CalcGangHours - carryCapacity = 0 in operation ",
                            (char*)operationName.c_str());
   if(workingSpeed==0)
      theMessage->FatalError("animalFertTech::CalcGangHours - fieldSpeed = 0 in operation ",
                            (char*)operationName.c_str());
   if(roadSpeed==0)
      theMessage->FatalError("animalFertTech::CalcGangHours - roadSpeed = 0 in operation ",
                             (char*)operationName.c_str());
   double A1= ((area*amount*1000/spreadingCapacity) +
        (turningTime*(sqrt(area*10000/2)-2*size)/size) +
         turningHeadland +
        (disturbanceAllowance*area)) *
        (1+relaxationAllowance)/area;

   double A2= (((amount*1000)/carryCapacity) *
          prepForSpreading) *
         (1+relaxationAllowance);

   double B = (((1.5*sqrt(area*10000/2))*amount*120) /
         (carryCapacity*workingSpeed)) *
         (1+relaxationAllowance);

   double T = ((distance*amount*0.12*1000) /
        (carryCapacity*roadSpeed)) *
        (1+relaxationAllowance);

   /*double L = (((amount*60)/((j*(-150.1+66.7*pumpSize) +
          carryCapacity)*1000) *
          prepLoading) +
         (amount*1000/loadingCapacity)) *
         (1+relaxationAllowance);          */
   double L;
   if (j==0)              // i.e. solid manure
   	L = (amount*1000/carryCapacity*prepLoading+
              amount*1000/loadingCapacity)*
              (1+relaxationAllowance);
   else                   // i.e. FLUID manure
   	L = (amount*1000/carryCapacity*prepLoading+
           amount*60/(-150.1+66.7*pumpSize))*
           (1+relaxationAllowance);


   double N = f*(ceil((A1+A2+B+T)/L+1))+g;

   double M = f*(N+1)+g;

   double S=0.0;
   if((A1+A2+B+T+L)/N > L)
      S = (((A1+A2+B+T+L)/N)*M/60)*1.1*area;
   else
      S = L*M*area/60;
/*
   double V=(60*S/1.1-(A1+A2+B+T+L)); // Waiting time
  	S+=V;
*/
   aGang->SetgangHours(S);
}

/****************************************************************************\
\****************************************************************************/
void animalFertTech::CalcGangFuel()
{
   linkList<techEquip> * aTechEquipList;
   linkList<techEquip>::PS aTechEquipNode;

   implement aImplement("implement",0,this);

   double aGangFuel = 0.0;
   double fuelConsumptionTransport = 0.0;

   aTechEquipList=aGang->GetTechEquipList();
   aTechEquipList->PeekHead(aTechEquipNode);
   while(aTechEquipNode!=NULL)
   {
      if(strcmp(aTechEquipNode->element->GetName(),"implement")==0)
      {
         aImplement= *(implement *)(aTechEquipNode->element);
         aGangFuel+= aImplement.GetfuelConsumption();
      }
      if(strcmp(aTechEquipNode->element->GetName(),"tractor")==0)
         fuelConsumptionTransport += ((tractor*) aTechEquipNode->element)->GetFuelConsumptionTransport();
      aTechEquipNode=aTechEquipNode->next;
   }
   aGangFuel*= area*theManure->GetAmount();

   // Fuel for transport
   aGangFuel+= theManure->GetAmount()*(2*distance/1000)*fuelConsumptionTransport;

   aGang->SetgangFuel(aGangFuel);
}

/****************************************************************************\
\****************************************************************************/
techEquip *animalFertTech::GetApplicationTech()
{
	linkList <techEquip> * techFarmList = theTechnics->GetFarmList();
   linkList <techEquip>::PS aTechEquipInFarmList;
	techFarmList->PeekHead(aTechEquipInFarmList);
   while (aTechEquipInFarmList!=NULL)
   {
//   cout << aTechEquipInFarmList->element->GettechEquipName() << endl;
      if (aTechEquipInFarmList->element->GettechEquipName()!=*techEquipList->ElementAtNumber(0))
      	aTechEquipInFarmList=aTechEquipInFarmList->next;
      else
	      return  aTechEquipInFarmList->element;
   }
   theMessage->WarningWithDisplay("Equipment missing: ",*techEquipList->ElementAtNumber(0));
	theMessage->FatalError("animalFertTech::GetApplicationTech () - Unable to find manure application equipment amongst farm's equipment");
	return NULL;
}



