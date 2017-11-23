/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __PRODUCTS_H
  #define __PRODUCTS_H

#include "product.h"
#include "plantItem.h"
#include "manure.h"
#include "../base/bstime.h"
#include "../base/linklist.h"
class products: public base
{
   private:

      int simulationYear;
      bool SellthePlantProducts;
      bool SelltheStrawProducts;
      bool SelltheNonRoughageProducts;

   protected:

      linkList<product>* theProductList;
      linkList<price>* thePriceList;
      linkList<manure>* specialManureList;

   private:
   	void InitializeNormal()  ;
      void InitializeGiven( int simulationLenght)  ;
      void AddElementPriceList( price* aPrice)  ;

      void ExistsPrice(price* somePrice)  ;

      // demand functions
      double demandOn(product* aProduct);
      void subDemand(product* aProduct);

      // GAMS functions
      void writeLivestockPrices(char * filename, char * filename1);
      void writeLivestock(char * filename);
      void writeLivestockPrHa(char *filename);
      void writeLivestockN_content(char * filename, char * filename1);
      void writeSellablePlantItems(char * filename);
   	void writePrimaryPlantProducts(char * filename);
   	void writeSecondaryPlantProducts(char * filename);
      void writeAllPlantItems(char * filename);
      void writePlantItemPrices(char * filename,int period);
      void writeLivestockCapacityUse(char * filename);
      void writeLivestockPrUnit(char * filename);
      void writeSellablePlantItemsN_content(char* filename);
      void writeFeedItemsN_content(char* filename);
      void writePricesHiredHours(char* filename);

   protected:
      void InitializeProduct(void);

   public:
      // Default Constructor
      products();
      // Constructor with arguments
      products(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      products(const products& aproducts);
      // Destructor
      ~products();
      // Operator for output
      friend ostream& operator << (ostream& os, const products& aproducts);


      // Other functions

      void InitializePrices(bsTime* astoptime);
      void InitializeProducts(string productFileName,string baseProductFileName);//,bool cattle);
      void GiveIndicator();
      void resetProducts(void);
      void YearlyUpdate();

      void SetSellthePlantProducts(bool aVal){SellthePlantProducts=aVal;};
      void SetSelltheStrawProducts(bool aVal){SelltheStrawProducts=aVal;};
      void SetSelltheNonRoughageProducts(bool aVal){SelltheNonRoughageProducts=aVal;};
		void SellPlantItem(product *itemToSell);

      // price functions
      price* GetPriceElement(const  char* ident)  ;
      price* GetPriceElement(string aPriceName)  ;
      double GetActualSellPrice( const  char* ident);
      double GetActualBuyPrice( const  char* ident);
      double GetExpectedSellPriceYear1( const  char* ident);
      double GetExpectedBuyPriceYear1( const  char* ident);

      // functions on the linklist
      void AddProduct(product* aProduct);
      void AddHarvestGrainProducts(linkList<plantItem>* aList);
      void AddHarvestStrawProducts(linkList<plantItem>* aList);
      void AddForageProducts(linkList<plantItem>* aList);
		void AddHarvestProducts(int feedCat, linkList<plantItem>* HarvestList);

      void SubtractProduct(product* aProduct);

      void SubtractProduct(product* aProduct, bool import);
		double ImportProduct(product* aProduct);

      linkList <product> * GetSubsetofProductList(int ObjType);
      void UpdateProductPtr(void);

      // functions on elements in the linklist
      void GiveProductInformation(product* aProduct);
      void AddElementProductList(product* aProduct);
      product* GetProductElement(const  char* ident, bool isManure=false);
      product* GetProductElement(string aProductName,bool isManure =false);
      product* GetProductPtr(string aProductName, bool isManure=false);
      void ExistsProduct(product* someProduct);
      void ExistsManure(manure* someProduct);
      // stream functions

      void ShowProducts(ostream * os,linkList <product> * aList);
      void ShowPrices(ostream *os);

      //Added by NJH to enable use of products by cattle
		void InitializeFeed(string FeedFileName);
		product* GetProductElement(int feedCode,bool isManure=false);
		product* GetCopyStoredFeedProduct(product * aProduct);
// moved from private by NJH (so can be called by cattle)
      price* GetPricePtr(string aPriceName)  ;
		int GetHarvestProductCode(linkList<plantItem>* HarvestList);
#ifdef NITROSCAPE
      //!Recovers a pointer to a product of the given type and index from products.dat
      /*!
      \param aType The type of product (see typer.h)
      \param anIndex The index of the product required from products.dat
      */
		product* GetProductPtr(productObjType aType, int anIndex);
#endif

};

extern products * theProducts;
#endif

