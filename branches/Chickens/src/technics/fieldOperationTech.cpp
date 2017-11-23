/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "fieldOperationTech.h"
#include "../products/energy.h"
#include "../products/products.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
fieldOperationTech::fieldOperationTech(const char * aName, const int aIndex, const base * aOwner)
   : operation(aName, aIndex, aOwner)
{
   area = 0.0;

   distance = 0.0;
   oldArea = 0.0;
   oldDistance = 0.0;
   workingSpeed = 0.0;
   turningTime = 0.0;
   turningHeadland = 0.0;
   disturbanceAllowance = 0.0;
   relaxationAllowance = 0.0;
   machinePoolPrice=0.0;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
fieldOperationTech::~fieldOperationTech()
{
}

/****************************************************************************\
\****************************************************************************/
void fieldOperationTech::ReadParameters(fstream * file)
{
   operation::ReadParameters(file);
   Setfile(file);
   UnsetCritical();
   GetParameter("machinePoolPrice",&machinePoolPrice);
   GetParameter("disturbanceAllowance",&disturbanceAllowance);
   GetParameter("relaxationAllowance",&relaxationAllowance);
   GetParameter("workingSpeed",&workingSpeed);
   GetParameter("turningTime",&turningTime);
   GetParameter("turningHeadland",&turningHeadland);
   Setfile(NULL);
}

/****************************************************************************\
\****************************************************************************/
void fieldOperationTech::CalcGangFuel()
{
   selfPropelled aSelfPropelled("selfPropelled",0,this);
   implement     aImplement("implement",0,this);
   double        aGangFuel=0.0;
   techEquip   * aMainTechEquip;

   aMainTechEquip= aGang->GetMainTechEquip();
   if(aMainTechEquip!=NULL)
   {
      if(strcmp(aMainTechEquip->GetName(),"selfPropelled")==0)
      {
         aSelfPropelled= *(selfPropelled *)aMainTechEquip;
         aGangFuel+= aSelfPropelled.GetfuelConsumption();
      }
      if(strcmp(aMainTechEquip->GetName(),"implement")==0)
      {
         aImplement= *(implement *)aMainTechEquip;
         aGangFuel+= aImplement.GetfuelConsumption();
      }
   }
   aGangFuel*=area;
   aGang->SetgangFuel(aGangFuel);
}

/****************************************************************************\
\****************************************************************************/
void fieldOperationTech::UpdateProducts(bool contractor)
{
   double diesel = aGang->GetgangFuel();
   if (contractor)
      theOutput->AddIndicator(economicIndicator,"20.03 Diesel consumption by contractor","l",diesel);
   else
   {
      energy * aEnergyProduct=new energy;
      aEnergyProduct->Setname("DIESEL");
      theProducts->GiveProductInformation(aEnergyProduct);
      aEnergyProduct->Setamount(diesel);
      theProducts->SubtractProduct(aEnergyProduct);
      theOutput->AddIndicator(economicIndicator,"20.01 Diesel consumption","l",diesel);
      delete aEnergyProduct;
   }
}


