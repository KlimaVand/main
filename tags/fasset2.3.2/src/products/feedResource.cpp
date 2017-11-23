/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Cattle feed class
NJH 21.06.00
\****************************************************************************/

#include "../base/common.h"
#include "feedResource.h"

/****************************************************************************\
Default constructor
\****************************************************************************/
feed_resource::feed_resource()
{
   area_occupied		= 0.0;
   time_since_dunged = 0.0;
   isSupplement		= false;
}

/****************************************************************************\
Copy constructor
\****************************************************************************/
feed_resource::feed_resource(const feed_resource& afeed_resource)
:feedItem(afeed_resource)   //Inclusion results in error when delete due to Original
{
   area_occupied     = afeed_resource.area_occupied;
   time_since_dunged = afeed_resource.time_since_dunged;
	isSupplement		= afeed_resource.isSupplement;
}

/****************************************************************************\
Copy constructor - copy from feedItem
\****************************************************************************/
feed_resource::feed_resource(const feedItem& afeedItem)
:feedItem(afeedItem)
{
   area_occupied     = 0.0;
   time_since_dunged = 0.0;
	isSupplement		= false;
}

/****************************************************************************\
Modified BMP
\****************************************************************************/
product* feed_resource::clone() const
{
   return new feed_resource(*this);
}

/****************************************************************************\
\****************************************************************************/
product& feed_resource::operator=(const product& someFeed_resource)
{
   feed_resource*  aFeed_resource;
	aFeed_resource = (feed_resource*)(&someFeed_resource);

   feedItem::operator=(someFeed_resource);

	area_occupied        = aFeed_resource->Getarea_occupied();
	time_since_dunged    = aFeed_resource->GetTime_since_dunged();
	isSupplement         = aFeed_resource->GetIsSupplement();
	return *this;
}

