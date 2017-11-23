/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "legislation.h"
#include "../products/product.h"
#include "../products/products.h"
#include "../products/premium.h"

/****************************************************************************\
  Default Constructor
\****************************************************************************/
legislate::legislate()
	: base()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
legislate::legislate(char * aName, int aIndex, base * aOwner)
	: base(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Destructor
\****************************************************************************/
legislate::~legislate()
{
	delete fieldAttribList;
   delete taxList;
}

/****************************************************************************\
\****************************************************************************/
void legislate::InitVariables()
{
	fieldAttribList = new linkList <product>;
   taxList = new linkList <taxLegislation>;
   simulationYear = 0;


}

/****************************************************************************\
\****************************************************************************/
void legislate::Initialize(string LawApplyFileName)
{
	OpenInputFileString(LawApplyFileName);
   if (fieldAttribList) delete fieldAttribList;
   fieldAttribList = theProducts->GetSubsetofProductList(premiumObj);
   InitializeTax();
   FindSection("general");

	CloseInputFile();
}

/****************************************************************************\
\****************************************************************************/
void legislate::InitializeTax()
{
   SetCritical();
   int first,num;
   GetSectionNumbers("legislat",&first,&num);
	for(int index=first;index<(first+num);index++)
   {
      taxLegislation* Alegislation = new taxLegislation("legislat",index,this);
      Alegislation->ReadParameters(file);
      taxList->InsertLast(Alegislation);
   }
   taxLegislation* Alegislation = new taxLegislation(0,0,0,1000,"legislat",-1,this);      // default tax
   taxList->InsertLast(Alegislation);
}



/****************************************************************************\
\****************************************************************************/
double legislate::GetContribution(char* aFieldName)
{
	premium* aPremium;
	for (int i=0; (i<fieldAttribList->NumOfNodes()); i++)
   {
   	aPremium = (premium*) fieldAttribList->ElementAtNumber(i);
      if (strcmp(aPremium->GetPlantName().c_str(),aFieldName)==0)
      	return aPremium->GetaPrice()->GetExpectedSellPriceYear(1);
   }
   return 0.0;
}

/****************************************************************************\
\****************************************************************************/
double legislate::GetCurrentTax(int type)
{

	for (int i=0;i<taxList->NumOfNodes();i++)
   {

   	if (((taxList->ElementAtNumber(i)->getType())==(type+1)) && (taxList->ElementAtNumber(i)->taxThisYear(simulationYear)))
      	return taxList->ElementAtNumber(i)->getTax();
   }
   return 0.0;                                     // default element at end of taxlist
}
double legislate::GetCurrentLevel(int type)
{

	for (int i=0;i<taxList->NumOfNodes();i++)
   {

   	if (((taxList->ElementAtNumber(i)->getType())==(type+1)) && (taxList->ElementAtNumber(i)->taxThisYear(simulationYear)))
      	return taxList->ElementAtNumber(i)->getLevel();
   }
   return 0.0;                                     // default element at end of taxlist
}
/****************************************************************************\
\****************************************************************************/
void legislate::Update()
{
   simulationYear++;
}

