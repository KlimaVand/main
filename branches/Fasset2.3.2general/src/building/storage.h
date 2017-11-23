/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
//!Basic storage class
/*!Basic storage class.  Should not be instanced except in inherited form.
 * */
#ifndef __storage_H  //Required for current class
  #define __storage_H

#include "../base/linklist.h"
#include "../products/product.h" //Required for include files
#include "../products/products.h"
#include "../base/commonData.h"
class storage: public base
{
	//!The capacity of the storage in appropriate units
 double theCapacity;
 //!The product stored
 product* theProduct;
private:


public:
 storage ();
  //!Constructor with arguments
  storage (const char* aname,const int aIndex,const base* aOwner,double aCapacity, commonData * runData, commonData * common);
  //!Copy constructor
  storage(storage& aStore);
  //!Destructor
 ~ storage ( ); //Destructor

   // Get and set functions for attributes
 //!Return the capacity of the store
   double GetCapacity() const {return theCapacity;}
   //!Set the capacity of the store
   void SetCapacity (double aCapacity){ theCapacity = aCapacity;}
   //!Return a pointer to the product stored
   product * GetProductPointer() const {return theProduct;};
   //!Set the pointer to the product stored
   void SetProductPointer(product * aProduct) {theProduct = aProduct;};
   //!Return amount of product stored in the appropriate units
   double Getamountstored();
   //!Return true if the stored product is the same as the desired product
   /*!
    * \param desiredProduct the product desired
    * */
   bool ProductCompare(const product* desiredProduct);
   //!Add a product to the store
   /*!
    * \param p the product to add
    * */
   virtual void AddToStore(product * p);
   //!Returns an amount of product from the store
   /*!
    * \param RequestedAmount the amount requested
    * */
   virtual product * GetFromStore(double RequestedAmount);
   //!Returns the name of the stored product
   string GiveNameStoredProduct();
   //!Initialise storage details from a text file
   void ReadParameters(fstream * file);

	//!Returned a copy of a stored feed product
  /*!
   * \param feedType reference number of the feed product
   * */
   product*  GetCopyStoredFeedProduct(int feedType);
	//!Returned a copy of a stored feed product
	/*!
	 * \param product1 the feed product to be copied
	 * */
   product*  GetCopyStoredFeedProduct(const product* product1);
	//!Returned a pointer to the stored feed product
   /*!
    * \param feedType reference number of the feed product
    * */
	product*  GetStoredFeedProductPtr(int feedType);

	   /*!Read default parameters
	    * \param file pointer to file containing default information
	    * */

};
#endif
