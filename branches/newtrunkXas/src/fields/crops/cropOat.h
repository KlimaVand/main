/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "crop.h"

#ifndef CROPOAT
  #define CROPOAT

class cropOat:public crop
{
public:
	cropOat(const char * aName, const int aIndex, const base * aOwner, string CropName);
   virtual double Update(double ActivePar);
};

#endif
