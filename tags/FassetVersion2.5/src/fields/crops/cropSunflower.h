/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "cropMaize.h"

class cropSunflower:public cropMaize
{
public:
	cropSunflower(const char * aName, const int aIndex, const base * aOwner, string cropName);
};
