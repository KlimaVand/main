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
 Describes a building that contains a product
\****************************************************************************/

#include <common.h>
#include <b_stora2.h>
#include <indicat.h>
#include <pplantit.h>
#include <pfeedit.h>
#include <plivemat.h>
#include <pmanure.h>
#include <products.h>

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
B_storage2::B_storage2(char *aName,const int aIndex,const base* aOwner)
	:building(aName,aIndex,aOwner)
{
	stores=new linkList<storage>;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
B_storage2::~B_storage2()
{
	delete stores;
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
B_storage2::B_storage2(const B_storage2& s)
	:building(s)
{
   stores=GetStorageClone();
}


/****************************************************************************\
\****************************************************************************/
void B_storage2::ReadParameters(fstream * file)
{
   Area=0.0;
   SetCritical();
 	building::ReadParameters(file);
   Setfile(NULL);
   UnsetCritical();
}

/****************************************************************************\
\****************************************************************************/
double B_storage2::GetCapacity(const int storenum)
{
   storage * temp_store = Getstorage()->ElementAtNumber(storenum-1);
   return temp_store->GetCapacity();
}

/****************************************************************************\
\****************************************************************************/
linkList<storage> * B_storage2::Getstorage()
{
   return stores;
}

/****************************************************************************\
double GetAvailableAmountOf(product * p)
returns the amount of product p stored in this building
\****************************************************************************/
double B_storage2::GetAvailableAmountOf(const product * p)
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

/****************************************************************************\
Adds amount in store to aProduct
\****************************************************************************/
void B_storage2::GetAvailableFeedProduct(product *aProduct)
{
	product *tempProduct;
	int index;
   for (index=0;index<stores->NumOfNodes();index++)
 	{
   	storage * aStore=stores->ElementAtNumber(index);
      tempProduct=aStore->GetStoredFeedProductPtr(((feedItem *)aProduct)->GetfeedCategory());
      if (tempProduct)
      	*aProduct + *tempProduct;
   }
}

/****************************************************************************\
\****************************************************************************/
int B_storage2::SendToStore(product * p)
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
         if (scalable)
         {
       	   double AmountStored = aStore->Getamountstored();
            if (aStore->GetCapacity() < AmountStored)
        	      aStore->SetCapacity(AmountStored);
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

/****************************************************************************\
	GetStorageClone returns a pointer to a clone of
   this->stores.
\****************************************************************************/
linkList<storage> *B_storage2::GetStorageClone()
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

/****************************************************************************\
	DailyUpdate handles all "automated" daily activity
\****************************************************************************/
void B_storage2::DailyUpdate()
{
	building::DailyUpdate();
   // sets an indicator
   GiveValueOfInventory();
}

/****************************************************************************\
	CalcLP output for Linear model
\****************************************************************************/
void B_storage2::CalcLP()
{
	building::CalcLP();
}

/****************************************************************************\
\****************************************************************************/
product * B_storage2::GetFromStore(double RequestedAmount,product * p)
{
   double Extractedamount=0;
	int NrOfStores=stores->NumOfNodes();
   product * productFound=p->clone();
   productFound->Setamount(0.0);
   product * tempProduct=NULL;
   for(int i=0;i<NrOfStores;i++)
   {
    	storage* aStore=stores->ElementAtNumber(i);
      if (aStore->ProductCompare(p))
      {
         tempProduct=aStore->GetFromStore(RequestedAmount,p);
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

/****************************************************************************\
	GiveIndicators calculates the value of the stored products
\****************************************************************************/
void B_storage2::GiveValueOfInventory()
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

/****************************************************************************\
\****************************************************************************/
ostream& operator<< (ostream& os,B_storage2 s)
{
  os <<  (building*) &s << endl;
  for(int i=0;i<s.stores->NumOfNodes();i++)
  {
  	  storage * aStore=s.stores->ElementAtNumber(i);
     os << setiosflags(ios::left) << setw(40) << "Capacity " << aStore->GetCapacity() << endl;
  }
  return os;
}

/****************************************************************************\
\****************************************************************************/
void B_storage2::EndBudget(bool show)
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

/****************************************************************************\
Returns const product pointer from first store that contains the target product
\****************************************************************************/
product * B_storage2::GetCopyStoredFeedProduct(const product * aProduct)
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

/****************************************************************************\
\****************************************************************************/
product * B_storage2::GetCopyStoredFeedProduct(int type)
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

/****************************************************************************\
\****************************************************************************/
void B_storage2::GiveIndicator(int indicatorType)
{
   double N_content = 0.0, C_content = 0.0;
   double totalAmount = 0.0;
   for(int i=0;i<stores->NumOfNodes();i++)
   {
      storage *aStore=stores->ElementAtNumber(i);
      product *aProduct=aStore->GetProductPointer();
      double amount = aProduct->GetAmount();
      if (amount > 0.0)
      {
         if ((aProduct->GetObjType()==plantItemObj)||(aProduct->GetObjType()==feedItemObj))
         {
            N_content += ((plantItem*) aProduct)->GetAllN().n*amount*1000.0;
            C_content += ((plantItem*) aProduct)->GetC_content()*amount*1000.0;
	      }
      }
      totalAmount+=aStore->GetCapacity();
   }
#ifndef _STANDALONE
	if (indicatorType==environmentalIndicator)
   {
      theOutput->AddStateIndicator(environmentalIndicator,"35.62 N in stored plant products","kg N",N_content);
	   theOutput->AddStateIndicator(environmentalIndicator,"45.62 C in stored plant products","kg C",C_content);
   }
#endif
	if (indicatorType==economicIndicator)
   {
      if (scalable)
         SetActualCapacity(totalAmount);
      building::GiveIndicator(indicatorType);  //must come last so that scaling is effective (if relevant)
   }
}

