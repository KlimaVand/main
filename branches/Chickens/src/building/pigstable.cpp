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
	ReadDefaultParameters();
}

pigstable::~pigstable()
{
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

void pigstable::ReadDefaultParameters()
{
	SetCritical();
	if (OpenInputFile("StandardBuildings.dat"))
	{
		FindSection("PigHousing");
		GetParameter("meanHeight",&meanHeight);
		GetParameter("planArea",&planArea);
		GetParameter("thermalTransWall",&thermalTransWall);
		GetParameter("thermalTransRoof",&thermalTransRoof);
		GetParameter("minVentilation",&minVentilation);
		GetParameter("maxVentilation",&maxVentilation);
		if ((minVentilation>maxVentilation)||(maxVentilation<=0.0))
			theMessage->FatalError("Stable:: minimum ventilation rate > maximum or maximum <= 0");
		GetParameter("targetTemperature",&targetTemperature);
		GetParameter("absorbCoeff",&absorbCoeff);
		GetParameter("emissivity",&emissivity);
		GetParameter("externSurfResis",&externSurfResis);
		UnsetCritical();
		// get info on sections
		Setcur_pos(0);
		string namestr;
		namestr=(string) "PigHousing"+".Section";
		int first,num;
		GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of sections associated with this instance
		//  double cumulativePartition=0.0;
		for(int inx=first;inx<(first+num);inx++)
		{
			pigstysection *aSection=new pigstysection((char *)namestr.c_str(),inx,this);
			aSection->SetScalable(true);
			aSection->ReadDefaultParameters(file);
			sections->InsertLast(aSection);
		}
		CloseInputFile();
	}

}
