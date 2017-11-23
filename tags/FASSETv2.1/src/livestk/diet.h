/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

// Class: diet                  //Borland C++ 5.02 - FASSET style
#ifndef __diet_H  //Required for current class
  #define __diet_H

#include <bstime.h>
#include <clonlist.h>
#include <product.h>
#include <feed_resource.h>

class diet: public base
{
 bsTime* startDate;
 cloneList <product> *normalfeedItemList;
 cloneList <product> *deficiencyfeedItemList;
 cloneList <product> *surplusfeedItemList;
 feed_resource *grazedItem;
 double housed;
 double propRoughage;		//proportion of energy demand that should be satisfied by roughage
 double propGrazed;
 bool autoFeed, zeroGrazing;

public:

//user created functions

 diet(const char * aName, const int aIndex, const base * aOwner);
	virtual diet* clone() const;
 diet(){};

 ~diet ()  ;

 bsTime* GetstartDate() {return startDate;}
 cloneList <product> * GetnormalfeedItemList() {return normalfeedItemList;}
 cloneList <product> * GetdeficiencyfeedItemList() {return deficiencyfeedItemList;}
 cloneList <product> * GetsurplusfeedItemList() {return surplusfeedItemList;}
 feed_resource * GetgrazedItem() {return grazedItem;};
 double GetHoused() {return housed;};
 void DailyUpdate();
 void ReadParameters (fstream * infile, bool isAutoFeed);
 bool IsGrazing();
 bool GetzeroGrazing() {return zeroGrazing;};
 feed_resource*  LoadFeedItem(bool autoFeed);
 double GetGrazingFeedRequirement(bool zeroGrazed);
 void ShowDiet(fstream * fs, bool header);
 void GetRoughageRequired(linkList <product> *aList, double numberOfAnimals);
 double GetGrassSilageRequired(double numberOfAnimals);
 double GetSilageRequired(double numberOfAnimals, int startCode, int endCode);
 void ClearLists();
 void CreateDefaultDiets(double differentiation);
 double GetpropRoughage() {return propRoughage;};		//proportion of energy demand that should be satisfied by roughage
 double GetpropGrazed() {return propGrazed;};		//proportion of roughage from grazing
 void SetpropGrazed(double aVal) {propGrazed=aVal;};		//proportion of roughage from grazing
 bool GetautoFeed() {return autoFeed;}
 product* FindConservedRoughageFeed();
 product* FindConcentrateFeed();
 product* FindGrazedFeed();
};
#endif

