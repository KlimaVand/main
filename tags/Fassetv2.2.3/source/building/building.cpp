/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
  Archive
   Author Lars Kroll lokal 1875
     Date 15/5 97
 Revision
 ----------------------------------------------------------------------------
      Log
 Describes a generalised building
\****************************************************************************/
#include "../base/common.h"
#include "building.h"
#include "../base/message.h"

/****************************************************************************\
  Default Constructor
\****************************************************************************/
building::building()
	:base()
{
   SetArea(0);
  	scalable				= false;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
building::~building()
{
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
building::building( const char *aName,const int aIndex,const base* aOwner)
	:base(aName,aIndex,aOwner)
{
   Area=0.0;
   scalable = false;
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
building::building(const building& b)
	:base(b)
{
	Area=b.Area;
   scalable = b.scalable;
}

/**
 * 	Reads data from file
 */
void building::ReadParameters(fstream* file)
{
   Setfile(file);
   if (!FindSection(Name,Index))
   	theMessage->FatalError("building::ReadParameters - cannot find section");
   GetParameter("Area",&Area);
//   UnsetCritical();   // set uncritical as otherwise there must be economic data for slurry pits etc
	initEconomy(this);
   Setfile(NULL);
}

/**
 * Does nothing
*/

void building::DailyUpdate()
{
//GiveIndicator();

}



/**
 * CalcLP  indicators for LP-model (not implemented yet)
 */

void building::CalcLP()
{

}

/****************************************************************************\
	GiveIndicator
\****************************************************************************/

void building::GiveIndicator(int indicatorType)
{
	if (indicatorType == economicIndicator)
			GiveEconomicIndicators();
}


/****************************************************************************\
	Operator << for output
\****************************************************************************/

 ostream& operator<< (ostream& os,building* b)
{
  //os << (base*) &b << endl;   // << isn't overloaded for base

  os << setiosflags(ios::left) << setw(40) << "Name " << b->GetName() << "(" << b->Index << ")" << endl;
  os << setiosflags(ios::left) << setw(40) << "Area " << b->GetArea() << endl;
  return os;
}



