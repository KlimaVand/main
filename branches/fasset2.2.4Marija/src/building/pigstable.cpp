/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
  Archive
   Author Lars Kroll lokal 1875
     Date 21/5 97
 Revision
 ----------------------------------------------------------------------------
      Log
\****************************************************************************/
#include "../base/common.h"
#include "../base/IndicatorOutput.h"
#include "../building/pigstable.h"
#include "../livestock/swineStock.h"
#include "../base/controlParameters.h"

pigstable::pigstable(char* aname,const int aIndex,const base* aOwner,
             animalStock* alivestock)
   	:stable(aname,aIndex,aOwner,alivestock)
{
   Livestock = NULL;
   batchInterval = 0.0;
}

pigstable::~pigstable()
{
}

/**
 * Summarize how many hours is needed to clean and feed the animal
 */
double pigstable::GetLabour()
{
	double hours = 0.0;
	for(int i=0;i<sections->NumOfNodes();i++)
   {
    pigstysection *aSection=(pigstysection *) sections->ElementAtNumber(i);
    hours+=aSection->GetLabour();
   }
   return hours;
}
/**
 * return "PIGS"
 */
string pigstable::GetAnimalType()
{
   return "PIG";
}

/**
 * Should calculate Calculate the total time required for a sow production cycle
 * but will trough a fatal error because of wrong implementation
 */

double pigstable::GetSowsProductionCyclus()
{
	theMessage->FatalError("pigstable::GetSowsProductionCyclus() is not implemented correct");
	double productionCyclus = 0.0;
	for(int i=0;i<sections->NumOfNodes();i++)
   {
    pigstysection *aSection=(pigstysection *) sections->ElementAtNumber(i);
    char * Farrowing=NULL;
    strcpy(Farrowing,"Farrowing");
    char * Mating=NULL;
    strcpy(Mating,"Mating");
    char * Pregnant=NULL;
    strcpy(Pregnant,"Pregnant");
    if ((aSection->GetName()==Farrowing)||(aSection->GetName()==Mating)||(aSection->GetName()==Pregnant))
    productionCyclus+=aSection->GetDaysInSection();
   }
   return productionCyclus;
}

/**
 * Should calculate Calculate the total time required for a finished pig production cycle
 * but will trough a fatal error because of wrong implementation
 */

double pigstable::GetBaconersProductionCyclus()
{
	theMessage->FatalError("pigstable::GetBaconersProductionCyclus() is not implemented correct");
   double productionCyclus = 0.0;
	for(int i=0;i<sections->NumOfNodes();i++)
   {    char * Weaning=NULL;
	   strcpy(Weaning,"Weaning");
	   char * Finishing=NULL;
	   strcpy(Finishing,"Finishing");
    pigstysection *aSection=(pigstysection *) sections->ElementAtNumber(i);
    if ((aSection->GetName()==Weaning)||(aSection->GetName()==Finishing))
    productionCyclus+=aSection->GetDaysInSection();
   }
   return productionCyclus;
}

/* Should Calculate the capacity for sow production
 * but will trough a fatal error because of wrong implementation
 */

double pigstable::GetSowsCapacity()
{
	theMessage->FatalError("pigstable::GetSowsCapacity() is not implemented correct");
   double capacity = 1e10;
   double productionCyclus = 0.0;
	for(int i=0;i<sections->NumOfNodes();i++)
   {
	   char * Farrowing=NULL;
	   strcpy(Farrowing,"Farrowing");
	   char * Mating=NULL;
	   strcpy(Mating,"Mating");
	   char * Pregnant=NULL;
	   strcpy(Pregnant,"Pregnant");
    pigstysection *aSection=(pigstysection *) sections->ElementAtNumber(i);
    if ((aSection->GetName()==Farrowing)||(aSection->GetName()==Mating)||(aSection->GetName()==Pregnant))
    productionCyclus+=aSection->GetDaysInSection();
    capacity = min(capacity,aSection->GetCapacity(GetSowsProductionCyclus()));
   }
	return capacity;
}

/**
 * Should Calculate the capacity for finished pig production
 * but will trough a fatal error because of wrong implementation
 */

double pigstable::GetBaconersCapacity()
{
	theMessage->FatalError("pigstable::GetBaconersCapacity() is not implemented correct");
   double capacity = 1e10;
   double productionCyclus = 0.0;
	for(int i=0;i<sections->NumOfNodes();i++)
   {
    pigstysection *aSection=(pigstysection *) sections->ElementAtNumber(i);
    char * Weaning=NULL;
    strcpy(Weaning,"Weaning");
    char * Finishing=NULL;
    strcpy(Finishing,"Finishing");
    if ((aSection->GetName()==Weaning)||(aSection->GetName()==Finishing))
    productionCyclus+=aSection->GetDaysInSection();
    capacity = min(capacity,aSection->GetCapacity(productionCyclus));
   }
	return capacity;//*theTime.daysInYear()/Finishing->GetDaysInSection();
}

/**
 * Set livestock pointer, so that the animal house can find out about the animals it has
 */

void pigstable::SetLivestock(swineStock* aLivestock)
{
   Livestock=aLivestock;
}

/**
 * Update the stable
 */
void pigstable::DailyUpdate()
{
   stable::DailyUpdate();
   //double hours = GetLabour();
   #ifdef STALDDEBUG
// 	cout << " pigstable::DailyUpdate: " << Nbudget << endl;
	#endif

   //electricity is commented out until I understand how it is calculated
/*	theOutput->AddIndicator(economicIndicator,"07.02 Labour, stable","hours",hours);
	theOutput->AddIndicator(economicIndicator,"07.03 Labour, maintance and management","hours",0.20*hours);  // 5% for building maintance + 15% for management
	theOutput->AddIndicator(economicIndicator,"07.04 Total labour","hours",hours*1.20);
                                                                                       */
}

void pigstable::ReadParameters(fstream * file)
{
   Setfile(file);
   Setcur_pos(0);
   FindSection("Pigstable",Index);  // find the correct place in the input file
   stable::ReadParameters(file);
   string namestr;
   char Indexstr[10];
   sprintf(Indexstr,"(%d)",Index);
//   FindSection("Pigstable",Index);  // find the correct place in the input file
   GetParameter("batchInterval",&batchInterval);

   // get info on sections
   Setcur_pos(0);
	namestr=(string) "Pigstable"+Indexstr+"."+"Section";
   int first,num;
   GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of sections associated with this instance
 //  double cumulativePartition=0.0;
	for(int inx=first;inx<(first+num);inx++)
   {
    pigstysection *aSection=new pigstysection((char *)namestr.c_str(),inx,this);
    aSection->SetScalable(true);
    aSection->ReadParameters(file);
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
 Setfile(NULL);
}

void pigstable::PutStockInSection()
{
   if (Livestock==NULL)
   	theMessage->Warning("pigstable::stable present but no livestock");
}

void pigstable::CalcLP()
{
   stable::CalcLP();

}

bool pigstable::CapacityOverflow()
{
   return ((GetSowsCapacity()>Livestock->GetNumberOfSwines()) || (GetBaconersCapacity()>Livestock->GetNumberOfbaconers()));
}

void pigstable::GiveIndicator(int indicatorType)
{
	stable::GiveIndicator(indicatorType);
}

/**************************************
Minimum ventilation in cubic metres per sec
**************************************/
double pigstable::CalcMinimumVentilationRate()
{
	return GetNrOfAnimals() * 6.0/3600.0;
};

/**************************************
Maximum ventilation in cubic metres per sec
**************************************/
double pigstable::CalcMaximumVentilationRate()
{
	return GetNrOfAnimals() * 125.0/3600.0;
};

