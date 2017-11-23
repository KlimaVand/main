/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
 \****************************************************************************/
#include "../base/common.h"
#include "decomposable.h"
#ifdef _STANDALONE
#include "../base/message.h"
#endif

/****************************************************************************\
  Default Constructor
 \****************************************************************************/
decomposable::decomposable() :
	liveMatter()
{
	InitVariables();
}

/****************************************************************************\
  Constructor with arguments
 \****************************************************************************/
decomposable::decomposable(const char * aName, const int aIndex,
		const base * aOwner) :
	liveMatter(aName, aIndex, aOwner)
{
	InitVariables();
}

/****************************************************************************\
  Copy Constructor
 \****************************************************************************/
decomposable::decomposable(const decomposable& d) :
	liveMatter(d)
{
	ObjType = d.ObjType;
	kAOM1 = d.kAOM1;
	kAOM2 = d.kAOM2;
	AOM1_fraction = d.AOM1_fraction;
	AOM2_fraction = d.AOM2_fraction;
	VFA_fraction = d.VFA_fraction;
	AOM1_Nfraction = d.AOM1_Nfraction;
	AOM2_Nfraction = d.AOM2_Nfraction;
	ReduceTurnover = d.ReduceTurnover;
}

/****************************************************************************\
  Destructor
 \****************************************************************************/
decomposable::~decomposable()
{
}

/****************************************************************************\
\****************************************************************************/
void decomposable::InitVariables()
{
	liveMatter::InitVariables();
	kAOM1 = 0.0;
	kAOM2 = 0.0;
	AOM1_fraction = -1.0;
	AOM2_fraction = -1.0;
	VFA_fraction = -1.0;
	AOM1_Nfraction = -1.0;
	AOM2_Nfraction = -1.0;
	ReduceTurnover = 0;

	ObjType = decomposableObj;
}

/*****************************************************************\
   Assignment operator
 \****************************************************************************/
product& decomposable::operator=(const product& somedecomposable)
{
	liveMatter::operator=(somedecomposable);
	decomposable* adecomposable = (decomposable*) (&somedecomposable);
	kAOM1 = adecomposable->kAOM1;
	kAOM2 = adecomposable->kAOM2;
	AOM1_fraction = adecomposable->AOM1_fraction;
	AOM2_fraction = adecomposable->AOM2_fraction;
	VFA_fraction = adecomposable->VFA_fraction;
	AOM1_Nfraction = adecomposable->AOM1_Nfraction;
	AOM2_Nfraction = adecomposable->AOM2_Nfraction;
	ReduceTurnover = adecomposable->ReduceTurnover;

	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& decomposable::operator+(const product& somedecomposable)
{
	if (ObjType == decomposableObj)
		theMessage->WarningWithDisplay(
				"decomposable::operator+ - decomposable should not be added");
	else
	{
		if (amount == 0 && somedecomposable.GetAmount() > 0)
		{
			decomposable* adecomposable = (decomposable*) (&somedecomposable);
			kAOM1 = adecomposable->kAOM1;
			kAOM2 = adecomposable->kAOM2;
			AOM1_fraction = adecomposable->AOM1_fraction;
			AOM2_fraction = adecomposable->AOM2_fraction;
			VFA_fraction = adecomposable->VFA_fraction;
			AOM1_Nfraction = adecomposable->AOM1_Nfraction;
			AOM2_Nfraction = adecomposable->AOM2_Nfraction;
		}
		liveMatter::operator+(somedecomposable);
	}
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& decomposable::operator-(const product& somedecomposable)
{
	if (ObjType == decomposableObj)
		theMessage->WarningWithDisplay(
				"decomposable::operator- - decomposable should not be subtracted");
	else
		liveMatter::operator-(somedecomposable);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product* decomposable::clone() const
{
	decomposable *aProduct = new decomposable(*this);
	return aProduct;
}

/****************************************************************************\
\****************************************************************************/
void decomposable::ReadParameters(fstream * file, bool criticals,const char *  sectionName)
{
	liveMatter::ReadParameters(file,criticals, sectionName);
	Setfile(file);
	if(FindSection(sectionName,Index)==true)
		{
			critical= criticals;
			GetParameter("kAOM1", &kAOM1);
			GetParameter("kAOM2", &kAOM2);

			GetParameter("AOM1_fraction", &AOM1_fraction);
			GetParameter("AOM2_fraction", &AOM2_fraction);
			GetParameter("VFA_fraction", &VFA_fraction);
			GetParameter("AOM1_Nfraction", &AOM1_Nfraction);
			GetParameter("AOM2_Nfraction", &AOM2_Nfraction);
			Setfile(NULL);
		}
}

