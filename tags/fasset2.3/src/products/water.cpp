/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "water.h"
#ifdef _STANDALONE
	#include "../base/message.h"
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

product& water::operator-(const product& someWater)
{
	water* aWater;
	aWater=(water*)(&someWater);
   double aN_content	= aWater->GetN_content();
   if (N_content != aN_content)
   	theMessage->FatalError("water::operator- - Water with different compositions subtracted");
	product::operator-(someWater);
	return *this;
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
void water::ReadParameters(fstream * file,bool criticals,const char *  sectionName)
{
	product::ReadParameters(file,criticals, sectionName);
	critical=criticals;
   Setfile(file);
   if(FindSection(sectionName,Index)==true)
   	{
	GetParameter("N_content",&N_content);
   	}
   Setfile(NULL);
}

