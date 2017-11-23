#include "../base/common.h"
#include "pigstysection.h"
#include "../data/commonData.h"
pigstysection::pigstysection(const char* aName,const int aIndex,const base* aOwner,commonData * runData,commonData * orignal)
:animalSection(aName,aIndex,aOwner, runData, orignal)
{


	runData->setCritical(true);
	runData->FindSection(Name,Index);
	runData->FindItem("Alias",&Alias);
	runData->setCritical(false);
	orignal->setCritical(true);
	int indexInOld;
		string place;
		orignal->findSection(aName,0,"Alias",Alias,&place,&indexInOld);
		orignal->FindSection(place,indexInOld);

	orignal->FindItem("DaysInSection",&DaysInSection);
	runData->FindItem("DaysInSection",&DaysInSection);


}
pigstysection::pigstysection(const pigstysection& s):animalSection(s)
{
	DaysInSection=s.DaysInSection;
}
pigstysection::pigstysection(const animalSection& s):animalSection(s)
{

}
pigstysection::~pigstysection()
{
}

double pigstysection::GetCapacity(double ProductionCyclus)
{
	return Places*ProductionCyclus/DaysInSection;
}


