/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
 \****************************************************************************/
#include "../base/common.h"
#include "../building/storage.h"
// Why does storage.cpp need these two when B_storage.cpp does not??
#include "../products/plantItem.h"
#include "../products/feedItem.h"
#include "../base/commonData.h"
/***********************************************************************
 Default constructor
 ************************************************************************/
storage::storage()
{

}

/***********************************************************************
 ************************************************************************/
storage::storage(const char* aname, const int aIndex, const base* aOwner,
		double aCapacity, commonData * runData, commonData * common):
	base(aname, aIndex, aOwner)
{
	string Alias;

		runData->FindSection(Name,Index);
		convert conv;

		runData->setCritical(true);
		runData->FindItem("Alias",&Alias);
		runData->setCritical(false);
		common->setCritical(true);

		int indexInOld=-1;
			string place="NULL";
			common->findSection(Name,0,"Alias",Alias,&place,&indexInOld);

			common->FindSection(place,indexInOld);

			common->FindItem("RefCapacity",&theCapacity);
		runData->FindItem("RefCapacity",&theCapacity);

		theProduct = NULL;
}
storage::~storage()
{
	if(theProduct!=NULL)
	{

		delete theProduct;
	}
}
/***********************************************************************
 ************************************************************************/
storage::storage(storage& aStore)
{
	theCapacity = aStore.theCapacity;
	if(aStore.theProduct!=NULL)
	{
		theProduct = aStore.theProduct->clone();
	}
	else
	{
		theProduct=NULL;
	}


}



/***********************************************************************
 ************************************************************************/
double storage::Getamountstored()
{
	return theProduct->GetAmount();
}

/**
 * returns true product if:
 * a. the stored product name is the same as the desired product name
 * b. the stored product trade name is the same as the desired product name
 * c. the stored product trade name is the same as the desired product plant name
 */
bool storage::ProductCompare(const product* desiredProduct)
{
	if (((theProduct->GetObjType() == plantItemObj)
			|| (theProduct->GetObjType() == feedItemObj))
			&& (desiredProduct->GetObjType() == feedItemObj))
	{
		if (((liveMatter*) theProduct)->GetCode()
				== ((liveMatter*) desiredProduct)->GetCode())
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		string desiredProductName = desiredProduct->GetName();
		string storedProductName = theProduct->GetName();
		string storedProductTradename = storedProductName;
		if ((theProduct->GetObjType() == plantItemObj)
				&& ((((plantItem*) theProduct)->GetTradeName()).compare("NONE")
						!= 0))
			storedProductTradename = ((plantItem*) theProduct)->GetTradeName();
		string desiredProductPlantname = "NONE";
		if (desiredProduct->GetObjType() == feedItemObj)
			desiredProductPlantname
					= ((feedItem*) desiredProduct)->GetPlantName();
		bool temp1 = false, temp2 = false, temp3 = false;
		if (storedProductName.compare(desiredProductName) == 0)
			temp1 = true;
		if (storedProductTradename.compare(desiredProductName) == 0)
			temp2 = true;
		if (storedProductTradename.compare(desiredProductPlantname) == 0)
			temp3 = true;

		if (temp1 || temp2 || temp3)
			return true;
		else
			return false;
	}
}

/*
 * adding a product to what is already stored
 */
void storage::AddToStore(product * p)
{
	*theProduct = *theProduct + *p;
}

/*
 * extract a product for store.
 *
 */
product * storage::GetFromStore(double RequestedAmount)
{
	double inventoryAmount = theProduct->GetAmount();
	double inventoryExtract = min(inventoryAmount, RequestedAmount);
	product * productFound = NULL;

	if (inventoryAmount > 0.0) // if nothing in storage then do not cast
	{
		theProduct->Setamount(inventoryExtract);
		productFound = theProduct->clone();
		theProduct->Setamount(inventoryAmount - inventoryExtract);
		//     cout << theProduct->GetName() << " inventoryAmount " << inventoryAmount << " inventoryExtract" << inventoryExtract << endl;
	}
	return productFound;
}

/*
 * returning the name of the product stored
 */
string storage::GiveNameStoredProduct()
{
	return theProduct->GetName();
}

/***********************************************************************
 ************************************************************************/
ostream& operator<<(ostream& os, const storage& s)
{
	os << setiosflags(ios::left) << setw(40) << "Capacity " << s.GetCapacity()
			<< endl;
	//s.theProduct->showstats(&os);
	//inventory ?
	return os;
}

/**
 * Returns a copy of what is stored if is the same as input. If someone accidently has stored a plantItemObj in this store fasset wil halt
 */
product* storage::GetCopyStoredFeedProduct(const product* product1)
{
	feedItem * aProductCopy = NULL;
	if (ProductCompare(product1))
	{
		if ((product1->GetObjType() == feedItemObj)
				&& (theProduct->GetObjType() == plantItemObj))
		{
			aProductCopy = new feedItem(*(feedItem *) product1);
			theMessage->FatalError(
					"all plant products should be stored as feedItems, so this should not happen");

			//      aProductCopy->CopyFromPlantItem(*(liveMatter*) theProduct);  //overwrite variables common to feedItem and plantItem
			aProductCopy->Setamount(theProduct->GetAmount());
		}
		if ((theProduct->GetObjType() == feedItemObj)
				&& (theProduct->GetAmount() > 0.0))
			aProductCopy = new feedItem(*(feedItem*) theProduct);
		return aProductCopy;
	}
	else
		return NULL;
}

/**
 * Makes a copy of what is stored if it is a feedItemObj, feedType and there is something stored.
 * Else it returns NULL
 */
product* storage::GetCopyStoredFeedProduct(int feedType)
{
	feedItem * aProductCopy;
	if (theProduct->GetObjType() == feedItemObj)
	{
		if ((((feedItem *) theProduct)->GetfeedCategory() == feedType)
				&& (theProduct->GetAmount() > 0.0))
		{
			aProductCopy = new feedItem(*((feedItem *) theProduct));
			aProductCopy->Setamount(theProduct->GetAmount());
			return aProductCopy;
		}
		else
			return NULL;
	}
	else
		return NULL;
}

/**
 * returning a pointer to what is stored if it is a feedItemObj, feedType and there is something stored.
 * Else it returns NULL
 */
product* storage::GetStoredFeedProductPtr(int feedType)
{
	if (theProduct->GetObjType() == feedItemObj)
	{
		if ((((feedItem *) theProduct)->GetfeedCategory() == feedType)
				&& (theProduct->GetAmount() > 0.0))
		{
			return theProduct;
		}
		else
			return NULL;
	}
	else
		return NULL;
}


