/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "cropPea.h"

class cropSoybean:public cropPea
{
public:
	cropSoybean(const char * aName, const int aIndex, const base * aOwner, string cropName);
};
