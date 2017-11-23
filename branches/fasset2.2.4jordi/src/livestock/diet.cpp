/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// Class: diet

#include "../base/common.h"
#include "diet.h"
#include "../products/feedItem.h"
#include "../products/products.h"

#ifdef _STANDALONE
	#include "../base/message.h"
#endif

// Functions for class diet
/**
 * constructor
 */
diet::diet(const char * aName, const int aIndex, const base * aOwner)
	: base(aName, aIndex, aOwner)
{
	startDate = new bsTime();
   normalfeedItemList = new cloneList <product>;
   deficiencyfeedItemList= new cloneList <product>;
   surplusfeedItemList= new cloneList <product>;
   grazedItem = NULL;
   housed = 0;
   propRoughage = 0.0;
   propGrazed = 0.0;
   zeroGrazing=false;
}


/**
 * clone
 */
diet* diet::clone() const
{
	diet * aDiet = new diet();
	aDiet->startDate=startDate->clone();
	aDiet->normalfeedItemList=normalfeedItemList->clone();
	aDiet->deficiencyfeedItemList=deficiencyfeedItemList->clone();
	aDiet->surplusfeedItemList=surplusfeedItemList->clone();
   aDiet->grazedItem = (feed_resource *) grazedItem->clone();
	aDiet->housed = housed;
   aDiet->zeroGrazing=zeroGrazing;
   return aDiet;
}

/**
 * destructor
 */
diet::~diet()
{
	delete startDate;
   if (normalfeedItemList) delete normalfeedItemList;
   if (deficiencyfeedItemList) delete deficiencyfeedItemList;
   if (surplusfeedItemList) delete surplusfeedItemList;
   delete grazedItem;
}
/**
 * Empty Class. Why do we have it?
 */
void diet::DailyUpdate()
{
}

/**
 *  Initialize diet from files
 */
void diet::ReadParameters(fstream * infile, bool isAutoFeed)
{
 Setfile(infile);
 SetCritical();
 Setcur_pos(0);
 autoFeed = isAutoFeed;
 FindSection(GetName(),GetIndex());  // find the correct place in the input file
 int day,month;
 int year = theTime.GetYear();
 GetParameter("Day",&day);
 GetParameter("Month",&month);
 //input expected grazing intake (if appropriate)
 UnsetCritical();
 bool gotGrazing = false;
 double grazing=0.0;
 gotGrazing=GetParameter("Grazing",&grazing);   //get amount DM grazed in kg
 double grazingSFU=0.0;
 if (!gotGrazing)
	 gotGrazing=GetParameter("GrazingSFU",&grazingSFU);
 zeroGrazing=false;
 SetCritical();
 if (!gotGrazing)
 {
	GetParameter("propRoughage",&propRoughage);
   gotGrazing=GetParameter("propGrazed",&propGrazed); //
   if (propGrazed==-1)
   	zeroGrazing=true;
 }
 if (!gotGrazing)
   theMessage->FatalError("cattle diet: grazing amount cannot be input as both amount and SFUs");
 if ((grazing>0.0)||(grazingSFU>0.0)|| (propGrazed>0.0) ||zeroGrazing)
 {
    int feedCode;
	 feedItem* aFeedItem;
    string theItemName;
	 UnsetCritical();
    feedCode=0;
    GetParameter("FeedCode",&feedCode);
	 GetParameter("Item",&theItemName);
    if ((feedCode==0)&&(theItemName==""))
      theMessage->FatalError("cattle diet: grazed feed type not defined by name or feed code");
    if (feedCode>0)
      aFeedItem = (feedItem*) theProducts->GetProductElement(feedCode); //get default product info using feed code
    else
      aFeedItem = (feedItem*) theProducts->GetProductElement(theItemName); //get default product info using name
    grazedItem= new feed_resource(*aFeedItem);
    grazedItem->SetObjType(feedItemObj);
    if (autoFeed == true)
	    grazedItem->Setamount(-1.0);   //will be overwritten later if autoFeed is true
	 else if (grazingSFU>0)
    {
      grazing = grazingSFU/aFeedItem->GetfeedUnitsPerItemUnit();  //convert to tonnes FW
    	grazedItem->Setamount(grazing);
    }
    else
    	grazedItem->Setamount(grazing);
    grazedItem->SetIsSupplement(false);
	 SetCritical();
 }

 GetParameter("Housed",&housed);
 startDate->SetTime(day, month, year);
 string namestr;
 char Indexstr[10];
 sprintf(Indexstr,"(%d)",Index);
 namestr=(string) GetName()+Indexstr+"."+"Normal.Supplement";
 int first,num;
 UnsetCritical();
 if (!autoFeed)
	 GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of supplements available in this period
 else
 {
	 first=0;
    num=2;
 }
 SetCritical();
 for(int inx=first;inx<(first+num);inx++)
 {
    FindSection(namestr.c_str(),inx);  // find the correct place in the input file
    normalfeedItemList->InsertLast(LoadFeedItem(autoFeed)); //load feed items into the feed item list
 }
 UnsetCritical();
 namestr=(string) GetName()+Indexstr+"."+"Deficiency.Supplement";
 GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of deficiency supplements
 for(int inx=first;inx<(first+num);inx++)
 {
	 FindSection(namestr.c_str(),inx);  // find the correct place in the input file
    deficiencyfeedItemList->InsertLast(LoadFeedItem(autoFeed));       //load feed items into the feed item list
 }
 namestr=(string) GetName()+Indexstr+"."+"Surplus.Supplement";
 GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of surplus supplements
 for(int inx=first;inx<(first+num);inx++)
 {
	 FindSection(namestr.c_str(),inx);  // find the correct place in the input file
    surplusfeedItemList->InsertLast(LoadFeedItem(autoFeed));       //load feed items into the feed item list
 }

 SetCritical();
 Setfile(NULL);
}
/**
 * Grazing or not
 */
bool diet::IsGrazing()
{
 if (grazedItem)
 {
 	if (grazedItem->GetAmount()>0.0)
  	  return true;
   else
	  return false;
 }
 else
 	return false;
};

/*
 * Loads the amount of each feed, finds the information about feed composition and convert from feedItem to feed_resource
 */
feed_resource* diet::LoadFeedItem(bool autoFeed)
{
 feed_resource* feed_resourceClone=new feed_resource();
 feedItem* aFeedItem;
 double amount=-1.0;
 string theItemName;
 int feedCode=0;
 double SFUamount=-1.0;
 UnsetCritical();
 GetParameter("Item",&theItemName);
 GetParameter("Amount",&amount);
 GetParameter("SFUamount",&SFUamount);
 GetParameter("FeedCode",&feedCode);
 SetCritical();
 if ((SFUamount>0)&&(amount>0))
   theMessage->FatalError("cattle diet: feed amount cannot be input as both amount and SFUs");
 if ((SFUamount==-1.0)&&(amount==-1.0)&&(!autoFeed))
   theMessage->FatalError("cattle diet: feed amount not specified as amount fresh weight or amount of SFUs");
 if ((feedCode==0)&&(theItemName==""))
   theMessage->FatalError("cattle diet: feed type not defined by name or feed code");

 if (feedCode>0)
   aFeedItem = (feedItem*) theProducts->GetProductElement(feedCode); //get default product info using feed code
 else
   aFeedItem = (feedItem*) theProducts->GetProductElement(theItemName); //get default product info using name
 if (!autoFeed)
 {
 	if (SFUamount>0)
   	amount = SFUamount/aFeedItem->GetfeedUnitsPerItemUnit();  //convert to tonnes FW
   else
   	amount /= (aFeedItem->GetdryMatter() * 1000.0);
 }
 else
 	amount = -1.0;
 if ((amount>0.0)||(autoFeed))
 {
    feed_resourceClone = new feed_resource(*aFeedItem); // LEAK (bmp) !!!
    feed_resourceClone->SetObjType(feedItemObj);
    feed_resourceClone->Setamount(amount);
    feed_resourceClone->SetIsSupplement(true);
 }
 else theMessage->FatalError("cattle diet: feed amount cannot be zero");
 feed_resourceClone->GotItAll();
 return feed_resourceClone;
};
/*
 * Get grazed feed required (kg DM/day)
 */
double diet::GetGrazingFeedRequirement(bool zeroGrazed)
{
 double grazedFeedReq = 0.0;
 if (grazedItem && (zeroGrazed==zeroGrazing))
 	grazedFeedReq=grazedItem->GetAmount() * grazedItem->GetdryMatter();
 return grazedFeedReq;
};

void diet::ShowDiet(fstream * fs, bool header)
{
	if (header)
   {
   	*fs << "Date" << "\t";
      if (grazedItem)
			*fs << "Code" << "\t" << "DM" << "\t" << "FE" << "\t" << "ME" << "\t";
		*fs <<  "housed" << "\t" <<  "propRoughage" << "\t" <<  "propGrazed" << "\t";
      for (int i=0; i<normalfeedItemList->NumOfNodes();i++)
			*fs << "Code" << "\t" << "DM" << "\t" << "FE" << "\t" << "ME" << "\t";
      if (deficiencyfeedItemList->NumOfNodes()>0)
      {
         for (int i=0; i<deficiencyfeedItemList->NumOfNodes();i++)
			*fs << "Code" << "\t" << "DM" << "\t" << "FE" << "\t" << "ME" << "\t";
      }
      if (surplusfeedItemList->NumOfNodes()>0)
      {
         for (int i=0; i<surplusfeedItemList->NumOfNodes();i++)
			*fs << "Code" << "\t" << "DM" << "\t" << "FE" << "\t" << "ME" << "\t";
      }
   }
	else
   {
      *fs << startDate->GetYear() << "-" << startDate->GetMonth() << "-" << startDate->GetDay() << "\t";
      if (grazedItem)
      {
         *fs << grazedItem->GetCode()<< "\t";
         *fs << 1000.0 * grazedItem->GetAmount()* grazedItem->GetdryMatter()<< "\t";
         *fs << grazedItem->GetAmount() * grazedItem->GetfeedUnitsPerItemUnit()<< "\t";
         *fs << grazedItem->GetAmount() * grazedItem->GetME()<< "\t";
      }
      else
         *fs << "0" << "\t"<< "0.0" << "\t"<< "0.0" << "\t"<< "0.0" << "\t";
      *fs <<  housed << "\t";
      *fs <<  propRoughage << "\t";
      *fs <<  propGrazed << "\t";
      for (int i=0; i<normalfeedItemList->NumOfNodes();i++)
      {
         feedItem * aFeedItem= ((feedItem *)normalfeedItemList->ElementAtNumber(i));
         *fs << aFeedItem->GetCode()<< "\t";
         *fs << 1000.0 * aFeedItem->GetAmount()* aFeedItem->GetdryMatter()<< "\t";
         *fs << aFeedItem->GetAmount() * aFeedItem->GetfeedUnitsPerItemUnit()<< "\t";
         *fs << aFeedItem->GetAmount() * aFeedItem->GetME()<< "\t";
      }
      if (deficiencyfeedItemList->NumOfNodes()>0)
      {
         for (int i=0; i<deficiencyfeedItemList->NumOfNodes();i++)
         {
            feedItem * aFeedItem= ((feedItem *)deficiencyfeedItemList->ElementAtNumber(i));
            *fs << aFeedItem->GetCode()<< "\t";
            *fs << 1000.0 * aFeedItem->GetAmount()* aFeedItem->GetdryMatter()<< "\t";
            *fs << aFeedItem->GetAmount() * aFeedItem->GetfeedUnitsPerItemUnit()<< "\t";
            *fs << aFeedItem->GetAmount() * aFeedItem->GetME()<< "\t";
         }
      }
      else
        for (int i=0; i<2;i++)
               *fs << "0" << "\t"<< "0.0" << "\t"<< "0.0" << "\t"<< "0.0" << "\t";

      if (surplusfeedItemList->NumOfNodes()>0)
      {
         for (int i=0; i<surplusfeedItemList->NumOfNodes();i++)
         {
            feedItem * aFeedItem= ((feedItem *)surplusfeedItemList->ElementAtNumber(i));
            *fs << aFeedItem->GetCode()<< "\t";
            *fs << 1000.0 * aFeedItem->GetAmount()* aFeedItem->GetdryMatter()<< "\t";
            *fs << aFeedItem->GetAmount() * aFeedItem->GetfeedUnitsPerItemUnit()<< "\t";
            *fs << aFeedItem->GetAmount() * aFeedItem->GetME()<< "\t";
         }
      }
      else
        for (int i=0; i<2;i++)
               *fs << "0" << "\t"<< "0.0" << "\t"<< "0.0" << "\t"<< "0.0" << "\t";
   }
}




/**
 * If normalfeedItemList and aList have the same item that item in normalfeedItemList's amount would be updated
 */
void diet::GetRoughageRequired(linkList <product> * aList, double numberOfAnimals)
{
//	double amount;
   for(int i=0;i<aList->NumOfNodes();i++)
   {
	   product * aProduct = aList->ElementAtNumber(i);
      for(int j=0;j<normalfeedItemList->NumOfNodes();j++)
      {
         product * aRequireProduct = normalfeedItemList->ElementAtNumber(j);
         if (!((feedItem*) aProduct)->GetCode() == ((feedItem*) aRequireProduct)->GetCode())
         	aProduct->Setamount(aProduct->GetAmount()+numberOfAnimals*aRequireProduct->GetAmount());
      }
   }
};
/**
 * return the amount of Silage multiplier by numberOfAnimals
 */
double diet::GetGrassSilageRequired(double numberOfAnimals)
{
	double amount=0.0;
   //product * aProduct;
   for(int j=0;j<normalfeedItemList->NumOfNodes();j++)
   {
      product * aProduct = normalfeedItemList->ElementAtNumber(j);
      if (((feedItem*) aProduct)->GetCode() == 565)
         amount+=numberOfAnimals*aProduct->GetAmount();
   }
   return amount;
};
/*
 * Returns winter silage requirement in tonnes DM
 */
double diet::GetSilageRequired(double numberOfAnimals, int startCode, int endCode)
{
	double amount=0.0;
//   product * aProduct;
   for(int j=0;j<normalfeedItemList->NumOfNodes();j++)
   {
      feedItem * aProduct = ((feedItem*) normalfeedItemList->ElementAtNumber(j));
      int code = aProduct->GetCode();
      if ((code >= startCode) && (code <= endCode))
         amount+=numberOfAnimals*aProduct->GetAmount() * aProduct->GetdryMatter();
   }
   return amount;
};
/**
 * Empty all list
 */
void diet::ClearLists()
{
   if (normalfeedItemList)
   	delete normalfeedItemList;
   if (deficiencyfeedItemList)
   	delete deficiencyfeedItemList;
   if (surplusfeedItemList)
   	delete surplusfeedItemList;
   delete grazedItem;
}


/**
 *
 */
void diet::CreateDefaultDiets(double differentiation)
{
	feedItem *aConcFeed;
	feedItem *aRoughFeed;
   feedItem * afeedItem;
   aRoughFeed = ((feedItem *) FindConservedRoughageFeed());
   aConcFeed = ((feedItem *) FindConcentrateFeed());
	// surplus feed list.
    afeedItem = new feedItem(*aConcFeed);
    afeedItem->SetObjType(feedItemObj);
    afeedItem->Setamount((1-differentiation) *afeedItem->GetAmount());
    surplusfeedItemList->InsertLast(afeedItem); //load feed items into the feed item list
    afeedItem = new feedItem(*aRoughFeed);
    afeedItem->SetObjType(feedItemObj);
    surplusfeedItemList->InsertLast(afeedItem); //load feed items into the feed item list

	// deficiency feed list.
    afeedItem = new feedItem(*aConcFeed);
    afeedItem->SetObjType(feedItemObj);
    afeedItem->Setamount((1+differentiation)*afeedItem->GetAmount());
    deficiencyfeedItemList->InsertLast(afeedItem); //load feed items into the feed item list
    afeedItem = new feedItem(*aRoughFeed);
    afeedItem->SetObjType(feedItemObj);
    deficiencyfeedItemList->InsertLast(afeedItem); //load feed items into the feed item list
};

/**
 * Find the first instance of roughage in normalfeedItemList
 */
product* diet::FindConservedRoughageFeed()
{
   int j=0;
   product * aProduct=new product();
   bool gotRough=false;
   while ((j<normalfeedItemList->NumOfNodes())&&(!gotRough))
   {
      aProduct = normalfeedItemList->ElementAtNumber(j);
      if (((feedItem*) aProduct)->GetfeedCategory() == 2)
         gotRough=true;
      j++;
   }
   if (!gotRough)
    theMessage->FatalError("diet: FindConservedRoughageFeed - feed not found");
   return aProduct;
};
/**
 *  Find the first instance of Concentrate in normalfeedItemList
 */
product* diet::FindConcentrateFeed()
{
   int j=0;
   product * aProduct=new product();
   bool gotConc=false;
   while ((j<normalfeedItemList->NumOfNodes())&&(!gotConc))
   {
      aProduct = normalfeedItemList->ElementAtNumber(j);
      if (((feedItem*) aProduct)->GetfeedCategory() == 1)
         gotConc=true;
      j++;
   }
   if (!gotConc)
    theMessage->FatalError("diet: FindConcentrateFeed - feed not found");
   return aProduct;
};

product* diet::FindGrazedFeed()
{
   if (grazedItem)
   	return grazedItem;
   else
   	return NULL;
};

