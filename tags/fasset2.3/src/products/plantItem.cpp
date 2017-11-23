/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "plantItem.h"
#ifdef _STANDALONE
	#include "../base/message.h"
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
   tradeable = aplantItem.tradeable;
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
   tradeable = 0;
   tradeName.resize(0);
	ObjType=plantItemObj;
}



/****************************************************************************\
\****************************************************************************/
product& plantItem::operator=(const product& somePlantItem)
{
	plantItem* aPlantItem;
	aPlantItem		=	(plantItem*)(&somePlantItem);
	decomposable::operator=(somePlantItem);
	tradeable      = aPlantItem->GetTradeable();
	tradeName      = aPlantItem->GetTradeName();
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& plantItem::operator+(const product& somePlantItem)
{
	plantItem* aPlantItem;
	aPlantItem=(plantItem*)(&somePlantItem);

	tradeable = tradeable && aPlantItem->GetTradeable();
	decomposable::operator+(somePlantItem);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& plantItem::operator-(const product& somePlantItem)
{
	plantItem* aPlantItem;
	aPlantItem=(plantItem*)(&somePlantItem);
	tradeable = tradeable && aPlantItem->GetTradeable();

	if (tradeName!=aPlantItem->GetTradeName())
		theMessage->WarningWithDisplay("plantItem::operator- - plants with different tradenames subtracted");
	decomposable::operator-(somePlantItem);
	return *this;
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
void plantItem::ReadParameters(fstream * file, bool criticals,const char *  sectionName)
{
	decomposable::ReadParameters(file,criticals, sectionName);
	critical=criticals;
	Setfile(file);
	if(FindSection(sectionName,Index)==true)
		{
	GetParameter("tradename",&tradeName);
	GetParameter("tradeable",&tradeable);
		}
	Setfile(NULL);
}

