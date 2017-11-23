/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef B_STORA2_H
  #define B_STORA2_H

#include "../building/building.h"
#include "../building/storage.h"
#include "../base/budget.h"
#include "../products/feedItem.h"
/****************************************************************************\
 Class: B_storage
 superclass of all storage buildings. Should not be instanced.
 Use one of the derived storages instead, or derive your own.
\****************************************************************************/
//!Superclass for all storage
/*!Superclass for storage.  Should not be instanted.  Use one of the derived storage classes
 * instead or derive one of your own.
 * */

class barnStorage : public building
{

private:
	// default constructor
   barnStorage() {};
   //automtaic scaling of storage if there are to little room for a product
   bool scalableBstore;

protected:
	//!List of stores
	linkList<storage> *stores;
//	budget Nbudget;
public:
   //! Constructor with arguments
	barnStorage(char *aName,const int aIndex,const base* aOwner);
   //! Copy constructor
   barnStorage(const barnStorage& s);
   //! Destructor
   ~barnStorage();
   //! Initialize storage from file
   void ReadParameters(fstream * file);


   //!Return amount available in storage of a given product
   /*!
   \param p contains details of the target product
   */
   virtual double GetAvailableAmountOf(const product * p);
   //!Request an amount of a product from storage
   /*!
    * \param amount the amount requested
    * \param p the product requested
    * */
   virtual product * GetFromStore(double amount,product * p);
   //!Send an amount of a product to a store
   /*!
    * \param p the product to be sent
    * */
   virtual int SendToStore(product * p);
   //! Handles all daily actions of the storage
   virtual void DailyUpdate();
   //!no idea what this does
   virtual void GiveValueOfInventory();

	//!Returns a clone of the list of storage
   linkList<storage> * GetStorageClone();

   //!Check nutrient budgets
	virtual void EndBudget(bool show = false);
	//!Return in the amount of a feed product that is available
	/*!
	 * \param aProduct the product of interest
	 * */
	virtual void GetAvailableFeedProduct(feedItem *aProduct);
	//!Returned a copy of a stored feed product
	/*!
	 * \param aProduct the feed product to be copied
	 * */
   virtual product * GetCopyStoredFeedProduct(const product * aProduct);
	//!Returned a copy of a stored feed product
   /*!
    * \param type reference number of the feed product
    * */
   virtual product * GetCopyStoredFeedProduct(int type);
   //!Generate indicator output
   /*!
    * \param indicatorType the type of indicating requested
    * */

};

#endif
