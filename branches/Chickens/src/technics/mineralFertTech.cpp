/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "mineralFertTech.h"
#include "../products/energy.h"
#include "../products/nitrogen.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
mineralFertTech::mineralFertTech(const char * aName, const int aIndex, const base * aOwner)
   : fieldOperationTech(aName, aIndex, aOwner)
{
   prepLoading = 0.0;
   roadSpeed = 0.0;
   vol = 0.0;
   fillingTimeSpreader = 0.0;
   prepFilling = 0.0;
   changingTrailer = 0.0;
   loadingTime = 0.0;
   theFertilizer1 = NULL;
   theFertilizer2 = NULL;
   theFertilizer3 = NULL;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
mineralFertTech::~mineralFertTech()
{
   if(theFertilizer1)
      delete theFertilizer1;
   if(theFertilizer2)
      delete theFertilizer2;
   if(theFertilizer3)
      delete theFertilizer3;
}

/****************************************************************************\
\****************************************************************************/
void mineralFertTech::ReadParameters(fstream * file)
{
   fieldOperationTech::ReadParameters(file);
   Setfile(file);
   GetParameter("prepLoading",&prepLoading);
   GetParameter("roadSpeed",&roadSpeed);
   GetParameter("vol",&vol);
   GetParameter("fillingTimeSpreader",&fillingTimeSpreader);
   GetParameter("prepFilling",&prepFilling);
   GetParameter("changingTrailer",&changingTrailer);
   GetParameter("loadingTime",&loadingTime);
   Setfile(NULL);
}

/****************************************************************************\
\****************************************************************************/
void mineralFertTech::SetTheFertilizers(fertilizer * aFertilizer1, fertilizer * aFertilizer2, fertilizer * aFertilizer3)
{
   if(aFertilizer1)
   {
      theFertilizer1 = new fertilizer;
      *theFertilizer1 = *aFertilizer1;
   }

   if(aFertilizer2)
   {
      theFertilizer2 = new fertilizer;
      *theFertilizer2 = *aFertilizer2;
   }

   if(aFertilizer3)
   {
      theFertilizer3 = new fertilizer;
      *theFertilizer3 = *aFertilizer3;
   }
}

/****************************************************************************\
\****************************************************************************/
void mineralFertTech::ClearTheFertilizers()
{
   if(theFertilizer1) {
      delete theFertilizer1;
      theFertilizer1=NULL;
   }
   if(theFertilizer2) {
      delete theFertilizer2;
      theFertilizer2=NULL;
   }
   if(theFertilizer3) {
      delete theFertilizer3;
      theFertilizer3=NULL;
   }
}



/****************************************************************************\
\****************************************************************************/
void mineralFertTech::CalcGangHours()
{
   double size = 0.0;
   techEquip aMainTechEquip("maintechequip",0,this);
   aMainTechEquip=*(aGang->GetMainTechEquip());
   size = aMainTechEquip.Getsize();

   double carryCapacity=0.0;
   implement aImplement("implement",0,this);
   linkList<techEquip> * aTechEquipList;
   aTechEquipList=aGang->GetTechEquipList();
   linkList<techEquip>::PS aTechEquipNode;
   aTechEquipList->PeekHead(aTechEquipNode);
   while(aTechEquipNode!=NULL)
   {
      if(strcmp(aTechEquipNode->element->GetName(),"implement")==0)
      {
         aImplement= *(implement *)(aTechEquipNode->element);
         carryCapacity+= aImplement.GetCarryCapacity();
      }
      aTechEquipNode=aTechEquipNode->next;
   }

   double amount=theFertilizer1->GetAmount();
   if (theFertilizer2)
      amount+=theFertilizer2->GetAmount();
   if (theFertilizer3)
      amount+=theFertilizer3->GetAmount();

   if(workingSpeed==0)
      theMessage->FatalError("workingSpeed = 0 in operation ",
                            (char*)operationName.c_str());
   if(size==0)
      theMessage->FatalError("size = 0 in operation ",
                            (char*)operationName.c_str());
   if(area==0)
      theMessage->FatalError("area = 0 in operation ",
                            (char*)operationName.c_str());
   if(roadSpeed==0)
      theMessage->FatalError("roadSpeed = 0 in operation ",
                            (char*)operationName.c_str());
   if(carryCapacity==0)
      theMessage->FatalError("carryCapacity = 0 in operation ",
                            (char*)operationName.c_str());

   double A = ((area*600)/(workingSpeed*size) +
        (turningTime*(sqrt(area*10000/2)-2*size))/size +
         turningHeadland +
        (disturbanceAllowance*area) ) / area;

	double I = ((fillingTimeSpreader*amount)/100) +
		 ((prepFilling*amount)/(vol*0.95));

	double T = ((((distance*0.12)/roadSpeed)+changingTrailer) *
			 (amount/carryCapacity));

	double L = ((loadingTime*amount)/100)+
		 ((prepLoading*amount)/carryCapacity);

	double S = ((A+I+T+L)*(1+relaxationAllowance)*1.1*area)/60;

	aGang->SetgangHours(S);
}

/****************************************************************************\
\****************************************************************************/
void mineralFertTech::UpdateFertilizer(fertilizer* aFertilizer)
{
   (*aFertilizer)*area;
   theProducts->SubtractProduct(aFertilizer);

   nitrogen N_content = aFertilizer->GetN_content()*aFertilizer->GetAmount();
	double P_content = (aFertilizer->GetAmount())*(aFertilizer->GetP_content());
	double K_content = (aFertilizer->GetAmount())*(aFertilizer->GetK_content());
	theOutput->AddIndicator(environmentalIndicator,"31.01 N from mineral fertilizer","kg N",N_content.n);
	theOutput->AddIndicator(environmentalIndicator,"50.01 Phosphorus from mineral fertilizer","kg P",P_content);
	theOutput->AddIndicator(environmentalIndicator,"60.01 Potassium from mineral fertilizer","kg K",K_content);
}

void mineralFertTech::UpdateProducts(bool contractor)
{
   fieldOperationTech::UpdateProducts(contractor);

   if (theFertilizer1)
      UpdateFertilizer(theFertilizer1);
   if (theFertilizer2)
      UpdateFertilizer(theFertilizer2);
   if (theFertilizer3)
      UpdateFertilizer(theFertilizer3);
}


