#include "../base/common.h"
#include "../base/IndicatorOutput.h"
#include "../building/PigHousing.h"
#include "../livestock/swineStock.h"
#include "../base/controlParameters.h"
#include "../base/commonData.h"
#include "../tools/convert.h"

#include <string>
PigHousing::PigHousing(const char* aname,const int aIndex,const base* aOwner,commonData * runData,commonData * common)
   	:stable(aname,aIndex,aOwner, runData, common)
{
	convert conv;
   Livestock = new animalStock(aname,aIndex,aOwner);
   string PigHouseName(aname);
   cout<<PigHouseName<<endl;
   PigHouseName+="(";
   PigHouseName+=conv.IntToString(aIndex);
   PigHouseName+=").Section";


   int numbersOfSections=0;
   int small=9999;

   runData->getSectionsNumbers(PigHouseName,&small,&numbersOfSections);
   for(int i=small;i<=numbersOfSections;i++)
   {

	   pigstysection *aSection=new pigstysection(PigHouseName.c_str(),i,this,runData, common);
	   sections->InsertLast(aSection);
   }
   // see if the flooring sends manure to an inhouse manure store
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

PigHousing::~PigHousing()
{
}
PigHousing::PigHousing(const PigHousing& aStable):stable(aStable)
{
	sections=new linkList<animalSection>();
	for (int i=0;i<aStable.sections->NumOfNodes();i++)
	   {
		animalSection *p = new pigstysection(*(pigstysection *)(aStable.sections->ElementAtNumber(i)));
		sections->InsertLast(p);
		p->GetContains();
	   }
	standartSections=new linkList<animalSection>();
	for (int i=0;i<aStable.standartSections->NumOfNodes();i++)
	   {
		animalSection *p = new pigstysection(*(aStable.standartSections->ElementAtNumber(i)));
		standartSections->InsertLast(p);
	   }
}
/**
 * Update the stable
 */
void PigHousing::DailyUpdate()
{
   stable::DailyUpdate();
   //double hours = GetLabour();
   #ifdef STALDDEBUG
// 	cout << " pigstable::DailyUpdate: " << Nbudget << endl;
	#endif

   //electricity is commented out until I understand how it is calculated
/*	theOutput->AddIndicator("07.02 Labour, stable","hours",hours);
	theOutput->AddIndicator("07.03 Labour, maintance and management","hours",0.20*hours);  // 5% for building maintance + 15% for management
	theOutput->AddIndicator("07.04 Total labour","hours",hours*1.20);
                                                                                       */
}




/**************************************
Minimum ventilation in cubic metres per sec
**************************************/
double PigHousing::CalcMinimumVentilationRate()
{
	return GetNrOfAnimals() * 6.0/3600.0;
};

/**************************************
Maximum ventilation in cubic metres per sec
**************************************/
double PigHousing::CalcMaximumVentilationRate()
{
	return GetNrOfAnimals() * 125.0/3600.0;
};

