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
#include "forageCutter.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
forageCutter::forageCutter(const char * aName, const int aIndex, const base * aOwner)
   : techEquip(aName, aIndex, aOwner)
{
   residualHeight=0.0;
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
forageCutter::forageCutter(const forageCutter& aforageCutter)
   : techEquip(aforageCutter)
{
         theMessage->FatalError("tetemanure.cpp::attempt to use undefined copy constructor");
}

/****************************************************************************\
  Destructor
\****************************************************************************/
forageCutter::~forageCutter()
{
}

/****************************************************************************\
\****************************************************************************/
void forageCutter::ReadParameters(fstream * file,char * sectionName)
{
   techEquip::ReadParameters(file,sectionName);
   Setfile(file);
   GetParameter("residualHeight",&residualHeight);
   Setfile(NULL);
}


