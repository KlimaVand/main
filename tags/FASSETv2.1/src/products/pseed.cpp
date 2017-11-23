/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "pseed.h"
#ifdef _STANDALONE
	#include <message.h>
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
void seed::showstats(ostream *os)
{
	liveMatter::showstats(os);
	*os << "standardDensity   " << standardDensity << endl;
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
void seed::ReadParameters(fstream * file)
{
	product::ReadParameters(file);
   SetCritical();
   Setfile(file);
	GetParameter("N_content",&orgN_content.n);
	GetParameter("dryMatter",&dryMatter);
   UnsetCritical();
   if (!GetParameter("C_content"    ,&C_content))
      C_content = 0.46 * dryMatter;
   GetParameter("standardDensity",&standardDensity);
   Setfile(NULL);
}


