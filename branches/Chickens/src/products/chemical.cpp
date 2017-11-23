/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "chemical.h"
#ifdef _STANDALONE
	#include <message.h>
#endif

/****************************************************************************\
  Default Constructor
\****************************************************************************/
chemical::chemical()
   : product()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
chemical::chemical(const char * aName, const int aIndex, const base * aOwner)
   : product(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
chemical::chemical(const chemical& achemical)
	: product(achemical)
{
	type = achemical.type;
	activeSubstancePerUnit = achemical.activeSubstancePerUnit;
	amountPerTFI = achemical.amountPerTFI;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
chemical::~chemical()
{
}

/****************************************************************************\
\****************************************************************************/
void chemical::InitVariables()
{
   type = 0;
	activeSubstancePerUnit = 0.0;
	amountPerTFI = 0.0;
	ObjType=chemicalObj;
}


/****************************************************************************\
\****************************************************************************/
product& chemical::operator=(const product& someChemical)
{
	chemical* aChemical;
	aChemical=(chemical*)(&someChemical);
   product::operator=(someChemical);
	type							= aChemical->Gettype();
	activeSubstancePerUnit	= aChemical->GetactiveSubstancePerUnit();     // denne var kommenteret før??   JB
	amountPerTFI				= aChemical->GetAmountPerTFI();
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& chemical::operator+(const product& someChemical)
{
	chemical* aChemical;
	aChemical=(chemical*)(&someChemical);
	if (aChemical->GetName()!=Name)
		theMessage->FatalError("chemical::operator+ - chemicals should not be added");
	if (type!=aChemical->Gettype())
		theMessage->FatalError("chemical::operator+ - chemicals with different types added");
	if (activeSubstancePerUnit != aChemical->GetactiveSubstancePerUnit() || amountPerTFI != aChemical->GetAmountPerTFI())
		theMessage->FatalError("chemical::operator+ - chemicals with different composition added");
   product::operator+(someChemical);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& chemical::operator-(const product& someChemical)
{
	chemical* aChemical;
	aChemical=(chemical*)(&someChemical);
	if (aChemical->GetName()!=Name)
		  	theMessage->FatalError("chemical::operator- - chemicals should not be subtracted");
	if (type!=aChemical->Gettype())
		theMessage->FatalError("chemical::operator- - chemicals with different types subtracted");
	if (activeSubstancePerUnit != aChemical->GetactiveSubstancePerUnit() || amountPerTFI != aChemical->GetAmountPerTFI())
		theMessage->FatalError("chemical::operator- - chemicals with different composition subtracted");
	product::operator-(someChemical);
   return *this;
}

product*  chemical::clone() const
{
	chemical* aProduct= new chemical(*this);
	return (product *) aProduct;
}


/****************************************************************************\
\****************************************************************************/
void chemical::SetByTFI(double aTFI)
{
	amount= aTFI*amountPerTFI;
}

/****************************************************************************\
\****************************************************************************/
int chemical::GetChemType(string aChemType)
{
	int typeRes=-1;
	if (aChemType=="herbicide")
   	typeRes=herbicide;
	if (aChemType=="fungicide")
   	typeRes=fungicide;
   if (aChemType=="pesticide")
   	typeRes=pesticide;
   if (aChemType=="microfertilizer")
   	typeRes=microfertilizer;
   if (aChemType=="insecticide")
   	typeRes=insecticide;
   if(typeRes==-1)
   {
 		theMessage->WarningWithDisplay("chemical::GetChemType - the chemical unit type <",(char*)aChemType.c_str(),"> does not exist");
      return -1;
   }
	else
		return typeRes;
}

/****************************************************************************\
\****************************************************************************/
void chemical::ReadParameters(fstream * file,bool criticals,const char *  sectionName )
{
	product::ReadParameters(file,criticals, sectionName);
   critical=criticals;
   Setfile(file);
	if(FindSection(sectionName,Index)==true)
	{
   char   aType [41];
   aType[0] = '\0';
	GetParameter("type",aType);
	GetParameter("activeSubstancePerUnit",&activeSubstancePerUnit);
	GetParameter("amountPerTFI",&amountPerTFI);
   if(amountPerTFI<=0)
	   theMessage->FatalError("Chemical product <",Name,"> has illegal value in field amountPerTFI");
	string aChemType=aType;
	Settype(GetChemType(aChemType));
   Setfile(NULL);
	}
}

