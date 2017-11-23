/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
NJH 27.4.00
Skeleton for cattle housing - needs to be provided with the functions required by GAMS
\****************************************************************************/
#include "../base/common.h"
#include "../base/IndicatorOutput.h"
#include "cattlehouse.h"
#include "../livestock/dairyStock.h"
#include "../base/controlParameters.h"
#include "animalSection.h"
#include "../data/commonData.h"
#include "../tools/convert.h"
cattlehouse::cattlehouse(char* aname,const int aIndex,const base* aOwner,commonData * runData,commonData * common)
   	:stable(aname,aIndex,aOwner,runData, common)
{
	Livestock=new dairyStock(aname,aIndex,aOwner);
	insideTemperature=theClimate->GetMeanTemperature();
	convert conv;
	string sectionsName(aname);
	sectionsName=sectionsName+"("+conv.IntToString(aIndex)+").Section";

	int large=0;
	int small=4444;
	runData->getSectionsNumbers(sectionsName,&small,&large);

	for(int inx=small;inx<=large;inx++)
	{
		animalSection *aSection=new animalSection((char *)sectionsName.c_str(),inx,this,runData, common);
		sections->InsertLast(aSection);
	}
	for(int i=0;i<inhouseStores->NumOfNodes();i++)
	{
		manurestore * aStore=inhouseStores->ElementAtNumber(i);
	 	for(int i=0;i<sections->NumOfNodes();i++)
	 	{
	 		animalSection * aSection=sections->ElementAtNumber(i);
	 		aSection->LinkManureStores(aStore);
		}
	 }
	 if (theControlParameters->GetvolatModel()>0)
	 		checkArea();
}
cattlehouse::cattlehouse(const cattlehouse& cattlehouse):stable(cattlehouse)
{
	Livestock=new dairyStock(*cattlehouse.Livestock);
}
cattlehouse::~cattlehouse()
{
}



/**
 * returns the string "CATTLE"
 */
string cattlehouse::GetAnimalType()
{
   return "CATTLE";
}

/**
 * Set livestock pointer, so that the animal house can find out about the animals it has
 */
void cattlehouse::SetLivestock(dairyStock* aLivestock)
{
   Livestock=aLivestock;
}

/**
 * calling stable::DailyUpdate()
 */
void cattlehouse::DailyUpdate()
{
   stable::DailyUpdate();
}


