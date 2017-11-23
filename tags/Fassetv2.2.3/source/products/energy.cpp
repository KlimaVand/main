/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
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
  Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os, const energy& aenergy)
{
  	os << (product &) aenergy;
  	os << setiosflags(ios::left) << setw(40) << "kWhPerUnit " << aenergy.kWhPerUnit << endl;
  	return os;
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
void energy::showstats(ostream *os)
{
	product::showstats(os);
	*os << "kWhPerUnit   " << kWhPerUnit << endl;
}

/****************************************************************************\
\****************************************************************************/
product* energy::clone() const
{
	energy* aProduct= new energy(*this);
	return (product *) aProduct;
}

/****************************************************************************\
\****************************************************************************/
void energy::ReadParameters(fstream * file)
{
	product::ReadParameters(file);
   SetCritical();
   Setfile(file);
	GetParameter("kWhPerUnit",&kWhPerUnit);
   Setfile(NULL);
}

