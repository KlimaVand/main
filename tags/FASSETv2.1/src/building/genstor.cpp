/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include <common.h>
#include <genstor.h>
// Why does storage.cpp need these two when B_storage.cpp does not??
#include <pplantit.h>
#include <pfeedit.h>

/***********************************************************************
Default constructor
************************************************************************/
storage::storage ()
  {
   theCapacity = 0;
   theProduct = NULL;
  }

/***********************************************************************
************************************************************************/
storage::  storage (const char* aname,const int aIndex,const base* aOwner,double aCapacity)
: base(aname,aIndex,aOwner)
  {
   theCapacity = aCapacity;
   theProduct = NULL;
  }

/***********************************************************************
************************************************************************/
storage::storage(storage& aStore)
{
   theCapacity = aStore.theCapacity;
   theProduct=aStore.theProduct->clone();
}

/***********************************************************************
************************************************************************/
void storage::ReadParameters(fstream * file)
{
   Setfile(file);
   SetCritical();
   FindSection(Name,Index); // perform this in the top of hierarchy - and only there
   GetParameter("RefCapacity",&theCapacity);
   Setfile(NULL);
}

/***********************************************************************
************************************************************************/
double storage::Getamountstored()
{
 return theProduct->GetAmount();
}

/***********************************************************************
************************************************************************/
bool storage::ProductCompare(const product* desiredProduct)
{
// returns a product if:
// a. the stored product name is the same as the desired product name
// b. the stored product trade name is the same as the desired product name
// c. the stored product trade name is the same as the desired product plant name
   if (((theProduct->GetObjType()==plantItemObj)||(theProduct->GetObjType()==feedItemObj)) && (desiredProduct->GetObjType()==feedItemObj))
   {
    if (((liveMatter*)theProduct)->GetCode() == ((liveMatter*)desiredProduct)->GetCode())
    {
//    cout << " storage::ProductCompare matching on code used for first time" << endl;
// cout << ((liveMatter*)theProduct)->GetCode();
    return true;
    }
    else
    	return false;
   }
   else
   {
      string desiredProductName = desiredProduct->GetName();
      string storedProductName=theProduct->GetName();
      string storedProductTradename=storedProductName;
      if ((theProduct->GetObjType()==plantItemObj) && ((((plantItem*) theProduct)->GetTradeName()).compare("NONE")!=0))
         storedProductTradename=((plantItem*) theProduct)->GetTradeName();
      string desiredProductPlantname= "NONE";
      if (desiredProduct->GetObjType()==feedItemObj)
         desiredProductPlantname=((feedItem*) desiredProduct)->GetPlantName();
      bool temp1=false,temp2=false,temp3=false;
      if (storedProductName.compare(desiredProductName)==0)
       temp1= true;
       if (storedProductTradename.compare(desiredProductName)==0)
       temp2= true;
      if (storedProductTradename.compare(desiredProductPlantname)==0)
          temp3= true;

      if (temp1 || temp2 || temp3)
      return true;
      else return false;
   }
}

/***********************************************************************
************************************************************************/
void storage::AddToStore(product * p)
{
 *theProduct=*theProduct+*p;
}

/***********************************************************************
************************************************************************/
product * storage::GetFromStore(double RequestedAmount,product * p)
{
  double inventoryAmount  = theProduct->GetAmount();
  double inventoryExtract = min(inventoryAmount,RequestedAmount);
  product * productFound = NULL;

	if (inventoryAmount>0.0)  // if nothing in storage then do not cast
  	{
	  theProduct->Setamount(inventoryExtract);
     productFound = theProduct->clone();
	  theProduct->Setamount(inventoryAmount-inventoryExtract);
//     cout << theProduct->GetName() << " inventoryAmount " << inventoryAmount << " inventoryExtract" << inventoryExtract << endl;
   }
  return productFound;
}

/***********************************************************************
************************************************************************/
string storage::GiveNameStoredProduct()
{
   return theProduct->GetName();
}

/***********************************************************************
************************************************************************/
ostream& operator<< (ostream& os,const storage& s)
{
   os << setiosflags(ios::left) << setw(40) << "Capacity " << s.GetCapacity() << endl;
   s.theProduct->showstats(&os);
	//inventory ?
   return os;
}

/***********************************************************************
Casts to appropriate type, with conversion if requesting feedItem but plantItem stored BUT the feedItem must have been initialised first!
************************************************************************/
product*  storage::GetCopyStoredFeedProduct(const product* product1)
{
  feedItem * aProductCopy = NULL;
  if (ProductCompare(product1))
  {
   if ((product1->GetObjType()==feedItemObj) && (theProduct->GetObjType()==plantItemObj))
   {
	   aProductCopy = new feedItem(*(feedItem *)product1);
      			theMessage->FatalError("all plant products should be stored as feedItems, so this should not happen");

//      aProductCopy->CopyFromPlantItem(*(liveMatter*) theProduct);  //overwrite variables common to feedItem and plantItem
      aProductCopy->Setamount(theProduct->GetAmount());
	}
   if ((theProduct->GetObjType()==feedItemObj)&&(theProduct->GetAmount()>0.0))
	   aProductCopy = new feedItem(*(feedItem*) theProduct);
   return aProductCopy;
  }
  else return NULL;
}

/***********************************************************************
************************************************************************/
product*  storage::GetCopyStoredFeedProduct(int feedType)
{
  feedItem * aProductCopy;
  if (theProduct->GetObjType()==feedItemObj)
   {
   	if ((((feedItem *)theProduct)->GetfeedCategory()==feedType)&&(theProduct->GetAmount()>0.0))
      {
         aProductCopy = new feedItem(*((feedItem *)theProduct));
         aProductCopy->Setamount(theProduct->GetAmount());
		   return aProductCopy;
      }
	   else return NULL;
	}
  else return NULL;
}

/***********************************************************************
************************************************************************/
product*  storage::GetStoredFeedProductPtr(int feedType)
{
  if (theProduct->GetObjType()==feedItemObj)
   {
   	if ((((feedItem *)theProduct)->GetfeedCategory()==feedType)&&(theProduct->GetAmount()>0.0))
      {
         return theProduct;
      }
     else
      return NULL;
	}
  else
  	return NULL;
}


