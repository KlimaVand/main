/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "fertilizer.h"
#ifdef _STANDALONE
	#include "../base/message.h"
#endif

/****************************************************************************\
  Constructor
\****************************************************************************/
fertilizer::fertilizer()
   : product()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
fertilizer::fertilizer(const char * aName, const int aIndex, const base * aOwner)
   : product(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
fertilizer::fertilizer(const fertilizer& afertilizer)
   : product(afertilizer)
{
   N_content 	= afertilizer.N_content;
   P_content 	= afertilizer.P_content;
   K_content 	= afertilizer.K_content;
   Ca_content 	= afertilizer.Ca_content;
   Cl_content 	= afertilizer.Cl_content;
   MineralForm = afertilizer.MineralForm;
   NH4_fraction= afertilizer.NH4_fraction;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
fertilizer::~fertilizer()
{
}

/****************************************************************************\
\****************************************************************************/
void fertilizer::InitVariables()
{
   N_content.Clear();
   P_content 	= 0.0;
   K_content 	= 0.0;
   Ca_content 	= 0.0;
   Cl_content 	= 0.0;
   NH4_fraction= 0.0;
   MineralForm = "SOLID";                      // default form
   ObjType		= fertilizerObj;
}

/****************************************************************************\
  Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os, const fertilizer& afertilizer)
{
//  	os << (product &) afertilizer;
   if (afertilizer.N_content.n>0.0)
	  	os << setiosflags(ios::left) << " N applied " << (afertilizer.N_content.n * afertilizer.amount) << " kg/ha ";
/*  	os << setiosflags(ios::left) << setw(40) << "P_content " << afertilizer.P_content << endl;
  	os << setiosflags(ios::left) << setw(40) << "K_content " << afertilizer.K_content << endl;
  	os << setiosflags(ios::left) << setw(40) << "Ca_content " << afertilizer.Ca_content << endl;*/
   return os;
}

/****************************************************************************\
\****************************************************************************/
product& fertilizer::operator=(const product& someFertilizer)
{
	fertilizer* aFertilizer;
	aFertilizer=(fertilizer*)(&someFertilizer);

	product::operator=(someFertilizer);

   N_content  = aFertilizer->GetN_content();
	P_content  = aFertilizer->GetP_content();
	K_content  = aFertilizer->GetK_content();
	Ca_content = aFertilizer->GetCa_content();
	Cl_content = aFertilizer->GetCl_content();
	MineralForm = aFertilizer->GetMineralForm();
   NH4_fraction= aFertilizer->GetNH4_fraction();

	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& fertilizer::operator+(const product& someFertilizer)
{
	fertilizer* aFertilizer;
	aFertilizer=(fertilizer*)(&someFertilizer);

	double aAmount=aFertilizer->GetAmount();
	nitrogen aN_content = aFertilizer->GetN_content();
	double aP_content = aFertilizer->GetP_content();
	double aK_content = aFertilizer->GetK_content();
	double aCa_content= aFertilizer->GetCa_content();
	double aCl_content= aFertilizer->GetCl_content();
   double aNH4_fraction = aFertilizer->GetNH4_fraction();
   string aMineralForm = aFertilizer->GetMineralForm();

   if (amount!=0)
   {
		N_content	= (N_content*amount + aN_content*aAmount)/(amount+aAmount);
		P_content	= (P_content*amount + aAmount*aP_content)/(amount+aAmount);
		K_content	= (K_content*amount + aAmount*aK_content)/(amount+aAmount);
		Ca_content	= (Ca_content*amount + aAmount*aCa_content)/(amount+aAmount);
		Cl_content	= (Cl_content*amount + aAmount*aCl_content)/(amount+aAmount);
      NH4_fraction= (NH4_fraction*amount + aAmount*aNH4_fraction)/(amount+aAmount);
      MineralForm = aMineralForm;
   }
   else
	{
   	N_content	= aN_content;
      P_content	= aP_content;
      K_content	= aK_content;
		Ca_content	= aCa_content;
		Cl_content	= aCl_content;
      NH4_fraction= aNH4_fraction;
   }

	product::operator+(someFertilizer);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& fertilizer::operator-(const product& someFertilizer)
{
	fertilizer* aFertilizer;
	aFertilizer=(fertilizer*)(&someFertilizer);

   if(N_content != aFertilizer->GetN_content() ||
		P_content != aFertilizer->GetP_content()||
		K_content != aFertilizer->GetK_content()||
		Ca_content!= aFertilizer->GetCa_content())
         theMessage->Warning("fertilizer::operator- - fertilizers with different composition subtracted");

	product::operator-(someFertilizer);
	return *this;
}



/****************************************************************************\
\****************************************************************************/
product*  fertilizer::clone() const
{
	fertilizer* aProduct= new fertilizer(*this);
	return (product*) aProduct;
}

/**
 * read parameter from file
 */
void fertilizer::ReadParameters(fstream * file, bool criticals,const char *  sectionName)
{
	product::ReadParameters(file, criticals,sectionName);
	  Setfile(file);
	if(FindSection(sectionName,Index)==true)
		{
   critical=criticals;

	GetParameter("N_content",&N_content.n);
	GetParameter("P_content",&P_content);
	GetParameter("K_content",&K_content);
	GetParameter("Ca_content",&Ca_content);
	GetParameter("Cl_content",&Cl_content);

   MineralForm = "SOLID";
   NH4_fraction = 0.0;
	GetParameter("NH4_fraction",&NH4_fraction);
   GetParameter("MineralForm",&MineralForm);

   Setfile(NULL);
		}
}

