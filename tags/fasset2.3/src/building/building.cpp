/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\

\****************************************************************************/
#include "../base/common.h"
#include "building.h"
#include "../tools/convert.h"
#include "../data/commonData.h"

/****************************************************************************\
  Default Constructor
\****************************************************************************/
building::building()
	:base()
{

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
building::building( const char *aName,const int aIndex,const base* aOwner,commonData * runData,commonData * orignal)
	:base(aName,aIndex,aOwner)
{
	string Alias;
   Area=0.0;
   scalable = false;

   	string sectionName(aName);
   	convert conv;
   	sectionName=sectionName+"("+conv.IntToString(aIndex)+")";
   	runData->setCritical(true);
   	runData->FindSection(Name,Index);
   	runData->FindItem("Alias",&Alias);
	runData->setCritical(false);
	orignal->setCritical(true);
	int indexInOld;
		string place;
		orignal->findSection(aName,0,"Alias",Alias,&place,&indexInOld);
		orignal->FindSection(place,indexInOld);

   orignal->FindItem("Area",&Area);
   runData->FindItem("Area",&Area);
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
 * Does nothing
*/

void building::DailyUpdate()
{
//GiveIndicator();

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

  //os << setiosflags(ios::left) << setw(40) << "Name " << b->GetName() << "(" << b->Index << ")" << endl;
  os << setiosflags(ios::left) << setw(40) << "Area " << b->GetArea() << endl;
  return os;
}




