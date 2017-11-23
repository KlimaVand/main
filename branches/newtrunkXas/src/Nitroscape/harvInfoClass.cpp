#ifdef NITROSCAPE
/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "harvInfoClass.h"

/****************************************************************************\
  Constructor
\****************************************************************************/
#include <iostream>
using namespace std;
harvInfoClass::harvInfoClass()
{
	amountDM=0.0;
	amountN=0.0;
	amountC=0.0;
	productName=new char[200];

	strcpy(productName, "No crop");

}


#endif
