/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef B_STORA2_H
  #define B_STORA2_H

#include "building.h"
#include "genstor.h"
#include <budget.h>

/****************************************************************************\
 Class: B_storage
 superclass of all storage buildings. Should not be instanced.
 Use one of the derived storages instead, or derive your own.
\****************************************************************************/
//!Superclass for all storage
/*!Superclass for storage.  Should not be instanted.  Use one of the derived storage classes
 * instead or derive one of your own.
 * */

class B_storage2 : public building
{

private:
	// default constructor
   B_storage2() {};

protected:
	//!List of stores
	linkList<storage> *stores;
//	budget Nbudget;
public:
   //! Constructor with arguments
   B_storage2(char *aName,const int aIndex,const base* aOwner);
   //! Copy constructor
   B_storage2(const B_storage2& s);
   //! Destructor
   ~B_storage2();
   //! Initialize storage from file
   void ReadParameters(fstream * file);
	//! Return capacity of a particular store
   /*!Return capacity of particular store
    * \param storenum logical number of the store whose capacity should be returned
    * */
	virtual double GetCapacity(const int storenum);
	//!Return area of storage
   double GetArea() {return Area;};
   //!Return pointer to the list of stores
   linkList<storage> *Getstorage();
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
   //!probably obsolete
	virtual void CalcLP();
	//!Returns a clone of the list of storage
   linkList<storage> * GetStorageClone();
   //!Outputs streaming
   friend ostream& operator <<(ostream& os,B_storage2 s);
   //!Check nutrient budgets
	virtual void EndBudget(bool show = false);
	//!Return in the amount of a feed product that is available
	/*!
	 * \param aProduct the product of interest
	 * */
	virtual void GetAvailableFeedProduct(product *aProduct);
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
   virtual void GiveIndicator(int indicatorType);
};

#endif
