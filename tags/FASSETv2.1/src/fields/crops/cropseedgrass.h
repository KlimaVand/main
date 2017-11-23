/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef cropSeedGrass_h
  #define cropSeedGrass_h

#include <clonlist.h>
#include <cropgrss.h>

class cropSeedGrass: public cropRyegrass
{
protected:
public:
	cropSeedGrass(const char * aName, const int aIndex, const base * aOwner);
   virtual void Harvest(decomposable* Storage, decomposable* Straw);
};

#endif

