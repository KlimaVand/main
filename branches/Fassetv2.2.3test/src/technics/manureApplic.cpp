/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Created by Nick Hutchings, Sept 2006
\****************************************************************************/
#include "../base/common.h"
#include "manureApplic.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
manureApplic::manureApplic(const char * aName, const int aIndex, const base * aOwner)
   : techEquip(aName, aIndex, aOwner)
{
   propBuried=0.0;
   propAreaWetted=1.0;
   volume=1E30;
	cropContamination = true;
   NH3emissionFactor=0.0;
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
manureApplic::manureApplic(const manureApplic& amanureApplic)
   : techEquip(amanureApplic)
{
         theMessage->FatalError("tetemanure.cpp::attempt to use undefined copy constructor");
}

/****************************************************************************\
  Destructor
\****************************************************************************/
manureApplic::~manureApplic()
{
}

/****************************************************************************\
\****************************************************************************/
void manureApplic::ReadParameters(fstream * file,char * sectionName)
{
   techEquip::ReadParameters(file,sectionName);
   Setfile(file);
   GetParameter("propBuried",&propBuried);
   GetParameter("propAreaWetted",&propAreaWetted);
   GetParameter("volume",&volume);
   GetParameter("NH3emissionFactor",&NH3emissionFactor);
   int dumint = 0;
   GetParameter("cropContamination",&dumint);
   if (dumint == 1)
   	cropContamination=true;
	else
   	cropContamination = false;
   Setfile(NULL);
}


