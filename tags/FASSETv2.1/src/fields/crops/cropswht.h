/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include <cropsbrl.h>
#ifndef CROPSWHT
  #define CROPSWHT

class cropSpringWheat:public cropSbarley
{
public:
	cropSpringWheat(const char * aName, const int aIndex, const base * aOwner);
};

#endif
