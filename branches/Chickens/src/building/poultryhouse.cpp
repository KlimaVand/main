/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "../base/IndicatorOutput.h"
#include "../building/poultryhouse.h"
#include "../livestock/swineStock.h"
#include "../base/controlParameters.h"

poultryhouse::poultryhouse(char* aname,const int aIndex,const base* aOwner,
             animalStock* alivestock)
   	:stable(aname,aIndex,aOwner,alivestock)
{
   Livestock = NULL;
   batchInterval = 0.0;
}

poultryhouse::~poultryhouse()
{
}

/**
 * Summarize how many hours is needed to clean and feed the animal
 */
double poultryhouse::GetLabour()
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
string poultryhouse::GetAnimalType()
{
   return "POULTRY";
}

/**
 * Should calculate Calculate the total time required for a sow production cycle
 * but will trough a fatal error because of wrong implementation
 */

double poultryhouse::GetProductionCycle()
{
	theMessage->FatalError("poultryhouse::GetProductionCycle() is not implemented correct");
	double productionCyclus = 0.0;
/*	for(int i=0;i<sections->NumOfNodes();i++)
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
   }*/
   return productionCyclus;
}

/**
 * Should calculate Calculate the total time required for a finished pig production cycle
 * but will trough a fatal error because of wrong implementation
 */
/*
double poultryhouse::GetBaconersProductionCyclus()
{
	theMessage->FatalError("poultryhouse::GetBaconersProductionCyclus() is not implemented correct");
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
  */
/* Should Calculate the capacity for sow production
 * but will trough a fatal error because of wrong implementation
 */
    /*
double poultryhouse::GetSowsCapacity()
{
	theMessage->FatalError("poultryhouse::GetSowsCapacity() is not implemented correct");
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
      */
/**
 * Should Calculate the capacity for finished pig production
 * but will trough a fatal error because of wrong implementation
 */
        /*
double poultryhouse::GetBaconersCapacity()
{
	theMessage->FatalError("poultryhouse::GetBaconersCapacity() is not implemented correct");
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
          */
/**
 * Set livestock pointer, so that the animal house can find out about the animals it has
 */

/*void poultryhouse::SetLivestock(swineStock* aLivestock)
{
   Livestock=aLivestock;
}
  */
/**
 * Update the stable
 */
void poultryhouse::DailyUpdate()
{
   stable::DailyUpdate();
   //double hours = GetLabour();
   #ifdef STALDDEBUG
// 	cout << " poultryhouse::DailyUpdate: " << Nbudget << endl;
	#endif

   //electricity is commented out until I understand how it is calculated
/*	theOutput->AddIndicator(economicIndicator,"07.02 Labour, stable","hours",hours);
	theOutput->AddIndicator(economicIndicator,"07.03 Labour, maintance and management","hours",0.20*hours);  // 5% for building maintance + 15% for management
	theOutput->AddIndicator(economicIndicator,"07.04 Total labour","hours",hours*1.20);
                                                                                       */
}

void poultryhouse::ReadParameters(fstream * file)
{
   Setfile(file);
   Setcur_pos(0);
   FindSection("poultryhouse",Index);  // find the correct place in the input file
   stable::ReadParameters(file);
   string namestr;
   char Indexstr[10];
   sprintf(Indexstr,"(%d)",Index);
//   FindSection("poultryhouse",Index);  // find the correct place in the input file
   GetParameter("batchInterval",&batchInterval);

   // get info on sections
   Setcur_pos(0);
	namestr=(string) "poultryhouse"+Indexstr+"."+"Section";
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

void poultryhouse::PutStockInSection()
{
   if (Livestock==NULL)
   	theMessage->Warning("poultryhouse::stable present but no livestock");
}

/*bool poultryhouse::CapacityOverflow()
{
   return 0; //((GetSowsCapacity()>Livestock->GetNumberOfSwines()) || (GetBaconersCapacity()>Livestock->GetNumberOfbaconers()));
}
*/
void poultryhouse::GiveIndicator(int indicatorType)
{
	stable::GiveIndicator(indicatorType);
}

/**************************************
Minimum ventilation in cubic metres per sec
**************************************/
double poultryhouse::CalcMinimumVentilationRate()
{
	return GetNrOfAnimals() * 6.0/3600.0;
};

/**************************************
Maximum ventilation in cubic metres per sec
**************************************/
double poultryhouse::CalcMaximumVentilationRate()
{
	return GetNrOfAnimals() * 125.0/3600.0;
};

