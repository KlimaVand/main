/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include <common.h>
#include "pwater.h"
#ifdef _STANDALONE
	#include <message.h>
#endif

/****************************************************************************\
  Default Constructor
\****************************************************************************/
water::water()
   : product()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
water::water(const char * aName, const int aIndex, const base * aOwner)
   : product(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
water::water(const water& awater)
   : product(awater)
{
   N_content = awater.N_content;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
water::~water()
{
}

/****************************************************************************\
\****************************************************************************/
void water::InitVariables()
{
   N_content 	= 0.0;
   ObjType		= waterObj;
}

/****************************************************************************\
  Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os, const water& awater)
{
  	os << (product &) awater;
  	os << setiosflags(ios::left) << setw(40) << "N_content " << awater.N_content << endl;
   return os;
}

/****************************************************************************\
   Assingment operator
\****************************************************************************/
product& water::operator=(const product& someWater)
{
	water* aWater;
	aWater=(water*)(&someWater);
	product::operator=(someWater);
	N_content = aWater->GetN_content();
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& water::operator+(const product& someWater)
{
	water* aWater;
	aWater=(water*)(&someWater);
	double aAmount		= aWater->GetAmount();
	double aN_content	= aWater->GetN_content();
	if (amount!=0)
   	N_content = (N_content*amount + aAmount*aN_content)/(amount+aAmount);
   else
   	N_content = aN_content;
	product::operator+(someWater);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& water::operator-(const product& someWater)
{
	water* aWater;
	aWater=(water*)(&someWater);
   double aN_content	= aWater->GetN_content();
   if (N_content != aN_content)
   	theMessage->Warning("water::operator- - Water with different compositions subtracted");
	product::operator-(someWater);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
void water::showstats(ostream *os)
{
	product::showstats(os);
	*os << "N_content         " << N_content << endl;
}

/****************************************************************************\
\****************************************************************************/
product* water::clone() const
{
	water* aProduct= new water(*this);
	return (product*) aProduct;

}

/****************************************************************************\
\****************************************************************************/
void water::ReadParameters(fstream * file)
{
	product::ReadParameters(file);
   Setfile(file);
	GetParameter("N_content",&N_content);
   Setfile(NULL);
}

