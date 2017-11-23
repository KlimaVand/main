#include "../base/common.h"
#include "animalSection.h"
#include "../products/plantItem.h"
#ifdef _STANDALONE
#include "../base/message.h"
#else
#include "../base/IndicatorOutput.h"
#endif
#include "../base/controlParameters.h"
#include "../base/commonData.h"
#include <string.h>
animalSection::animalSection(const char* aName,const int aIndex,const base* aOwner,commonData * &runData,commonData * &original, bool controlledVent)
:base(aName,aIndex,aOwner)
{
	animalHeatOp=0.0;
    NrOfAnimals=0.0;;
   AreaPrAnimal=0.0;
   Places=0.0;
   SpiltDrinkingWater=0.0;
   ElectricityUse=0.0;
   faecesPerYear=0.0;
   urinePerYear=0.0;
	string sectionName(aName);
	convert conv;
	NrOfAnimals=0;
	runData->setCritical(true);
	runData->FindSection(sectionName,Index);
	runData->FindItem("Alias",&Alias);
	runData->setCritical(false);
	original->setCritical(true);

	int index;
	string place;
	original->findSection(Name,0,"Alias",Alias,&place,&index);
	original->FindSection(place,index);

	original->FindItem("Contains",&Contains);
	runData->FindItem("Contains",&Contains);

	original->FindItem("Places",&Places);
	runData->FindItem("Places",&Places);

	original->FindItem("SpiltDrinkingWater",&SpiltDrinkingWater);
	runData->FindItem("SpiltDrinkingWater",&SpiltDrinkingWater);

	original->FindItem("AreaPrAnimal",&AreaPrAnimal);
	runData->FindItem("AreaPrAnimal",&AreaPrAnimal);
	if(controlledVent==true)
	{
	original->FindItem("minVentilation",&minVentilation);
	runData->FindItem("minVentilation",&minVentilation);
	original->FindItem("maxVentilation",&maxVentilation);
	runData->FindItem("maxVentilation",&maxVentilation);
	if ((minVentilation>maxVentilation)||(maxVentilation<=0.0))
		theMessage->FatalError("Stable:: minimum ventilation rate > maximum or maximum <= 0");
	}
	
	original->FindItem("ElectricityUse",&ElectricityUse);
	runData->FindItem("ElectricityUse",&ElectricityUse);

	flooring=new linkList<floorstore>;
	string floorName=sectionName+"("+conv.IntToString(Index)+").floor";

	 int large=0;
	 int small=0;

	 runData->getSectionsNumbers(floorName,&small,&large);
	
	 //calculate area of section - this may be zero, if scaling is being used
	 double Area = Places * AreaPrAnimal;
	double totalAreaPartition=0.0;

	for(int inx=small;inx<=large;inx++)
	{
		floorstore *floorInst=new floorstore((char*) floorName.c_str(),inx,this,runData,original,(large-small)+1);
		//set area of each floor
		floorInst->SetArea(Area * floorInst->GetAreaPartition());
		totalAreaPartition+=floorInst->GetAreaPartition();
		flooring->InsertLast(floorInst);
	}
	if(totalAreaPartition!=1.0)
	{
		theMessage->FatalError("animalSection: wrong size of AreaPartition in floors");
	}
}
animalSection::~animalSection()
{

	delete flooring;
}
animalSection::animalSection(const animalSection& aAnimalSection)
:base(aAnimalSection)
{
	theMessage->FatalError("animalSection::animalSection has not been used for a while");
	NrOfAnimals  		= aAnimalSection.NrOfAnimals;
	Places            = aAnimalSection.Places;
	Alias					= aAnimalSection.Alias;
	animalHeatOp      = aAnimalSection.animalHeatOp;
	faecesPerYear		= aAnimalSection.faecesPerYear;
	urinePerYear		= aAnimalSection.urinePerYear;
	Contains = aAnimalSection.Contains;
	flooring=new linkList<floorstore>;
	 for (int i=0;i<aAnimalSection.flooring->NumOfNodes();i++)
	   {
		 floorstore *p = new floorstore(*(aAnimalSection.flooring->ElementAtNumber(i)));
		   flooring->InsertLast(p);
	   }
}


/****************************************************************************\
Volatiles N from a section  - complex model
\****************************************************************************/

void animalSection::Volatize(double airVelocity, double humidity, double temperature,
		double *volatAmmonia, double *waterEvap, double *N2Emission, double *N2OEmission, double *CH4Emission,  double *CO2Emission)
{
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		double tempvolatAmmonia=0.0, tempwaterEvap=0.0, tempN2Emission=0.0, tempN2OEmission=0.0, tempCH4Emission=0.0, tempCO2Emission=0.0;
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
	manure * tmpManure = nullptr;
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


	}
#ifdef STRICT_BALANCES
	EndBudget(false);
#endif
	if (aManure->GetAmount()==0.0)
			{
				delete aManure;
				aManure = nullptr;
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
		if(aFaeces)
		{
			*someSolid= *aFaeces;
			someSolid->Setamount(aFaeces->GetAmount() * aFloor-> GetexcretaPartition());
			Nbudget.AddInput(someSolid->GetAmount()*someSolid->GetAllN().n);
			Cbudget.AddInput(someSolid->GetAmount()*someSolid->GetC_content());
			aFloor-> RcvManure(someFluid,someSolid);
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
			floorInst->LinkManureStores(aManureStore);
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
			manurestore * aStore=floorInst->GetaNextStore(j);
			if ((!aStore->GetnextStore())&&(floorInst->GetmanureToName()!="nullptr"))
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
 * Check N budget kun til debugning
 */
void animalSection::EndBudget(bool show)
{
	if (show)
		cout << "Balance for animal section " << GetName() << endl;
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
	manure * aManure = new manure();
	product * manureFound = nullptr;
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
		aManure=nullptr;
	}
	return aManure;
}

product * animalSection::TakeAllManureFromSection(bool checkAccess)
{
	manure * aManure = new manure();
	product * manureFound = nullptr;
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore * floorInst=flooring->ElementAtNumber(i);
		manureFound=floorInst ->TakeAllManureFromFloor(checkAccess);
			if (manureFound)
			*aManure+*manureFound;
	}
	if (aManure->GetAmount()==0.0)
	{
		delete aManure;
		aManure=nullptr;
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
		RcvManure(aManure,nullptr);  //partitions spilt drinking water in the same way as manure input
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
			theOutput->AddIndicator(3563,"35.63 N in bedding","kg N",someStraw->GetAmount()*someStraw->GetAllN().n*1000.0);
			theOutput->AddIndicator(4563,"45.63 C in bedding","kg C",someStraw->GetAmount()*someStraw->GetC_content()*1000.0);
#endif
			theProducts->SubtractProduct(someStraw);
			floorInst->DailyUpdate();
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
	//set area based on number of animals * area per animal
	double Area = NrOfAnimals * AreaPrAnimal;
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore * floorInst=flooring->ElementAtNumber(i);
		floorInst->SetArea(Area*floorInst->GetAreaPartition());
		if (NrOfAnimals>0.0)
			annualManureToFloor=floorInst->GetexcretaPartition() * (faecesPerYear + urinePerYear);
		annualManureToFloor+=floorInst->GetexcretaPartition() *NrOfAnimals*365.0*(floorInst->GetWashingWater()+SpiltDrinkingWater)/1000.0;  //in cubic metres
		floorInst->ScaleSize(annualManureToFloor);
	}


}

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




void animalSection::CheckFloorIntegrity()
{
	// nic checker hvorfor denne ikke bliver kaldt
	double cumulativePartition=0;

	// ensure cumulative excreta partitioned to flooring = 1
	for(int i=0;i<flooring->NumOfNodes();i++)
	{
		floorstore * floorInst=flooring->ElementAtNumber(i);
		cumulativePartition+=floorInst->GetexcretaPartition();
	}
	if (cumulativePartition!=1.0)
		theMessage->FatalError("Stable:: cumulative excreta partitioning to flooring not equal to 1.0");
}
