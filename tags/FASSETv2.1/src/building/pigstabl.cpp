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
#include <common.h>
#include <indicat.h>
#include <pigstabl.h>
#include <swinestk.h>
#include <contrparm.h>

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

string pigstable::GetAnimalType()
{
   return "PIG";
}

/****************************************************************************\
Calculate the total time required for a sow production cycle
\****************************************************************************/

double pigstable::GetSowsProductionCyclus()
{
	double productionCyclus = 0.0;
	for(int i=0;i<sections->NumOfNodes();i++)
   {
    pigstysection *aSection=(pigstysection *) sections->ElementAtNumber(i);
    if ((aSection->GetName()=="Farrowing")||(aSection->GetName()=="Mating")||(aSection->GetName()=="Pregnant"))
    productionCyclus+=aSection->GetDaysInSection();
   }
   return productionCyclus;
}

/****************************************************************************\
Calculate the total time required for a finished pig production cycle
\****************************************************************************/

double pigstable::GetBaconersProductionCyclus()
{
   double productionCyclus = 0.0;
	for(int i=0;i<sections->NumOfNodes();i++)
   {
    pigstysection *aSection=(pigstysection *) sections->ElementAtNumber(i);
    if ((aSection->GetName()=="Weaning")||(aSection->GetName()=="Finishing"))
    productionCyclus+=aSection->GetDaysInSection();
   }
   return productionCyclus;
}

/****************************************************************************\
Calculate the capacity for sow production
\****************************************************************************/

double pigstable::GetSowsCapacity()
{
   double capacity = 1e10;
   double productionCyclus = 0.0;
	for(int i=0;i<sections->NumOfNodes();i++)
   {
    pigstysection *aSection=(pigstysection *) sections->ElementAtNumber(i);
    if ((aSection->GetName()=="Farrowing")||(aSection->GetName()=="Mating")||(aSection->GetName()=="Pregnant"))
    productionCyclus+=aSection->GetDaysInSection();
    capacity = min(capacity,aSection->GetCapacity(GetSowsProductionCyclus()));
   }
	return capacity;
}

/****************************************************************************\
Calculate the capacity for finished pig production
\****************************************************************************/

double pigstable::GetBaconersCapacity()
{
   double capacity = 1e10;
   double productionCyclus = 0.0;
	for(int i=0;i<sections->NumOfNodes();i++)
   {
    pigstysection *aSection=(pigstysection *) sections->ElementAtNumber(i);
    if ((aSection->GetName()=="Weaning")||(aSection->GetName()=="Finishing"))
    productionCyclus+=aSection->GetDaysInSection();
    capacity = min(capacity,aSection->GetCapacity(productionCyclus));
   }
	return capacity;//*theTime.daysInYear()/Finishing->GetDaysInSection();
}

/****************************************************************************\
Set livestock pointer, so that the animal house can find out about the animals it has
\****************************************************************************/

void pigstable::SetLivestock(swineStock* aLivestock)
{
   Livestock=aLivestock;
}


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
	return GetGetNrOfAnimals() * 6.0/3600.0;
};

/**************************************
Maximum ventilation in cubic metres per sec
**************************************/
double pigstable::CalcMaximumVentilationRate()
{
	return GetGetNrOfAnimals() * 125.0/3600.0;
};

