/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Created 3/11/99 by NJH
Describes a section of an animal house. Can be any sort of animal
\****************************************************************************/
#include "../base/common.h"
#include "animalSection.h"
#include "../products/plantItem.h"
#ifdef _STANDALONE
#include "../base/message.h"
#else
#include "../base/IndicatorOutput.h"
#endif
#include "../base/controlParameters.h"

animalSection::animalSection(char* aName,const int aIndex,const base* aOwner)
:base(aName,aIndex,aOwner),GenAsset()
{
	NrOfAnimals  		= 0.0;
	Places            = 0.0;
	Alias					= "";
	VariableFeeding	= 0.0;
	VariableCleaning	= 0.0;
	ConstantFeeding	= 0.0;
	ConstantCleaning	= 0.0;
	animalHeatOp      = 0.0;
	faecesPerYear		= 0.0;
	urinePerYear		= 0.0;
	flooring=new linkList<floorstore>;
}
animalSection::~animalSection()
{
	delete flooring;
}

/**
 * read the input file for the animals. Does also initialize the floor store.
 * If floor stores is to great or small Fasset will with this error:
 * Stable:: cumulative excreta partitioning to flooring not equal to 1.0
 */
void animalSection::ReadParameters(fstream *file)
{
	Setfile(file);
	string namestr;
	char Indexstr[10];
	sprintf(Indexstr,"(%d)",Index);
	namestr=(string) GetName()+Indexstr;
	FindSection(namestr);
	GetParameter("Alias",&Alias);
	GetParameter("Contains",&Contains);
	Places = RefCapacity;
	GetParameter("SpiltDrinkingWater",&SpiltDrinkingWater);
	GetParameter("ElectricityUse",&ElectricityUse);
	GetParameter("VariableFeeding",&VariableFeeding);
	GetParameter("VariableCleaning",&VariableCleaning);
	GetParameter("ConstantFeeding",&ConstantFeeding);
	GetParameter("ConstantCleaning",&ConstantCleaning);

	// get info on flooring
	namestr=(string) GetName()+Indexstr+".floor";
	int first,num;
	GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of floors associated with this section
	double cumulativePartition=0.0;
	for(int inx=first;inx<(first+num);inx++)
	{
		floorstore *floorInst=new floorstore((char*) namestr.c_str(),inx,this);
		floorInst->ReadParameters(file);
		cumulativePartition+=floorInst->GetexcretaPartition();
		flooring->InsertLast(floorInst);
	}
	// ensure cumulative excreta partitioned to flooring = 1
	if (cumulativePartition!=1.0)
		theMessage->FatalError("Stable:: cumulative excreta partitioning to flooring not equal to 1.0");
	Setfile(NULL);
}

/****************************************************************************\
Volatiles N from a section  - complex model
\****************************************************************************/

void animalSection::Volatize(double airVelocity, double humidity, double temperature,
		double *volatAmmonia, double *waterEvap, double *N2Emission, double *N2OEmission, double *CH4Emission,  double *CO2Emission)
{
	double tempvolatAmmonia=0.0, tempwaterEvap=0.0, tempN2Emission=0.0, tempN2OEmission=0.0, tempCH4Emission=0.0, tempCO2Emission=0.0;
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore* aFloor=flooring->ElementAtNumber(i);
		aFloor-> Volatize(airVelocity, humidity, temperature, &tempvolatAmmonia,
				&tempwaterEvap, &tempN2Emission, &tempN2OEmission, &tempCH4Emission, &tempCO2Emission);
		*volatAmmonia+=tempvolatAmmonia;
		*waterEvap+= tempwaterEvap;
		*N2Emission+=tempN2Emission;
		*N2OEmission+=tempN2OEmission;
		*CH4Emission+=tempCH4Emission;
		*CO2Emission+=tempCO2Emission;
	}
	Nbudget.AddOutput(*volatAmmonia + *N2Emission + *N2OEmission);
	Cbudget.AddOutput(*CH4Emission + *CO2Emission);
};

/****************************************************************************\
Volatilise N from a section  - static model
\****************************************************************************/

void animalSection::Volatize(double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CH4Emission,  double *CO2Emission)
{
	double tempvolatAmmonia=0.0, tempN2Emission=0.0, tempN2OEmission=0.0, tempCH4Emission=0.0, tempCO2Emission=0.0;
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore* aFloor=flooring->ElementAtNumber(i);
		aFloor-> Volatize(&tempvolatAmmonia,&tempN2Emission, &tempN2OEmission, &tempCH4Emission, &tempCO2Emission);
		*volatAmmonia+=tempvolatAmmonia;
		*N2Emission+=tempN2Emission;
		*N2OEmission+=tempN2OEmission;
		*CH4Emission+=tempCH4Emission;
		*CO2Emission+=tempCO2Emission;
	}
	Nbudget.AddOutput(*volatAmmonia + *N2Emission + *N2OEmission);
	Cbudget.AddOutput(*CH4Emission + *CO2Emission);
};
/**
 * Cleans the section. Manure is sent to appropriate store by floorstore class. More than one floor possible
 * Can force cleaning of all floors (whether scheduled or not) by setting forceIt to "force cleaning"
 */

manure *animalSection::CleanSection(string forceIt)
{
	//bool ret_val=false;
	manure * tmpManure = NULL;
	manure *aManure = new manure();
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore* aFloor=flooring->ElementAtNumber(i);
		if ((forceIt=="force cleaning")||(aFloor->GettimeUncleaned()+1>=aFloor->GetcleaningInterval()))
		{
			if (aFloor->GetManureAmount()>0.0)
			{
				tmpManure=aFloor-> CleanFloor(NrOfAnimals);
				if (tmpManure)
				{
					Nbudget.AddOutput(tmpManure->GetAmount()*tmpManure->GetAllN().n);
					Cbudget.AddOutput(tmpManure->GetAmount()*tmpManure->GetC_content());
					*aManure + *tmpManure;
					delete tmpManure;
				}
			}
		}
		else aFloor->SettimeUncleaned(aFloor->GettimeUncleaned()+1);
		if (aManure->GetAmount()==0.0)
		{
			delete aManure;
			aManure = NULL;
		}
	}
	return aManure;
};

/**
 * Receive manure from animals in the housing - is called by the animals  - NJH Nov 99
 * Modified June 2001 - simple volatilization occurs when manure is added - does not enter building at all
 */
void animalSection::RcvManure(manure* aUrine,manure* aFaeces)
{
	manure* someFluid=new manure();
	manure* someSolid=new manure();
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		*someFluid= *aUrine;
		floorstore* aFloor=flooring->ElementAtNumber(i);
		someFluid->Setamount(aUrine->GetAmount() * aFloor-> GetexcretaPartition());
		Nbudget.AddInput(someFluid->GetAmount()*someFluid->GetAllN().n);
		Cbudget.AddInput(someFluid->GetAmount()*someFluid->GetC_content());
		/*  if (theControlParameters->GetvolatModel()==0)
  {
		Nbudget.AddOutput(aFloor->Volatize(someFluid)); //static emission - remove ammonia before adding to stores
  } */
		aFloor-> RcvManure(someFluid);
		if (aFaeces)
		{
			*someSolid= *aFaeces;
			someSolid->Setamount(aFaeces->GetAmount() * aFloor-> GetexcretaPartition());
			aFloor->RcvManure(someSolid);
			Nbudget.AddInput(someSolid->GetAmount()*someSolid->GetAllN().n);
			Cbudget.AddInput(someSolid->GetAmount()*someSolid->GetC_content());
		}
	}
	delete someFluid;
	delete someSolid;
}

/**
 * Link up flooring to the manure storage named in the input file  - NJH Nov 99
 */
void animalSection::LinkManureStores(manurestore * aManureStore)
{
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore * floorInst=flooring->ElementAtNumber(i);
		for(int j=0;j<floorInst->GetStoreNumber();j++)
		{
			manurestore * aStore=floorInst->GetaStore(j);
			string namestr;
			char Indexstr[10];
			int IndexNum=((base*)Owner)->GetIndex();
			sprintf(Indexstr,"(%d)",IndexNum);
			namestr=(string) Owner->GetName()+Indexstr+"."+aStore->GetManureToName();
			if ((aManureStore->GetName()==namestr)&& (aManureStore->GetIndex() == aStore->GetManureToIndex()))
			{aStore->SetnextStore(aManureStore);}
		}
	}
}

/**
 * Check that floors link to stores etc - NJH Nov 99
 */

void animalSection::CheckLinks()
{
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore * floorInst=flooring->ElementAtNumber(i);
		for(int j=0;j<floorInst->GetStoreNumber();j++)
		{
			manurestore * aStore=floorInst->GetaStore(j);
			if ((aStore->GetnextStore()==NULL) && (aStore->GetManureToName()!="NONE"))
			{
				string outString1 =  "animalSection:: ";
				string outString2 =  " is not linked to a manure store";
				string outString =  outString1  + floorInst->GetName() + outString2;
				theMessage->FatalError(outString.c_str());
				//   			theMessage->FatalError("animalSection:: animal section floor is not linked to a manure store");
			}
		}
	}
}

/**
 * Check N budget
 */

void animalSection::EndBudget(bool show)
{
	if (show)
		cout << "Balance for animal section " << GetAlias() << endl;
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore * floorInst=flooring->ElementAtNumber(i);
		floorInst ->EndBudget(show);
	}
}

/**
 * Get total manure N
 */
double animalSection::GetStoredN()
{
	double tmp=0.0;
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore * floorInst=flooring->ElementAtNumber(i);
		tmp+=floorInst ->GetTotalNStored();
	}
	return tmp;
};

/**
 * Get total manure C
 */
double animalSection::GetStoredC()
{
	double tmp=0.0;
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore * floorInst=flooring->ElementAtNumber(i);
		tmp+=floorInst ->GetTotalCStored();
	}
	return tmp;
};

/**
 * Get total manure DM
 */
double animalSection::GetStoredDM()
{
	double tmp=0.0;
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore * floorInst=flooring->ElementAtNumber(i);
		tmp+=floorInst ->GetTotalDMStored();
	}
	return tmp;
};


/**
 * Get manure available
 */

double animalSection::GetManureAmount()
{
	double ManureAmount=0;
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore * floorInst=flooring->ElementAtNumber(i);
		ManureAmount+=floorInst ->GetManureAmount();
	}
	return ManureAmount;
}

/**
 * Get manure from store
 * Returns a new manure object
 */

product * animalSection::GetFromManureStore(double RequestedAmount, product * p, bool checkAccess)
{
	product * aManure = new manure();
	product * manureFound = NULL;
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore * floorInst=flooring->ElementAtNumber(i);
		manureFound=floorInst ->GetFromManureStore(RequestedAmount, p,checkAccess);
		if (manureFound)
			*aManure+*manureFound;
	}
	if (aManure->GetAmount()==0.0)
	{
		delete aManure;
		aManure=NULL;
	}
	return aManure;
}

/**
 * Calculate manure that is washed away each day.
 * Calculate how much WHEATSTRAW that is used for bedding. That straw is removed from theProducts. Nbudget and Cbudget is updated with the n and  content from the strew
 * The C and N conent is added to the indicator file
 */

void animalSection::DailyUpdate()
{
	if (NrOfAnimals>0.0)
	{
		manure * aManure=new manure();
		aManure->Setname("WASHWATER");
		theProducts->GiveProductInformation(aManure);
		aManure->Setamount(NrOfAnimals * SpiltDrinkingWater/1000.0);
		RcvManure(aManure,NULL);  //partitions spilt drinking water in the same way as manure input
		delete aManure;

		plantItem * someStraw = new plantItem();
		someStraw->Setname("WHEATSTRAW");
		theProducts->GiveProductInformation(someStraw);    // sets the default values for wheat straw
		for(int i=0;i<flooring->NumOfNodes();i++)
		{
			floorstore * floorInst=flooring->ElementAtNumber(i);
			someStraw->Setamount(NrOfAnimals * floorInst->GetStrawAdded() * floorInst-> GetexcretaPartition()/1000.0); //add bedding material
			floorInst->RcvBedding(someStraw);
			Nbudget.AddInput(someStraw->GetAmount()*someStraw->GetAllN().n);
			Cbudget.AddInput(someStraw->GetAmount()*someStraw->GetC_content());
#ifndef _STANDALONE
			theOutput->AddIndicator(environmentalIndicator,"35.63 N in bedding","kg N",someStraw->GetAmount()*someStraw->GetAllN().n*1000.0);
			theOutput->AddIndicator(environmentalIndicator,"45.63 C in bedding","kg C",someStraw->GetAmount()*someStraw->GetC_content()*1000.0);
#endif
			theProducts->SubtractProduct(someStraw);
		}
		delete someStraw;
		//       EndBudget(false);
	}
}


/**
 * Scale animal section according to the number of animals present
 */

void animalSection::ScaleSize()
{
	double annualManureToFloor = 0.0;
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore * floorInst=flooring->ElementAtNumber(i);
		annualManureToFloor=floorInst->GetexcretaPartition() * (faecesPerYear + urinePerYear);
		annualManureToFloor+=floorInst->GetexcretaPartition() *NrOfAnimals*365.0*(floorInst->GetWashingWater()+SpiltDrinkingWater)/1000.0;  //in cubic metres
		floorInst->ScaleSize(NrOfAnimals,annualManureToFloor);
	}
	SetActualCapacity(NrOfAnimals);
}

/**
 * Ask floors how much space is allocated to each animal
 */
double animalSection::GetAreaPrAnimal()
{
	double area=0.0;
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore * floorInst=flooring->ElementAtNumber(i);
		area += floorInst->GetAreaPrAnimal();
	}
	return area;
};

/**
 * Get total area of section
 */
double animalSection::GetTotalArea()
{
	double area=0.0;
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore * floorInst=flooring->ElementAtNumber(i);
		area += floorInst->GetArea();
	}
	return area;
};


/**
 * Set the animal parameters for the section. Called by grazing manager or swinestock
 */
void animalSection::SetAnimalParameters(double numberOfAnimals,double faecesPerYear, double urinePerYear, double heatOp)
{
	setNrOfAnimals(numberOfAnimals);
	SetfaecesPerYear(faecesPerYear);
	SeturinePerYear(urinePerYear);
	animalHeatOp=heatOp;
}

void animalSection::ReadDefaultParameters(fstream *file)
{
	Setfile(file);
	string namestr;
	char Indexstr[10];
	sprintf(Indexstr,"(%d)",Index);
	namestr=(string) GetName()+Indexstr;
	FindSection(namestr);
	SetCritical();
	GetParameter("Alias",&Alias);
	GetParameter("Contains",&Contains);
	Places = RefCapacity;
	GetParameter("SpiltDrinkingWater",&SpiltDrinkingWater);
	GetParameter("ElectricityUse",&ElectricityUse);

	// get info on flooring
	namestr=(string) GetName()+Indexstr+".floor";
	int first,num;
	GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of floors associated with this section
	double cumulativePartition=0.0;
	for(int inx=first;inx<(first+num);inx++)
	{
		floorstore *floorInst=new floorstore((char*) namestr.c_str(),inx,this);
		floorInst->ReadDefaultParameters(file);
		cumulativePartition+=floorInst->GetexcretaPartition();
		flooring->InsertLast(floorInst);
	}
	// ensure cumulative excreta partitioned to flooring = 1
	if (cumulativePartition!=1.0)
		theMessage->FatalError("Stable:: cumulative excreta partitioning to flooring not equal to 1.0");
	Setfile(NULL);
}
