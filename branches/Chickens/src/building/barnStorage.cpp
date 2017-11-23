/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Author Lars Kroll
Date 22/5 1997
 Revision
 ----------------------------------------------------------------------------
      Log
 Major revision 8/99 Nick Hutchings

  * Describes a building that contains a product
  */

#include "../base/common.h"
#include "barnStorage.h"
#include "../base/IndicatorOutput.h"
#include "../products/plantItem.h"
#include "../products/feedItem.h"
#include "../products/liveMatter.h"
#include "../products/manure.h"
#include "../products/products.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
barnStorage::barnStorage(char *aName,const int aIndex,const base* aOwner)
	:building(aName,aIndex,aOwner)
{
	stores=new linkList<storage>;
	scalableBstore=true;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
barnStorage::~barnStorage()
{
	delete stores;
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
barnStorage::barnStorage(const barnStorage& s)
	:building(s)
{
   stores=GetStorageClone();
}


/**
 * read user define parameters from file
 */
void barnStorage::ReadParameters(fstream * file)
{
   Area=0.0;
   SetCritical();

 	building::ReadParameters(file);
   Setfile(file);
 	FindSection("Store",Index);
 	GetParameter("scalableBstore",&scalableBstore);

   Setfile(NULL);
   UnsetCritical();
}





/**
 * returns the amount of product p stored in this building
 */
double barnStorage::GetAvailableAmountOf(const product * p)
{
	int index;
   double AvailAmount=0.0;
   for (index=0;index<stores->NumOfNodes();index++)
 	{
   	storage * aStore=stores->ElementAtNumber(index);
	   if (aStore->ProductCompare(p))
	      AvailAmount=aStore->Getamountstored();
   }
	return AvailAmount;
}

/**
 * Adds amount in store to aProduct
 */
void barnStorage::GetAvailableFeedProduct(feedItem *aProduct)
{
	product *tempProduct;
	int index;
   for (index=0;index<stores->NumOfNodes();index++)
 	{
   	storage * aStore=stores->ElementAtNumber(index);
      tempProduct=aStore->GetStoredFeedProductPtr(aProduct->GetfeedCategory());
      if (tempProduct)
      	*aProduct + *tempProduct;
   }
}

/**
 * Adding amount product p to the store.
 * If there there is not enough room for it and barnStorage is scalable there will be made room for it
 * If barnStorage cannot bescale and there is not enough room Fasset will halt halt with this error: "barnStorage::SendToStore: not enoug room in store for product: product name
 */
int barnStorage::SendToStore(product * p)
{
	int rval;
   storage * aStore;
   int pindex=-1;
	int NrOfStores=stores->NumOfNodes();

   for(int i=0;i<NrOfStores;i++)
   {
    	aStore=stores->ElementAtNumber(i);
      if (!aStore->ProductCompare(p)==0)
      {
     	   pindex=i;
         aStore->AddToStore(p);
         double AmountStored = aStore->Getamountstored();
         if (aStore->GetCapacity() < AmountStored)
         {
        	 if (scalable)
        	 {
        	      aStore->SetCapacity(AmountStored);
        	 }
        	 else
        	 {
        		 theMessage->FatalError("barnStorage::SendToStore: not enoug room in store for product: ",p->GetName());
        	 }
         }
      }
   }
   if(pindex==-1) //then there aren't any products with that name in inventory - create one but warn
   {
      product* p2=p->clone();
      p2->Setamount(0.0);
      string aName =p2->GetName();
      storage * temp_store = new storage();
      temp_store->SetCapacity(0);                       //doesn't currently matter if overflows
      temp_store->SetProductPointer(p2);
      stores->InsertLast(temp_store);
//      theMessage->Warning("default creation of product store by B_storage2"); No longer relevant
      temp_store->AddToStore(p);
   }
   rval=pindex;
	return rval;
}

/**
 * GetStorageClone returns a pointer to a clone of this->stores.
 */

linkList<storage> *barnStorage::GetStorageClone()
{
	linkList<storage> *newlist;
   newlist=new linkList<storage>;
   for (int i=0;i<stores->NumOfNodes();i++)
   {
      storage *p = new storage(*(stores->ElementAtNumber(i)));
      newlist->InsertLast(p);
   }
	return newlist;
}

/**
 * 	DailyUpdate handles all "automated" daily activity
 */
void barnStorage::DailyUpdate()
{
	building::DailyUpdate();
   // sets an indicator
   GiveValueOfInventory();
}


/**
 * Remove RequestedAmount from barnStorage of product p.
 */
product * barnStorage::GetFromStore(double RequestedAmount,product * p)
{

	int NrOfStores=stores->NumOfNodes();
   product * productFound=p->clone();
   productFound->Setamount(0.0);
   product * tempProduct=NULL;
   for(int i=0;i<NrOfStores;i++)
   {
      storage* aStore=stores->ElementAtNumber(i);
      if (aStore->ProductCompare(p))
      {
         tempProduct=aStore->GetFromStore(RequestedAmount);
         if (tempProduct)
         	*productFound + * tempProduct;
         delete tempProduct;
         tempProduct=NULL;
      }
   }
   if (productFound->GetAmount()==0.0)
   {
   	delete productFound;
      productFound=NULL;
   }
   return productFound;
}

/**
 * Adding the value for everything stored to in indicat file.
 * Does also calculate N and C stored for plantItemObj and feedItemObj
 */
void barnStorage::GiveValueOfInventory()
{
	double result=0;
	double amount=0;
   string indicatorString = "";
	double price = 0;
   double N_content = 0.0, C_content = 0.0;

	for(int i=0;i<stores->NumOfNodes();i++)
   {
		storage *aStore=stores->ElementAtNumber(i);
		product *aProduct=aStore->GetProductPointer();
      amount = aProduct->GetAmount();
      if (amount > 0.0)
      {
         if ((aProduct->GetObjType()==plantItemObj)||(aProduct->GetObjType()==feedItemObj))
         {
            N_content += ((plantItem*) aProduct)->GetAllN().n*amount*1000.0;
            C_content += ((plantItem*) aProduct)->GetC_content()*amount*1000.0;
         }
         if ((string) aProduct->GetName()=="WNWHT")
            indicatorString = "06.01 W.wheat store";
         if ((string) aProduct->GetName()=="WNBRL")
            indicatorString = "06.02 W.barley store";
         if ((string) aProduct->GetName()=="SPBRL")
            indicatorString = "06.03 S.barley store";
         if ((string) aProduct->GetName()=="PEA")
            indicatorString = "06.04 Pea store";

         if (aProduct->GetaPrice()!=NULL)
         {
            if (aProduct->GetaPrice()->GetactualSellPrice()!=0)
               price = aProduct->GetaPrice()->GetactualSellPrice();
            else
               price = aProduct->GetaPrice()->GetactualBuyPrice();
            result += amount*price;
         }
      }
   }
#ifndef _STANDALONE
	theOutput->AddStateIndicator(economicIndicator,"03.05 Value of stored products","Dkr",result);
   theOutput->AddStateIndicator(environmentalIndicator,"35.62 N in stored plant products","kg N",N_content);
   theOutput->AddStateIndicator(environmentalIndicator,"45.62 C in stored plant products","kg C",C_content);
#endif
}



/**
 * calculate how much N that is left in seedObj, fertilizerObj and feedItemObj
 */
void barnStorage::EndBudget(bool show)
{
	double NRemaining=0;
	for (int index=0;index<stores->NumOfNodes();index++)
	{
   	storage * aStore=stores->ElementAtNumber(index);
   	switch(aStore->GetProductPointer()->GetObjType())
      case seedObj:
      case fertilizerObj:
      case feedItemObj:
      {
         if (show)
            cout << "Balance for " << GetName() << endl;
         liveMatter *p= (liveMatter*) aStore->GetProductPointer();
         NRemaining+= p->GetAmount()*p->GetAllN().n;
         break;
      }
   }
}
/**
 * Returns const product pointer from first store that contains the target product
 */
product * barnStorage::GetCopyStoredFeedProduct(const product * aProduct)
{
	product * aStoredProductPtr = NULL;
	for (int index=0;index<stores->NumOfNodes();index++)
	{
   	storage * aStore=stores->ElementAtNumber(index);
      if (!aStoredProductPtr)
         aStoredProductPtr = aStore->GetCopyStoredFeedProduct(aProduct);
   }
   return aStoredProductPtr;
}

/**
 * return a copy of what is stored if it is a feedItemObj, feedType and there is something stored.
 * Else it returns NULL
 */
product * barnStorage::GetCopyStoredFeedProduct(int type)
{
	product * aStoredProductPtr = NULL;
	for (int index=0;index<stores->NumOfNodes();index++)
	{
   	storage * aStore=stores->ElementAtNumber(index);
      if (!aStoredProductPtr)
         aStoredProductPtr = aStore->GetCopyStoredFeedProduct(type);
   }
   return aStoredProductPtr;
}

