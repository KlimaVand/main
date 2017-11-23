/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include <common.h>
#include "panimal.h"
#ifdef _STANDALONE
	#include <message.h>
#endif

/****************************************************************************\
  Default Constructor
\****************************************************************************/
animalProduct::animalProduct()
   : liveMatter()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
animalProduct::animalProduct(const char * aName, const int aIndex, const base * aOwner)
   : liveMatter(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
animalProduct::animalProduct(const animalProduct& aanimalProduct)
   : liveMatter(aanimalProduct)
{
   animalPrHa  = aanimalProduct.animalPrHa;
	startWeight = aanimalProduct.startWeight;
   endWeight   = aanimalProduct.endWeight;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
animalProduct::~animalProduct()
{
}

/****************************************************************************\
\****************************************************************************/
void animalProduct::InitVariables()
{
	animalPrHa         = 0.0;
	startWeight        = 0.0;
   endWeight          = 0.0;
   ObjType		       = animalObj;
}

/****************************************************************************\
  Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os, const animalProduct& aanimalProduct)
{
  	os << (product &) aanimalProduct;
  	os << setiosflags(ios::left) << setw(40) << "startWeight " << aanimalProduct.startWeight << endl;
  	os << setiosflags(ios::left) << setw(40) << "endWeight " << aanimalProduct.endWeight << endl;
  	os << setiosflags(ios::left) << setw(40) << "animalPrHa " << aanimalProduct.animalPrHa << endl;
  	return os;
}

/****************************************************************************\
\****************************************************************************/
product& animalProduct::operator=(const product& someanimalProduct)
{
	animalProduct* aanimalProduct;
	aanimalProduct=(animalProduct*)(&someanimalProduct);
	liveMatter::operator=(someanimalProduct);
	startWeight	= aanimalProduct->GetStartWeight();
	endWeight	= aanimalProduct->GetEndWeight();
   animalPrHa  = aanimalProduct->GetAnimalPrHa();
	return *this;
}


/****************************************************************************\
\****************************************************************************/
product& animalProduct::operator+(const product& someanimalProduct)
{
   animalProduct* ananimalProduct=(animalProduct*)(&someanimalProduct);
   double aAmount           = ananimalProduct->GetAmount();
	double astartWeight        = ananimalProduct->GetStartWeight();
	double aendWeight        = ananimalProduct->GetEndWeight();
   if(aAmount!=0)
  	{
   	startWeight     = (startWeight*amount + astartWeight*aAmount)/(amount+aAmount);
		endWeight   	  = (endWeight*amount + aendWeight*aAmount)/(amount+aAmount);
   }
	liveMatter::operator+(someanimalProduct);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& animalProduct::operator-(const product& someanimalProduct)
{
	liveMatter::operator-(someanimalProduct);
   if (strcmp(Name,someanimalProduct.GetName())!=0)
	   theMessage->WarningWithDisplay("animalProduct::operator-: different animal products should not be subtracted");
	return *this;
}

/****************************************************************************\
\****************************************************************************/
void animalProduct::showstats(ostream *os)
{
	liveMatter::showstats(os);
	*os << "startWeight        " << startWeight << endl;
   *os << "endWeight          " << endWeight << endl;
   *os << "animalPrHa         " << animalPrHa << endl;
}

/****************************************************************************\
\****************************************************************************/
product* animalProduct::clone() const
{
	animalProduct* aProduct= new animalProduct(*this);
	return aProduct;
}

/****************************************************************************\
\****************************************************************************/
void animalProduct::ReadParameters(fstream * file)
{
	product::ReadParameters(file);  //do not use liveMatter's function - has irrelevant paramters
   Setfile(file);
	SetCritical();
   double N_content;
	GetParameter("animalPrHa",&animalPrHa);
   GetParameter("N_content",&N_content);
   orgN_content=N_content;
	GetParameter("startWeight",&startWeight);
	GetParameter("endWeight",&endWeight);
//	GetParameter("P_content"    ,&P_content);
//	GetParameter("K_content"    ,&K_content);
   UnsetCritical();
   if (!GetParameter("C_content"    ,&C_content))
      C_content = 0.46;
   UnsetCritical();
   Setfile(NULL);
}

