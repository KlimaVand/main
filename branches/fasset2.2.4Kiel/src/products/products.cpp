/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "../base/message.h"
#include "products.h"
#include "price.h"
#include "product.h"
#include "animalProduct.h"
#include "seed.h"
#include "fertilizer.h"
#include "chemical.h"
#include "water.h"
#include "energy.h"
#include "manure.h"
#include "milk.h"
#include "decomposable.h"
#include "feedItem.h"
#include "premium.h"
#include "feedResource.h"
#include "../data/systemData.h"
#ifndef __ECOSYSTEM__
#include "../manager/manureManager.h"
#ifndef _STANDALONE
   #include "../building/thebuild.h"
   #endif
#endif
   #include "../base/IndicatorOutput.h"


/****************************************************************************\
  Default Constructor
\****************************************************************************/
products::products()
   : base()
{

   theProductList = new linkList <product>;

   thePriceList  = new linkList <price>;
   simulationYear = 0;
	SellthePlantProducts=false;
	SelltheStrawProducts=false;
	SelltheNonRoughageProducts=false;
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
products::products(const char * aName, const int aIndex, const base * aOwner)
	: base(aName, aIndex, aOwner)
{

   theProductList   = new linkList <product>;

   thePriceList  = new linkList <price>;
   simulationYear = 0;
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
products::products(const products& aproducts)
   : base(aproducts)
{

}

/****************************************************************************\
  Destructor
\****************************************************************************/
products::~products()
{/*
   delete theProductSubList;   // Added by BMP and PL
	delete theProductAddList;   // Added by BMP and PL
	if(theProductList)
      delete theProductList;
	if(theDemandList)
      delete theDemandList;
	if(thePriceList)
      delete thePriceList;*/
}

/****************************************************************************\
  Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os, const products& aproducts)
{
  	os << setiosflags(ios::left) << setw(40) << "theProductSubList " << endl;
  	os << setiosflags(ios::left) << setw(40) << "theProductAddList " <<  endl;
	return os;
}

/****************************************************************************\
  Read product prices
\****************************************************************************/
void products::InitializePrices(bsTime* astoptime)
{
   int  index;
   char priceSection[81];
   int SimulationInYearDuration = astoptime->GetYear() - theTime.GetYear();
	int aSimulationLength=SimulationInYearDuration+4;
   SetCritical();
	OpenInputFileString("prices.dat");
   strcpy(priceSection,"price");
   int first,num;
   GetSectionNumbers("price",&first,&num);
   for(index=first;index<(first+num);index++)
   {
      price* priceInstance = new price(priceSection,index,this);
      priceInstance->ReadParameters(file,aSimulationLength);
      AddElementPriceList(priceInstance);
   }
   CloseInputFile();
}

/****************************************************************************\
\****************************************************************************/
void products::InitializeProducts(string productFileName)//,bool cattle)
{
	fileAccess hd;
	hd.changeDir(theSystemData->getInputDirectory());
   int first,num,index;
   char productSection[81];
   SetCritical();
	OpenInputFileString(productFileName);

  	strcpy(productSection,"product");
   GetSectionNumbers(productSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      product* productInstance = new product(productSection,index,this);
      productInstance->ReadParameters(file);
      AddElementProductList(productInstance);
   }

   strcpy(productSection,"animal");
   GetSectionNumbers(productSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      animalProduct* animalInstance = new animalProduct(productSection,index,this);
      animalInstance->ReadParameters(file);
      AddElementProductList(animalInstance);
   }

   strcpy(productSection,"decomposable");
   GetSectionNumbers(productSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      decomposable* decomposableInstance = new decomposable(productSection,index,this);
      decomposableInstance->ReadParameters(file);
      AddElementProductList(decomposableInstance);
   }

   strcpy(productSection,"plantitem");
   GetSectionNumbers(productSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      plantItem* plantInstance = new plantItem(productSection,index,this);
      plantInstance->ReadParameters(file);
      AddElementProductList(plantInstance);
   }

   strcpy(productSection,"seed");
   GetSectionNumbers(productSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      seed* seedInstance = new seed(productSection,index,this);
      seedInstance->ReadParameters(file);
      AddElementProductList(seedInstance);
   }

   strcpy(productSection,"chemical");
   GetSectionNumbers(productSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      chemical* chemicalInstance = new chemical(productSection,index,this);
      chemicalInstance->ReadParameters(file);
      AddElementProductList(chemicalInstance);
   }

   strcpy(productSection,"manure");
   GetSectionNumbers(productSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      manure* manureInstance = new manure(productSection,index,this);
      manureInstance->ReadParameters(file);
      AddElementProductList(manureInstance);
   }

   strcpy(productSection,"milk");
   UnsetCritical();
   GetSectionNumbers(productSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      milk* milkInstance = new milk(productSection,index,this);
      milkInstance->ReadParameters(file);
      AddElementProductList(milkInstance);
   }
   SetCritical();

   strcpy(productSection,"water");
   GetSectionNumbers(productSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      water* waterInstance = new water(productSection,index,this);
      waterInstance->ReadParameters(file);
      AddElementProductList(waterInstance);
   }

   strcpy(productSection,"energy");
   GetSectionNumbers(productSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      energy* energyInstance = new energy(productSection,index,this);
      energyInstance->ReadParameters(file);
      AddElementProductList(energyInstance);
   }

   strcpy(productSection,"fertilizer");
   GetSectionNumbers(productSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      fertilizer* fertilizerInstance = new fertilizer(productSection,index,this);
      fertilizerInstance->ReadParameters(file);
      AddElementProductList(fertilizerInstance);
   }

   strcpy(productSection,"premium");
   GetSectionNumbers(productSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      premium* premiumInstance = new premium(productSection,index,this);
      premiumInstance->ReadParameters(file);
      AddElementProductList(premiumInstance);
   }
   CloseInputFile();

   // Read cattle feed data from tables
   // if (cattle)

}

/****************************************************************************\
\****************************************************************************/
double products::GetExpectedSellPriceYear1(const  char* ident)
{
	price* aPrice;
	aPrice=GetPriceElement(ident);
	return aPrice->GetExpectedSellPriceYear(1);
}

/****************************************************************************\
\****************************************************************************/
double products::GetExpectedBuyPriceYear1(const  char* ident)
{
	price* aPrice;
	aPrice=GetPriceElement(ident);
	return aPrice->GetExpectedBuyPriceYear(1);
}

/****************************************************************************\
\****************************************************************************/
double products::GetActualSellPrice(const  char* ident)
{
	price* aPrice;
	aPrice=GetPriceElement(ident);
	return aPrice->GetactualSellPrice();
}

/****************************************************************************\
\****************************************************************************/
double products::GetActualBuyPrice(const  char* ident)
{
	price* aPrice;
	aPrice=GetPriceElement(ident);
	return aPrice->GetactualBuyPrice();
}




/****************************************************************************\
Handles the registration of the sale of agricultural products
\****************************************************************************/
void products::AddProduct(product* aProduct)
{
#ifndef _STANDALONE
	double income=0.0;
//   string pname=aProduct->Getname();
	char nameStr[80];
   strcpy(nameStr,aProduct->GetName());
//   char* indstr = "99.99 unsp sold";

   price *aPrice = aProduct->GetaPrice();
   if ((!aPrice)&& (aProduct->GetObjType()==animalObj))    //some products have no price (e.g. home produced silage)
		theMessage->FatalError("Products::AddProduct - Cannot find price for ",nameStr);
   double NSent = 0.0, CSent = 0.0;
   switch(aProduct->GetObjType())
   {
     case feedItemObj :
     case plantItemObj :
#ifndef __ECOSYSTEM__
     							theBuildings->StoreProduct(aProduct);
     							NSent = aProduct->GetAmount() * ((liveMatter*) aProduct)->GetAllN().n * 1000.0;
                        theOutput->AddIndicator(environmentalIndicator,"35.04 N from harvest to store","kg N",NSent);
     							CSent = aProduct->GetAmount() * ((liveMatter*) aProduct)->GetC_content() * 1000.0;
                        theOutput->AddIndicator(environmentalIndicator,"45.04 C from harvest to store","kg C",CSent);
                        if (aProduct->GetaPrice()!=NULL)
                        {
                        	income=((aProduct->GetAmount())*((aProduct->GetaPrice())->GetactualSellPrice()) );
                        	theOutput->AddIndicator(economicIndicator,"17.01 Gross output of plant production","Dkr",income);
                        }
#else
     							NSent = aProduct->GetAmount() * ((liveMatter*) aProduct)->GetAllN().n * 1000.0;
                        theOutput->AddIndicator(environmentalIndicator,"35.20 N in sold plant products","kg N",NSent);
#endif
                        break;
/*     case milkObj:
         theOutput->AddIndicator(environmentalIndicator,"21.34 N in milk","kg N",Number * milkN/1000.0);*/

#ifndef __ECOSYSTEM__
		case animalObj: income=((aProduct->GetAmount())*(aPrice->GetactualSellPrice()) );
                     if ((strcmp(nameStr,"SUCKLINGPIG")==0)||(strcmp(nameStr,"PIGLETS")==0)||(strcmp(nameStr,"BACONERS")==0)
                              ||(strcmp(nameStr,"SOWS")==0)||(strcmp(nameStr,"NEWSOWS")==0))
                     {
                     	NSent =((animalProduct*)aProduct)->GetN_content()* aProduct->GetAmount();
                        theOutput->AddIndicator(environmentalIndicator,"34.20 N in sold pigs","kg N",NSent*1000.0);
                     	CSent =((animalProduct*)aProduct)->GetC_content()* aProduct->GetAmount();
                        theOutput->AddIndicator(environmentalIndicator,"44.62 C in sold pigs","kg C",CSent*1000.0);
                         if (strcmp(nameStr,"SUCKLINGPIG")==0)
                        {
	                        theOutput->AddIndicator(economicIndicator,"05.34 Weaners sold","Dkr",income);
                           theOutput->AddIndicator(economicIndicator,"19.05 Sold weaners","no",((animalProduct*)aProduct)->GetNumber());
                        }
                        if (strcmp(nameStr,"PIGLETS")==0)
                        {
	                        theOutput->AddIndicator(economicIndicator,"05.32 Piglets sold","Dkr",income);
                           theOutput->AddIndicator(economicIndicator,"19.06 Sold piglets","no",((animalProduct*)aProduct)->GetNumber());
                        }
                        if (strcmp(nameStr,"BACONERS")==0)
                        {
                            theOutput->AddIndicator(economicIndicator,"05.31 Baconers sold","Dkr",income);
                            theOutput->AddIndicator(economicIndicator,"19.07 Sold baconers","no",((animalProduct*)aProduct)->GetNumber());
                        }
                       	if (strcmp(nameStr,"NEWSOWS")==0)
                        {
                            theOutput->AddIndicator(economicIndicator,"05.30 Sows sold","Dkr",income);
                            theOutput->AddIndicator(economicIndicator,"19.08 Sold new sows","no",((animalProduct*)aProduct)->GetNumber());
                        }
                        if (strcmp(nameStr,"SOWS")==0)
                        {
                            theOutput->AddIndicator(economicIndicator,"05.33 Cast sows sold","Dkr",income);
                            theOutput->AddIndicator(economicIndicator,"19.09 Sold cast sows","no",((animalProduct*)aProduct)->GetNumber());
                        }
                     }  //end of piggy matters
                     if ((strcmp(nameStr,"HEIFERS1")==0)||(strcmp(nameStr,"HEIFERS2")==0)||(strcmp(nameStr,"HEIFERS3")==0)||(strcmp(nameStr,"DAIRY")==0)
                          || (strcmp(nameStr,"FBEEF1")==0) || (strcmp(nameStr,"FBEEF2")==0) || (strcmp(nameStr,"FBEEF3")==0) ||(strcmp(nameStr,"SUCKLER")==0)
                          || (strcmp(nameStr,"MBEEF1")==0) || (strcmp(nameStr,"MBEEF2")==0) || (strcmp(nameStr,"MBEEF3")==0)
                          ||(strcmp(nameStr,"CALF")==0))
                     {
                     	NSent =((animalProduct*)aProduct)->GetN_content()* aProduct->GetAmount();
                        theOutput->AddIndicator(environmentalIndicator,"33.21 N in sold cattle","kg N",NSent);
                     	CSent =((animalProduct*)aProduct)->GetC_content()* aProduct->GetAmount();
                        theOutput->AddIndicator(environmentalIndicator,"43.21 C in sold cattle","kg C",CSent);

                        if ((strcmp(nameStr,"HEIFERS1")==0)||(strcmp(nameStr,"HEIFERS2")==0))
                        {
                            theOutput->AddIndicator(economicIndicator,"05.50 Young 1st year heifers sold","Dkr",income);
                            theOutput->AddIndicator(economicIndicator,"19.35 Sold 1st year heifers","no",((animalProduct*)aProduct)->GetNumber());
                        }
                        if (strcmp(nameStr,"HEIFERS3")==0)
                        {
                            theOutput->AddIndicator(economicIndicator,"05.51 Young 2nd year heifers sold","Dkr",income);
                            theOutput->AddIndicator(economicIndicator,"19.36 Sold 2nd year heifers","no",((animalProduct*)aProduct)->GetNumber());
                        }
                        if (strcmp(nameStr,"DAIRY")==0)
                        {
                            theOutput->AddIndicator(economicIndicator,"05.52 Dairy cattle sold","Dkr",income);
                            theOutput->AddIndicator(economicIndicator,"19.37 Sold dairy cattle","no",((animalProduct*)aProduct)->GetNumber());
                        }
                        if (strcmp(nameStr,"CALVES")==0)
                        {
                            theOutput->AddIndicator(economicIndicator,"05.53 Beef cattle calves sold","Dkr",income);
                            theOutput->AddIndicator(economicIndicator,"19.38 Sold beef calves","no",((animalProduct*)aProduct)->GetNumber());
                        }
                     	if ((strcmp(nameStr,"FBEEF1")==0) || (strcmp(nameStr,"FBEEF2")==0) || (strcmp(nameStr,"FBEEF3")==0)
                          || (strcmp(nameStr,"MBEEF1")==0) || (strcmp(nameStr,"MBEEF2")==0) || (strcmp(nameStr,"MBEEF3")==0)
                          ||(strcmp(nameStr,"CALF")==0))
                        {
                            theOutput->AddIndicator(economicIndicator,"05.54 Mature beef cattle sold","Dkr",income);
                            theOutput->AddIndicator(economicIndicator,"19.39 Sold beef cattle","no",((animalProduct*)aProduct)->GetNumber());
                        }
                     } //end of cattle matters

                  break;
#endif
/*      default:    char * messString = "Products::AddProduct - Unknown product ";
                  strcat(messString,nameStr);
                  theMessage->FatalError(messString);
                  break;*/
   }  //end of switch
#endif
}

void products::AddHarvestGrainProducts(linkList<plantItem>* aList)
{
	AddHarvestProducts(1,aList);
}

void products::AddHarvestStrawProducts(linkList<plantItem>* aList)
{
	AddHarvestProducts(2,aList);
}

void products::AddForageProducts(linkList<plantItem>* aList)
{
	AddHarvestProducts(2,aList);
}

/***************************************************************************'
Mix all the products to form a single new product
Assign a product code to the new product
Assign a feed code to the new product
****************************************************************************/
void products::AddHarvestProducts(int feedCat, linkList<plantItem>* HarvestList)
{
   // Check om der er mere end nul, hvis der er g� videre, ellers udskriv en advarsel !!!
   int productCode=0;
   feedItem * itemToStore = NULL;
   productCode=GetHarvestProductCode(HarvestList); //load product info for this product
   if (productCode!=0) //single product with a code from the feed table was found
   {
      feedItem * tempItem = (feedItem *)GetProductElement(productCode); //get pointer to product
      if (tempItem != NULL)
      {
         itemToStore=new feedItem(*(tempItem)); //create new feed item
         //if a single harvested product is present, itemToStore is a copy of the harvested product.
         //If more than one harvested product is present, they are all added to itemToStore
         for(int i=0;i<HarvestList->NumOfNodes();i++)
         {
			 plantItem *HarvestItem = HarvestList->ElementAtNumber(i);
			 if (HarvestItem->GetAmount()>0) //add the plantItems to the single feedItem
				itemToStore->AddPlantItem(*((plantItem*) HarvestItem), feedCat);
         }
      }
      else productCode = 0;  //no product with this code was found
   }
   
   if (productCode==0) //no single product was found in feed table, so use the product name(s) to look for a product or products in products list
   {
	   if (HarvestList->NumOfNodes()>1) //this function always produces a single product. If two or more products are present and no product code is found, throw an error
		   theMessage->FatalError("products::AddHarvestProducts - no mixed product found for two or more products in harvest");
	   else
	   {
		   plantItem *HarvestItem = HarvestList->ElementAtNumber(0);
		   itemToStore = new feedItem();
		   plantItem *aplantItem = ((plantItem*) GetProductPtr(HarvestItem->GetName()));
		   if (!aplantItem)
			   theMessage->FatalError("products::Harvested product unknown");
		   else
		   {
			   if (HarvestItem->GetAmount()>0)
			   {
				   *aplantItem + *((plantItem*)HarvestItem);
				   itemToStore->AddPlantItem(*((plantItem*) HarvestItem), feedCat);
			   }
		   }
	   }
   }
   if (itemToStore->GetAmount()>0)
   {
	   itemToStore->SetCode(productCode);
	   itemToStore->SetfeedCategory(feedCat);
	   AddProduct(itemToStore);
	   if ((SellthePlantProducts)||((SelltheStrawProducts)&&(itemToStore->GetfeedCategory()==4))||
			   ((SelltheNonRoughageProducts)&&(itemToStore->GetfeedCategory()!=2)))
		   SellPlantItem(itemToStore);
   }
  	delete itemToStore;
}


/****************************************************************************\
Sells all plant items
\****************************************************************************/
void products::SellPlantItems()
{
#ifndef _STANDALONE
	#ifndef __ECOSYSTEM__
	feedItem* anItem;

   linkList <product> * AllPlantItems;
	AllPlantItems = theProducts->GetSubsetofProductList(feedItemObj);

   for(int i=0; i<AllPlantItems->NumOfNodes(); i++)
   {
      anItem = (feedItem*) AllPlantItems->ElementAtNumber(i);
      if ((SellthePlantProducts)||((SelltheStrawProducts)&&(anItem->GetfeedCategory()==4))||
      			((SelltheNonRoughageProducts)&&(anItem->GetfeedCategory()!=2)))
      {
         anItem->Setamount(1000000.0);
         SellPlantItem(anItem);
      }
 	}
   delete AllPlantItems;
#endif
#endif
}

/****************************************************************************\
Sells a single plant item
\****************************************************************************/
void products::SellPlantItem(product *itemToSell)
{
#ifndef _STANDALONE
	plantItem* aPlantItem = (plantItem*) itemToSell;
   double amountStored, income=0.0;
 //  char* indstr;

   string pname=itemToSell->Getname();
#ifndef __ECOSYSTEM__
   amountStored = theBuildings->AmountProductStored(aPlantItem);
#endif
   if (amountStored>0.0)
   {
      if (amountStored<aPlantItem->GetAmount())
          aPlantItem->Setamount(amountStored);
	   SubtractProduct(aPlantItem);

      double kgTotalN = aPlantItem->GetAmount()*aPlantItem->GetTotalN().n*1000.0;
      double kgTotalC = aPlantItem->GetAmount()*aPlantItem->GetC_content()*1000.0;
	   theOutput->AddIndicator(economicIndicator,"19.71 Exported plant products","t",aPlantItem->GetAmount());
      theOutput->AddIndicator(environmentalIndicator,"35.20 N in sold plant products","kg",kgTotalN);
      theOutput->AddIndicator(environmentalIndicator,"45.20 C in sold plant products","kg",kgTotalC);
      if (aPlantItem->GetaPrice()!=NULL)
      {
         income=((aPlantItem->GetAmount())*((aPlantItem->GetaPrice())->GetactualSellPrice()));
         theOutput->AddIndicator(economicIndicator,"05.10 Plant products sold","Dkr",income);
      }

		if (pname.compare("WNWHT")==0)  theOutput->AddIndicator(economicIndicator,"05.11 W.wheat sold","Dkr",income);
		if (pname.compare("SPWHT")==0)  theOutput->AddIndicator(economicIndicator,"05.12 S.wheat sold","Dkr",income);
		if (pname.compare("WNRPE")==0)  theOutput->AddIndicator(economicIndicator,"05.13 W.rape sold","Dkr",income);
		if (pname.compare("SPRAPE")==0)  theOutput->AddIndicator(economicIndicator,"05.14 S.rape sold","Dkr",income);
		if (pname.compare("WNBRL")==0)  theOutput->AddIndicator(economicIndicator,"05.15 W.barley sold","Dkr",income);
		if (pname.compare("BARLEY")==0) theOutput->AddIndicator(economicIndicator,"05.16 S.barley sold","Dkr",income);
		if (pname.compare("RYE")==0)    theOutput->AddIndicator(economicIndicator,"05.17 Rye sold","Dkr",income);
		if (pname.compare("OAT")==0)    theOutput->AddIndicator(economicIndicator,"05.18 Oat sold","Dkr",income);
		if (pname.compare("PEA")==0)    theOutput->AddIndicator(economicIndicator,"05.19 Pea sold","Dkr",income);
		if (pname.compare("POTATO")==0) theOutput->AddIndicator(economicIndicator,"05.20 Potato sold","Dkr",income);
		if (pname.compare("WNWHT")==0)  theOutput->AddIndicator(economicIndicator,"05.21 Grass for seed sold","Dkr",income);
		if (pname.compare("MAIZE")==0)  theOutput->AddIndicator(economicIndicator,"05.22 Maize sold","Dkr",income);
		if (pname.compare("BEET")==0)   theOutput->AddIndicator(economicIndicator,"05.23 Beet sold","Dkr",income);
		if (pname.compare("STRAW")>0)   theOutput->AddIndicator(economicIndicator,"05.24 Straw sold","DKr",income);
		if (pname.compare("FEEDRYEGRASS")==0 || pname.compare("FEEDGRASSCLOVER")==0)
      	theOutput->AddIndicator(economicIndicator,"05.25 Roughage sold","DKr",income);

   }
#endif
}

/****************************************************
Get feed of a particular type (e.g. roughage), don't care about the exact type
*****************************************************/
product * products::GetProductType(product* aProduct)
{
	double amountThisProduct = 0.0;
   double amountRequired = aProduct->GetAmount();
   int type = ((feedItem *)aProduct)->GetfeedCategory();
   feedItem * aFeed = NULL;
#ifndef __ECOSYSTEM__
   if (amountRequired > 0.0)
   {
      bool endit = false;
      while ((amountThisProduct<amountRequired)&&(!endit))
      {
         aFeed = ((feedItem *) (theBuildings)->GetStoredFeed(type)); //is a copy of the stored feed
         if (aFeed)
               amountThisProduct = aFeed->GetAmount();
         if ((!aFeed)||(amountThisProduct<amountRequired))
            endit = true;
      }
   }
#endif
   return aFeed;
}

/****************************************************************************\
\****************************************************************************/
void products::SubtractProduct(product* aProduct)
{
	SubtractProduct(aProduct,true);
}

/****************************************************************************\
\****************************************************************************/
void products::SubtractProduct(product* aProduct, bool import)
{
#ifndef _STANDALONE
   product* storedProduct = NULL; 	 //taken from storage
	product* importedProduct = NULL;       //imported to farm
   double req = aProduct->GetAmount();
   double fromStore = 0.0, imported = 0.0;
	char nameStr[80];
   strcpy(nameStr,aProduct->GetName());
#ifndef __ECOSYSTEM__
   if (req>0.0)
	   storedProduct=theBuildings->ExtractProduct(aProduct);
   if (storedProduct)
   {
	   fromStore=storedProduct->GetAmount();
   	double NRemoved=0.0, CRemoved=0.0;
      switch(aProduct->GetObjType())
      {
         case feedItemObj	 : NRemoved=((liveMatter* ) storedProduct)->GetAllN().n*  fromStore *1000;
                              theOutput->AddIndicator(environmentalIndicator,"35.82 N removed from stored feed products","kg N",NRemoved);
                              CRemoved=((liveMatter* ) storedProduct)->GetC_content()*  fromStore *1000;
                              theOutput->AddIndicator(environmentalIndicator,"45.82 C removed from stored feed products","kg C",CRemoved);
										break;
	      case plantItemObj  : NRemoved=((liveMatter* ) storedProduct)->GetAllN().n*  fromStore *1000;
                              theOutput->AddIndicator(environmentalIndicator,"35.81 N removed from stored plant products","kg N",NRemoved);
                              CRemoved=((liveMatter* ) storedProduct)->GetC_content()*  fromStore *1000;
                              theOutput->AddIndicator(environmentalIndicator,"45.81 C removed from stored plant products","kg C",CRemoved);
										break;
         case manureObj     : theManure_manager->AddManureExtracted(fromStore);
                              NRemoved=((liveMatter* ) storedProduct)->GetAllN().n*  fromStore *1000;
                              theOutput->AddIndicator(environmentalIndicator,"35.21 N removed from manure storage","kg N",NRemoved);
                              CRemoved=((liveMatter* ) storedProduct)->GetC_content()*  fromStore *1000;
                              theOutput->AddIndicator(environmentalIndicator,"45.21 C removed from manure storage","kg C",CRemoved);
                              if (aProduct->GetmainUnitName()==3)   //if manure requested in kg N not tonnes FW
                                 fromStore = NRemoved;
	                           break;
      }
   }

	aProduct->Setamount(0);   //zero product before adding stored material (and imported, if requested)
   imported = req-fromStore;    //amounts in tonnes or kg, depending on mainUnitName
   //Note: if the material taken from store is insufficient to satisfy demand, the remainder is imported
   if ((import) && (imported>0.0))
   {
		importedProduct = aProduct->clone();
   	importedProduct->Setamount(imported);
	   ImportProduct(importedProduct);
	   *aProduct + *importedProduct;
   }
	delete importedProduct;                        // JB added this

   if ((aProduct->GetObjType()==manureObj)&&(aProduct->GetmainUnitName()==3))   //requested in kg but returned in tonnes FW
      aProduct->SetmainUnitName(2);
   if (fromStore>0.0)
   {
   	*aProduct + *storedProduct;
		delete storedProduct;                        // JB added this
   }
   #endif
#endif
}

/****************************************************************************\
\****************************************************************************/
double products::ImportProduct(product* aProduct)
{
	double imported = aProduct->GetAmount();
	product* importedProduct = ((feedItem*) aProduct)->clone();       //imported to farm
	char nameStr[80];
   strcpy(nameStr,aProduct->GetName());
  	double cost = 0.0;
   char* indstr=new char[200];
   strcpy(indstr,"99.99 unspecified object bought");
   if (imported>0.0)
   {
#ifndef __ECOSYSTEM__
	   if (aProduct->GetObjType()==manureObj)
      {
	      if (importedProduct->GetmainUnitName()==3)	//amount requested is expressed in kg N not t FW, NJH Feb 2009
         {
	         imported = importedProduct->GetAmount()/(((manure *) importedProduct)->GetAllN().n * 1000.0);
            importedProduct->Setamount(imported);
         }
         imported = theManure_manager->GetImportableManure(imported);
      }
#endif
	   importedProduct->Setamount(imported);
   	double NImported=0.0, CImported=0.0;
      if (importedProduct->GetaPrice()!=NULL)
         cost=((importedProduct->GetAmount())*((importedProduct->GetaPrice())->GetactualBuyPrice()));

      switch(importedProduct->GetObjType())
      {
         case productObj    : if (strcmp(importedProduct->GetName(),"hiredHours")==0)
								 strcpy(indstr,"06.70 Costs hired hours");
                              if(strcmp(importedProduct->GetName(),"VET")==0)
                            	  strcpy(indstr,"06.33 Cost of vet and production control");
                              break;
         case waterObj      : strcpy(indstr,""); break;                    // assuming that water has no price!
         case seedObj       : strcpy(indstr,"06.11 Costs seed"); break;
         case fertilizerObj : strcpy(indstr,"06.12 Costs mineral fertilizer"); break;
         case chemicalObj	 : strcpy(indstr,"06.13 Costs chemicals"); break;
         case energyObj		 : if(strcmp(importedProduct->GetName(),"DIESEL")==0)
                                 strcpy(indstr,"06.14 Costs fuel");
                              else
                                 strcpy(indstr,"06.15 Costs electricity");
                              break;
         case plantItemObj  : strcpy(indstr,"06.32 Costs feed and bedding");
                              NImported=((plantItem* ) importedProduct)->GetAllN().n* importedProduct->GetAmount()*1000;
                              theOutput->AddIndicator(environmentalIndicator,"35.03 N from imported bedding","kg N",NImported);
                              CImported=((plantItem* ) importedProduct)->GetC_content()* importedProduct->GetAmount()*1000;
                              theOutput->AddIndicator(environmentalIndicator,"45.03 C from imported bedding","kg C",CImported);
// should be straw registered here                              theOutput->AddIndicator(economicIndicator,"10.20 Imported manure","t",importedProduct->GetAmount());
                              break;
         case feedItemObj	 : strcpy(indstr,"06.32 Costs feed and bedding");
         							NImported=((feedItem*) importedProduct)->GetAllN().n * importedProduct->GetAmount()*1000;
                              theOutput->AddIndicator(environmentalIndicator,"35.01 N from imported feed","kg N",NImported);
          							CImported=((feedItem*) importedProduct)->GetC_content() * importedProduct->GetAmount()*1000;
                              theOutput->AddIndicator(environmentalIndicator,"45.01 C from imported feed","kg C",CImported);
                              //this needs changing if we want to import roughage other than grass!
                              if (strcmp(importedProduct->GetName(),"FEEDRYEGRASS")==0 || strcmp(importedProduct->GetName(),"FEEDGRASSCLOVER")==0)
                              	theOutput->AddIndicator(economicIndicator,"19.52 Imported roughage","t",importedProduct->GetAmount());
                              else
                              	theOutput->AddIndicator(economicIndicator,"19.51 Imported cereals","t",importedProduct->GetAmount());
                              break;
         case manureObj     : strcpy(indstr,"06.16 Costs imported manure");
                              theOutput->AddIndicator(economicIndicator,"19.50 Imported manure","t",importedProduct->GetAmount());
                              NImported=((manure* ) importedProduct)->GetAllN().n* importedProduct->GetAmount()*1000;
                              theOutput->AddIndicator(environmentalIndicator,"35.02 N from imported manure","kg N",NImported);
                              CImported=((manure* ) importedProduct)->GetC_content()* importedProduct->GetAmount()*1000;
                              theOutput->AddIndicator(environmentalIndicator,"45.02 C from imported manure","kg C",CImported);
                              break;
         case animalObj     : if ((strcmp(nameStr,"SUCKLINGPIG")==0)||(strcmp(nameStr,"PIGLETS")==0)||(strcmp(nameStr,"BACONERS")==0)
         									||(strcmp(nameStr,"SOWS")==0)||(strcmp(nameStr,"NEWSOWS")==0))
         							{
                              	NImported=((animalProduct*)importedProduct)->GetN_content()* importedProduct->GetAmount()*1000.0;
	         							theOutput->AddIndicator(environmentalIndicator,"34.04 N in bought pigs","kg N",NImported);
                              	CImported=((animalProduct*)importedProduct)->GetC_content()* importedProduct->GetAmount()*1000.0;
	         							theOutput->AddIndicator(environmentalIndicator,"44.04 C in bought pigs","kg C",CImported);
											if (strcmp(nameStr,"SUCKLINGPIG")==0)
                                 {
                                 	strcpy(indstr,  "06.28 Weaners bought");
											   theOutput->AddIndicator(economicIndicator,"19.02 Bought weaners","no",((animalProduct*)aProduct)->GetNumber());
                                 }
											if (strcmp(nameStr,"PIGLETS")==0)
                                 {
                                 	strcpy(indstr,  "06.29 Piglets bought");
											   theOutput->AddIndicator(economicIndicator,"19.03 Bought piglets","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                                 if (strcmp(nameStr,"BACONERS")==0)
                                 {
                                 	strcpy(indstr,"06.30 Baconers bought");
											   theOutput->AddIndicator(economicIndicator,"19.04 Bought baconers","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                                 if ((strcmp(nameStr,"SOWS")==0)||(strcmp(nameStr,"NEWSOWS")==0))
                                 {
                                     strcpy(indstr,"06.31 Sows bought");
			                            theOutput->AddIndicator(economicIndicator,"19.01 Bought sows","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                              }
										if ((strcmp(nameStr,"HEIFERS1")==0)||(strcmp(nameStr,"HEIFERS2")==0)||(strcmp(nameStr,"HEIFERS3")==0)||(strcmp(nameStr,"DAIRY")==0)
										     || (strcmp(nameStr,"FBEEF1")==0)||(strcmp(nameStr,"FBEEF2")==0)||(strcmp(nameStr,"FBEEF3")==0)||(strcmp(nameStr,"SUCKLER")==0)||
                                 		(strcmp(nameStr,"MBEEF1")==0)||(strcmp(nameStr,"MBEEF2")==0)||(strcmp(nameStr,"MBEEF3")==0))
                              {
                              	NImported=((animalProduct*)importedProduct)->GetN_content()* importedProduct->GetAmount();
         								theOutput->AddIndicator(environmentalIndicator,"33.03 N in bought cattle","kg N",NImported);
                              	CImported=((animalProduct*)importedProduct)->GetC_content()* importedProduct->GetAmount();
         								theOutput->AddIndicator(environmentalIndicator,"43.03 C in bought cattle","kg C",CImported);
                                 if ((strcmp(nameStr,"HEIFERS1")==0) || (strcmp(nameStr,"HEIFERS2")==0))
                                 {
                                     strcpy(indstr,"06.50 Young 1st year heifers bought");
	                                  theOutput->AddIndicator(economicIndicator,"19.30 Bought 1st year heifers","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                                 if (strcmp(nameStr,"HEIFERS3")==0)
                                 {
                                     strcpy(indstr,"06.51 Young 2nd year heifers bought");
	                                  theOutput->AddIndicator(economicIndicator,"19.31 Bought 2nd year heifers","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                                 if (strcmp(nameStr,"DAIRY")==0)
                              	{
                                     strcpy(indstr,"06.52 Dairy cattle bought");
   	                               theOutput->AddIndicator(economicIndicator,"19.32 Bought dairy cattle","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                                 if ((strcmp(nameStr,"FBEEF1")==0)||(strcmp(nameStr,"FBEEF2")==0)||(strcmp(nameStr,"FBEEF3")==0)||
                                 		(strcmp(nameStr,"MBEEF1")==0)||(strcmp(nameStr,"MBEEF2")==0)||(strcmp(nameStr,"MBEEF3")==0))
                                 {
                                     strcpy(indstr,"06.54 Beef cattle calves bought");
	                                  theOutput->AddIndicator(economicIndicator,"19.33 Bought beef calves","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                                 if (strcmp(nameStr,"SUCKLER")==0)
                                 {
                                     strcpy(indstr,"06.55 Mature beef cattle bought");
	                                  theOutput->AddIndicator(economicIndicator,"19.34 Bought beef cattle","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                              }
                              break;

         default	          : if(strcmp(importedProduct->GetName(),"VET")==0)
                                 strcpy(indstr,"06.33 Cost of vet and production control");
      }
	}

	aProduct->Setamount(0);
   if (imported>0.0)
	   *aProduct + *importedProduct;
	delete importedProduct;                        // JB added this

   if (cost>0.0)
	   theOutput->AddIndicator(economicIndicator,indstr,"Dkr",-cost);
   delete []  indstr;
   return imported;
}

/**
 * Find a product with the same name as the input product and replace it with input.
 * It will hold nothing
 */
void products::GiveProductInformation(product* aProduct)
{
	product* theProduct;
	theProduct= GetProductElement(aProduct->Getname());
	*aProduct=*theProduct;
	aProduct->Setamount(0.0);
}

/****************************************************************************\
\****************************************************************************/
linkList <product> * products::GetproductLists()
{
	return theProductList;
}

/****************************************************************************\
\****************************************************************************/
void products::AddElementPriceList( price* aPrice)
{
	ExistsPrice(aPrice);
	thePriceList->InsertLast(aPrice);
}

/****************************************************************************\
\****************************************************************************/
void products::AddElementProductList(product* aProduct)
{
	ExistsProduct(aProduct);
	theProductList->InsertLast(aProduct);
}


/**
 * find a product with a given name. If it cannot be found a null price will be returned
 */
product* products::GetProductPtr(string aProductName)
{
	product* aProduct;
	bool found=false;
	linkList <product>::PS aProductNode;
	theProductList->PeekHead(aProductNode);
	while (aProductNode!=NULL && !found)
   {
		aProduct=(product*) (aProductNode->element);
		if ( ((aProduct->Getname())==aProductName))
		found=true;
		else
		 	aProductNode=aProductNode->next;
   }

 	if(!found)
		return NULL;
	else
   	return aProduct;
}

/**
 * find a product with a given name. If the product does not exist fasset will halt
 */
product* products::GetProductElement( const  char* ident)
{
	string aProductName;
	aProductName=ident;
	return GetProductElement(aProductName);
}

/**
 * find a product with a given name. If the product does not exist fasset will halt
 */
product* products::GetProductElement(string aProductName)
{
	product* aProduct;

	aProduct=GetProductPtr(aProductName);
	if(aProduct==NULL)
   	theMessage->FatalError("products::GetProductElement - Cannot find product <",(char*)aProductName.c_str(),">");

	return aProduct;
}

/**
 * finds a price with a given name. If it cannot be found a null price will be returned
 */
price* products::GetPricePtr(string aPriceName)
{
 	price* aPrice;
	bool found=false;
	linkList <price>::PS aPriceNode;
	thePriceList->PeekHead(aPriceNode);
	while (aPriceNode!=NULL && !found)
	{
   	aPrice=(price*) (aPriceNode->element);
	  	if ( ((aPrice->Getname())==aPriceName))
			found=true;
	   else
      	aPriceNode=aPriceNode->next;
   }
   if(!found)
   	return NULL;
	else
   	return aPrice;
}

/**
 * return a price with a given name. If the prices does not exist fasset will halt
 */
price* products::GetPriceElement( const  char* ident)
{
	price* aPrice;
   string aPriceName;
   aPriceName=ident;
   aPrice=GetPriceElement(aPriceName);
   return aPrice;
}

/**
 * return a price with a given name. If the prices does not exist fasset will halt
 */
price* products::GetPriceElement(string aPriceName)
{
	price* aPrice;
	aPrice=GetPricePtr(aPriceName);
   if(aPrice==NULL)
   	theMessage->FatalError("products::GetPriceElement - Cannot find price <",(char*)aPriceName.c_str(),">");
	return aPrice;
}

/**
 * Checks if a price has already been added. It is compared by object and by name.
 * If the priec has been added fasset will halt
 */
void products::ExistsPrice(price* somePrice)
{
	price* aPrice;
  	bool found=false;
  	linkList <price>::PS aPriceNode;
  	thePriceList->PeekHead(aPriceNode);
   while (aPriceNode!=NULL && !found)
   {
		aPrice=(price*) (aPriceNode->element);
		if (somePrice==aPrice)
		{
		found=true;
			theMessage->FatalError("products::ExistsPrice - Object <",(char*)(somePrice->Getname()).c_str(), "> exists allready as a reference");
      }
      if ( (aPrice->Getname())==(somePrice->Getname()))
      {
      	found=true;
			theMessage->FatalError("products::ExistsPrice - Object named <",(char*)(somePrice->Getname()).c_str(), "> exists allready");
      }
      aPriceNode=aPriceNode->next;
   }
}

/**
 * Checks if a products has already been added. It is compared by object, by name and feedcode.
 * If the product has been added fasset will halt
 *
 */
void products::ExistsProduct(product* someProduct)
{
	product* aProduct;
	bool found=false;
	linkList <product>::PS aProductNode;
  	theProductList->PeekHead(aProductNode);
	while (aProductNode!=NULL && !found)
   {
		aProduct=(product*) (aProductNode->element);
		if (someProduct==aProduct)
      	theMessage->FatalError("products::ExistsProduct - Product <",(char*)(someProduct->Getname()).c_str(), "> exists Allready as a reference");
      if (aProduct->GetObjType()==feedItemObj)
      {
      	if ((((feedItem*) aProduct)->GetCode())==(((feedItem*)someProduct)->GetCode()))
         {
         	char Indexstr[10];
			   sprintf(Indexstr,"(%d)",((feedItem*)someProduct)->GetCode());
	   		theMessage->FatalError("products::ExistsProduct - Product <",Indexstr, "> exists Allready with the same code");
         }
      }
      else if ((aProduct->Getname())==(someProduct->Getname()))
   			theMessage->FatalError("products::ExistsProduct - Product <",(char*)(someProduct->Getname()).c_str(), "> exists Allready with the same name");
      aProductNode=aProductNode->next;
	}
}

/**
 * update all the prices
 */
void products::YearlyUpdate()
{
   simulationYear++;


	linkList <price>::PS aPriceNode;
	thePriceList->PeekHead(aPriceNode);
	while (aPriceNode!=NULL)
   {
		price* aPrice;
		aPrice=(price*) (aPriceNode->element);

		aPrice->UpdatePrices(simulationYear);
		aPriceNode=aPriceNode->next;
	}
}

/**
 * find a matching price for each products
 */
void  products::UpdateProductPtr(void)
{
   linkList <product>::PS aProductNode;
   theProductList->PeekHead(aProductNode);

   while (aProductNode!=NULL)
   {
   	product* aProduct;
      aProduct=(product*)(aProductNode->element);
	   price* aPrice;

	 	aPrice=GetPricePtr(aProduct->Getname());
		if (aPrice!=NULL)
      	aProduct->SetaPrice(aPrice);
      aProductNode=aProductNode->next;  // BMP
   }
}

/**
 * print out all information about all products
 */
void products::ShowProducts(ostream *os)
{
	linkList <product>::PS aProductNode;
	theProductList->PeekHead(aProductNode);

	while (aProductNode!=NULL)
	{
		product* aProduct;
		aProduct=(product*) (aProductNode->element);

		aProduct->showstats(os);
		*os << endl;
		aProductNode=aProductNode->next;
   }




}



/**
 * sets the amount in each products
 */
void products::resetProducts(void)
{
	linkList <product>::PS aProductNode;
	theProductList->PeekHead(aProductNode);

	while (aProductNode!=NULL)
   {
		product* aProduct;
		aProduct=(product*) (aProductNode->element);
	   aProduct->Setamount(0.0);
		aProductNode=aProductNode->next;
   }


}

/**
 * returns a link list of products with the same type as ObjType
 */
linkList <product> * products::GetSubsetofProductList(int ObjType)
{
	linkList <product> * aSubsetOfProdList;
	aSubsetOfProdList = new linkList<product>;

	linkList <product>::PS aProductNode;
	theProductList->PeekHead(aProductNode);

	while (aProductNode!=NULL)
   {
		product* aProduct;
		aProduct=(product*) (aProductNode->element);
      if ( (aProduct->GetObjType()==ObjType))
      {
      	product* aCopyOfaProduct;
         aCopyOfaProduct=aProduct->clone();
         aSubsetOfProdList->InsertLast(aCopyOfaProduct);
      }
		aProductNode=aProductNode->next;
   }
	return aSubsetOfProdList;
}


/**
 * Returns copy of a product if it is in a store, otherwise return NULL
 */
product* products::GetCopyStoredFeedProduct(product * aProduct)
{
#ifndef _STANDALONE
	#ifndef __ECOSYSTEM__
	product * p;
	p=theBuildings->GetCopyStoredFeedProduct(aProduct);
   return p;
   #endif
#else
	return NULL;
#endif
}

/**
 * Reads feed data from cattle feed tables (file must be output from Excel as tab-delimited text
 * Cannot use comma-delimited format as the first field often contains commas
 * NJH 21.06.00
 */
void products::InitializeFeed(string FeedFileName)
{
 char buffer[2000];
 if (OpenInputFileString(FeedFileName))
 {
   file->seekg(0);
   file->getline(buffer,2000,'\n');	//read file header and ignore it
   file->getline(buffer,2000,'\n');	//read file header and ignore it
   while (*file)
   {
		feedItem * temp_resource = new feedItem();
      if (temp_resource->ReadParams(file))
	      AddElementProductList(temp_resource);
      else
   		delete temp_resource;
	} //while
	CloseInputFile();
 } //if file ok
 else
  theMessage->FatalError("products::InitializeCattleFeed - file not found:",FeedFileName.c_str());
}

/**
 * Returns pointer to cattlefeed, based on feed code
 */
product* products::GetProductElement(int feedCode)
{
	product* aProduct;

	bool found=false;
	linkList <product>::PS aProductNode;
	theProductList->PeekHead(aProductNode);
	while (aProductNode!=NULL && !found)
   {
		aProduct=(product*) (aProductNode->element);
      if (aProduct->GetObjType()==feedItemObj)
			if (((feedItem*) aProduct)->GetCode()==feedCode)
   	   	found=true;
	 	aProductNode=aProductNode->next;
   }
	if(!found)
   {
        return NULL;
   }
	return aProduct;
}

/**
 * Intepret standard feed code from crop or crop mixture
 */
int products::GetHarvestProductCode(linkList<plantItem>* HarvestList)
{
   int productCode=0;
   bool barley=false, pea=false, grass=false, clover=false, maize=false,
        rape=false, rye=false, oat=false, beet=false, wheat=false, potatoes=false,
        beettop=false, wheatstraw=false, barleystraw=false, rapestraw=false,oatstraw=false,
   	  ryestraw=false, peastraw=false, maizestraw=false, potatotop =false, barleypea=false,
        soybean = false, soybeanstraw = false, sunflower = false, sunflowerstraw = false,
         lucerne = false  //CK,
         //CK lucernestraw = false
         ;
   bool makeSilage=false;
   bool makeHay=false;
   string s;

	for (int i=0;i<HarvestList->NumOfNodes();i++)
   {
      plantItem *HarvestItem = HarvestList->ElementAtNumber(i);
      if (HarvestItem==NULL)
         theMessage->FatalError("products::GetHarvestProductCode - about to call an object with a NULL pointer");
      if (HarvestItem->GetCode()==1)
         	makeSilage=true;
      if (HarvestItem->GetCode()==2)
         	makeHay=true;
      const char* cid = HarvestItem->GetName();
      if ((strcmp(cid,"WHEAT")==0)||(strcmp(cid,"WNWHT")==0)||(strcmp(cid,"SPWHT")==0))     //SPWHT added MEL 2009
         wheat=true;
      if ((strcmp(cid,"BARLEY")==0)||(strcmp(cid,"WNBRL")==0))
         barley = true;
      if ((strcmp(cid,"RAPE")==0)||(strcmp(cid,"WNRPE")==0) || (strcmp(cid,"SPRAPE")==0))
         rape = true;
      if (strcmp(cid,"OAT")==0)
         oat = true;
      if (strcmp(cid,"RYE")==0)
         rye = true;
      if (strcmp(cid,"PEA")==0)  //pea crop (not whole-crop)
         pea=true;
      if (strcmp(cid,"MAIZE")==0)
         maize = true;
      if (strcmp(cid,"BEET")==0)
         beet=true;
      if (strcmp(cid,"POTATO")==0)
         potatoes=true;
      if (strcmp(cid,"CLOVER")==0)
         clover = true;
      if (strcmp(cid,"GRASS")==0 ||strcmp(cid,"RYEGRASS")==0 ||strcmp(cid,"ITALRYEG")==0)
         grass=true;
      if (strcmp(cid,"CLOVERGRASS")==0)
      {
         grass=true;
         clover=true;
      }
      if (strcmp(cid,"BEETTOP")==0)
      	beettop=true;
      if (strcmp(cid,"WHEATSTRAW")==0)
      	wheatstraw=true;
      if (strcmp(cid,"BARLEYSTRAW")==0)
         barleystraw=true;
      if (strcmp(cid,"RAPESTRAW")==0)
         rapestraw=true;
      if (strcmp(cid,"OATSTRAW")==0)
         oatstraw=true;
      if (strcmp(cid,"RYESTRAW")==0)
         ryestraw=true;
      if (strcmp(cid,"PEASTRAW")==0)
         peastraw=true;
      if (strcmp(cid,"MAIZESTRAW")==0)
         maizestraw=true;
      if (strcmp(cid,"POTATOTOP")==0)
         potatotop=true;
      if (strcmp(cid,"BARLEYPEA")==0)
         barleypea=true;
      if (strcmp(cid,"SOYBEAN")==0)
         soybean=true;
      if (strcmp(cid,"SOYBEANSTRAW")==0)
         soybeanstraw=true;
      if (strcmp(cid,"SUNFLOWER")==0)
         sunflower=true;
      if (strcmp(cid,"SUNFLOWERSTRAW")==0)
         sunflowerstraw=true;
      if (strcmp(cid,"LUCERNE")==0)
         lucerne=true;
      //CK if (strcmp(cid,"LUCERNESTRAW")==0)
      //CK    lucernestraw = true;
      s=cid; // For error checking
   }
   // NJH May 2009 all 'cereal && cerealstraw' changed to 'cereal || cerealstraw'. Forage harvesting creates only one product (usually the straw)
   //initially assume single crop
   if (wheat) productCode = 203;
   if (wheatstraw)
   {
      if (makeSilage)
      {
         productCode = 590;

      }
      else
      	if (HarvestList->NumOfNodes()==1)
		      productCode = 788;
         else
      		theMessage->FatalError("products::whole-crop wheat not in feed table"); //NJH May 2009, just in case
   }
   if (barley) productCode = 201;
   if (barleystraw) productCode = 781;
   if (barley && barleystraw)
   {
      if (makeSilage)
      {
         productCode = 583;

      }
      else
	   	productCode = 483;
   }
   if (pea) productCode = 216;
   if (peastraw) productCode = 799;
   if (pea && peastraw)
   {
      if (makeSilage)
      {
         productCode = 478;

      }
      else
	   	productCode = 499;
   }
   if (rape) productCode = 213;
   if (rapestraw) productCode = 781;  //should be something else but cannot find rape straw in feed table
/*   if (rape && rapestraw)
   {
      if (makeSilage)
         productCode = 478;
      else
	   	productCode = 499;
   }*/
   if (oat) productCode = 202;
   if (beet) productCode = 385;
   if (beettop) productCode = 353;
   if (rye) productCode = 207;
   if (grass)
   {
      if (makeSilage)
      {
         productCode = 565;

		}
      else
	      if (makeHay)
         {
		   	productCode = 665;

		   }
         else
         {
		   	productCode = 465;

		   }
   }
   if (lucerne)
   {
      if (makeSilage)
      {
         productCode = 501;

		}
      else
	      if (makeHay)
         {
		   	productCode = 602;

		   }
         else
         {
		   	productCode = 402;

		   }
   }
   if ((maize) && (!makeSilage)) productCode = 204;
   if ((maizestraw) && (!makeSilage)) productCode = 786;
   if ((maize && maizestraw)&& (makeSilage)) // BMP changed from "&&" May 2007.  NJH changed it again May 2009
   {
	   	productCode = 593;


//      else
  //	   	productCode = 786;
   }
   if (potatoes) productCode = 395;
   if (oatstraw) productCode = 785;
   if (ryestraw) productCode = 796;

   if (clover)
      productCode = 473;

   if (grass && clover)
   {
      if (makeSilage)
   	{
         productCode = 525;

      }
      else
	      if (makeHay)
         {
		   	productCode = 625;

		   }
         else
         {
	 	   	productCode = 423;

         }
   }

   if ((barley && pea) || barleypea)
   {
      if (makeSilage)
      {
         productCode = 596;

      }
      else
	   	productCode = 483;
   }
   if (potatotop)
   	productCode = 282;   //actually is beet top; do not have potato top in feed tables (but who is looking....?)

   if (soybean) productCode = 214;
   if (soybeanstraw) productCode = 155;

   if (sunflower) productCode = 217;
   if (sunflowerstraw) productCode = 165;


   return productCode;
}


