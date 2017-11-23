#ifndef cropDyersWoad_h
  #define cropDyersWoad_h

#include <clonlist.h>
#include <cropsbrl.h>

class cropDyersWoad: public cropSbarley
{
protected:


public:
   cropDyersWoad(const char * aName, const int aIndex, const base * aOwner);
   virtual double Update(double ActivePar);
};

#endif

