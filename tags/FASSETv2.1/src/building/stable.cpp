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
Modified by NJH 24/8/99
Modified by NJH 3/11/99
Now acts as base class from which pig and cattle housing are derived
\****************************************************************************/
#include <common.h>
#include "stable.h"
#include "floorst.h"
#include "instore.h"
#include "message.h"
#include <products.h>
#include <climclas.h>
#include <bstime.h>
#include <nixcbits.h>
#include <contrparm.h>
#include <fstream>
#include <output.h>
#ifndef _STANDALONE
	#include "indicat.h"
	#include <animstk.h>
#endif
/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
stable::stable(char* aname,const int aIndex,const base* aOwner,
               animalStock* alivestock, int aVolatModel)
	:building(aname,aIndex,aOwner)
{
   Livestock=alivestock;
   slurrySystem = 1;
 	sections=new linkList<animalSection>;
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
   inUse = false;
};

/****************************************************************************\
  Destructor
\****************************************************************************/
stable::~stable()
{
   delete sections;
   delete inhouseStores;
   if (theOutputControl->GetManureManageDetails())
		CloseDetailsFile();
};

/****************************************************************************\
	ReadParameters() reads information from file. File should be opened and handled
   by Owner.
\****************************************************************************/

void stable::ReadParameters(fstream * file)
{
// name of instance must be set before running this function
//	building::ReadParameters(file);  cost of animal housing is now the sum of the constituent parts (animal sections etc)
   Setfile(file);
	SetCritical();
	GetParameter("Alias",&Alias);
   UnsetCritical();
   switch (theControlParameters->GetvolatModel())
   {
	 case 0:
      break;
	 case 1:
     {
      SetCritical(); // Must find parameters now - undo with UnsetCritical()
//		GetParameter("Ventilationrate",&ventilationrate);


		GetParameter("meanHeight",&meanHeight);
		GetParameter("planArea",&planArea);
		GetParameter("controlledVent",&controlledVent);
		GetParameter("thermalTransWall",&thermalTransWall);
		GetParameter("thermalTransRoof",&thermalTransRoof);
      if (controlledVent>0)
      {
         GetParameter("minVentilation",&minVentilation);
         GetParameter("maxVentilation",&maxVentilation);
         if ((minVentilation>maxVentilation)||(maxVentilation<=0.0))
           theMessage->FatalError("Stable:: minimum ventilation rate > maximum or maximum <= 0");
         GetParameter("targetTemperature",&targetTemperature);
         targetTemperature+=273.13;
      }
      else
      {
         GetParameter("apertureHeight",&apertureHeight);
//         GetParameter("apertureWidth",&apertureWidth);
         GetParameter("absorbCoeff",&absorbCoeff);
         GetParameter("emissivity",&emissivity);
         GetParameter("externSurfResis",&externSurfResis);
      }
      UnsetCritical();
      break;
      }

   }

    // Construct in-house manure stores
   Setcur_pos(0);
  	int first,num;
   string namestr;
   char Indexstr[10];
   sprintf(Indexstr,"(%d)",Index);
   namestr=(string) GetName()+Indexstr+"."+"inhouse";;
   GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of inhouse stores associated with this instance
   FindSection((char*) namestr.c_str(),0);  // find the correct place in the input file
//   string instanceName;
   for(int inx=first;inx<(first+num);inx++)
   {
    instore *manureInst=new instore((char *)namestr.c_str(),inx,this);
    manureInst->SetScalable(true);
    manureInst->ReadParameters(file);
    inhouseStores->InsertLast(manureInst);
   }
	calcWallArea();
}

/****************************************************************************\
   GetanimalSection() is called by animals to find out where they belong
\****************************************************************************/

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
/****************************************************************************\
   DailyUpdate() should be called everyday
\****************************************************************************/

void stable::DailyUpdate()
{
	building::DailyUpdate();
 	for(int i=0;i<sections->NumOfNodes();i++)
 	{
   	animalSection* aSection=sections->ElementAtNumber(i);
   	aSection->DailyUpdate();
 	}
   if (theControlParameters->GetvolatModel()!=2)
   	{Volatize();}

   CleanManure();
#ifndef _STANDALONE
   theOutput->AddStateIndicator(environmentalIndicator,"35.61 N bound in stable manure","kg N",GetTotalN()*1000.0);
   theOutput->AddStateIndicator(environmentalIndicator,"45.61 C bound in stable manure","kg C",GetTotalC()*1000.0);

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
   theOutput->AddIndicator(economicIndicator,"20.02 Electricity consumption","KWh",electricity);
#endif
		EndBudget(true);

//   GiveIndicator();
}

/****************************************************************************\
 	Sets the herd of the stable
\****************************************************************************/

void stable::SetLivestock(animalStock* aLivestock)
{
	Livestock=aLivestock;
}


/****************************************************************************\
   Volatize() Called everyday by DailyActions()
   Computes the daily volatization of N and C gasses from the dirty stable floor
\****************************************************************************/
void stable::Volatize()
{
 volatAmmonia=0.0, waterEvap=0.0, N2Emission=0.0, N2OEmission=0.0, CH4Emission=0.0, CO2Emission=0.0;
 switch (theControlParameters->GetvolatModel())
 //
 {
 	case 0:
   {
    for(int i=0;i<sections->NumOfNodes();i++)
    {
	   double tempvolatAmmonia=0.0, tempN2Emission=0.0, tempN2OEmission=0.0, tempCH4Emission=0.0, tempCO2Emission=0.0;
      animalSection* aSection=sections->ElementAtNumber(i);
      aSection->Volatize(&tempvolatAmmonia, &tempN2Emission, &tempN2OEmission, &tempCH4Emission, &tempCO2Emission);
		volatAmmonia+=tempvolatAmmonia;
      N2Emission+=tempN2Emission;
      N2OEmission+=tempN2OEmission;
      CH4Emission+= tempCH4Emission;
      CO2Emission+=tempCO2Emission;
    }
    for(int i=0;i<inhouseStores->NumOfNodes();i++)
    {
	   double tempvolatAmmonia=0.0, tempN2Emission=0.0, tempN2OEmission=0.0, tempCH4Emission=0.0, tempCO2Emission=0.0;
      instore * aStore=inhouseStores->ElementAtNumber(i);
      aStore->Volatize(&tempvolatAmmonia, &tempN2Emission, &tempN2OEmission, &tempCH4Emission, &tempCO2Emission);
		volatAmmonia+=tempvolatAmmonia;
      N2Emission+=tempN2Emission;
      N2OEmission+=tempN2OEmission;
      CH4Emission+= tempCH4Emission;
      CO2Emission+=tempCO2Emission;
    }
    break;
	}
   case 1:
   {
      if (inUse)   //could be zero if using scaling and no animals for this housing type
      {
       double Ameantemp,Amin_temp,
            Amax_temp,Aprecip,Aradiation,AEPot,Ahumidity,Awindspeed;
       theClimate->GetClimate(Ameantemp, Amin_temp,Amax_temp, Aprecip, Aradiation,AEPot, Awindspeed, Ahumidity);
      //Ameantemp+=5.0;
       double heatOP = 0.0;
       double airVelocity = 0.0;
       for(int i=0;i<sections->NumOfNodes();i++)
       {
         animalSection* aSection=sections->ElementAtNumber(i);
         heatOP+=aSection->GetanimalHeatOp();
       }
       double sensibleHeatOp=CalcPropSensible()*heatOP;
       if (controlledVent>0)
       {
         double supplementaryHeat=0.0;
         controlled(sensibleHeatOp, Ameantemp, &supplementaryHeat);
          airVelocity=ventilationRate/planArea;
       }
       else
       {
         uncontrolled(sensibleHeatOp, Ameantemp, Awindspeed, Aradiation);
          airVelocity=0.05*ventilationRate/planArea;
       }

   //    airVelocity = 4.62 * (ventilationRate/planArea) + 0.106;  //from Aarnick and Elzing, 1998

       for(int i=0;i<sections->NumOfNodes();i++)
       {
         double tempvolatAmmonia=0.0, tempwaterEvap=0.0, tempN2Emission=0.0, tempN2OEmission=0.0, tempCH4Emission=0.0, tempCO2Emission=0.0;
         animalSection* aSection=sections->ElementAtNumber(i);
         aSection->Volatize(airVelocity,Ahumidity,insideTemperature,
            &tempvolatAmmonia, &tempwaterEvap, &tempN2Emission, &tempN2OEmission, &tempCH4Emission, &tempCO2Emission);
          if (!controlledVent)
            aSection->SetanimalHeatOp(0.0);  // zero heat production ready for next day but only for cattle
         volatAmmonia+=tempvolatAmmonia;
         waterEvap+= tempwaterEvap;
         N2Emission+=tempN2Emission;
         N2OEmission+=tempN2OEmission;
         CH4Emission+= tempCH4Emission;
         CO2Emission+=tempCO2Emission;
       }
       for(int i=0;i<inhouseStores->NumOfNodes();i++)
       {
         double tempvolatAmmonia=0.0, tempwaterEvap=0.0, tempN2Emission=0.0, tempN2OEmission=0.0, tempCH4Emission=0.0, tempCO2Emission=0.0;
         instore * aStore=inhouseStores->ElementAtNumber(i);
         aStore->inhouseVolatize(airVelocity,Ahumidity,insideTemperature,1,
            &tempvolatAmmonia, &tempwaterEvap, &tempN2Emission, &tempN2OEmission, &tempCH4Emission, &tempCO2Emission);
         volatAmmonia+=tempvolatAmmonia;
         waterEvap+= tempwaterEvap;
         N2Emission+=tempN2Emission;
         N2OEmission+=tempN2OEmission;
         CH4Emission+= tempCH4Emission;
         CO2Emission+=tempCO2Emission;
       }
      }
	   if (theOutputControl->GetManureManageDetails())
			ShowStable(detailsFile,false);
      break;
   } //end of case
  } // end of switch
#ifndef _STANDALONE
   theOutput->AddIndicator(environmentalIndicator,"35.40 NH3-N volatilisation from animal house","kg N",volatAmmonia*1000.0);
   theOutput->AddIndicator(environmentalIndicator,"35.41 N2-N volatilisation from animal house","kg N",N2Emission*1000.0);
   theOutput->AddIndicator(environmentalIndicator,"35.42 N2O-N volatilisation from animal house","kg N",N2OEmission*1000.0);
   theOutput->AddIndicator(environmentalIndicator,"45.40 CO2-C volatilisation from animal house","kg C",CO2Emission*1000.0);
   theOutput->AddIndicator(environmentalIndicator,"45.41 CH4-C volatilisation from animal house","kg C",CH4Emission*1000.0);
/*   fstream * filehandle = theMessage->GiveDebugHandle();
   *filehandle << " hous " << (volatAmmonia*1000.0) << " " ;
   theMessage->CloseDebugHandle();*/
//   cout << "house " <<  volatAmmonia*1000.0 << " ";
#endif
//   double tmp;
//   EndBudget(&tmp);
}


/****************************************************************************\
Remove manure from flooring and transfer to relevant manure store
\****************************************************************************/
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
	theOutput->AddIndicator(environmentalIndicator,"35.80 N transf. from stable to store","kg N",(aManure->GetAllN().n*aManure->GetAmount())*1000.0);
	theOutput->AddIndicator(environmentalIndicator,"45.80 C transf. from stable to store","kg C",(aManure->GetC_content()*aManure->GetAmount())*1000.0);
#endif
 delete aManure;
}

/****************************************************************************\
	CapacityOverflow() returns true iff there isn't enough space in the stable
   for the animals
\****************************************************************************/

bool stable::CapacityOverflow()
{
	for(int i=0;i<sections->NumOfNodes();i++)
	 {
   	animalSection * aSection=sections->ElementAtNumber(i);
	   if (aSection->GetNrOfAnimals()>aSection->GetPlaces()) {return true;};
	 }
   return false;
}


/****************************************************************************\
	Check to see if an external manure store receives manure from this housing
   Also link flooring to in-house storage, if necessary
\****************************************************************************/
void stable::LinkManureStores(manurestore * aManureStore)
{
 for(int i=0;i<inhouseStores->NumOfNodes();i++)
 {
   instore * aStore=inhouseStores->ElementAtNumber(i);
   if ((aManureStore->GetName()==aStore->GetManureToName())&& (aManureStore->GetIndex() == aStore->GetManureToIndex()))
           {aStore->SetnextStore(aManureStore);}
 }
 for(int i=0;i<sections->NumOfNodes();i++)
 {
   animalSection * aSection=sections->ElementAtNumber(i);
   aSection->LinkManureStores(aManureStore);
 }
}

void stable::CheckLinks()
{
 for(int i=0;i<inhouseStores->NumOfNodes();i++)
 {
   instore * aStore=inhouseStores->ElementAtNumber(i);
   if ((aStore->GetnextStore()==NULL) && (aStore->GetManureToName()!="NONE"))
           theMessage->FatalError("Stable:: an inhouse manure store is not linked to an external store");
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

/****************************************************************************\
	Check N budget at end of simulation
   Note: checks budget of constituent parts - stable itself does not have a budget class now
\****************************************************************************/

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

/****************************************************************************\
	Get N input from constituents
   Stable itself does not have a budget class
\****************************************************************************/

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

/****************************************************************************\
	Get N output from constituents
   Stable itself does not have a budget class
\****************************************************************************/

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

/****************************************************************************\
	Get all N present in the stable
\****************************************************************************/

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

/****************************************************************************\
	Get all C present in the stable
\****************************************************************************/

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


/****************************************************************************\
	Get amount manure in the stable
\****************************************************************************/
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

/****************************************************************************\
	Get amount manure from the stable
\****************************************************************************/
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

/****************************************************************************\
	Scale animal section (and inhouse storage) to animal numbers NJH June 2001
\****************************************************************************/
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


void stable::GiveIndicator(int indicatorType)
{
 for(int i=0;i<inhouseStores->NumOfNodes();i++)
 {
   instore * aStore=inhouseStores->ElementAtNumber(i);
   aStore->GiveIndicator(indicatorType);
 }
 for(int i=0;i<sections->NumOfNodes();i++)
 {
   animalSection * aSection=sections->ElementAtNumber(i);
   aSection->GiveEconomicIndicators();
 }

#ifndef _STANDALONE
 if (indicatorType==environmentalIndicator)
   theOutput->AddStateIndicator(environmentalIndicator,"35.61 N bound in stable manure","kg N",GetTotalN()*1000.0);
#endif
}
/****************************************************************************\
	CalcLP  indicators for LP-model
\****************************************************************************/

void stable::CalcLP()
{
	building::CalcLP();

};

/****************************************************************************\
	Calculate total floor area of housing
\****************************************************************************/

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

/****************************************************************************\
	Calculate total daily electricity use of housing
\****************************************************************************/

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


/****************************************************************************\

\****************************************************************************/

void stable::Initialise(char * InputFilename)
{
//   InstanceName="Cattlestable";
  if (OpenInputFile(InputFilename))
  {
   Setcur_pos(0);
   FindSection("Cattlestable",0);
   ReadParameters(file);
  }
  else
  	theMessage->FatalError("Building:: error in opening building initilization file");
}


/****************************************************************************\
check to make sure total area of animal sections is less than plan area of building
\****************************************************************************/
void stable::checkArea()
{
  if (planArea<GetTotalAreaSections())
  		theMessage->FatalError("Stable:: total area of animal sections > plan area of housing");
}

/****************************************************************************\
\****************************************************************************/
void stable::calcWallArea()
{
  wallArea=4 * meanHeight * pow(planArea,0.5); //sq m - area of walls
}

/****************************************************************************\
Housing with controlled climate
Ventilation rate in cubic metres per sec
Note that solar heat input is ignored
\****************************************************************************/
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
  if (targetTemperature>outsideAirTemp) //can possibly maintain target temp
  {
    ventilationRate=((heatOP/(targetTemperature-outsideAirTemp))- meanThermalTrans*surfaceArea)
    										/(GetspecificHeatCapAir()*airDensity);
//    deltaTemper=heatOP/(GetspecificHeatCapAir()*airDensity* ventilationRate + meanThermalTrans*surfaceArea);
    insideTemperature=targetTemperature  - 273.13;
  }
  if ((ventilationRate > maxVentilation)||(targetTemperature<=outsideAirTemp)) //set ventilation to max
  {
    ventilationRate=maxVentilation;
    deltaTemper=heatOP/(GetspecificHeatCapAir()*airDensity* ventilationRate + meanThermalTrans*surfaceArea);
    insideTemperature= outsideAirTemp + deltaTemper - 273.13;
  }
  if (ventilationRate< minVentilation)  //need some ventilation
  {
     ventilationRate= minVentilation;
     *supplementaryHeat= (targetTemperature - outsideAirTemp)*(GetspecificHeatCapAir()*airDensity * ventilationRate + meanThermalTrans*surfaceArea);
     /*if (*supplementaryHeat > maxsupplementaryHeat)
     {
        *supplementaryHeat = maxsupplementaryHeat;
        deltaTemper=(heatOP+maxsupplementaryHeat)/(GetspecificHeatCapAir()*airDensity * ventilationRate + meanThermalTrans*surfaceArea);
     }
     else */

  }
}

/****************************************************************************\

\****************************************************************************/
void stable::uncontrolled(double heatOP,double outsideAirTemp, double windspeed, double solarRad)
{
  double surfaceArea = planArea + wallArea; //sq meters - surface area of house
  apertureWidth = pow(planArea,0.5);
  double meanThermalTrans = (planArea * thermalTransRoof + wallArea * thermalTransWall)/surfaceArea;

  outsideAirTemp+=273.15;
  double skyTemp = outsideAirTemp; //K
  double surfaceTemp = outsideAirTemp; //K
  double longWaveDown = GetlongWaveDown(skyTemp);
  double longWaveUp = GetlongWaveUp(emissivity,surfaceTemp);
  double longWave = longWaveDown - longWaveUp;
  double airDensity = GetdensityAir(GetstandardAirPressure(),outsideAirTemp,GetsaturatedWaterVapourPressure(outsideAirTemp));
             //should include a water balance!
  double optVel = 0.250;   //velocity in m/s
  double minTemp=5.0 + 273.15;
  double maxTemp=20.0+273.15;
  double pmin=0.05;        //min proportion of wall opened for ventilation
  double pmax=0.25;        //max proportion of wall opened for ventilation
  ventilationRate = 0; //cubic meters per sec
  double deltaTemper, tempDeltaTemp;

  double tempSol = outsideAirTemp + externSurfResis *(absorbCoeff *
                solarRad - emissivity * longWave);  // eqn 9 in Cooper et al
  double deltaSol = tempSol - outsideAirTemp;
  double q = thermalTransRoof * planArea * deltaSol;

  ventilationRate=optVel* planArea;
  deltaTemper=(heatOP-q)/(GetspecificHeatCapAir()*airDensity* ventilationRate + meanThermalTrans*surfaceArea);
  if ((outsideAirTemp+deltaTemper>maxTemp) && (outsideAirTemp<maxTemp))
  {
  		ventilationRate=(((heatOP-q)/(maxTemp-outsideAirTemp))- meanThermalTrans*surfaceArea)
    										/(GetspecificHeatCapAir()*airDensity);
	   deltaTemper=(heatOP-q)/(GetspecificHeatCapAir()*airDensity* ventilationRate + meanThermalTrans*surfaceArea);
  }
  if (outsideAirTemp+deltaTemper<minTemp)
  {
  		ventilationRate=(((heatOP-q)/(minTemp-outsideAirTemp))- meanThermalTrans*surfaceArea)
    										/(GetspecificHeatCapAir()*airDensity);
	   deltaTemper=(heatOP-q)/(GetspecificHeatCapAir()*airDensity* ventilationRate + meanThermalTrans*surfaceArea);
  }
  double minVent;
  double apertureArea = pmin * meanHeight * apertureWidth; // sq m - total area of openings in windward wall
  CalcFreeVentilation(airDensity, meanThermalTrans, surfaceArea, apertureArea,
			windspeed, outsideAirTemp, heatOP, q, &minVent, &tempDeltaTemp);
  if (ventilationRate<minVent)
  {
  		ventilationRate=minVent;
      deltaTemper=tempDeltaTemp;
  }
  double maxVent;
  apertureArea = pmax * meanHeight * apertureWidth;
  CalcFreeVentilation(airDensity, meanThermalTrans, surfaceArea, apertureArea,
			windspeed, outsideAirTemp, heatOP, q, &maxVent, &tempDeltaTemp);
  if (ventilationRate>maxVent)
  {
  		ventilationRate=maxVent;
      deltaTemper=tempDeltaTemp;
  }

   insideTemperature = outsideAirTemp+deltaTemper - 273.0;
}

void stable::CalcFreeVentilation(double airDensity, double thermalTrans, double surfaceArea, double apertureArea,
			double windspeed, double outsideAirTemp, double heatOP, double q, double *ventilationRate, double *deltaTemper)
{
  *ventilationRate=0.0;
  double Cd = 0.6;
  double B = 9.0 * outsideAirTemp/(pow(Cd,2)* pow(apertureWidth,2) * pow(apertureHeight,3) * 9.81);

  double  a1 = 0.0, b1 = 0.0, c1 = 0.0, d1 = 0.0;
  double  x[3];
  int     solutions;

   a1 = GetspecificHeatCapAir() * airDensity;
   b1 = thermalTrans * surfaceArea;

   double naturalVent=0;
   double windforcedVent = 0.5 * apertureArea * windspeed; //assume wind direct on wall      , //cubic m per sec
   c1 = -pow(windforcedVent,2)* GetspecificHeatCapAir() * airDensity;
   d1 = -thermalTrans * surfaceArea * pow(windforcedVent,2) - (heatOP + q)/B;
   SolveCubic(a1, b1, c1, d1, &solutions, x);
   int index=0;
   double test=0.0;
   for (int j=0; j<solutions; j++)
   {
    if (x[j]>test)
    {
     test=x[j];
     index=j;
    }
 //  double testout = a1*pow(x[index],3) + b1*pow(x[index],2) +c1*x[index] + d1;
   if (x[index]<= windforcedVent)
        naturalVent=0.0;
   else
       naturalVent = pow((pow(x[index],2)-pow(windforcedVent,2)),0.5);
   *deltaTemper =  B * pow(naturalVent,2);
   *ventilationRate = x[index];
//      cout << testout << endl;
//      cout << " u " << windspeed << " deltaT " << *deltaTemper << " x " << x[index] << " Vf " << windforcedVent << " Vn " << naturalVent << endl;
  }
}


/****************************************************************************\
Get number of animals present
\****************************************************************************/

double stable::GetGetNrOfAnimals()
{
 double ret_val=0.0;
 for(int i=0;i<sections->NumOfNodes();i++)
 {
   animalSection * aSection=sections->ElementAtNumber(i);
   ret_val+=aSection->GetNrOfAnimals();
 }
 return ret_val;
}


double stable::CalcPropSensible()
{
	double ret_val=0.0;
   ret_val = 0.8 -0.38*pow(insideTemperature,2)/1000.0;
   ret_val *= 0.85;      //assumes wet surfaces so 0.85
   return ret_val;
}


/****************************************************************************\
	Operator << for output
\****************************************************************************/

 ostream& operator<< (ostream& os,stable* b)
{
  os << (building*) &b;   //first output the building specifics
  os << setiosflags(ios::left) << setw(40) << "Ventilation rate" << b->GetventilationRate() << endl;
  os << "animals" << endl;
  os << b->Livestock << endl;
  return os;
};


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
#ifdef  __BCplusplus__
   char buffer[MAX_TXT];
   getcwd(buffer, MAX_TXT);
	chdir(theOutputControl->GetoutputDirectory());
#endif
   string namestr="housing";
   char Indexstr[10];
   sprintf(Indexstr,"(%d)",Index);
	namestr=namestr+Indexstr+".xls";
	detailsFile = new fstream(namestr.c_str(),ios::out);
	ShowStable(detailsFile,true);
#ifdef  __BCplusplus__
	chdir(buffer);
#endif
}

void stable::CloseDetailsFile()
{
	detailsFile->close();
   delete detailsFile;
}

void stable::ShowStable(fstream *afile, bool header)
{
	if (header)
   {
      *detailsFile << "Day" << "\t"<< "HouseName" << "\t" << "ventilationRate" << "\t"<<  "insideTemperature" << "\t"
         << "CH4Emission" << "\t" << "CO2Emission" << "\t" << "volatAmmonia" << "\t" << "N2Emission" << "\t" << "N2OEmission" << "\t" ;
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
         << CH4Emission << "\t" << CO2Emission << "\t" << volatAmmonia << "\t" << N2Emission << "\t" << N2OEmission<< "\t" ;
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
