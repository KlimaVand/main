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


   theWater = NULL;
   theChemical1 = NULL;
   theChemical2 = NULL;
   theChemical3 = NULL;
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
void plantCareTech::ReadParameters(fstream * file)
{
   Setfile(file);
   fieldOperationTech::ReadParameters(file);
   Setfile(file);
   GetParameter("roadSpeed",&roadSpeed);
   GetParameter("specificArea",&specificArea);
   GetParameter("prepFillingTank",&prepFillingTank);
   GetParameter("fillingTime",&fillingTime);
   Setfile(NULL);
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
      theWater=NULL;
   }
   if(theChemical1) {
      delete theChemical1;
      theChemical1=NULL;
   }
   if(theChemical2) {
      delete theChemical2;
      theChemical2=NULL;
   }
   if(theChemical3) {
      delete theChemical3;
      theChemical3=NULL;
   }
}


/****************************************************************************\
\****************************************************************************/
void plantCareTech::CalcGangHours()
{
   techEquip aMainTechEquip("maintechequip",0,this);
   aMainTechEquip=*(aGang->GetMainTechEquip());
   double size = aMainTechEquip.Getsize();
   double tankSize = aMainTechEquip.GetCarryCapacity();

   if(workingSpeed==0)
      theMessage->FatalError("workingSpeed = 0 in operation ",
                            (char*)operationName.c_str());
   if(size==0)
      theMessage->FatalError("size = 0 in operation ",
                            (char*)operationName.c_str());
   if(area==0)
      theMessage->FatalError("area = 0 in operation ",
                            (char*)operationName.c_str());
   if(tankSize==0)
      theMessage->FatalError("tankSize = 0 in operation ",
                            (char*)operationName.c_str());
   if(roadSpeed==0)
      theMessage->FatalError("roadSpeed = 0 in operation ",
                            (char*)operationName.c_str());
   if(specificArea==0)
      theMessage->FatalError("specificArea = 0 in operation ",
                            (char*)operationName.c_str());

   double A = (((area*600)/(workingSpeed*size))+
        ((turningTime*(sqrt(area*10000/2)-2*size))/size)+
         (turningHeadland+disturbanceAllowance*area) )/area;

   double n = ceil(specificArea*theWater->GetAmount()/tankSize);

   double T = (distance*n*0.12)/(roadSpeed*specificArea);

   double I = ((prepFillingTank*n) +
        (fillingTime*specificArea*theWater->GetAmount()/100)) /
         specificArea;

   double S = ((A+T+I)*(1+relaxationAllowance)*1.1*area)/60;

   aGang->SetgangHours(S);
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
	      theOutput->AddIndicator(environmentalIndicator,"80.01 Herbicide consumption","TFI",aAmount);
			break;
      case fungicide:
      	theOutput->AddIndicator(environmentalIndicator,"80.02 Fungicide consumption","TFI",aAmount);
			break;
      case insecticide:
      	theOutput->AddIndicator(environmentalIndicator,"80.03 Insecticide consumption","TFI",aAmount);
         break;
		default:
			break;
   }
}

