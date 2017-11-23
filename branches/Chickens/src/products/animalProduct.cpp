/**
 * This class represent animal that are produced
 */
#include "../base/common.h"
#include "animalProduct.h"
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

	startWeight = aanimalProduct.startWeight;
	endWeight = aanimalProduct.endWeight;

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

	startWeight        = 0.0;
   endWeight          = 0.0;
   ObjType		       = animalObj;
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
product* animalProduct::clone() const
{
	animalProduct* aProduct= new animalProduct(*this);
	return aProduct;
}

/**
 * Read the parameter from file
 */
void animalProduct::ReadParameters(fstream * file, bool criticals,const char * sectionName)
{
	product::ReadParameters(file, criticals,sectionName);  //do not use liveMatter's function - has irrelevant paramters
	Setfile(file);
	if(FindSection(sectionName,Index)==true)
	  {


	critical=criticals;
   double N_content;
   GetParameter("orgN_content",&N_content);
   orgN_content=N_content;
	GetParameter("startWeight",&startWeight);
	GetParameter("endWeight",&endWeight);

  GetParameter("C_content"    ,&C_content);
   Setfile(NULL);
	  }
}

