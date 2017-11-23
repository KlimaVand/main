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
#include <common.h>
#include <indicat.h>
#include <catstabl.h>
#include <dairystk.h>
#include <contrparm.h>

catstable::catstable(char* aname,const int aIndex,const base* aOwner,
             animalStock* alivestock)
   	:stable(aname,aIndex,aOwner,alivestock)
{
	insideTemperature=theClimate->GetMeanTemperature();
}

catstable::~catstable()
{
}


/*double catstable::GetLabour()
{
	double hours = 0.0;
	for(int i=0;i<sections->NumOfNodes();i++)
   {
    pigstysection *aSection=(pigstysection *) sections->ElementAtNumber(i);
    hours+=aSection->GetLabour();
   }
   return hours;
}
  */
string catstable::GetAnimalType()
{
   return "CATTLE";
}

/****************************************************************************\
Set livestock pointer, so that the animal house can find out about the animals it has
\****************************************************************************/

void catstable::SetLivestock(dairyStock* aLivestock)
{
   Livestock=aLivestock;
}


void catstable::DailyUpdate()
{
   stable::DailyUpdate();
}

void catstable::ReadParameters(fstream * file)
{
   Setfile(file);
   Setcur_pos(0);
   FindSection("Cattlestable",Index);  // find the correct place in the input file
   stable::ReadParameters(file);
   string namestr;
   char Indexstr[10];
   Setcur_pos(0);
   sprintf(Indexstr,"(%d)",Index);

   // get info on sections
   Setcur_pos(0);
	namestr=(string) "Cattlestable"+Indexstr+"."+"Section";
   int first,num;
   GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of sections associated with this instance
  // double cumulativePartition=0.0;
	for(int inx=first;inx<(first+num);inx++)
   {
    animalSection *aSection=new animalSection((char *)namestr.c_str(),inx,this);
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

void catstable::PutStockInSection()
{
   if (Livestock==NULL)
   	theMessage->Warning("catstable::stable present but no livestock");
}

void catstable::CalcLP()
{
   stable::CalcLP();

}

bool catstable::CapacityOverflow()
{
//   return ((GetSowsCapacity()>Livestock->GetNumberOfSwines()) || (GetBaconersCapacity()>Livestock->GetNumberOfSlaughterSwines()));
// Do we need this? return false just in case
 return false;
}

void catstable::GiveIndicator(int indicatorType)
{
	stable::GiveIndicator(indicatorType);
}

