/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef cropOilRadish_h
  #define cropOilRadish_h

#include <clonlist.h>
#include <cropbeet.h>

class cropOilRadish: public cropBeet
{
protected:


public:
   cropOilRadish(const char * aName, const int aIndex, const base * aOwner);
   virtual double Update(double ActivePar);
};

#endif

