/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include <common.h>
#include <message.h>
#include <products.h>
#include <price.h>
#include <product.h>
#include <panimal.h>
#include <pseed.h>
#include <pfertil.h>
#include <pchemic.h>
#include <pwater.h>
#include <penergy.h>
#include <pmanure.h>
#include <pmilk.h>
#include <pdecomp.h>
#include <pfeedit.h>
#include <premium.h>
#include <feed_resource.h>
#ifndef __ECOSYSTEM__
#include <manure_manager.h>

#ifndef _STANDALONE
   #include <thebuild.h>
   #endif
   #include <indicat.h>
#endif

/****************************************************************************\
  Default Constructor
\****************************************************************************/
products::products()
   : base()
{
   theProductSubList = new linkList<product>;
   theProductAddList = new linkList<product>;
   theProductList = new linkList <product>;
   theDemandList = new linkList <product>;
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
   theProductSubList = new linkList<product>;
   theProductAddList = new linkList<product>;
   theProductList   = new linkList <product>;
   theDemandList   = new linkList <product>;
   thePriceList  = new linkList <price>;
   simulationYear = 0;
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
products::products(const products& aproducts)
   : base(aproducts)
{
	theProductSubList = aproducts.theProductSubList;
   theProductAddList = aproducts.theProductAddList;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
products::~products()
{
   delete theProductSubList;   // Added by BMP and PL
	delete theProductAddList;   // Added by BMP and PL
	if(theProductList)
      delete theProductList;
	if(theDemandList)
      delete theDemandList;
	if(thePriceList)
      delete thePriceList;
}

/****************************************************************************\
  Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os, const products& aproducts)
{
  	os << setiosflags(ios::left) << setw(40) << "theProductSubList " << aproducts.theProductSubList << endl;
  	os << setiosflags(ios::left) << setw(40) << "theProductAddList " << aproducts.theProductAddList << endl;
	return os;
}

/****************************************************************************\
  Read product prices
\****************************************************************************/
void products::InitializePrices(char* priceFileName,bsTime* astarttime,  bsTime* astoptime)
{
   int  index;
   char priceSection[81];
   int SimulationInYearDuration = astoptime->GetYear() - astarttime->GetYear();
	int aSimulationLength=SimulationInYearDuration+4;
   SetCritical();
	OpenInputFile(priceFileName);
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
void products::InitializeProducts(char* productFileName)//,bool cattle)
{
   int first,num,index;
   char productSection[81];
   SetCritical();
	OpenInputFile(productFileName);

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
      InitializeFeed("FeedTable2004.dat");
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
\****************************************************************************/
double products::demandOn(product* aProduct)
{
	double rval=0;
	string aName=aProduct->GetName();
   if (aProduct->GetObjType()==plantItemObj)
   	aName=((plantItem*) aProduct)->GetTradeName();
   if (aName.compare("NONE")==0) aName=aProduct->GetName();

	for(int i=0;i<theDemandList->NumOfNodes();i++)
	{
   	string elmName=(theDemandList->ElementAtNumber(i))->GetName();
      string derName=((feedItem*) theDemandList->ElementAtNumber(i))->DerivedFrom();
	   if((aName.compare(elmName)==0) || (aName.compare(derName)==0))
	       	rval+=(theDemandList->ElementAtNumber(i))->GetAmount();
   }
   if (rval>0) rval +=100;                                      // added by JB to improve storage handling
	return rval;
}

/****************************************************************************\
 Function:   addDemand
 Purpose:    adds a demand to the stuff that will be needed
 Parameters: product* aProduct
 Comments:   JB corrected demand to include what is stored
\****************************************************************************/
void products::addDemand(product* aProduct)
{
	string aName=aProduct->GetName();
   bool found=false;
   double demand = aProduct->GetAmount();
#ifndef __ECOSYSTEM__
#ifndef _STANDALONE
   demand -= theBuildings->AmountProductStored(aProduct);
#endif
#endif
   if (demand>0)
   {
		for(int i=0;i<theDemandList->NumOfNodes();i++)
		{
	   	string elmName=(theDemandList->ElementAtNumber(i))->GetName();
	    	string derName=((feedItem*) theDemandList->ElementAtNumber(i))->DerivedFrom();
	    	if((aName.compare(elmName)==0)||(aName.compare(derName)==0))
	    	{
	      	double am=(theDemandList->ElementAtNumber(i))->GetAmount();
		      found=true;
		      am=am+(aProduct->GetAmount());
		      (theDemandList->ElementAtNumber(i))->Setamount(am);
	      }
	   }

	}
   if(!found)
	{
	   product* aProductClone = aProduct->clone();
	   theDemandList->InsertLast(aProductClone);
   }
}

/****************************************************************************\
\****************************************************************************/
void products::subDemand(product* aProduct)
{
	string aName=aProduct->GetName();
   if (aProduct->GetObjType()==plantItemObj)
   	aName=((plantItem*) aProduct)->GetTradeName();
   double am = 0.0;
	for(int i=0;i<theDemandList->NumOfNodes();i++)
	{
   	string elmName=(theDemandList->ElementAtNumber(i))->GetName();
      string derName=(theDemandList->ElementAtNumber(i))->DerivedFrom();
      if((aName.compare(elmName)==0)||(aName.compare(derName)==0))
    	{
      	am=(theDemandList->ElementAtNumber(i))->GetAmount();
      	am=am-(aProduct->GetAmount());
      	(theDemandList->ElementAtNumber(i))->Setamount(am);
      }
   }
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
     case plantItemObj :theBuildings->StoreProduct(aProduct);
     							NSent = aProduct->GetAmount() * ((liveMatter*) aProduct)->GetAllN().n * 1000.0;
                        theOutput->AddIndicator(environmentalIndicator,"35.04 N from harvest to store","kg N",NSent);
     							CSent = aProduct->GetAmount() * ((liveMatter*) aProduct)->GetC_content() * 1000.0;
                        theOutput->AddIndicator(environmentalIndicator,"45.04 C from harvest to store","kg C",CSent);
                        if (aProduct->GetaPrice()!=NULL)
                        {
                        	income=((aProduct->GetAmount())*((aProduct->GetaPrice())->GetactualSellPrice()) );
                        	theOutput->AddIndicator(economicIndicator,"17.01 Gross output of plant production","Dkr",income);
                        }
                        break;
/*     case milkObj:
         theOutput->AddIndicator(environmentalIndicator,"21.34 N in milk","kg N",Number * milkN/1000.0);*/

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
   feedItem * storedItem = NULL;
   productCode=GetHarvestProductCode(HarvestList); //load product info for this product
   if (productCode!=0) //single product was found in feed table
   {
	   storedItem=new feedItem(*((feedItem *)GetProductElement(productCode)));
      plantItem *HarvestItem = HarvestList->ElementAtNumber(0);
      if (HarvestItem->GetAmount()>0)
         storedItem->CopyFromPlantItem(*((plantItem*) HarvestItem), feedCat); //gather the products into one
   }
   else //no single product was found in feed table, so look for one in products list
   {
   	if (HarvestList->NumOfNodes()>1)
	      theMessage->FatalError("products::AddHarvestProducts - one or more unknown product in harvest");
      else
      {
	      plantItem *HarvestItem = HarvestList->ElementAtNumber(0);
      	storedItem = new feedItem();
//      	feedItem *tempFeedItem = new feedItem();
//         tempFeedItem->Setamount(0.0);
			plantItem *aplantItem = ((plantItem*) GetProductPtr(HarvestItem->GetName()));
         if (!aplantItem)
		      theMessage->FatalError("products::Harvested product unknown");
         else
         {
		      if (HarvestItem->GetAmount()>0)
            {
	         	*aplantItem + *((plantItem*)HarvestItem);
		         storedItem->CopyFromPlantItem(*aplantItem, feedCat); //gather the products into one
            }
         }
/*         *storedItem + *tempFeedItem;
//         delete tempFeedItem;
//         if (storedItem)
         {
         	storedItem->CopyFromPlantItem(*((plantItem*) HarvestItem), feedCat);
            //GetProductPtr(HarvestItem->GetName()));
         }
  */
//      cout << "Product: <" << s << ">" << endl;
		}
   }
   if (storedItem->GetAmount()>0)
   {
      storedItem->SetCode(productCode);
      storedItem->SetfeedCategory(feedCat);
   	AddProduct(storedItem);
	   if ((SellthePlantProducts)||((SelltheStrawProducts)&&(storedItem->GetfeedCategory()==4))||
      			((SelltheNonRoughageProducts)&&(storedItem->GetfeedCategory()!=2)))
      	SellPlantItem(storedItem);
   }
  	delete storedItem;
}


/****************************************************************************\
Sells all plant items
\****************************************************************************/
void products::SellPlantItems()
{
#ifndef _STANDALONE
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
}

/****************************************************************************\
Sells a single plant item
\****************************************************************************/
void products::SellPlantItem(product *itemToSell)
{
#ifndef _STANDALONE
	plantItem* aPlantItem = (plantItem*) itemToSell;
   double amountStored, income;
 //  char* indstr;

   string pname=itemToSell->Getname();
   amountStored = theBuildings->AmountProductStored(aPlantItem);
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
		if (pname.compare("SPRPE")==0)  theOutput->AddIndicator(economicIndicator,"05.14 S.rape sold","Dkr",income);
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
   char* indstr="99.99 unspecified object bought";
   if (imported>0.0)
   {
	   if (aProduct->GetObjType()==manureObj)
      {
	      if (importedProduct->GetmainUnitName()==3)	//amount requested is expressed in kg N not t FW, NJH Feb 2009
         {
	         imported = importedProduct->GetAmount()/(((manure *) importedProduct)->GetAllN().n * 1000.0);
            importedProduct->Setamount(imported);
         }
         imported = theManure_manager->GetImportableManure(imported);
      }

	   importedProduct->Setamount(imported);
   	double NImported=0.0, CImported=0.0;
      if (importedProduct->GetaPrice()!=NULL)
         cost=((importedProduct->GetAmount())*((importedProduct->GetaPrice())->GetactualBuyPrice()));

      switch(importedProduct->GetObjType())
      {
         case productObj    : if (strcmp(importedProduct->GetName(),"hiredHours")==0)
                                 indstr="06.70 Costs hired hours";
                              if(strcmp(importedProduct->GetName(),"VET")==0)
                                 indstr="06.33 Cost of vet and production control";
                              break;
         case waterObj      : indstr=""; break;                    // assuming that water has no price!
         case seedObj       : indstr="06.11 Costs seed"; break;
         case fertilizerObj : indstr="06.12 Costs mineral fertilizer"; break;
         case chemicalObj	 : indstr="06.13 Costs chemicals"; break;
         case energyObj		 : if(strcmp(importedProduct->GetName(),"DIESEL")==0)
                                 indstr="06.14 Costs fuel";
                              else
                                 indstr="06.15 Costs electricity";
                              break;
         case plantItemObj  : indstr="06.32 Costs feed and bedding";
                              NImported=((plantItem* ) importedProduct)->GetAllN().n* importedProduct->GetAmount()*1000;
                              theOutput->AddIndicator(environmentalIndicator,"35.03 N from imported bedding","kg N",NImported);
                              CImported=((plantItem* ) importedProduct)->GetC_content()* importedProduct->GetAmount()*1000;
                              theOutput->AddIndicator(environmentalIndicator,"45.03 C from imported bedding","kg C",CImported);
// should be straw registered here                              theOutput->AddIndicator(economicIndicator,"10.20 Imported manure","t",importedProduct->GetAmount());
                              break;
         case feedItemObj	 : indstr="06.32 Costs feed and bedding";
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
         case manureObj     : indstr="06.16 Costs imported manure";
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
                                 	indstr = "06.28 Weaners bought";
											   theOutput->AddIndicator(economicIndicator,"19.02 Bought weaners","no",((animalProduct*)aProduct)->GetNumber());
                                 }
											if (strcmp(nameStr,"PIGLETS")==0)
                                 {
                                 	indstr = "06.29 Piglets bought";
											   theOutput->AddIndicator(economicIndicator,"19.03 Bought piglets","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                                 if (strcmp(nameStr,"BACONERS")==0)
                                 {
                                 	indstr = "06.30 Baconers bought";
											   theOutput->AddIndicator(economicIndicator,"19.04 Bought baconers","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                                 if ((strcmp(nameStr,"SOWS")==0)||(strcmp(nameStr,"NEWSOWS")==0))
                                 {
                                     indstr = "06.31 Sows bought";
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
                                     indstr="06.50 Young 1st year heifers bought";
	                                  theOutput->AddIndicator(economicIndicator,"19.30 Bought 1st year heifers","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                                 if (strcmp(nameStr,"HEIFERS3")==0)
                                 {
                                     indstr="06.51 Young 2nd year heifers bought";
	                                  theOutput->AddIndicator(economicIndicator,"19.31 Bought 2nd year heifers","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                                 if (strcmp(nameStr,"DAIRY")==0)
                              	{
                                     indstr="06.52 Dairy cattle bought";
   	                               theOutput->AddIndicator(economicIndicator,"19.32 Bought dairy cattle","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                                 if ((strcmp(nameStr,"FBEEF1")==0)||(strcmp(nameStr,"FBEEF2")==0)||(strcmp(nameStr,"FBEEF3")==0)||
                                 		(strcmp(nameStr,"MBEEF1")==0)||(strcmp(nameStr,"MBEEF2")==0)||(strcmp(nameStr,"MBEEF3")==0))
                                 {
                                     indstr="06.54 Beef cattle calves bought";
	                                  theOutput->AddIndicator(economicIndicator,"19.33 Bought beef calves","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                                 if (strcmp(nameStr,"SUCKLER")==0)
                                 {
                                     indstr="06.55 Mature beef cattle bought";
	                                  theOutput->AddIndicator(economicIndicator,"19.34 Bought beef cattle","no",((animalProduct*)aProduct)->GetNumber());
                                 }
                              }
                              break;

         default	          : if(strcmp(importedProduct->GetName(),"VET")==0)
                                 indstr="06.33 Cost of vet and production control";
      }
	}

	aProduct->Setamount(0);
   if (imported>0.0)
	   *aProduct + *importedProduct;
	delete importedProduct;                        // JB added this

   if (cost>0.0)
	   theOutput->AddIndicator(economicIndicator,indstr,"Dkr",-cost);
   return imported;
}

/****************************************************************************\
\****************************************************************************/
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

/****************************************************************************\
\****************************************************************************/
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

/****************************************************************************\
\****************************************************************************/
product* products::GetProductElement( const  char* ident)
{
	string aProductName;
	aProductName=ident;
	return GetProductElement(aProductName);
}

/****************************************************************************\
\****************************************************************************/
product* products::GetProductElement(string aProductName)
{
	product* aProduct;

	aProduct=GetProductPtr(aProductName);
	if(aProduct==NULL)
   	theMessage->FatalError("products::GetProductElement - Cannot find product <",(char*)aProductName.c_str(),">");

	return aProduct;
}

/****************************************************************************\
\****************************************************************************/
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


/****************************************************************************\
\****************************************************************************/
price* products::GetPriceElement( const  char* ident)
{
	price* aPrice;
   string aPriceName;
   aPriceName=ident;
   aPrice=GetPriceElement(aPriceName);
   return aPrice;
}

/****************************************************************************\
\****************************************************************************/
price* products::GetPriceElement(string aPriceName)
{
	price* aPrice;
	aPrice=GetPricePtr(aPriceName);
   if(aPrice==NULL)
   	theMessage->FatalError("products::GetPriceElement - Cannot find price <",(char*)aPriceName.c_str(),">");
	return aPrice;
}

/****************************************************************************\
\****************************************************************************/
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

/****************************************************************************\
\****************************************************************************/
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
      else
      	if ((aProduct->Getname())==(someProduct->Getname()))
   			theMessage->FatalError("products::ExistsProduct - Product <",(char*)(someProduct->Getname()).c_str(), "> exists Allready with the same name");
      aProductNode=aProductNode->next;
	}
}

/****************************************************************************\
\****************************************************************************/
void products::YearlyUpdate()
{
   simulationYear++;
   if (theDemandList)
      theDemandList->Reset();

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

/****************************************************************************\
\****************************************************************************/
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

/****************************************************************************\
\****************************************************************************/
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

	linkList <product>::PS aProductAddNode;
	theProductAddList->PeekHead(aProductAddNode);

	while (aProductAddNode!=NULL)
	{
		product* aProduct;
		aProduct=(product*) (aProductAddNode->element);

		aProduct->showstats(os);
		*os << endl;
		aProductAddNode=aProductAddNode->next;
	}

	linkList <product>::PS aProductSubNode;
	theProductSubList->PeekHead(aProductSubNode);

	while (aProductSubNode!=NULL)
	{
		product* aProduct;
		aProduct=(product*) (aProductSubNode->element);

		*os << "********* start of record ***********" << endl;
		*os << "*********    PRODUCT SUB  ***********" << endl;
		aProduct->showstats(os);
		*os << "********* end of record   ***********" << endl << endl;
		aProductSubNode=aProductSubNode->next;
	}
}

/****************************************************************************\
\****************************************************************************/
void products::ShowProducts(ostream * os,linkList <product> * aList)
{
	linkList <product>::PS aProductNode;
	aList->PeekHead(aProductNode);

	while (aProductNode!=NULL)
	{
		product* aProduct;
		aProduct=(product*) (aProductNode->element);

		*os << "********* start of record ***********" << endl;
		*os << "*********    PRODUCT      ***********" << endl;
		aProduct->showstats(os);
		*os << "********* end of record   ***********" << endl << endl;

		aProductNode=aProductNode->next;
	}
}

/****************************************************************************\
\****************************************************************************/
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

	delete theProductAddList;
	delete theProductSubList;
	theProductAddList= new linkList <product>;
	theProductSubList= new linkList <product>;
}

/****************************************************************************\
\****************************************************************************/
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


/****************************************************************************\
Returns copy of a product if it is in a store, otherwise return NULL
\****************************************************************************/
product* products::GetCopyStoredFeedProduct(product * aProduct)
{
#ifndef _STANDALONE
	product * p;
	p=theBuildings->GetCopyStoredFeedProduct(aProduct);
   return p;
#else
	return NULL;
#endif
}

/****************************************************************************\
Reads feed data from cattle feed tables (file must be output from Excel as tab-delimited text
Cannot use comma-delimited format as the first field often contains commas
NJH 21.06.00
\****************************************************************************/
void products::InitializeFeed(char* FeedFileName)
{
 char buffer[2000];
 if (OpenInputFile(FeedFileName))
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
  theMessage->FatalError("products::InitializeCattleFeed - file not found:",FeedFileName);
}

/****************************************************************************\
Returns pointer to cattlefeed, based on feed code
\****************************************************************************/
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
      char outstring[4];
	   sprintf(outstring,"%d",feedCode);
   	theMessage->FatalError("products::GetProductElement - Cannot find feed with code<",outstring,">");
   }
	return aProduct;
}

/**********************************************************************
Intepret standard feed code from crop or crop mixture
*******************************************************************/
int products::GetHarvestProductCode(linkList<plantItem>* HarvestList)
{
   int productCode=0;
   bool barley=false, pea=false, grass=false, clover=false, maize=false,
        rape=false, rye=false, oat=false, beet=false, wheat=false, potatoes=false,
        beettop=false, wheatstraw=false, barleystraw=false, rapestraw=false,oatstraw=false,
   	  ryestraw=false, peastraw=false, maizestraw=false, potatotop =false, barleypea=false,
        soybean = false, soybeanstraw = false, sunflower = false, sunflowerstraw = false,
        lucerne = false, lucernestraw = false;
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
      if (strcmp(cid,"LUCERNESTRAW")==0)
          lucernestraw = true;
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
			BlendProducts(HarvestList,productCode);
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
			BlendProducts(HarvestList,productCode);
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
			BlendProducts(HarvestList,productCode);
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
			BlendProducts(HarvestList,productCode);
		}
      else
	      if (makeHay)
         {
		   	productCode = 665;
				BlendProducts(HarvestList,productCode);
		   }
         else
         {
		   	productCode = 465;
				BlendProducts(HarvestList,productCode);
		   }
   }
   if (lucerne)
   {
      if (makeSilage)
      {
         productCode = 501;
			BlendProducts(HarvestList,productCode);
		}
      else
	      if (makeHay)
         {
		   	productCode = 602;
				BlendProducts(HarvestList,productCode);
		   }
         else
         {
		   	productCode = 402;
				BlendProducts(HarvestList,productCode);
		   }
   }
   if ((maize) && (!makeSilage)) productCode = 204;
   if ((maizestraw) && (!makeSilage)) productCode = 786;
   if ((maize && maizestraw)&& (makeSilage)) // BMP changed from "&&" May 2007.  NJH changed it again May 2009
   {
	   	productCode = 593;
			BlendProducts(HarvestList,productCode);

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
			BlendProducts(HarvestList,productCode);
      }
      else
	      if (makeHay)
         {
		   	productCode = 625;
				BlendProducts(HarvestList,productCode);
		   }
         else
         {
	 	   	productCode = 423;
				BlendProducts(HarvestList,productCode);
         }
   }

   if ((barley && pea) || barleypea)
   {
      if (makeSilage)
      {
         productCode = 596;
			BlendProducts(HarvestList,productCode);
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

void products::BlendProducts(linkList<plantItem>* HarvestList, int feedCode)
{
	linkList <plantItem>::PS aNode;
	HarvestList->PeekHead(aNode);
	plantItem *NewHarvestItem = aNode->element;
	int i=1;
   while (HarvestList->NumOfNodes()>1)
   {
   	HarvestList->PeekTail(aNode);
      plantItem *HarvestItem = aNode->element;
      *NewHarvestItem + *HarvestItem;
      HarvestList->Release(aNode);
   }
   NewHarvestItem->SetCode(feedCode);
}
