/**
 * Represent a form of energy such a fuel and electricity
 */
#include "../base/common.h"
#include "energy.h"
#ifdef _STANDALONE
	#include <message.h>
#endif

/****************************************************************************\
  Default Constructor
\****************************************************************************/
energy::energy()
   : product()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/

energy::energy(const char * aName, const int aIndex, const base * aOwner)
   : product(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
energy::energy(const energy& aenergy)
   : product(aenergy)
{
   kWhPerUnit = aenergy.kWhPerUnit;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
energy::~energy()
{
}

/****************************************************************************\
\****************************************************************************/
void energy::InitVariables()
{
   kWhPerUnit 	= 0.0;
   ObjType		= energyObj;
}



/****************************************************************************\
\****************************************************************************/
product& energy::operator=(const product& someEnergy)
{
	energy* aEnergy;
	aEnergy=(energy*)(&someEnergy);
   product::operator=(someEnergy);
	kWhPerUnit=aEnergy->GetkWhPerUnit();
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& energy::operator+(const product& someEnergy)
{
	energy* aEnergy;
	aEnergy=(energy*)(&someEnergy);
	double aAmount		= aEnergy->GetAmount();
	double akWhPerUnit= aEnergy->GetkWhPerUnit();
	if (amount!=0)
   	kWhPerUnit	= (kWhPerUnit*amount + akWhPerUnit*aAmount)/(amount+aAmount);
   else
      kWhPerUnit 	= akWhPerUnit;
	product::operator+(someEnergy);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& energy::operator-(const product& someEnergy)
{
	energy* aEnergy;
	aEnergy=(energy*)(&someEnergy);
   if (kWhPerUnit!=aEnergy->GetkWhPerUnit())
   	theMessage->WarningWithDisplay("energy::operator- - energy with different composition subtracted");
	product::operator-(someEnergy);
	return *this;
}


/****************************************************************************\
\****************************************************************************/
product* energy::clone() const
{
	energy* aProduct= new energy(*this);
	return (product *) aProduct;
}

/**
 * Reading the input file
 */
void energy::ReadParameters(fstream * file,bool criticals,const char *  sectionName)
{
	product::ReadParameters(file, criticals,sectionName);
	 Setfile(file);
	if(FindSection(sectionName,Index)==true)
		{
   critical=criticals;

	GetParameter("kWhPerUnit",&kWhPerUnit);
   Setfile(NULL);
		}
}

