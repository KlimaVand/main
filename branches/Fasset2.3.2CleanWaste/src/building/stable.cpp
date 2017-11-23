#include "../base/common.h"
#include "stable.h"
#include "floorstore.h"
#include "instore.h"
#include "../base/message.h"
#include "../products/products.h"
#include "../base/climate.h"
#include "../base/bstime.h"
#include "../base/nixcbits.h"
#include "../base/controlParameters.h"
#include "../base/settings.h"
#include "../tools/fileAccess.h"
#include "../tools/convert.h"


#ifndef _STANDALONE
#include "../base/IndicatorOutput.h"
#include "../livestock/animalStock.h"
#endif
/****************************************************************************\
  Constructor with arguments
 */
stable::stable(const char* aname,const int aIndex,const base* aOwner,commonData *  &runData,commonData * &orignal)
:building(aname,aIndex,aOwner, runData, orignal)
{

	Livestock=new animalStock();
	slurrySystem = 1;
	sections=new linkList<animalSection>;
	standartSections=new linkList<animalSection>;
	inhouseStores=new linkList<instore>;
	Emissionfactor=0;
	Alias="";
	wallArea = 0.0; //area of one wall, assume housing is cubic
	minVentilation=0.0; //min ventilation rate in cubic metres per sec
	maxVentilation=0.0; //max ventilation rate in cubic metres per sec
	planArea=0.0; // plan area of house
	insideTemperature=0.0;
	ventilationRate=0.0;			// ventilation rate (cubic metres per second)
	apertureWidth=0.0;
	maxapertureHeight=0.0;
	minPropApertureHeight=0.0;
	inUse = false;
	minTemperature=0.0;
	maxTemperature=0.0;

	runData->setCritical(true);
	runData->FindSection(Name,Index);
	runData->FindItem("Alias",&Alias);
	runData->setCritical(false);
	orignal->setCritical(true);
	string sectionNameOrg(aname);
	int indexInOld;
		string place;
		orignal->findSection(Name,0,"Alias",Alias,&place,&indexInOld);
		orignal->FindSection(place,indexInOld);
	switch (theControlParameters->GetvolatModel())
	{
		case 0:
			break;
		case 1:
		{
			orignal->FindItem("meanHeight",&meanHeight);
			runData->FindItem("meanHeight",&meanHeight);

			orignal->FindItem("planArea",&planArea);
			runData->FindItem("planArea",&planArea);

			orignal->FindItem("controlledVent",&controlledVent);
			runData->FindItem("controlledVent",&controlledVent);

			orignal->FindItem("thermalTransWall",&thermalTransWall);
			runData->FindItem("thermalTransWall",&thermalTransWall);

			orignal->FindItem("thermalTransRoof",&thermalTransRoof);
			runData->FindItem("thermalTransRoof",&thermalTransRoof);
;
			if (controlledVent>0)
			{
				orignal->FindItem("minVentilation",&minVentilation);
				runData->FindItem("minVentilation",&minVentilation);


				orignal->FindItem("maxVentilation",&maxVentilation);

				if ((minVentilation>maxVentilation)||(maxVentilation<=0.0))
					theMessage->FatalError("Stable:: minimum ventilation rate > maximum or maximum <= 0");
				orignal->FindItem("targetTemperature",&targetTemperature);
				runData->FindItem("targetTemperature",&targetTemperature);

				targetTemperature+=273.13;
			}
			else
			{
				orignal->FindItem("maxapertureHeight",&maxapertureHeight);
				runData->FindItem("maxapertureHeight",&maxapertureHeight);

				orignal->FindItem("minPropApertureHeight",&minPropApertureHeight);
				runData->FindItem("minPropApertureHeight",&minPropApertureHeight);

				orignal->FindItem("apertureWidth",&apertureWidth);
				runData->FindItem("apertureWidth",&apertureWidth);

				orignal->FindItem("optimumAirVelocity",&optimumAirVelocity);
				runData->FindItem("optimumAirVelocity",&optimumAirVelocity);

				orignal->FindItem("minTemperature",&minTemperature);
				runData->FindItem("minTemperature",&minTemperature);

				orignal->FindItem("maxTemperature",&maxTemperature);
				runData->FindItem("maxTemperature",&maxTemperature);

				orignal->FindItem("absorbCoeff",&absorbCoeff);
				runData->FindItem("absorbCoeff",&absorbCoeff);

				orignal->FindItem("emissivity",&emissivity);
				runData->FindItem("emissivity",&emissivity);

				orignal->FindItem("externSurfResis",&externSurfResis);
				runData->FindItem("externSurfResis",&externSurfResis);

			}
			break;
		}
	}
	string sectionName(Name);
	sectionName+=".instore";
	int large=0;
	int small=0;
	runData->setCritical(false);
	runData->getSectionsNumbers(sectionName,&small,&large);
	runData->setCritical(true);
	for(int inx=small;inx<large;inx++)
	{
		instore *manureInst=new instore((char *)sectionName.c_str(),inx,this,runData,orignal);
		manureInst->SetScalable(true);
		inhouseStores->InsertLast(manureInst);
	}
	calcWallArea();
};

/****************************************************************************\
  Destructor
 */
stable::~stable()
{
	delete sections;
	delete inhouseStores;
	delete standartSections;
	if (globalSettings->DetailsData.getManureManageDetails()==true)
		CloseDetailsFile();
};
stable::stable(const stable& aStable):building(aStable)
{
	Livestock=new animalStock(*aStable.Livestock);
	slurrySystem = aStable.slurrySystem;

	inhouseStores=new linkList<instore>();
		for (int i=0;i<aStable.inhouseStores->NumOfNodes();i++)
		   {
			instore *p = new instore(*(aStable.inhouseStores->ElementAtNumber(i)));
			inhouseStores->InsertLast(p);
		   }
	Emissionfactor=aStable.Emissionfactor;
	Alias=aStable.Alias;
	wallArea = aStable.wallArea; //area of one wall, assume housing is cubic
	minVentilation=aStable.minVentilation; //min ventilation rate in cubic metres per sec
	maxVentilation=aStable.maxVentilation; //max ventilation rate in cubic metres per sec
	planArea=aStable.planArea; // plan area of house
	insideTemperature=aStable.insideTemperature;
	ventilationRate=aStable.ventilationRate;			// ventilation rate (cubic metres per second)
	apertureWidth=aStable.apertureWidth;
	maxapertureHeight=aStable.maxapertureHeight;
	minPropApertureHeight=aStable.minPropApertureHeight;
	inUse = aStable.inUse;
	minTemperature=aStable.minTemperature;
	maxTemperature=aStable.maxTemperature;
}

/**
 *   GetanimalSection() is called by animals to find out where they belong
 */

animalSection * stable::GetanimalSection(const char * name)
{
	animalSection* aSection;
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		aSection=sections->ElementAtNumber(i);
		if (aSection->GetContains()==name)
		{
			return aSection;
		}
	}
	return NULL;
}
/**
 * DailyUpdate() should be called everyday
 * Updates every animalSection in the stable.
 * Update the Ammonia, water, N2, N2O, CH4Emission and CO2Emission in the manure.
 */

void stable::DailyUpdate()
{
	building::DailyUpdate();
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection* aSection=sections->ElementAtNumber(i);
		aSection->DailyUpdate();
	}
	if (theControlParameters->GetvolatModel()==1||theControlParameters->GetvolatModel()==0)
	{
		Volatize();
	}
	else
	{	convert conv;

	theMessage->FatalError("stable::DailyUpdate: volat model ",conv.IntToString(theControlParameters->GetvolatModel()).c_str(), " not supported");
	}

	CleanManure();
#ifndef _STANDALONE
	theOutput->AddStateIndicator(3561,"35.61 N bound in stable manure","kg N",GetTotalN()*1000.0);
	theOutput->AddStateIndicator(4561,"45.61 C bound in stable manure","kg C",GetTotalC()*1000.0);

#endif
	double electricity = GetDailyElectricity();
	//   electricity = electricity/theTime.daysInYear();                  // converts to days
	product* Energy = new product;
	Energy->Setname("ELECTRICITY");
	theProducts->GiveProductInformation(Energy);
	Energy->Setamount(electricity);
	theProducts->SubtractProduct(Energy);
	delete Energy;

#ifndef _STANDALONE
	theOutput->AddIndicator(2002,"20.02 Electricity consumption","KWh",electricity);
#endif
	EndBudget(true);

	//   GiveIndicator();
}

/**
 * Sets the herd of the stable
 */

void stable::SetLivestock(animalStock* aLivestock)
{
	Livestock=aLivestock;
}


/**
 * Volatize() Called everyday by DailyActions()
 * Computes the daily volatization of N and C gasses from the dirty stable floor
 */
void stable::Volatize()
{
	NH3Emission=0.0, waterEvap=0.0, N2Emission=0.0, NOEmission=0.0, N2OEmission=0.0, CH4Emission=0.0, CO2Emission=0.0;
	switch (theControlParameters->GetvolatModel())
	//
	{
	case 0://!simple emission model, using emission factors
	{
		//!Calculating emissions for animal sections
		for(int i=0;i<sections->NumOfNodes();i++)
		{
			double tempvolatAmmonia=0.0, tempN2Emission=0.0, tempN2OEmission=0.0, tempCH4Emission=0.0, tempCO2Emission=0.0;
			animalSection* aSection=sections->ElementAtNumber(i);
			aSection->Volatize(&tempvolatAmmonia, &tempN2Emission, &tempN2OEmission, &tempCH4Emission, &tempCO2Emission);
			NH3Emission+=tempvolatAmmonia;
			N2Emission+=tempN2Emission;
			N2OEmission+=tempN2OEmission;
			CH4Emission+= tempCH4Emission;
			CO2Emission+=tempCO2Emission;
		}
		//!Calculating emissions for in-house storage
		for(int i=0;i<inhouseStores->NumOfNodes();i++)
		{
			double tempvolatAmmonia=0.0, tempN2Emission=0.0, tempN2OEmission=0.0, tempCH4Emission=0.0, tempCO2Emission=0.0;
			instore * aStore=inhouseStores->ElementAtNumber(i);
			aStore->Volatize(&tempvolatAmmonia, &tempN2Emission, &tempN2OEmission, &tempCH4Emission, &tempCO2Emission);
			NH3Emission+=tempvolatAmmonia;
			N2Emission+=tempN2Emission;
			N2OEmission+=tempN2OEmission;
			CH4Emission+= tempCH4Emission;
			CO2Emission+=tempCO2Emission;
		}
		break;
	}
	case 1://!dynamic emission model
	{
		if (inUse)   //could be zero if using scaling and no animals for this housing type
		{
			double Ameantemp,Amin_temp,
			Amax_temp,Aprecip,Aradiation,AEPot,Ahumidity,Awindspeed;
			theClimate->GetClimate(Ameantemp, Amin_temp,Amax_temp, Aprecip, Aradiation,AEPot, Awindspeed, Ahumidity);
			//Ameantemp+=5.0;
			double heatOP = 0.0;
			double airVelocity = 0.0;
			//!get animal heat production
			for(int i=0;i<sections->NumOfNodes();i++)
			{
				animalSection* aSection=sections->ElementAtNumber(i);
				heatOP+=aSection->GetanimalHeatOp();
			}
			//!calculate the amount of sensible heat produced in the animal housing
			double sensibleHeatOp=CalcPropSensible()*heatOP;
			//!calculate the air velocity, using the appropriate functions for controlled or freely ventilated systems
			if (controlledVent>0)
			{
				double supplementaryHeat=0.0;
				controlled(sensibleHeatOp, Ameantemp, &supplementaryHeat);
				airVelocity=ventilationRate/planArea;
			}
			else
			{
				uncontrolled(sensibleHeatOp, Ameantemp, Awindspeed, Aradiation);
				//!factor 0.05 is a nasty fudge. It reduces the wind speed near the floor surface
				airVelocity=0.05*ventilationRate/planArea;
			}

			//    airVelocity = 4.62 * (ventilationRate/planArea) + 0.106;  //from Aarnick and Elzing, 1998
			//!calculate gaseous emissions for animal sections
			for(int i=0;i<sections->NumOfNodes();i++)
			{
				double tempvolatAmmonia=0.0, tempN2Emission=0.0, tempN2OEmission=0.0, tempCH4Emission=0.0, tempCO2Emission=0.0, tempwaterEvap=0.0;
				animalSection* aSection=sections->ElementAtNumber(i);
				aSection->Volatize(airVelocity,Ahumidity,insideTemperature,
						&tempvolatAmmonia, &tempwaterEvap, &tempN2Emission, &tempN2OEmission, &tempCH4Emission, &tempCO2Emission);
				if (!controlledVent)
					aSection->SetanimalHeatOp(0.0);  // zero heat production ready for next day but only for cattle
				NH3Emission+=tempvolatAmmonia;
				waterEvap+= tempwaterEvap;
				N2Emission+=tempN2Emission;
				N2OEmission+=tempN2OEmission;
				CH4Emission+= tempCH4Emission;
				CO2Emission+=tempCO2Emission;
			}
			//!calculate emissions for in-house storage
			for(int i=0;i<inhouseStores->NumOfNodes();i++)
			{
				double tempvolatAmmonia=0.0, tempN2Emission=0.0, tempN2OEmission=0.0, tempCH4Emission=0.0, tempCO2Emission=0.0, tempwaterEvap=0.0;
				instore * aStore=inhouseStores->ElementAtNumber(i);
				aStore->inhouseVolatize(airVelocity,Ahumidity,insideTemperature,1,
						&tempvolatAmmonia, &tempwaterEvap, &tempN2Emission, &tempN2OEmission, &tempCH4Emission, &tempCO2Emission);
				NH3Emission+=tempvolatAmmonia;
				waterEvap+= tempwaterEvap;
				N2Emission+=tempN2Emission;
				N2OEmission+=tempN2OEmission;
				CH4Emission+= tempCH4Emission;
				CO2Emission+=tempCO2Emission;
			}
		}
		//!If detailed output data has been requested, write this to file
		if (globalSettings->DetailsData.getManureManageDetails()==true)
			ShowStable(false);
		break;
	} //end of case
	} // end of switch
#ifndef _STANDALONE
	//!Record gaseous nitrogen and carbon losses
	theOutput->AddIndicator(3540,"35.40 NH3-N volatilisation from animal house","kg N",NH3Emission*1000.0);
	theOutput->AddIndicator(3541,"35.41 N2-N volatilisation from animal house","kg N",N2Emission*1000.0);
	theOutput->AddIndicator(3542,"35.42 N2O-N volatilisation from animal house","kg N",N2OEmission*1000.0);
	//   theOutput->AddIndicator("35.43 NO-N volatilisation from animal house","kg N",NOEmission*1000.0);
	theOutput->AddIndicator(4540,"45.40 CO2-C volatilisation from animal house","kg C",CO2Emission*1000.0);
	theOutput->AddIndicator(4541,"45.41 CH4-C volatilisation from animal house","kg C",CH4Emission*1000.0);
	/*   fstream * filehandle = theMessage->GiveDebugHandle();
	 *filehandle << " hous " << (NH3Emission*1000.0) << " " ;
   theMessage->CloseDebugHandle();*/
	//   cout << "house " <<  NH3Emission*1000.0 << " ";
#endif
	//   double tmp;
	//   EndBudget(&tmp);
}


/**
 * Remove manure from flooring and transfer to relevant manure store
 */
void stable::CleanManure()
{
	// cout<<GetTotalN();
	manure * aManure=new manure();

	//clean animal sections
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection* aSection=sections->ElementAtNumber(i);
		//empty flooring - send parameter "Force cleaning" if you want to clean everything now
		manure * tmpManure=aSection->CleanSection("");  //new manure();
		if (tmpManure)
		{
			*aManure + *tmpManure;
			delete tmpManure;
		}
	}

	// empty inhouse storage
	for(int i=0;i<inhouseStores->NumOfNodes();i++)
	{
		instore * aManureStore=inhouseStores->ElementAtNumber(i);
		//	manure * tmpManure=new manure();
		manure *tmpManure=(manure*) aManureStore->GetManure()->clone();
		tmpManure->Setamount(0.0);
		//empty inhouse store - needs to be updated for periodic emptying
		if (aManureStore->CleanStore(tmpManure))
			*aManure + *tmpManure;
		delete tmpManure;
	}
#ifndef _STANDALONE
	theOutput->AddIndicator(3580,"35.80 N transf. from stable to store","kg N",(aManure->GetAllN().n*aManure->GetAmount())*1000.0);
	theOutput->AddIndicator(4580,"45.80 C transf. from stable to store","kg C",(aManure->GetC_content()*aManure->GetAmount())*1000.0);
#endif
	delete aManure;
}

/**
 * 	CapacityOverflow() returns true if there isn't enough space in the stable
 *  for the animals
 */

bool stable::CapacityOverflow()
{
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection * aSection=sections->ElementAtNumber(i);
		if (aSection->GetNrOfAnimals()>aSection->GetPlaces()) {return true;};
	}
	return false;
}


/**
 * Check to see if an external manure store receives manure from this housing
 * Also link flooring to in-house storage, if necessary
 */
void stable::LinkManureStores(manurestore * aManureStore)
{
	for(int i=0;i<inhouseStores->NumOfNodes();i++)
	{
		instore * aStore=inhouseStores->ElementAtNumber(i);
		if ((aManureStore->getManureToID())==aStore->getmanureStoreID())
		{aStore->SetnextStore(aManureStore);}
		else
			if ((aManureStore->GetName()==aStore->GetManureToName())&& (aManureStore->GetIndex() == aStore->GetManureToIndex()))
			{aStore->SetnextStore(aManureStore);}
	}
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection * aSection=sections->ElementAtNumber(i);
		aSection->LinkManureStores(aManureStore);
	}
}
/**
 * check if every inhouseStores knows the next inhouseStores. Does also check if each flooring knows the next flooring
 */
void stable::CheckLinks()
{
	for(int i=0;i<inhouseStores->NumOfNodes();i++)
	{
		instore * aStore=inhouseStores->ElementAtNumber(i);
		if ((aStore->GetnextStore()==NULL) && (aStore->GetManureToName()!="NONE"))
		{
			cout << "Failed to find a receipient for manure from inhouse storage " << aStore->GetAlias() << endl;
			theMessage->FatalError("Stable:: an inhouse manure store is not linked to an external store");
		}
	}
	// For debugging routing of manure
	//   fstream route;   // added by NJH
	//   route.open("route.txt",ios::out);

	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection * aSection=sections->ElementAtNumber(i);
		aSection->CheckLinks();
		// For debugging routing of manure
		//   route << *aSection;
	}

	// For debugging routing of manure
	// route.close();
}

/**
	Check N budget at end of simulation
   Note: checks budget of constituent parts - stable itself does not have a budget class now
 */
/**
 * does not do any anything
 */
void stable::EndBudget(bool show)
{
#ifdef STALDDEBUG
	if (show)
		cout << "Balance for animal house " << GetAlias() << endl;
	for(int i=0;i<inhouseStores->NumOfNodes();i++)
	{
		instore * aStore=inhouseStores->ElementAtNumber(i);
		aStore->EndBudget(show);
	}
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection * aSection=sections->ElementAtNumber(i);
		aSection->EndBudget(show);
	}
#endif
}

/**
	Get N input from constituents
   Stable itself does not have a budget class
 */
double stable::GetNInput()
{
	double NInput=0;
	for(int i=0;i<inhouseStores->NumOfNodes();i++)
	{
		instore * aStore=inhouseStores->ElementAtNumber(i);
		NInput+=aStore->GetNInput();
	}
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection * aSection=sections->ElementAtNumber(i);
		NInput+=aSection->GetNInput();
	}
	return NInput;
}

/**
 * 	Get N output from constituents
 *    Stable itself does not have a budget class
 */

double stable::GetNOutput()
{
	double NInput=0;
	for(int i=0;i<inhouseStores->NumOfNodes();i++)
	{
		instore * aStore=inhouseStores->ElementAtNumber(i);
		NInput+=aStore->GetNOutput();
	}
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection * aSection=sections->ElementAtNumber(i);
		NInput+=aSection->GetNOutput();
	}
	return NInput;
}

/**
 * 	Get all N present in the stable
 */

double stable::GetTotalN()
{
	double tmp=0.0;
	for(int i=0;i<inhouseStores->NumOfNodes();i++)
	{
		instore * aStore=inhouseStores->ElementAtNumber(i);
		tmp+=aStore->GetTotalNStored();
	}
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection* aSection=sections->ElementAtNumber(i);
		tmp+=aSection->GetStoredN();
	}
	return tmp;
}

/**
 * 	Get all C present in the stable
 */

double stable::GetTotalC()
{
	double tmp=0.0;
	for(int i=0;i<inhouseStores->NumOfNodes();i++)
	{
		instore * aStore=inhouseStores->ElementAtNumber(i);
		tmp+=aStore->GetTotalCStored();
	}
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection* aSection=sections->ElementAtNumber(i);
		tmp+=aSection->GetStoredC();
	}
	return tmp;
}


/**
 * 	Get amount manure in the stable
 */
double stable::GetManureAmount()
{
	double CurrentAmount=0;
	for(int i=0;i<inhouseStores->NumOfNodes();i++)
	{
		instore * aStore=inhouseStores->ElementAtNumber(i);
		CurrentAmount+=aStore->GetAmountManure();
	}
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection * aSection=sections->ElementAtNumber(i);
		CurrentAmount+=aSection->GetManureAmount();
	}
	return CurrentAmount;
}

/**
 * 	Get amount manure from the stable
 */
product * stable::GetFromManureStore(double RequestedAmount, product * p)
{
	product * aManure = new manure(*(manure *) p);
	aManure->Setamount(0.0);
	product * manureFound = NULL;
	for(int i=0;i<inhouseStores->NumOfNodes();i++)
	{
		instore * aStore=inhouseStores->ElementAtNumber(i);
		manureFound=aStore->GetFromManureStore(RequestedAmount, p);
		if (manureFound)
		{
			*aManure + *manureFound;
			delete manureFound;
		}
	}
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection * aSection=sections->ElementAtNumber(i);
		manureFound=aSection->GetFromManureStore(RequestedAmount, p, true);
		if (manureFound)
		{
			*aManure + *manureFound;
			delete manureFound;
		}
	}
	if (aManure->GetAmount()==0.0)
	{
		delete aManure;
		aManure=NULL;
	}
	return aManure;
}

/**
 * 	Scale animal section (and inhouse storage) to animal numbers NJH June 2001
 */
void stable::ScaleSize()
{
	if (theControlParameters->GetvolatModel()> 0)
	{
		//    double oldplanArea = planArea;
		//   double totalAreaSections;// = GetTotalAreaSections();
		//    planArea=1.2 * totalAreaSections;  //allow for passageways etc
		if (inUse)
		{
			for(int i=0;i<inhouseStores->NumOfNodes();i++)
			{
				instore * aStore=inhouseStores->ElementAtNumber(i);     //start by zeroing existing capacities (scaling accumulates capacity from floors)
				aStore->SetsurfaceArea(0.0);
				aStore->SetCapacity(0.0);
			}
			for(int i=0;i<sections->NumOfNodes();i++)
			{
				animalSection * aSection=sections->ElementAtNumber(i);
				aSection->ScaleSize();
			}
			planArea = GetTotalAreaSections() * 1.2;  //increase by factor to allow for passageways etc
			calcWallArea();
			if (controlledVent>0)
			{
				minVentilation=/*planArea */ CalcMinimumVentilationRate();//meanHeight/minairChange;
				maxVentilation=/*planArea */ CalcMaximumVentilationRate();//meanHeight/maxairChange;
			}
		}
	}
}

/**
	Calculate total floor area of housing
 */
double stable::GetTotalAreaSections()
{
	double area=0.0;
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection * aSection=sections->ElementAtNumber(i);
		area+=aSection->GetTotalArea();
	}
	return area;
}

/**
	Calculate total daily electricity use of housing
 */
double stable::GetDailyElectricity()
{
	double val=0.0;
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection * aSection=sections->ElementAtNumber(i);
		val+=aSection->GetDailyElectricityUse();
	}
	return val;
}





/**
 * check to make sure total area of animal sections is less than plan area of building
 */
void stable::checkArea()
{
	if (planArea<GetTotalAreaSections())
		theMessage->FatalError("Stable:: total area of animal sections > plan area of housing");
}

/**
 * Calculate the area of the wall, Assuming that the stable is cubic
 */
void stable::calcWallArea()
{
	wallArea=4 * meanHeight * pow(planArea,0.5); //sq m - area of walls
	//pow(planArea,0.5) is the length of a wall.
}

//!Calculates the ventilation and inside temperature of animal housing with forced ventilation systems
/*!
 * \param heatOP output of sensible heat by the animals in the housing (in Watts)
 * \param outsideAirTemp outside air temperature in Celsius
 * \param supplementaryHeat supplementary heating required to maintain the inside temperature at the target level (in Watts)
 */
void stable::controlled(double heatOP, double outsideAirTemp, double *supplementaryHeat)
{
	double surfaceArea = planArea + wallArea; //sq meters - surface area of house

	double meanThermalTrans = (planArea * thermalTransRoof + wallArea * thermalTransWall)/surfaceArea;   //in W per metre square per K
	outsideAirTemp+=273.13;
	double deltaTemper=0.0;
	*supplementaryHeat=0.0;
	ventilationRate=0.0;
	double airDensity = GetdensityAir(GetstandardAirPressure(),targetTemperature,GetsaturatedWaterVapourPressure(targetTemperature));
	//should include a water balance!
	if (targetTemperature>outsideAirTemp) //it will be possible to maintain target temp
	{
		ventilationRate=((heatOP/(targetTemperature-outsideAirTemp))- meanThermalTrans*surfaceArea)
    														/(GetspecificHeatCapAir()*airDensity);
		insideTemperature=targetTemperature  - 273.13;
	}
	if ((ventilationRate > maxVentilation)||(targetTemperature<=outsideAirTemp)) //it will not be possible to maintain target temp, so set ventilation to max
	{
		ventilationRate=maxVentilation;
		deltaTemper=heatOP/(GetspecificHeatCapAir()*airDensity* ventilationRate + meanThermalTrans*surfaceArea);
		//calculate the inside temperature
		insideTemperature= outsideAirTemp + deltaTemper - 273.13;
	}
	if (ventilationRate< minVentilation)  //to keep the animals healthy, there needs to be some ventilation
	{
		ventilationRate= minVentilation;
		//calculate the supplementary heat input necessary to maintain the inside temperature at the target temperature
		*supplementaryHeat= (targetTemperature - outsideAirTemp)*(GetspecificHeatCapAir()*airDensity * ventilationRate + meanThermalTrans*surfaceArea);
	}
}

/**
 * Nic need to document this one
 */
void stable::uncontrolled(double heatOP,double outsideAirTemp, double windspeed, double solarRad)
{
	double surfaceArea = planArea + wallArea; //sq meters - total surface area of walls and roof of housing
	double meanThermalTrans = (planArea * thermalTransRoof + wallArea * thermalTransWall)/surfaceArea;

	outsideAirTemp+=273.15;
	double skyTemp = outsideAirTemp; //apparent temperature of sky, K
	//!Temperature of outside surface of roof
	double surfaceTemp = outsideAirTemp; //rough estimate of temperature of roof surface, K
	double longWaveDown = GetlongWaveDown(skyTemp); //longwave radiation input to surface, Watts per square metre
	double longWaveUp = GetlongWaveUp(emissivity,surfaceTemp);//longwave radiation output of surface, Watts per square metre; note small error here, if surface temperature not equal to air temperature
	double longWave = longWaveDown - longWaveUp;//net longwave energy exchange, Watts per square metre
	double airDensity = GetdensityAir(GetstandardAirPressure(),outsideAirTemp,GetsaturatedWaterVapourPressure(outsideAirTemp));//density of the air in kg per cubic metre
	//should include a water balance!
	ventilationRate = 0; //cubic meters per sec
	double deltaTemper, tempDeltaTemp;
	//!tempSol = outside air temperature which, in the absence of solar radiation, would give the same temperature distribution and rate of energy transfer
	//!through a wall or roof as that which exists with the actual air temperature and incident radiation
	double tempSol = outsideAirTemp + externSurfResis *(absorbCoeff *
			solarRad - emissivity * longWave);  // eqn 9 in Cooper et al
	//!Temperature difference between outside surfaces of roof and air temperature
	double deltaSol = tempSol - outsideAirTemp;
	//!Heat input or output to housing through the roof material
	double q = thermalTransRoof * planArea * deltaSol;//Watts
	//!First set ventilation rate to that which is necessary to have the optimum air velocity
	double targetventilationRate=optimumAirVelocity* planArea;
	//!Calculate the temperature difference that will be achieved by the ventilation with outside air
	deltaTemper=(heatOP+q)/(GetspecificHeatCapAir()*airDensity* ventilationRate + meanThermalTrans*surfaceArea);

	//!if the temperature achieved obtaining the optimal ventilation rate exceeds the maximum permitted, recalculate the ventilation rate to prevent this
	if ((outsideAirTemp+deltaTemper>maxTemperature) && (outsideAirTemp<maxTemperature))
	{
		targetventilationRate=(((heatOP+q)/(maxTemperature-outsideAirTemp))- meanThermalTrans*surfaceArea)
    														/(GetspecificHeatCapAir()*airDensity);
		deltaTemper=(heatOP+q)/(GetspecificHeatCapAir()*airDensity* ventilationRate + meanThermalTrans*surfaceArea);
	}
	//!if the temperature achieved obtaining the optimal ventilation rate is below the minimum permitted, recalculate the ventilation rate to prevent this
	if (outsideAirTemp+deltaTemper<minTemperature)
	{
		targetventilationRate=(((heatOP+q)/(minTemperature-outsideAirTemp))- meanThermalTrans*surfaceArea)
    														/(GetspecificHeatCapAir()*airDensity);
		deltaTemper=(heatOP+q)/(GetspecificHeatCapAir()*airDensity* ventilationRate + meanThermalTrans*surfaceArea);
	}
	//!now we know the target ventilation rate, we need to see if this can be achieved by adjusting the aperture size
	//!assume initially that this is possible
	ventilationRate=targetventilationRate;
	double minVent;
	//!calculate ventilation with the minimum ventilation
	double apertureHeight = minPropApertureHeight * maxapertureHeight;
	CalcFreeVentilation(airDensity, meanThermalTrans, surfaceArea, apertureHeight,
			windspeed, outsideAirTemp, heatOP, q, &minVent, &tempDeltaTemp);
	//!if the target ventilation is less than the minimum, use the minimum
	if (targetventilationRate<minVent)
	{
		ventilationRate=minVent;
		deltaTemper=tempDeltaTemp;
	}
	double maxVent;
	//!calculate ventilation with the maximum ventilation
	apertureHeight = maxapertureHeight;
	CalcFreeVentilation(airDensity, meanThermalTrans, surfaceArea, apertureHeight,
			windspeed, outsideAirTemp, heatOP, q, &maxVent, &tempDeltaTemp);
	//!If ventilation exceeds the maximum, use the maximum
	if (targetventilationRate>maxVent)
	{
		ventilationRate=maxVent;
		deltaTemper=tempDeltaTemp;
	}

	insideTemperature = outsideAirTemp+deltaTemper - 273.0;
}
/**
 * Nic need to document this one
 */
void stable::CalcFreeVentilation(double airDensity, double thermalTrans, double surfaceArea, double apertureHeight,
		double windspeed, double outsideAirTemp, double heatOP, double q, double *ventilationRate, double *deltaTemper)
{
	//! Model from A THERMAL BALANCE MODEL FOR LIVESTOCK BUILDINGS, Cooper et al, 1998
	*ventilationRate=0.0;
	double Cd = 0.6; //!discharge coefficient for sharp-edged openings
	//!B = factor to simplify subsequent equations
	double B = 9.0 * outsideAirTemp/(pow(Cd,2)* pow(apertureWidth,2) * pow(apertureHeight,3) * 9.81);

	double  a1 = 0.0, b1 = 0.0, c1 = 0.0, d1 = 0.0;
	double  x[3];
	int     solutions;
	//! a1, b1, c1, d1 are coefficients of the cubic equation number 21 in Cooper et al
	double spheatCap = GetspecificHeatCapAir();
	a1 =  spheatCap * airDensity;
	b1 = thermalTrans * surfaceArea;


	//!windforcedVent is the ventilation that occurs due to the effect of the wind blowing air through the apertures in the building
	double windforcedVent = 0.25 * apertureHeight * apertureWidth * windspeed;  //we assume a square building, with only one wall facing the wind, cubic m per sec
	c1 = -pow(windforcedVent,2)* spheatCap * airDensity;
	d1 = -thermalTrans * surfaceArea * pow(windforcedVent,2) - (heatOP + q)/B;
	//!solve for ventilation rate
	SolveCubic(a1, b1, c1, d1, &solutions, x);
	int index=0;
	double test=0.0;
	//solution is the highest value in the matrix
	for (int j=0; j<solutions; j++)
	{
		if (x[j]>test)
		{
			test=x[j];
			index=j;
		}

		*deltaTemper =  (heatOP + q)/(spheatCap * airDensity * x[index] + thermalTrans * surfaceArea);
		*ventilationRate = x[index];
	}
}


/**
 * Get number of animals present
 */
double stable::GetNrOfAnimals()
{
	double ret_val=0.0;
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection * aSection=sections->ElementAtNumber(i);
		ret_val+=aSection->GetNrOfAnimals();
	}
	return ret_val;
}

/**
 * Nic need to document this one
 */
double stable::CalcPropSensible()
{
	double ret_val=0.0;
	ret_val = 0.8 -0.38*pow(insideTemperature,2)/1000.0;
	ret_val *= 0.85;      //assumes wet surfaces so 0.85
	return ret_val;
}


/**
	Operator << for output
 */

ostream& operator<< (ostream& os,stable* b)
{
	os << (building*) &b;   //first output the building specifics
	os << setiosflags(ios::left) << setw(40) << "Ventilation rate" << b->GetventilationRate() << endl;
	os << "animals" << endl;
	os << b->Livestock << endl;
	return os;
};

/**
 * remove everything from the stable
 */
product * stable::EmptyManureFromStore()
{
	product * aManure = new manure();
	aManure->Setamount(0.0);
	product * manureFound = NULL;
	for(int i=0;i<inhouseStores->NumOfNodes();i++)
	{
		instore * aStore=inhouseStores->ElementAtNumber(i);
		manureFound=aStore->EmptyManureStore();
		if (manureFound)
		{
			*aManure + *manureFound;
			delete manureFound;
		}
	}
	for(int i=0;i<sections->NumOfNodes();i++)
	{
		animalSection * aSection=sections->ElementAtNumber(i);
		manureFound=aSection->GetFromManureStore(aSection->GetManureAmount(), NULL, true);
		if (manureFound)
		{
			*aManure + *manureFound;
			delete manureFound;
		}
	}
	if (aManure->GetAmount()==0.0)
	{
		delete aManure;
		aManure=NULL;
	}
	return aManure;
}

void stable::OpenDetailsFile()
{
	fileAccess hd;
	string oldPath=hd.getCurrentPath();
	hd.changeDir(globalSettings->getOutputDirectory());
#ifdef  __BCplusplus__
	char buffer[MAX_TXT];
	getcwd(buffer, MAX_TXT);

	chdir(globalSettings->getOutputDirectory().c_str());
#endif
	string namestr="housing";
	char Indexstr[10];
	sprintf(Indexstr,"(%d)",Index);
	namestr=namestr+Indexstr+".xls";
	detailsFile = new fstream(namestr.c_str(),ios::out);
	ShowStable(true);
#ifdef  __BCplusplus__
	chdir(buffer);
#endif
	hd.changeDir(oldPath);
}

void stable::CloseDetailsFile()
{
	detailsFile->close();
	delete detailsFile;
}

void stable::ShowStable(bool header)
{
	if (header)
	{
		*detailsFile << "Day" << "\t"<< "HouseName" << "\t" << "ventilationRate" << "\t"<<  "insideTemperature" << "\t"
				<< "CH4Emission" << "\t" << "CO2Emission" << "\t" << "NH3Emission" << "\t" << "N2Emission" << "\t" << "N2OEmission" << "\t" ;
		for(int i=0;i<sections->NumOfNodes();i++)
		{
			*detailsFile << "SectionName" << "\t" << "ManureAmount" << "\t" << "DM" << "\t"
					<< "StoredC" << "\t" << "StoredN" << "\t";
		}
		for(int i=0;i<inhouseStores->NumOfNodes();i++)
		{
			*detailsFile << "StoreName" << "\t" << "StoreAmount" << "\t" << "StoreDM" << "\t"
					<< "StoreC" << "\t" << "StoreN" << "\t";
		}
		*detailsFile << endl;
	}
	else
	{
		*detailsFile << theTime.GetJulianDay() << "\t"<< GetAlias() << "\t" << ventilationRate << "\t"<<  insideTemperature << "\t"
				<< CH4Emission << "\t" << CO2Emission << "\t" << NH3Emission << "\t" << N2Emission << "\t" << N2OEmission<< "\t" ;
		for(int i=0;i<sections->NumOfNodes();i++)
		{
			animalSection* aSection=sections->ElementAtNumber(i);
			*detailsFile << aSection->GetAlias() << "\t" << aSection->GetManureAmount() << "\t" << aSection->GetStoredDM() << "\t"
					<< aSection->GetStoredC() << "\t" << aSection->GetStoredN()<< "\t";
		}
		for(int i=0;i<inhouseStores->NumOfNodes();i++)
		{
			instore * aStore=inhouseStores->ElementAtNumber(i);
			*detailsFile << aStore->GetAlias() << "\t" << aStore->GetAmountManure() << "\t" << aStore->GetTotalDMStored() << "\t"
					<< aStore->GetTotalCStored() << "\t" << aStore->GetTotalNStored()<< "\t";
		}
		*detailsFile << endl;
	}
}
