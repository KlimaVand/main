/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
  Archive
   Author NJH
     Date jan 2000
 Revision
 ----------------------------------------------------------------------------
      Log
\****************************************************************************/
#include "../base/common.h"
#include "thebuild.h"
#include "../base/controlParameters.h"
#include "../base/IndicatorOutput.h"

#include "../data/outputData.h"
/****************************************************************************\
  Default Constructor
\****************************************************************************/
buildings::buildings()
:base()
{
	theStables = new linkList<stable>;
	theStores = new linkList<barnStorage>;
	theManurestores = new linkList<manurestore>;
   defaultpigstable=NULL;
   defaultcattlehouse=NULL;
   defaultslurrystore=NULL;
   defaultbiogasReactor=NULL;
   defaultdigestateStore=NULL;
   defaultdungheap=NULL;
   defaultbarnStorage=NULL;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
buildings::~buildings()
{
	delete file;
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
	if(s->Livestock==NULL)
		rval=1;
	return rval;
}

// return 1 if it is usable and otherwise
int IsFreePigStable(stable *s)
{
	int rval=0;
	if((s->Livestock==NULL)&&(s->GetAlias()=="Pigstable"))
		rval=1;
	return rval;
}

/**
 *  Looks through animal housing to find an animal section that matches requirements
 */

animalSection * buildings::PlaceAnimal(const char * name)
{  //Note - this allows more than one animal type to be placed in a given section
	animalSection* aSection = NULL;
	int numHouses = theStables->NumOfNodes();
	int i=0;
	while ((!aSection) && (i<numHouses))
	{
		stable *aStable=theStables->ElementAtNumber(i);
		if (!aSection)
			aSection=aStable->GetanimalSection(name);
		if (aSection)
			aStable->SetinUse(true);    //this housing will possibly be used at some time
		i++;
	}

	//stop program if cannot find an animal house for this animal
	if (!aSection)
	{
		theMessage->FatalError("buildings: animal section not found for ",name);
		return NULL;  //this is here just to stop the compiler from generating a warning
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
	while (stb!=NULL)
	{
		amount+=stb->element->GetAvailableAmountOf(p);
		theStores->OneStep(stb);
	}

	linkList<manurestore>::PS slb;
	theManurestores->PeekHead(slb);
	while (slb!=NULL)
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
	product * tempProduct = NULL;
	double requestedamount=p->GetAmount(),gottenamount=0;
	barnStorage* stbuilding;
	productFound->Setamount(0.0);                                      // JB did this, and NJH did it again
	if (p->GetObjType()!=manureObj)
	{
		linkList<barnStorage>::PS stb;
		theStores->PeekHead(stb);
		while ((stb!=NULL)&&(gottenamount<requestedamount))
		{
			stbuilding=stb->element;
			tempProduct=stbuilding->GetFromStore(requestedamount-gottenamount,p);
			if (tempProduct)
			{
				*productFound + *tempProduct;
				gottenamount+= tempProduct->GetAmount();
				delete tempProduct;
				tempProduct = NULL;
			}
			theStores->OneStep(stb);
		}
	}

	if (p->GetObjType()==manureObj)
	{
		manurestore* slbuilding;
		linkList<manurestore>::PS slb;
		theManurestores->PeekHead(slb);
		while ((slb!=NULL)&&(gottenamount<requestedamount))
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
				tempProduct=NULL;
			}
			theManurestores->OneStep(slb);
		}

		stable* aStable;
		linkList<stable>::PS s;
		theStables->PeekHead(s);

		while ((s!=NULL)&&(gottenamount<requestedamount))
		{
			aStable=s->element;
			tempProduct=aStable->GetFromManureStore(requestedamount-gottenamount, p);
			if (tempProduct)
			{
				if (p->GetmainUnitName()==3)
					gottenamount+=tempProduct->GetAmount() * (((manure *) tempProduct)->GetTotalN().n * 1000.0);
				else
					gottenamount+=tempProduct->GetAmount();
				theOutput->AddIndicator(environmentalIndicator,"35.80 N transf. from stable to store","kg N",(((manure *)tempProduct)->GetAllN().n*tempProduct->GetAmount())*1000.0);
				theOutput->AddIndicator(environmentalIndicator,"45.80 C transf. from stable to store","kg C",(((manure *)tempProduct)->GetC_content()*tempProduct->GetAmount())*1000.0);
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
		return NULL;
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

	/*   for (int i=0;i<theStables->NumOfNodes();i++)
   {
      stable *s=theStables->ElementAtNumber(i);
      s->CleanManure();
   } */
}

/**
 * initialization of user defines, that is stored in  InputFilename
 * Cattlestable is only Initialize if dairy or beef is true.
 * Slurrystore is only Initialize if there is anymal on the farm
 *
 */

void buildings::InitializeAll(string InputFilename, bool pigs, bool dairy, bool beef)
{

	if (OpenInputFileString(InputFilename))
	{
		Setcur_pos(0);
		SetCritical(); // Must find parameters now - undo with UnsetCritical()
		char * InstanceName=new char[200];
		strcpy(InstanceName,"                 ");
		int first,num;
		if (pigs || dairy || beef)
		{ // ---------------
			UnsetCritical();
			//First build the manure stores - currently only slurry
			strcpy(InstanceName,"Slurrystore");
			GetSectionNumbers(InstanceName,&first,&num);
			if (num>0)
			{
				defaultslurrystore = new slurrystore(InstanceName,0,this);
				defaultslurrystore->ReadDefaultParameters();
			}
			Setcur_pos(0);
			for(int inx=first;inx<(first+num);inx++)
			{
				SetCritical();
				FindSection(InstanceName,inx);
				slurrystore *manureInst=new slurrystore(*defaultslurrystore);
//				slurrystore *manureInst=new slurrystore(InstanceName,inx,this);
				manureInst->SetScalable(true);
				manureInst->ReadParameters(file);
				theManurestores->InsertLast(manureInst);
				if (theOutputData->DetailsData.getManureManageDetails()==true)
					manureInst->OpenDetailsFile();
			}
			UnsetCritical();
			// Dungheap not tested

			strcpy(InstanceName,"Dungheap");
			GetSectionNumbers(InstanceName,&first,&num);
			Setcur_pos(0);
			for(int inx=first;inx<(first+num);inx++)
			{
				SetCritical();
				FindSection(InstanceName,inx);
				dungheap *manureInst=new dungheap(InstanceName,inx,this);
				manureInst->SetScalable(true);
				manureInst->ReadParameters(file);
				manureInst->Setexternal(true);
				theManurestores->InsertLast(manureInst);
				if (theOutputData->DetailsData.getManureManageDetails()==true)
					manureInst->OpenDetailsFile();
			}
			UnsetCritical();

			strcpy(InstanceName,"BiogasReactor");
			GetSectionNumbers(InstanceName,&first,&num);
			Setcur_pos(0);
			for(int inx=first;inx<(first+num);inx++)
			{
				SetCritical();
				FindSection(InstanceName,inx);
				biogasReactor *manureInst=new biogasReactor(InstanceName,inx,this);
				manureInst->SetScalable(true);
				manureInst->ReadParameters(file);
				theManurestores->InsertLast(manureInst);
				if (theOutputData->DetailsData.getManureManageDetails()==true)
					manureInst->OpenDetailsFile();
			}
			UnsetCritical();

			strcpy(InstanceName,"DigestateStore");
			GetSectionNumbers(InstanceName,&first,&num);
			Setcur_pos(0);
			for(int inx=first;inx<(first+num);inx++)
			{
				SetCritical();
				FindSection(InstanceName,inx);
				digestateStore *manureInst=new digestateStore(InstanceName,inx,this);
				manureInst->SetScalable(true);
				manureInst->ReadParameters(file);
				theManurestores->InsertLast(manureInst);
				if (theOutputData->DetailsData.getManureManageDetails()==true)
					manureInst->OpenDetailsFile();
			}
		} // ----------
		// Build the product stores

		strcpy(InstanceName,"Store");
		GetSectionNumbers(InstanceName,&first,&num);
		Setcur_pos(0);
		for(int inx=first;inx<(first+num);inx++)
		{
			barnStorage * storeInst = new barnStorage(InstanceName,inx,this);
			storeInst->SetScalable(true);
			storeInst->ReadParameters(file);
			theStores->InsertLast(storeInst);
		}

		// And now the animal housing
		if (pigs)
		{

			strcpy(InstanceName,"Pigstable");
			GetSectionNumbers(InstanceName,&first,&num);
			Setcur_pos(0);
			defaultpigstable = new pigstable();
			defaultpigstable->ReadDefaultParameters();

			for(int InstanceIndex=first;InstanceIndex<(first+num);InstanceIndex++)
			{
				pigstable *stableInstance=new pigstable(*defaultpigstable);
//				pigstable *stableInstance=new pigstable(InstanceName,InstanceIndex,this,NULL);
				stableInstance->ReadParameters(file);
				theStables->InsertLast(stableInstance);
				if (theOutputData->DetailsData.getManureManageDetails()==true)
					stableInstance->OpenDetailsFile();
			}
		}
		if ((dairy)||(beef))
		{

			strcpy(InstanceName,"Cattlestable");
			GetSectionNumbers(InstanceName,&first,&num);
			Setcur_pos(0);
			for(int InstanceIndex=first;InstanceIndex<(first+num);InstanceIndex++)
			{
				cattlehouse *stableInstance=new cattlehouse(InstanceName,InstanceIndex,this,0);
				stableInstance->ReadParameters(file);
				theStables->InsertLast(stableInstance);
				if (theOutputData->DetailsData.getManureManageDetails()==true)
					stableInstance->OpenDetailsFile();
			}
		}

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
		CloseInputFile();
		delete file;
		delete [] InstanceName;
		Setfile(NULL);
	}
	else
		theMessage->FatalError("Building:: error in opening building.dat");

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
 * returns a pointer to the produced stored. If it cannot be found NULL will be returned
 */
product * buildings::GetCopyStoredFeedProduct(product* p)
{
	product * aProduct = NULL;

	linkList<barnStorage>::PS stb;
	theStores->PeekHead(stb);
	while (stb!=NULL)
	{
		aProduct=stb->element->GetCopyStoredFeedProduct(p);
		theStores->OneStep(stb);
	}

	return aProduct;
}


/**
 * ScaleBuildings is called by Initialise function in Event and scales animal housing
 *  and manure storage to animal numbers
 */

void buildings::ScaleBuildings()
{
	if (theControlParameters->GetvolatModel()>0)
	{
		for(int i=0;i<theManurestores->NumOfNodes();i++)
		{
			manurestore* asltore=theManurestores->ElementAtNumber(i);
			asltore->SetsurfaceArea(0.0);
			asltore->SetCapacity(0.0);
		}
		for(int i=0;i<theStables->NumOfNodes();i++)
		{
			stable* astable=theStables->ElementAtNumber(i);
			astable->ScaleSize();
		}
		for (int i=0;i<theStores->NumOfNodes();i++)
		{
			barnStorage* store=theStores->ElementAtNumber(i);
			store->SetScalable(true);
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
	manure *tempManure = NULL;
	manurestore* slbuilding;
	linkList<manurestore>::PS slb;
	theManurestores->PeekHead(slb);

	while (slb!=NULL)
	{
		slbuilding=slb->element;
		tempManure =(manure *)slbuilding->EmptyManureStore();
		if (tempManure)
		{
			* aManure + * tempManure;
			delete tempManure;
			tempManure=NULL;
		}
		theManurestores->OneStep(slb);
	}

	stable* aStable;
	linkList<stable>::PS s;
	theStables->PeekHead(s);

	while (s!=NULL)
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

