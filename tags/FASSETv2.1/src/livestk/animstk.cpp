/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include <common.h>
#include <animstk.h>
#include <thebuild.h>

/****************************************************************************\
  Default Constructor
\****************************************************************************/
animalStock::animalStock()
   : base()
{
   aStable = NULL;
	theStock = new linkList <animal>;
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
animalStock::animalStock(const char * aName, const int aIndex, const base * aOwner)
	: base(aName, aIndex, aOwner)
{
   aStable = NULL;
	theStock = new linkList <animal>;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
animalStock::~animalStock()
{
 if (theStock) delete theStock;
}

/****************************************************************************\
  Initialize
\****************************************************************************/
void animalStock::Initialize()
{
}

//************************************************

void animalStock::checkBalances(bool show)
{
 for(int i=0;i<theStock->NumOfNodes();i++)
 {
  theStock->ElementAtNumber(i)->checkBalances(show);
 }
}

