/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "irrigateTech.h"
#include "../products/energy.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
irrigateTech::irrigateTech(const char * aName, const int aIndex, const base * aOwner)
   : fieldOperationTech(aName, aIndex, aOwner)
{
   workingLength = 0.0;
   constantPerShift = 0.0;
   variablePerLength = 0.0;
   hoursPerDay = 0.0;
   theWater = NULL;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
irrigateTech::~irrigateTech()
{
   if(theWater)
      delete theWater;
}

/****************************************************************************\
\****************************************************************************/
void irrigateTech::ReadParameters(fstream * file)
{
   fieldOperationTech::ReadParameters(file);
   Setfile(file);
   GetParameter("workingLength",&workingLength);
   GetParameter("constantPerShift",&constantPerShift);
   GetParameter("variablePerLength",&variablePerLength);
   GetParameter("hoursPerDay",&hoursPerDay);
   Setfile(NULL);
}

/****************************************************************************\
\****************************************************************************/
void irrigateTech::SetTheWater(water * aWater)
{
   theWater = new water(*aWater);
}

/****************************************************************************\
\****************************************************************************/
void irrigateTech::ClearTheWater()
{
   if(theWater) {
      delete theWater;
      theWater=NULL;
   }
}


/****************************************************************************\
\****************************************************************************/
void irrigateTech::CalcGangHours()
{
   linkList<techEquip> * aTechEquipList;
   aTechEquipList=aGang->GetTechEquipList();
   linkList<techEquip>::PS aTechEquipNode;

   double size=0.0;
   techEquip aMainTechEquip("maintechequip",0,this);
   aMainTechEquip=*(aGang->GetMainTechEquip());
   size = aMainTechEquip.Getsize();

   double pumpCapacity=0.0;
   installation aWaterPump("installation",0,this);
   aTechEquipList->PeekHead(aTechEquipNode);
   while(aTechEquipNode!=NULL)
   {
      if((strcmp(aTechEquipNode->element->GetName(),"installation")==0) &&
         (aTechEquipNode->element->GettechEquipName()=="WaterPump"))
         break;
      aTechEquipNode=aTechEquipNode->next;
   }
   if(aTechEquipNode!=NULL)
   {
      aWaterPump= *(installation *) aTechEquipNode->element;
      pumpCapacity=aWaterPump.Getcapacity();
   }

   if(pumpCapacity==0)
      theMessage->FatalError("pumpCapacity = 0 in operation ",
                            (char*)operationName.c_str());

   double h1 = (size*workingLength)/10000;                    // area/placing

   double E =  (constantPerShift+(variablePerLength*workingLength/100))*
        (1+relaxationAllowance);                       // movingtime pr. placing

   double S =  ((E/h1)*1.1*area)/60;                          // hours total

   aGang->SetgangHours(S);
}

/****************************************************************************\
\****************************************************************************/
void irrigateTech::CalcGangElectricity()
{
   double aGangElectricity=0.0;

   installation * aInstallation;
   aInstallation = new installation("installation",0,this);

   techEquip * aMainTechEquip;
   aMainTechEquip= aGang->GetMainTechEquip();

   if (strcmp(aMainTechEquip->GetName(),"installation")==0)
   {
      *aInstallation= * (installation *) aMainTechEquip;
      aGangElectricity+= aInstallation->GetelectricityConsumption();
   }
   delete aInstallation;

   double totalWaterAmount=theWater->GetAmount()*area;

   aGangElectricity*=totalWaterAmount;
   aGang->SetgangElectricity(int(aGangElectricity));
}

/****************************************************************************\
\****************************************************************************/
void irrigateTech::UpdateProducts(bool contractor)
{
   energy * aEnergyProduct=new energy;
   aEnergyProduct->Setname("ELECTRICITY");
   theProducts->GiveProductInformation(aEnergyProduct);
   aEnergyProduct->Setamount(aGang->GetgangElectricity());
   theOutput->AddIndicator(economicIndicator,"20.02 Electricity consumption","KWh",aEnergyProduct->GetAmount());
   theProducts->SubtractProduct(aEnergyProduct);
   delete aEnergyProduct;
   (*theWater)*area;
   theOutput->AddIndicator(environmentalIndicator,"70.02 Irrigation","cubic metre",theWater->GetAmount());
   theProducts->SubtractProduct(theWater);
   #ifdef DEVELOPING
   contractor=contractor;
   #endif
}


