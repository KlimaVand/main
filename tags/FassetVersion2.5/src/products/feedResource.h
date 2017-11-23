/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Cattle feed resource
NJH 21.06.00
Note that like feedItem, all amounts are on a fresh weight basis (tonnes per tonne FW)
\****************************************************************************/
#ifndef __PFEEDRES_H
  #define __PFEEDRES_H

#include "feedItem.h"

class feed_resource : public feedItem
{
protected:
double
   area_occupied,          // proportion of field occupied by this patch
   time_since_dunged;
   bool isSupplement;

public:
   //Default constructor
   feed_resource();

	// BMP: only constructor above used!!! feed_resource(const char * aName, const int aIndex, const base * aOwner);

   //Copy constructor
   feed_resource(const feedItem& afeed_resource);

   feed_resource(const feed_resource& afeed_resource);
	virtual product* clone() const; // BMP added "virtual" added, without this no reason to call "clone"
   //Destructor
   virtual ~feed_resource(){}; // BMP: if "clone" should be used as supposed, destructor must be virtual to
   // Get and set functions for attributes
   double Getarea_occupied() const {return area_occupied;};
   double GetTime_since_dunged() const {return time_since_dunged;};
   bool GetIsSupplement() const {return isSupplement;};

//   double GetSFUamount() {return SFUamount;}
   void Setarea_occupied (double aarea_occupied){area_occupied = aarea_occupied;};
   void SetTime_since_dunged (double atime_since_dunged){time_since_dunged = atime_since_dunged;};
   void SetIsSupplement(bool aSupplement) {isSupplement = aSupplement;};
//   product& operator+(const product& someFeed_resource);
	product& operator=(const product& someFeed_resource);
//	void AddfeedItem(const liveMatter& somefeedItem);

};
#endif
