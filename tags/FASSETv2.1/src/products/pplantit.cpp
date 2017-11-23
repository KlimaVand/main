/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include <common.h>
#include "pplantit.h"
#ifdef _STANDALONE
	#include <message.h>
#endif

/****************************************************************************\
  Constructor
\****************************************************************************/
plantItem::plantItem()
	: decomposable()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
plantItem::plantItem(const char * aName, const int aIndex, const base * aOwner)
	: decomposable(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
plantItem::plantItem(const plantItem& aplantItem)
	: decomposable(aplantItem)
{
	S_content = aplantItem.S_content;
   tradeable = aplantItem.tradeable;
   primaryProduct = aplantItem.primaryProduct;
   if (aplantItem.tradeName.length()>0)
	   tradeName = aplantItem.tradeName;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
plantItem::~plantItem()
{
}

/****************************************************************************\
\****************************************************************************/
void plantItem::InitVariables()
{
	S_content = 0.0;
   tradeable = 0;
   primaryProduct = 0;
   tradeName.resize(0);
	ObjType=plantItemObj;
}

/****************************************************************************\
  Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os, const plantItem& aplantItem)
{
  	os << (decomposable &) aplantItem;
  	os << setiosflags(ios::left) << setw(40) << "S_content " << aplantItem.S_content << endl;
	os << setiosflags(ios::left) << setw(40) << "tradeable " << aplantItem.tradeable << endl;
	os << setiosflags(ios::left) << setw(40) << "primaryProduct " << aplantItem.primaryProduct << endl;
	os << setiosflags(ios::left) << setw(40) << "tradeName " << aplantItem.tradeName << endl;
	return os;
}

/****************************************************************************\
\****************************************************************************/
product& plantItem::operator=(const product& somePlantItem)
{
   plantItem* aPlantItem;
	aPlantItem		=	(plantItem*)(&somePlantItem);
   decomposable::operator=(somePlantItem);
   S_content	 	= aPlantItem->GetS_content();
   tradeable      = aPlantItem->GetTradeable();
   primaryProduct = aPlantItem->GetPrimaryProduct();
   tradeName      = aPlantItem->GetTradeName();
   return *this;
}

/****************************************************************************\
\****************************************************************************/
product& plantItem::operator+(const product& somePlantItem)
{
	plantItem* aPlantItem;
	aPlantItem=(plantItem*)(&somePlantItem);
	double aAmount		= aPlantItem->GetAmount();
	double aS_content	= aPlantItem->GetS_content();
	if(amount!=0)
		S_content		= (S_content*amount + aAmount*aS_content)/(amount+aAmount);
   else
   	S_content		= aS_content;
   tradeable = tradeable && aPlantItem->GetTradeable();
   primaryProduct = primaryProduct && aPlantItem->GetPrimaryProduct();
   decomposable::operator+(somePlantItem);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& plantItem::operator-(const product& somePlantItem)
{
	plantItem* aPlantItem;
	aPlantItem=(plantItem*)(&somePlantItem);
	if (S_content	!= aPlantItem->GetS_content())
      theMessage->Warning("plantItem::operator- - plantitems with different composition subtracted");
   tradeable = tradeable && aPlantItem->GetTradeable();
   primaryProduct = primaryProduct && aPlantItem->GetPrimaryProduct();
   if (tradeName!=aPlantItem->GetTradeName())
   	theMessage->WarningWithDisplay("plantItem::operator- - plants with different tradenames subtracted");
   decomposable::operator-(somePlantItem);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
void plantItem::showstats(ostream *os)
{
   decomposable::showstats(os);
	*os << "S_content      " << S_content << endl;
   *os << "tradeable      " << tradeable << endl;
   *os << "primaryProduct " << primaryProduct << endl;
   *os << "tradeName      " << tradeName << endl;
}

/****************************************************************************\
\****************************************************************************/
product* plantItem::clone() const
{
	plantItem *aProduct= new plantItem(*this);
	return aProduct;
}

/****************************************************************************\
\****************************************************************************/
void plantItem::ReadParameters(fstream * file)
{
	decomposable::ReadParameters(file);
   SetCritical();
   Setfile(file);
	GetParameter("S_content",&S_content);
   GetParameter("tradename",&tradeName);
	GetParameter("tradeable",&tradeable);
   GetParameter("primaryprod",&primaryProduct);
   Setfile(NULL);
}

