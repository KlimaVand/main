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
void fieldOperationTech::ReadParameters(commonData * &file)
{
   operation::ReadParameters(file);

   file->setCritical(false);

   file->FindItem("machinePoolPrice",&machinePoolPrice);

   file->FindItem("disturbanceAllowance",&disturbanceAllowance);
   file->FindItem("relaxationAllowance",&relaxationAllowance);
   file->FindItem("workingSpeed",&workingSpeed);
   file->FindItem("turningTime",&turningTime);
   file->FindItem("turningHeadland",&turningHeadland);

}


/****************************************************************************\
\****************************************************************************/
void fieldOperationTech::UpdateProducts(bool contractor)
{
	// double diesel = aGang->GetgangFuel();

   // theOutput->AddIndicator(2003,"20.03 Diesel consumption by contractor","l",diesel);

}


