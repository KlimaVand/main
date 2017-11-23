/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef CROPBARE_H
   #define CROPBARE_H

#include <soil.h>
#include <crop.h>

class cropbare:public crop
{
public:
   cropbare(const char * aName, const int aIndex, const base * aOwner);
	virtual void GetStatus (double * aRootRadius,
									double * aRootpF,
									double * aNitrogenDemand,
									double * aNitrateUptakeRate,
									double * aAmmoniumUptakeRate,
                           double * aMinimumSoilNitrate,
                           double * aMinimumSoilAmmonium);
	virtual double Update(double ActivePar);
   virtual bool IsBareCrop() {return true;};
   virtual void Harvest(decomposable* Storage,decomposable *Straw);
};

#endif

