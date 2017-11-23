/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "animalStock.h"
#include "../building/thebuild.h"

/**
 *  Default Constructor
 */
animalStock::animalStock()
   : base()
{

	theStock = new linkList <animal>;

}


/**
 * Constructor with arguments
 * Also calles:
 * base(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
 */
animalStock::animalStock(const char * aName, const int aIndex, const base * aOwner)
	: base(aName, aIndex, aOwner)
{

	theStock = new linkList <animal>;

}

/**
 * Destructor
 */
animalStock::~animalStock()
{
 if (theStock) delete theStock;
}

animalStock::animalStock(const animalStock& a)
{

	theStock=new linkList<animal>;

   for (int i=0;i<a.theStock->NumOfNodes();i++)
   {
	   animal *p = new animal(*(a.theStock->ElementAtNumber(i)));
	   theStock->InsertLast(p);
   }

}
/**
 * Nice to know: Why do we have it when it is empty?
 */
void animalStock::Initialize()
{
}

/**
 * Call checkBalances() for each animal in theStock,
 * Nice to know: why do we have this function sinec it is not used and is calling a function that just exit Fasset
 */

void animalStock::checkBalances(bool show)
{
 for(int i=0;i<theStock->NumOfNodes();i++)
 {
  theStock->ElementAtNumber(i)->checkBalances(show);
 }
}

