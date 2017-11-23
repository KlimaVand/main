/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef CRROPSWHTH
   #define CRROPSWHTH
#include "cropSbarley.h"

class cropSpringWheat:public cropSbarley
{
private:
	double SenescenceHeatThres;
	double SenescenceHeatResp;
public:
	cropSpringWheat(const char * aName, const int aIndex, const base * aOwner);
	virtual void CalcLeafAreaIndices();
	void ReadParameters(string filename);
};

#endif
