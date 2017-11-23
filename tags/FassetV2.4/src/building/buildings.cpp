#include "../base/common.h"
#include "buildings.h"
#include "../base/controlParameters.h"
#include "../base/IndicatorOutput.h"

#include "../base/settings.h"
/****************************************************************************\
  Default Constructor
\****************************************************************************/
buildings::buildings()
:base()
{
	theStables = new linkList<stable>;
	theStores = new linkList<barnStorage>;
	theManurestores = new linkList<manurestore>;

}

/****************************************************************************\
  Destructor
\****************************************************************************/
buildings::~buildings()
{

	if (theStores) {delete theStores;}
	if (theManurestores) {delete theManurestores;}
	if (theStables) {delete theStables;}
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
buildings::buildings(char *aName, const int aIndex, const base * aOwner)
:base(aName,aIndex,aOwner)
{
	theStables = new linkList<stable>;
	theStores = new linkList<barnStorage>;
	theManurestores= new linkList<manurestore>;
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
buildings::buildings(const buildings& b)
:base(b)
{
	theStables = new linkList<stable>;
	theStores = new linkList<barnStorage>;
	theManurestores= new linkList<manurestore>;
}


// return 0 if the stable is unusable and 1 if it is usable
int IsFreeStable(stable *s)
{
	int rval=0;
	if(s->Livestock==nullptr)
		rval=1;
	return rval;
}

// return 1 if it is usable and otherwise
int IsFreePigStable(stable *s)
{
	int rval=0;
	if((s->Livestock==nullptr)&&(s->GetAlias()=="PigHousing"))
		rval=1;
	return rval;
}

/**
 *  Looks through animal housing to find an animal section that matches requirements
 */

animalSection * buildings::PlaceAnimal(const char * name)
{  //Note - this allows more than one animal type to be placed in a given section
	animalSection* aSection = nullptr;
	int numHouses = theStables->NumOfNodes();
	int i=0;
	while ((!aSection) && (i<numHouses))
	{
		stable *aStable=theStables->ElementAtNumber(i);
		if (!aSection)
			aSection=aStable->GetanimalSection(name);
		if (aSection)
		{
			aStable->SetinUse(true);    //this housing will possibly be used at some time
		}
		i++;
	}

	//stop program if cannot find an animal house for this animal
	if (!aSection)
	{
		theMessage->FatalError("buildings: animal section not found for ",name);
		return nullptr;  //this is here just to stop the compiler from generating a warning
	}
	else {return aSection;}
}

/**
 * 	inserting products in the storage facilities.
 */
void buildings::StoreProduct(product* p)
{
	// could be made faster by dropping out when a store is found
	for (int i=0;i<theStores->NumOfNodes();i++)
	{
		barnStorage* store=theStores->ElementAtNumber(i);
		store->SendToStore(p);
	}
}

/**
 * Returns the amount of the product in the storage facilities. A bit unelegant, in that manurestores
 *  aren't treated as true specializations of storage, but what the heck.
 */
double buildings::AmountProductStored(product* p)
{
	double amount = 0.0;

	linkList<barnStorage>::PS stb;
	theStores->PeekHead(stb);
	while (stb!=nullptr)
	{
		amount+=stb->element->GetAvailableAmountOf(p);
		theStores->OneStep(stb);
	}

	linkList<manurestore>::PS slb;
	theManurestores->PeekHead(slb);
	while (slb!=nullptr)
	{
		amount+=slb->element->GetAvailableAmountOf(p);
		theManurestores->OneStep(slb);
	}
	return amount;
}
/**
 * remove product p from buildings
 */
product * buildings::ExtractProduct(product* p)
{
	product * productFound = p->clone();
	product * tempProduct = nullptr;
	double requestedamount=p->GetAmount(),gottenamount=0;
	barnStorage* stbuilding;
	productFound->Setamount(0.0);                                      // JB did this, and NJH did it again
	if (p->GetObjType()!=manureObj)
	{
		linkList<barnStorage>::PS stb;
		theStores->PeekHead(stb);
		while ((stb!=nullptr)&&(gottenamount<requestedamount))
		{
			stbuilding=stb->element;
			tempProduct=stbuilding->GetFromStore(requestedamount-gottenamount,p);
			if (tempProduct)
			{
				*productFound + *tempProduct;
				gottenamount+= tempProduct->GetAmount();
				delete tempProduct;
				tempProduct = nullptr;
			}
			theStores->OneStep(stb);
		}
	}

	if (p->GetObjType()==manureObj)
	{
		manurestore* slbuilding;
		linkList<manurestore>::PS slb;
		theManurestores->PeekHead(slb);
		while ((slb!=nullptr)&&(gottenamount<requestedamount))
		{
			slbuilding=slb->element;
			tempProduct =slbuilding->GetFromManureStore(requestedamount-gottenamount, p, true);
			if (tempProduct)
			{
				if (p->GetmainUnitName()==3)
					gottenamount+=tempProduct->GetAmount() * (((manure *) tempProduct)->GetTotalN().n * 1000.0);
				else
					gottenamount+=tempProduct->GetAmount();
				* productFound + * tempProduct;
				delete tempProduct;
				tempProduct=nullptr;
			}
			theManurestores->OneStep(slb);
		}

		stable* aStable;
		linkList<stable>::PS s;
		theStables->PeekHead(s);

		while ((s!=nullptr)&&(gottenamount<requestedamount))
		{
			aStable=s->element;
			tempProduct=aStable->GetFromManureStore(requestedamount-gottenamount, p);
			if (tempProduct)
			{
				if (p->GetmainUnitName()==3)
					gottenamount+=tempProduct->GetAmount() * (((manure *) tempProduct)->GetTotalN().n * 1000.0);
				else
					gottenamount+=tempProduct->GetAmount();
				theOutput->AddIndicator(3580,"35.80 N transf. from stable to store","kg N",(((manure *)tempProduct)->GetAllN().n*tempProduct->GetAmount())*1000.0);
				theOutput->AddIndicator(4580,"45.80 C transf. from stable to store","kg C",(((manure *)tempProduct)->GetC_content()*tempProduct->GetAmount())*1000.0);
				*productFound + *tempProduct;
				delete tempProduct;
			}
			theStables->OneStep(s);
		}
	}
	if ((gottenamount<requestedamount) && (gottenamount>0))
		theMessage->Warning("building::not enough ",p->GetName()," stored, the rest bought");
	if (productFound->GetAmount()==0.0)
	{
		delete productFound;
		return nullptr;
	}
	else
		return productFound;
}


/**
 * DailyUpdate
 * Updates the stores, the stables and the manure stores
 */

void buildings::DailyUpdate()
{
	//iterate through theStables and theStores and DailyUpdate 'em
	for (int i=0;i<theStores->NumOfNodes();i++)
	{
		barnStorage *bs=theStores->ElementAtNumber(i);
		bs->DailyUpdate();
	}

	for (int i=0;i<theStables->NumOfNodes();i++)
	{
		stable *s=theStables->ElementAtNumber(i);
		if (s->GetinUse())
			s->DailyUpdate();
	}

	for (int i=0;i<theManurestores->NumOfNodes();i++)
	{
		manurestore *ss=theManurestores->ElementAtNumber(i);
		ss->DailyUpdate();
	}

   for (int i=0;i<theStables->NumOfNodes();i++)
   {
      stable *s=theStables->ElementAtNumber(i);
      s->CleanManure();
   }
   EndBudget();
}

/**
 * initialization of any buildings on the farm.
 * Includes crop storage, animal housing, manure storage
 */

void buildings::InitializeAll(string InputFilename, bool pigs, bool dairy, bool beef)
{
	commonData * runData=new commonData();

	runData->setCritical(false);
	if (runData->readFile(InputFilename))
	{
		commonData * common =new commonData();
		//* read default structure and variables from file
		common->readFile("StandardBuildings.dat");
		int large;
		int small;
		//* read manure storage details even if no animals are present - manure could be imported
		//* read details of any slurry storage
		runData->getSectionsNumbers("Slurrystore",&small,&large);
		for(int inx=small;inx<=large;inx++)
		{

			slurrystore *manureInst=new slurrystore("Slurrystore",inx,this,runData,common);
			manureInst->SetScalable(true);
			manureInst->SetIndex(inx);

			theManurestores->InsertLast(manureInst);
			if (globalSettings->DetailsData.getManureManageDetails()==true)
				manureInst->OpenDetailsFile();
		}
		//* read details of dung heaps
		runData->setCritical(false);
		runData->getSectionsNumbers("Dungheap",&small,&large);
		runData->setCritical(true);
		for(int inx=small;inx<=large;inx++)
		{
			dungheap *manureInst=new dungheap("Dungheap",inx,this,runData,common);
			manureInst->SetIndex(inx);
			manureInst->Setexternal(true);
			theManurestores->InsertLast(manureInst);
			if (globalSettings->DetailsData.getManureManageDetails()==true)
				manureInst->OpenDetailsFile();
		}
		//* only read details of manure treatment if animals are present
		if (pigs || dairy || beef)
		{
			runData->setCritical(false);
			runData->getSectionsNumbers("BiogasReactor",&small,&large);

			for(int inx=small;inx<=large;inx++)
			{
				theMessage->FatalError("buildings::InitializeAll - biogas not currently implemented");
				biogasReactor *manureInst=new biogasReactor("BiogasReactor",inx,this,runData,common);

				theManurestores->InsertLast(manureInst);
				if (globalSettings->DetailsData.getManureManageDetails()==true)
					manureInst->OpenDetailsFile();
			}


			runData->getSectionsNumbers("DigestateStore",&small,&large);
			for(int inx=small;inx<=large;inx++)
			{
				theMessage->FatalError("buildings::InitializeAll - digestatestore not currently implemented");
				runData->setCritical(true);
				runData->FindSection("DigestateStore",inx);
				digestateStore *manureInst=new digestateStore("DigestateStore",inx,this,runData,common);


				theManurestores->InsertLast(manureInst);
				if (globalSettings->DetailsData.getManureManageDetails()==true)
					manureInst->OpenDetailsFile();
			}
/*
			runData->setCritical(false);
			runData->getSectionsNumbers("Separator",&small,&large);

			for(int inx=small;inx<=large;inx++)
			{
				theMessage->FatalError("buildings::InitializeAll - biogas not currently implemented");
				separator *manureInst=new biogasReactor("BiogasReactor",inx,this,runData,common);

				theManurestores->InsertLast(manureInst);
			}
*/
		} // ----------

		// Build the product stores
		runData->getSectionsNumbers("Store",&small,&large);
		for(int inx=small;inx<=large;inx++)
		{
			barnStorage * storeInst = new barnStorage("Store",inx,this,runData,common);

			theStores->InsertLast(storeInst);
		}

		// And now the animal housing
		if (pigs)
		{
			runData->getSectionsNumbers("Pigstable",&small,&large);
			for(int inx=small;inx<=large;inx++)
			{
				//cout<<defaultpigstable->Livestock->theStock<<endl;;
				PigHousing *stableInstance=new PigHousing("Pigstable",inx,this,runData,common);


				theStables->InsertLast(stableInstance);
				if (globalSettings->DetailsData.getManureManageDetails()==true)
					stableInstance->OpenDetailsFile();
			}
		}
		if ((dairy)||(beef))
		{

			string input="CattleHousing";
			runData->getSectionsNumbers(input,&small,&large);
			for(int inx=small;inx<=large;inx++)
			{
//				cattlehouse *stableInstance=new cattlehouse(InstanceName,InstanceIndex,this,0);
				cattlehouse *stableInstance=new cattlehouse("CattleHousing",inx,this,runData,common);

				theStables->InsertLast(stableInstance);
				if (globalSettings->DetailsData.getManureManageDetails()==true)
					stableInstance->OpenDetailsFile();
			}

		}
		delete runData;
		delete common;


		/*      InstanceName="HardStanding";
      GetSectionNumbers(InstanceName,&first,&num);
      Setcur_pos(0);
      for(int inx=first;inx<(first+num);inx++)
      {
       B_storage2 * storeInst = new B_storage2(InstanceName,inx,this);
       storeInst->SetScalable(true);
       storeInst->ReadParameters(file);
       theStores->InsertLast(storeInst);
      }
		 */
		// Link manure storage with the animal housing
		for(int i=0;i<theManurestores->NumOfNodes();i++)
		{
			manurestore *sstore=theManurestores->ElementAtNumber(i);
			for(int i=0;i<theStables->NumOfNodes();i++)
			{
				stable* aStable=theStables->ElementAtNumber(i);
				aStable->LinkManureStores(sstore);
			}
		}
		// Check to see if any animal house stores which should be linked to external storage are not.
		for(int i=0;i<theStables->NumOfNodes();i++)
		{
			stable *aStable=theStables->ElementAtNumber(i);
			aStable->CheckLinks();
		}

	}
	else
	{
		string error="Building:: no buildings at all";
		theMessage->Warning(error.c_str());
	}

}



/****************************************************************************\
	GiveIndicators is called once pr. year
\****************************************************************************/
/*
void buildings::GiveIndicator(int indicatorType)
{
 	for(int i=0;i<theStores->NumOfNodes();i++)
	{
	   B_storage2* astore=theStores->ElementAtNumber(i);
	   astore->GiveIndicator(indicatorType);
   }
	for(int i=0;i<theManurestores->NumOfNodes();i++)
	{
	   manurestore* asltore=theManurestores->ElementAtNumber(i);
	   asltore->GiveIndicator(indicatorType);
   }
	for(int i=0;i<theStables->NumOfNodes();i++)
	{
	   stable* astable=theStables->ElementAtNumber(i);
	   astable->GiveIndicator(indicatorType);
   }
}
 */

/**
 * Check N budget of all stores etc and return the amount of N remaining
 */
void buildings::EndBudget(bool show)
{
	for(int i=0;i<theManurestores->NumOfNodes();i++)
	{
		theManurestores->ElementAtNumber(i)->EndBudget(show);
	}
	int NrOfStables=theStables->NumOfNodes();
	for(int i=0 ;i<NrOfStables;i++)
	{
		theStables->ElementAtNumber(i)->EndBudget(show);
	}
	int NrOfStores=theStores->NumOfNodes();
	for(int i=0 ;i<NrOfStores;i++)
	{
		theStores->ElementAtNumber(i)->EndBudget(show);
	}
}

/****************************************************************************\
	Get N input to all stores etc
\****************************************************************************/
double buildings::GetNInput()
{
	double NInput=0;
	for(int i=0;i<theManurestores->NumOfNodes();i++)
	{
		NInput+=theManurestores->ElementAtNumber(i)->GetNInput();
	}
	int NrOfStables=theStables->NumOfNodes();
	for(int i=0 ;i<NrOfStables;i++)
	{
		NInput+=theStables->ElementAtNumber(i)->GetNInput();
	}
	/*  int NrOfStores=theStores->NumOfNodes();
  not implemented for stores
  for(int i=0 ;i<NrOfStores;i++)
  {
   NInput+=theStores->ElementAtNumber(i)->GetNInput();
  }
	 */
	return NInput;
}

/****************************************************************************\
	Get N output from all stores etc
\****************************************************************************/
double buildings::GetNOutput()
{
	double NOutput=0;
	for(int i=0;i<theManurestores->NumOfNodes();i++)
	{
		NOutput+=theManurestores->ElementAtNumber(i)->GetNOutput();
	}
	int NrOfStables=theStables->NumOfNodes();
	for(int i=0 ;i<NrOfStables;i++)
	{
		NOutput+=theStables->ElementAtNumber(i)->GetNOutput();
	}
	/*  int NrOfStores=theStores->NumOfNodes();
  not implemented for stores
  for(int i=0 ;i<NrOfStores;i++)
  {
   NOutput+=theStores->ElementAtNumber(i)->GetNOutput();
  }
	 */
	return NOutput;
}
/**
 * returns a pointer to the produced stored. If it cannot be found nullptr will be returned
 */
product * buildings::GetCopyStoredFeedProduct(product* p)
{
	product * aProduct = nullptr;

	linkList<barnStorage>::PS stb;
	theStores->PeekHead(stb);
	while (stb!=nullptr)
	{
		aProduct=stb->element->GetCopyStoredFeedProduct(p);
		theStores->OneStep(stb);
	}

	return aProduct;
}


/**
 * ScaleBuildings is called by Initialise function in Event and scales animal housing
 *  and manure storage to animal numbers
 * Note that the scaling of animal housing feeds through to a scaling of the storage that receives manure from the housing
 */

void buildings::ScaleBuildings()
{
	if (theControlParameters->GetvolatModel()>0)
	{
		for(int i=0;i<theStables->NumOfNodes();i++)
		{
			stable* astable=theStables->ElementAtNumber(i);
			astable->ScaleSize();
		}
	}
}

/**
 * Get N in all manure storage (including animal housing)
 */
double buildings::GetTotalManureNStored()
{
	double NPresent=0;
	double temp1=0.0;
	double temp2=0.0;
	for(int i=0;i<theManurestores->NumOfNodes();i++)
	{
		temp1+=theManurestores->ElementAtNumber(i)->GetNInput();
		temp2+=theManurestores->ElementAtNumber(i)->GetNOutput();
	}
	NPresent=temp1-temp2;
	temp1=0.0;
	temp2=0.0;
	int NrOfStables=theStables->NumOfNodes();
	for(int i=0 ;i<NrOfStables;i++)
	{
		temp1+=theStables->ElementAtNumber(i)->GetNInput();
		temp2+=theStables->ElementAtNumber(i)->GetNOutput();
	}
	NPresent+=temp1-temp2;
	return NPresent;
}

/**
 * Get total capacity of slurry tanks
 */
double buildings::GetTotalSlurryCap()
{
	double capacity = 0.0;
	for(int i=0;i<theManurestores->NumOfNodes();i++)
	{
		manurestore * aStore=theManurestores->ElementAtNumber(i);
		if (!strcmp(aStore->GetName(),"Slurrystore"))
			capacity+=aStore->GetCapacity();
	}
	return capacity;
}

/**
 * Get N in all manure storage (including animal housing)
 */
double buildings::GetTotalSlurryStored()
{
	double amountPresent=0;
	for(int i=0;i<theManurestores->NumOfNodes();i++)
	{
		manurestore * aStore=theManurestores->ElementAtNumber(i);
		if (!strcmp(aStore->GetName(),"Slurrystore"))
			amountPresent+=aStore->GetAmountManure();
	}
	return amountPresent;
}




/**
 * Get the product if it is available
 */
void buildings::GetAvailableStoredFeed(feedItem *aProduct)
{
	int i=0;
	while (i<theStores->NumOfNodes())
	{
		barnStorage * aStore=theStores->ElementAtNumber(i);
		aStore->GetAvailableFeedProduct(aProduct);
		i++;
	}
};

/**
 * remove everything from the building
 */
product * buildings::EmptyManureStores()
{
	manure *aManure = new manure();
	manure *tempManure = nullptr;
	manurestore* slbuilding;
	linkList<manurestore>::PS slb;
	theManurestores->PeekHead(slb);

	while (slb!=nullptr)
	{
		slbuilding=slb->element;
		tempManure =(manure *)slbuilding->EmptyManureStore(true);
		if (tempManure)
		{
			* aManure + * tempManure;
			delete tempManure;
			tempManure=nullptr;
		}
		theManurestores->OneStep(slb);
	}

	stable* aStable;
	linkList<stable>::PS s;
	theStables->PeekHead(s);

	while (s!=nullptr)
	{
		aStable=s->element;
		tempManure=(manure *) aStable->EmptyManureFromStore();
		if (tempManure)
		{
			* aManure + * tempManure;
			delete tempManure;
		}
		theStables->OneStep(s);
	}
	return aManure;
}
