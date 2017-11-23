#include "../base/common.h"
#include "pigstysection.h"
#include "../base/commonData.h"
pigstysection::pigstysection(const char* aName,const int aIndex,const base* aOwner,commonData *  &runData,commonData * &original, bool acontrolledVent)
:animalSection(aName,aIndex,aOwner, runData, original, acontrolledVent)
{


	runData->setCritical(true);
	runData->FindSection(Name,Index);
	runData->FindItem("Alias",&Alias);
	runData->setCritical(false);
	original->setCritical(true);
	int indexInOld;
		string place;
		original->findSection(aName,0,"Alias",Alias,&place,&indexInOld);
		original->FindSection(place,indexInOld);

	original->FindItem("DaysInSection",&DaysInSection);
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

double pigstysection::GetCapacity(double ProductionCycle)
{
	return Places*ProductionCycle/DaysInSection;
}


