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
cattlehouse::cattlehouse(char* aname,const int aIndex,const base* aOwner,
             animalStock* alivestock)
   	:stable(aname,aIndex,aOwner,alivestock)
{
	insideTemperature=theClimate->GetMeanTemperature();
}

cattlehouse::~cattlehouse()
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
/**
 * reading input files
 */
void cattlehouse::ReadParameters(fstream * file)
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
/**
 * Checks is some animal is present. If not fasset will give this warning: "catstable::stable present but no livestock"
 */
void cattlehouse::PutStockInSection()
{
   if (Livestock==NULL)
   	theMessage->Warning("catstable::stable present but no livestock");
}
/**
 * CalcLP  indicators for LP-model (not implemented yet)
 */
void cattlehouse::CalcLP()
{
   stable::CalcLP();

}


void cattlehouse::GiveIndicator(int indicatorType)
{
	stable::GiveIndicator(indicatorType);
}

