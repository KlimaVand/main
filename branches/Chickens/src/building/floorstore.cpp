/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/*
 * Created 8/99 by Nick Hutchings
 * Describes the floor of an animal house, mainly in relation to the accumulation of dung and urine
 * Differs from manurestore in that several manure types can be stored on a floor
 * (essential to represent systems where there is separation of liquid and solids)
 */
#include "../base/common.h"
#include "floorstore.h"
#include "dungheap.h"
#include "instore.h"
#include "../base/controlParameters.h"
#ifdef _STANDALONE
	#include "../base/OutputControl.h"
	#include "../base/message.h"
#endif
	#include "../base/IndicatorOutput.h"

/**
 * Default Constructor
 */

floorstore::floorstore(): base()
{
   excretaPartition=0;
   ventilated=false;
   theStores = new linkList<instore>;
   volumeToArea		= 0.0;
   NH3Emissionfactor = 0.0;
}

/**
  Destructor
 */
floorstore::~floorstore()
{
 delete theStores;
}

/**
  Constructor with arguments
 */
floorstore::floorstore(char *aName,const int aIndex,const base* aOwner):
     base(aName,aIndex,aOwner)
{
   excretaPartition=0;
   area = 0;
   cleaningInterval = 0;
   timeUncleaned = 0;
   theStores = new linkList<instore>;
   Nbudget.SetNames("floor","N");
   Cbudget.SetNames("floor","C");
   volumeToArea		= 0.0;
   NH3Emissionfactor = 0.0;
}

/**
  Copy Constructor
 */
floorstore::floorstore(floorstore& s)
{
   theStores = s.theStores;
   Nbudget = s.Nbudget;
   Cbudget = s.Cbudget;
}

/**
	Init reads the relevant values from its owners file
 */

void floorstore::ReadParameters(fstream * file)
{
	Setfile(file);
	FindSection(GetName(),GetIndex());
	GetParameter("Area",&area);
	GetParameter("CleaningInterval",&cleaningInterval);
	GetParameter("Manuretype",&manuretype);
	switch (theControlParameters->GetvolatModel())
	{
	case 0:
		GetParameter("NH3Emissionfactor",&NH3Emissionfactor);
		break;
	case 1:
		GetParameter("AreaPrAnimal",&AreaPrAnimal);
		GetParameter("ProportionFouled",&proportionFouled);
	}
	double postCleanDepth=0.0;
	GetParameter("PostCleanDepth",&postCleanDepth);
	GetParameter("WashingWater",&WashingWater);
	GetParameter("StrawAdded",&StrawAdded);

	//   if (manuretype=="SLURRY")	// only need one store as will be mixed in slurry store
	{
		instore *aStore = new instore(GetName(),0,this);
		aStore ->SetpostCleanDepth(postCleanDepth);
		aStore->ReadParameters(file);
		theStores->InsertLast(aStore);
	}
	if (manuretype=="SEPARATED")	// need separate as liquid and solid stored separately
	{

		theMessage->FatalError("FloorStore: attempt to use Separated manure system - is not supported");
	}
	if ((manuretype=="SOLID-MANURE")||(manuretype=="FYM"))
	{
		instore *aStore = new instore(GetName(),0,this);
		string isVentilated;
		GetParameter("Ventilated",&isVentilated);
		if (isVentilated=="TRUE") ventilated = true;
		else ventilated = false;
		aStore->ReadParameters(file);
		theStores->InsertLast(aStore);
	}
	GetParameter("ExcretaPartition",&excretaPartition);
	Setfile(NULL);
}

/**
	Dailyactions
 */

void floorstore::DailyUpdate()
{
//	instore::DailyUpdate();
}

/**
Volatize from the floorstore
 */

void floorstore::Volatize(double airVelocity, double humidity, double temperature,
			double *volatAmmonia, double *waterEvap, double *N2Emission, double *N2OEmission, double *CH4Emission,  double *CO2Emission)
{
   double tempvolatAmmonia=0.0, tempwaterEvap=0.0, tempN2Emission=0.0, tempN2OEmission=0.0, tempCH4Emission=0.0, tempCO2Emission=0.0;
	for(int i=0;i<theStores->NumOfNodes();i++)
	{
		manure * aManure=(manure *) GetaStore(i)->GetManure();
 // handle emission from different manures appropriately
  		{
         instore * ainstore=(instore *)theStores->ElementAtNumber(i);
         double areaSoiled = aManure->GetAmount()/ainstore->GetpostCleanDepth();
         if (areaSoiled>area*proportionFouled)
         	areaSoiled = area*proportionFouled;
         double duration = 24.0;//duration in hours
         if (areaSoiled>0.0)
  				ainstore->floorVolatize(airVelocity, humidity, temperature, areaSoiled, duration,
  	 					&tempvolatAmmonia, &tempwaterEvap, &tempN2Emission, &tempN2OEmission, &tempCH4Emission, &tempCO2Emission);
         *volatAmmonia+=tempvolatAmmonia;
         *waterEvap+= tempwaterEvap;
         *N2Emission+=tempN2Emission;
         *N2OEmission+=tempN2OEmission;
         *CH4Emission+=tempCH4Emission;
         *CO2Emission+=tempCO2Emission;
  		}
	}
	Nbudget.AddOutput(*volatAmmonia + *N2Emission + *N2OEmission);
	Cbudget.AddOutput(*CH4Emission+ *CO2Emission);
}

/**
Volatize ammonia from the floorstore - static model
 */

void floorstore::Volatize(double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CH4Emission,
		double *CO2Emission)
{
   double tempvolatAmmonia=0.0, tempN2Emission=0.0, tempN2OEmission=0.0, tempCH4Emission=0.0, tempCO2Emission=0.0;
	for(int i=0;i<theStores->NumOfNodes();i++)
	{
     instore * ainstore=(instore *)theStores->ElementAtNumber(i);
     ainstore->Volatize(&tempvolatAmmonia, &tempN2Emission, &tempN2OEmission, &tempCH4Emission, &tempCO2Emission);
     *volatAmmonia+=tempvolatAmmonia;
     *N2Emission+=tempN2Emission;
     *N2OEmission+=tempN2OEmission;
     *CH4Emission+= tempCH4Emission;
     *CO2Emission+=tempCO2Emission;
   }
}




/**
 Receive manure from animals
 */

void floorstore::RcvManure(manure *aManure)
{
	Nbudget.AddInput(aManure->GetAmount()*aManure->GetAllN().n);
	Cbudget.AddInput(aManure->GetAmount()*aManure->GetC_content());
   if (manuretype=="SEPARATED")
   {
	   theMessage->FatalError("floorstore::RcvManure: SEPARATED not supported. Talk to nick");
    for(int i=0;i<theStores->NumOfNodes();i++)
     {
      instore * ainstore=(instore *) theStores->ElementAtNumber(i);
//note - the manure store sorts out if it is permitted to receive the relevant manure type
		ainstore->RcvManure(aManure);
     }
   }
   else
   {
	  	GetaStore(0)->RcvManure(aManure);
	   instore * ainstore=(instore *) theStores->ElementAtNumber(0);
	   if (!ainstore->GetnextStore())//must be solid manure stored on floor - record amount produced here
			theOutput->AddIndicator(economicIndicator,"19.73 Solid manure produced","t",aManure->GetAmount());
   }
}


/**
 Receive bedding
 */

void floorstore::RcvBedding(plantItem *aBeddingMaterial)
{
	Nbudget.AddInput(aBeddingMaterial->GetAmount()*aBeddingMaterial->GetAllN().n);
	Cbudget.AddInput(aBeddingMaterial->GetAmount()*aBeddingMaterial->GetC_content());
   //save old name before changing to name of manure
  	char oldName[40];
   strcpy(oldName,aBeddingMaterial->GetName());
   if (manuretype=="SEPARATED")
   {
		aBeddingMaterial->Setname("SOLID-MANURE");
      for(int i=0;i<theStores->NumOfNodes();i++)
      {
      	instore * ainstore=(instore *) theStores->ElementAtNumber(i);
   //note - the manure store sorts out if it is permitted to receive the relevant manure type
         ainstore->ReceiveBedding(aBeddingMaterial);
   	}
   }
	else
   {
      aBeddingMaterial->Setname(manuretype);  //change name and get info, otherwise manure is rejected by storage
      GetaStore(0)->ReceiveBedding(aBeddingMaterial);
   }
 aBeddingMaterial->Setname(oldName);  //change name back or funny things could happen to budgetting later!!
}


/**
Clean the floor of all manure - manure sent to the relevant store
 */
manure * floorstore::CleanFloor(double numberOfAnimals)
{
 manure *aManure = new manure();
 for(int i=0;i<theStores->NumOfNodes();i++)
  {
   instore * ainstore=(instore *) theStores->ElementAtNumber(i);
   manure *tmpManure=(manure*) ainstore->GetManure()->clone();
   tmpManure->Setamount(0.0);
   manure *water=new manure("WATER",0,NULL);   //is this deleted later??
   water->SetmainUnitName(t); //otherwise product the later addition to manure will get upset over the units
   water->SetpH(aManure->GetpH()); //water does not affect pH
   water->Setamount(WashingWater*numberOfAnimals/1000.0);
   RcvManure(water);
   delete water;
   if (ainstore->GetnextStore())
   {
     	ainstore->CleanStore(tmpManure);
     	Nbudget.AddOutput(tmpManure->GetAmount()*tmpManure->GetAllN().n);
     	Cbudget.AddOutput(tmpManure->GetAmount()*tmpManure->GetC_content());
     	if (ainstore->GetManureToName()!="inhouse") // only add to manure object if manure is exported from housing
	    	*aManure + *tmpManure;
   }
   else //must be solid manure stored on floor - record amount water added here
			theOutput->AddIndicator(economicIndicator,"19.73 Solid manure produced","t",water->GetAmount());

   delete tmpManure;
  }
  if (aManure->GetAmount()==0.0)
  {
  		delete aManure;
   return NULL;
  }
  else
	  return aManure;
}

/**
Return total amount of N in store
 */

double floorstore::GetTotalNStored()
{
 double CurrentAmount=0;
 for(int i=0;i<theStores->NumOfNodes();i++)
  {
   instore * ainstore=(instore *) theStores->ElementAtNumber(i);
	CurrentAmount+=ainstore->GetTotalNStored();
  }
 return CurrentAmount;
}

/**
Return total amount of C in store
 */

double floorstore::GetTotalCStored()
{
 double CurrentAmount=0;
 for(int i=0;i<theStores->NumOfNodes();i++)
  {
   instore * ainstore=(instore *) theStores->ElementAtNumber(i);
	CurrentAmount+=ainstore->GetTotalCStored();
  }
 return CurrentAmount;
}

/**
Return total amount of DM in store
 */

double floorstore::GetTotalDMStored()
{
 double CurrentAmount=0;
 for(int i=0;i<theStores->NumOfNodes();i++)
  {
   instore * ainstore=(instore *) theStores->ElementAtNumber(i);
	CurrentAmount+=ainstore->GetTotalDMStored();
  }
 return CurrentAmount;
}

/**
Get total amount of manure in store
 */

double floorstore::GetManureAmount()
{
 double CurrentAmount=0;
 for(int i=0;i<theStores->NumOfNodes();i++)
  {
   instore * ainstore=(instore *) theStores->ElementAtNumber(i);
	CurrentAmount+=ainstore->GetAmountManure();
  }
 return CurrentAmount;
}


/**
Get some manure from store
 */

product * floorstore::GetFromManureStore(double RequestedAmount, product * p, bool checkAccess)
{
 product * aManure= NULL;
 product* manureFound = new product();
 for(int i=0;i<theStores->NumOfNodes();i++)
  {
   instore * ainstore=(instore *) theStores->ElementAtNumber(i);
	aManure=ainstore->GetFromManureStore(RequestedAmount, p, checkAccess);
   if (aManure)
   {
   	*manureFound + *aManure;
      delete aManure;
      aManure=NULL;
   }
  }
 if (manureFound->GetAmount()==0.0)
 {
 	delete manureFound;
   manureFound=NULL;
 }
 return manureFound;
}

/**
Check if all N is accounted for
 */

void floorstore::EndBudget(bool show)
{
 if (show) cout << "Balance for " << GetName() << endl;
//N budget
	double NRemaining=GetTotalNStored();
	if (!Nbudget.Balance(NRemaining))
   {
      if (show)
      {
         cout << Nbudget;
         cout << "NRemaining " << NRemaining << endl;
      }
   }
//C budget
	double CRemaining=GetTotalCStored();
	if (!Cbudget.Balance(CRemaining))
   {
	   if (show)
      {
   		cout << Cbudget;
         cout << "CRemaining " << CRemaining << endl;
      }
   }
 for(int i=0;i<theStores->NumOfNodes();i++)
  {
   instore * ainstore=(instore *) theStores->ElementAtNumber(i);
   ainstore->EndBudget(show);
  }
}


/**
	Scale floor to number of animals present
 */
void floorstore::ScaleSize(double numberOfAnimals, double annualManure)
{
 if (AreaPrAnimal==0.0)
               theMessage->FatalError("floorstore:: AreaPrAnimal should not be zero here!");
 area = numberOfAnimals * AreaPrAnimal;
 for(int i=0;i<theStores->NumOfNodes();i++)
 {
   manurestore * aStore=theStores->ElementAtNumber(i);
   aStore->SetsurfaceArea(area);
   aStore->SetCapacity(area * aStore->Getdepth());
   if (aStore->GetnextStore())
   	((instore *) aStore->GetnextStore())->ScaleInstore(annualManure);
 }
}


/**
	Operator << for output
 */

ostream& operator<< (ostream& os,const floorstore& s)
{
 os << setiosflags(ios::left) << setw(40) << "Floor details for " << s.GetName() << endl;
 os << setiosflags(ios::left) << setw(40) << "excreta partition " << s.excretaPartition << endl;
 os << setiosflags(ios::left) << setw(40) << "manure type " << s.manuretype << endl;
 os << setiosflags(ios::left) << setw(40) << "floor area " << s.area << endl;
 os << setiosflags(ios::left) << setw(40) << "cleaning interval " << s.cleaningInterval << endl;
  for(int i=0;i<s.theStores->NumOfNodes();i++)
  {
   instore * ainstore=(instore *) s.theStores->ElementAtNumber(i);
   os << *ainstore;
  }
  return os;
}

void floorstore::ReadDefaultParameters(fstream *file)
{
	SetCritical();
	Setfile(file);
	FindSection(GetName(),GetIndex());
	GetParameter("Area",&area);
	GetParameter("CleaningInterval",&cleaningInterval);
	GetParameter("Manuretype",&manuretype);
	switch (theControlParameters->GetvolatModel())
	{
	case 0:
		GetParameter("NH3Emissionfactor",&NH3Emissionfactor);
		break;
	case 1:
		GetParameter("AreaPrAnimal",&AreaPrAnimal);
		GetParameter("ProportionFouled",&proportionFouled);
	}
	double postCleanDepth=0.0;
	GetParameter("PostCleanDepth",&postCleanDepth);
	GetParameter("WashingWater",&WashingWater);
	GetParameter("StrawAdded",&StrawAdded);

	//   if (manuretype=="SLURRY")	// only need one store as will be mixed in slurry store
	{
		instore *aStore = new instore(GetName(),0,this);
		aStore ->SetpostCleanDepth(postCleanDepth);
		aStore->ReadParameters(file);
		theStores->InsertLast(aStore);
	}
	if (manuretype=="SEPARATED")	// need separate as liquid and solid stored separately
	{

		theMessage->FatalError("FloorStore: attempt to use Separated manure system - is not supported");
	}
	if ((manuretype=="SOLID-MANURE")||(manuretype=="FYM"))
	{
		instore *aStore = new instore(GetName(),0,this);
		string isVentilated;
		GetParameter("Ventilated",&isVentilated);
		if (isVentilated=="TRUE") ventilated = true;
		else ventilated = false;
		aStore->ReadParameters(file);
		theStores->InsertLast(aStore);
	}
	GetParameter("ExcretaPartition",&excretaPartition);
	Setfile(NULL);
}

string floorstore::getmanureName()
{
	if (!theStores->NumOfNodes()==1)
		theMessage->FatalError("floorstore::getmanureName - more than one manure type stored");
	else
		return theStores->ElementAtNumber(0)->getManureName();

}
