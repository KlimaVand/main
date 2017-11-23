
#ifndef BIOGASREACTOR_H
  #define BIOGASREACTOR_H

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


   biogasReactor() {};

public:
   //! Constructor with arguments
   biogasReactor(char *aName,const int aIndex,const base* aOwner,commonData * runData,commonData * common);
   //! Copy constructor
   biogasReactor(biogasReactor& s);
   //! Destructor
   virtual ~biogasReactor();
   //! call every day
	virtual void DailyUpdate();
	//! scale size to manure production
	virtual void Scale(double annualAmount);
	//!update global information about contents of reactor
	void GiveIndicator(int indicatorType);
	//!use to output information about the reactor
   friend ostream& operator <<(ostream& os,biogasReactor s);
};


#endif
