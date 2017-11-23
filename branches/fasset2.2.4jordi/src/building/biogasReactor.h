/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 NJH - 2.7.99
  ----------------------------------------------------------------------------
      Log
\****************************************************************************/

#ifndef BIOGASREACTOR_H
  #define BIOGASREACTOR_H
//#define STALDDEBUG

#include "manstore.h"
#include "building.h"
#include "../products/manure.h"
#include "../base/common.h"
#include "../base/budget.h"
/****************************************************************************\
 Class: biogas reactor class

\****************************************************************************/

class biogasReactor: public manurestore, public building
{
private:


//   biogasReactor() {};

public:
   // Constructor with arguments
   biogasReactor(char *aName,const int aIndex,const base* aOwner);
   // Copy constructor
   biogasReactor(biogasReactor& s);
   // Destructor
   virtual ~biogasReactor();
   virtual void ReadParameters(fstream * file);
	virtual void DailyUpdate();
   virtual void GiveIndicator(int indicatorType);
	virtual void Scale(double annualAmount);
   friend ostream& operator <<(ostream& os,biogasReactor s);
};


#endif
