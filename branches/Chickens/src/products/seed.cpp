/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "seed.h"
#ifdef _STANDALONE
	#include "../base/message.h"
#endif

/****************************************************************************\
  Default Constructor
\****************************************************************************/
seed::seed()
   : liveMatter()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
seed::seed(const char * aName, const int aIndex, const base * aOwner)
   : liveMatter(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
seed::seed(const seed& aseed)
   : liveMatter(aseed)
{
   standardDensity = aseed.standardDensity;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
seed::~seed()
{
}

/****************************************************************************\
\****************************************************************************/
void seed::InitVariables()
{
   liveMatter::InitVariables();
   standardDensity = 0.0;
   ObjType         = seedObj;
}

/****************************************************************************\
  Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os, const seed& aseed)
{
//  	os << (liveMatter &) aseed;
   os << setiosflags(ios::left) << " Amount applied " << (aseed.amount) << " kg/ha " << "standardDensity " << aseed.standardDensity << " kg/ha ";
   return os;
}

/****************************************************************************\
   Assignment operator
\****************************************************************************/
product& seed::operator=(const product& someSeed)
{
	seed* aSeed;
	aSeed=(seed*)(&someSeed);
   liveMatter::operator=(someSeed);
	standardDensity = aSeed->GetStandardDensity();
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& seed::operator+(const product& someSeed)
{
	seed* aSeed;
	aSeed=(seed*)(&someSeed);
	double aAmount		      = aSeed->GetAmount();
   double astandardDensity = aSeed->GetStandardDensity();
   if (amount!=0)
   {
		standardDensity	= (standardDensity*amount + aAmount*astandardDensity)/(amount+aAmount);
   }
   else
   {
		standardDensity	= astandardDensity;
   }
   liveMatter::operator+(someSeed);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& seed::operator-(const product& someSeed)
{
   liveMatter::operator-(someSeed);
	return *this;
}



/****************************************************************************\
\****************************************************************************/
product* seed::clone() const
{
	seed* aProduct= new seed(*this);
	return (product*) aProduct;
}

/****************************************************************************\
\****************************************************************************/
void seed::ReadParameters(fstream * file, bool criticals,const char *  sectionName)
{
	product::ReadParameters(file, criticals, sectionName);
   critical=criticals;
   Setfile(file);
   if(FindSection(sectionName,Index)==true)
   	{
	GetParameter("orgN_content",&orgN_content.n);
	GetParameter("dryMatter",&dryMatter);
   GetParameter("C_content"    ,&C_content);
   GetParameter("standardDensity",&standardDensity);
   	}
   Setfile(NULL);
}


