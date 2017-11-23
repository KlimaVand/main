/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "feedItem.h"
#include "../base/nixcbits.h"
#include "../base/bstime.h"
#ifdef _STANDALONE
	#include "../base/message.h"
#endif

/****************************************************************************\
  Default Constructor
\****************************************************************************/
feedItem::feedItem()
   : liveMatter()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
feedItem::feedItem(const char * aName, const int aIndex, const base * aOwner)
   : liveMatter(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
feedItem::feedItem(const feedItem& afeedItem)
   : liveMatter(afeedItem)
{
   proteinN_digestibility 			= afeedItem.proteinN_digestibility;
   feedUnitsPerItemUnit 	= afeedItem.feedUnitsPerItemUnit;
   Ca_content 					= afeedItem.Ca_content;
   Na_content 					= afeedItem.Na_content;
   Mg_content 					= afeedItem.Mg_content;
   Cu_content 					= afeedItem.Cu_content;
   Methionin 					= afeedItem.Methionin;
   Threonin 					= afeedItem.Threonin;
   Lysin 						= afeedItem.Lysin;
   fat 							= afeedItem.fat;
   sugar 						= afeedItem.sugar;
   starch 						= afeedItem.starch;
   organic 						= afeedItem.organic;
   chew 							= afeedItem.chew;
   aat 							= afeedItem.aat;
   pbv 							= afeedItem.pbv;
   cell 							= afeedItem.cell;
   fill 							= afeedItem.fill;
   fillU 						= afeedItem.fillU;
//   plantName					= afeedItem.plantName;
   P_digest 					= afeedItem.P_digest;
   MaxPercentage				= afeedItem.MaxPercentage;
   OMD		 					= afeedItem.OMD;
   propRDP				= afeedItem.propRDP;
   propAAinUDP			= afeedItem.propAAinUDP;
   propUDPdigestible		= afeedItem.propUDPdigestible;
   microAAFactor		= afeedItem.microAAFactor;
   CHO_content			= afeedItem.CHO_content;
   propDigestCHO		= afeedItem.propDigestCHO;
   ME 					= afeedItem.ME;
   feedCategory		= afeedItem.feedCategory;
   pigFeedUnitsPerItemUnit=afeedItem.pigFeedUnitsPerItemUnit;

}

/****************************************************************************\
  Destructor
\****************************************************************************/
feedItem::~feedItem()
{
//   plantName = "";
}

/****************************************************************************\
\****************************************************************************/
void feedItem::InitVariables()
{
   OMD                     = 0.0;
   proteinN_digestibility 	= 0.0;
   feedUnitsPerItemUnit 	= 0.0;
   Ca_content 					= 0.0;
   Na_content 					= 0.0;
   Mg_content 					= 0.0;
   Cu_content 					= 0.0;
   Methionin 					= 0.0;
   Threonin 					= 0.0;
   Lysin 						= 0.0;
   fat 							= 0.0;
   sugar 						= 0.0;
   starch 						= 0.0;
   organic 						= 0.0;
   chew 							= 0.0;
   aat 							= 0.0;
   pbv 							= 0.0;
   cell							= 0.0;
   fill 							= 0.0;
   fillU 			  		  	= 0.0;
//   plantName					= "";
   P_digest 					= 0.0;
   MaxPercentage				= 0.0;
   propRDP				= 0.0;			// proportion of crude protein N that is rumen degradable (RDP)
   propAAinUDP			= 0.0;				// proportion of amino acid in protein that is not degraded in rumen (UDP)
   propUDPdigestible		= 0.0;     		// proportion of amino acid in UDP that is digestible in small intestine
   microAAFactor		= 0.0;				// g amino acid per kg digestible carbohydrate
   CHO_content			= 0.0; 				// proportion digestible carbohydrate in fresh weight
   propDigestCHO		= 0.0;					// proportion of carbohydrate that is digestible
   ME					= 0.0;
   feedCategory	= 0;
   pigFeedUnitsPerItemUnit=0.0;
   ObjType						= feedItemObj;
}

/****************************************************************************\
\****************************************************************************/
product& feedItem::operator=(const product& someFeedItem)
{
   feedItem*  aFeedItem;
	aFeedItem = (feedItem*)(&someFeedItem);

   liveMatter::operator=(someFeedItem);

   proteinN_digestibility         = aFeedItem->GetproteinN_digestibility();
	feedUnitsPerItemUnit = aFeedItem->GetfeedUnitsPerItemUnit();
	Ca_content             = aFeedItem->GetCa_content();
	Na_content             = aFeedItem->GetNa_content();
	Mg_content             = aFeedItem->GetMg_content();
	Cu_content             = aFeedItem->GetCu_content();
	Threonin               = aFeedItem->GetThreonin();
	Methionin              = aFeedItem->GetMethionin();
	Lysin                  = aFeedItem->GetLysin();
	fat                    = aFeedItem->Getfat();
	sugar                  = aFeedItem->Getsugar();
	starch                 = aFeedItem->Getstarch();
	chew                   = aFeedItem->Getchew();
	aat                    = aFeedItem->Getaat();
	pbv                    = aFeedItem->Getpbv();
	cell                   = aFeedItem->Getcell();
	fill                   = aFeedItem->Getfill();
	fillU                  = aFeedItem->GetfillU();
	plantName				  = aFeedItem->GetPlantName();
	P_digest					  = aFeedItem->GetP_digest();
   MaxPercentage			  = aFeedItem->GetMaxPercentage();
	OMD						  = aFeedItem->GetOMD();
	propRDP              = aFeedItem->GetPropRDP();
	microAAFactor        = aFeedItem->GetmicroAAFactor();
	propAAinUDP          = aFeedItem->GetpropAAinUDP();
	propUDPdigestible       = aFeedItem->GetpropUDPdigestible();
	CHO_content          = aFeedItem->GetCHOContent();
	propDigestCHO        = aFeedItem->GetpropDigestCHO();
	ME                   = aFeedItem->GetME();
   pigFeedUnitsPerItemUnit = aFeedItem->GetpigFeedUnitsPerItemUnit();
   feedCategory			= aFeedItem->GetfeedCategory();

	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& feedItem::operator+(const product& someFeedItem)
{
	feedItem*  aFeedItem;
   feedItem aPlantToFeedItem;
	aFeedItem = (feedItem*)(&someFeedItem);

 	if (aFeedItem->GetObjType()==feedItemObj)
   {
	   double aAmount                 = aFeedItem->GetAmount();
	   double aproteinN_digestibility         = aFeedItem->GetproteinN_digestibility();
		double afeedUnitsPerItemUnit = aFeedItem->GetfeedUnitsPerItemUnit();
		double aCa_content             = aFeedItem->GetCa_content();
		double aNa_content             = aFeedItem->GetNa_content();
		double aMg_content             = aFeedItem->GetMg_content();
		double aCu_content             = aFeedItem->GetCu_content();
		double aprotein_N              = aFeedItem->Getprotein_N();
		double aMethionin              = aFeedItem->GetMethionin();
		double aThreonin               = aFeedItem->GetThreonin();
		double aLysin                  = aFeedItem->GetLysin();
		double afat                    = aFeedItem->Getfat();
		double asugar                  = aFeedItem->Getsugar();
		double astarch                 = aFeedItem->Getstarch();
		double achew                   = aFeedItem->Getchew();
		double aaat                    = aFeedItem->Getaat();
		double apbv                    = aFeedItem->Getpbv();
		double acell                   = aFeedItem->Getcell();
		double afill                   = aFeedItem->Getfill();
		double afillU                  = aFeedItem->GetfillU();
	   double aP_content					 = aFeedItem->GetP_content();
		double aP_digest               = aFeedItem->GetP_digest();
	   double aMaxPercentage  			 = aFeedItem->GetMaxPercentage();
		double aOMD							 = aFeedItem->GetOMD();
		double apropRDP 					 = aFeedItem->GetPropRDP();
		double apropAAinUDP            = aFeedItem->GetpropAAinUDP();
		double apropUDPdigestible         = aFeedItem->GetpropUDPdigestible();
		double amicroAAFactor          = aFeedItem->GetmicroAAFactor();
		double aCHO_content            = aFeedItem->GetCHOContent();
		double apropDigestCHO          = aFeedItem->GetpropDigestCHO();
		double aME              		 = aFeedItem->GetME();
      double apigFeedUnitsPerItemUnit= aFeedItem->GetpigFeedUnitsPerItemUnit();
      int afeedCategory           = aFeedItem->GetfeedCategory();
      double aorganic          		 = aFeedItem->Getorganic();


		if(amount!=0)
		{
	   	proteinN_digestibility	= (proteinN_digestibility*Getprotein_N() +aprotein_N*aproteinN_digestibility)/ (aprotein_N +Getprotein_N());   // notice not relative to amount !!
		   feedUnitsPerItemUnit = (amount*feedUnitsPerItemUnit+aAmount*afeedUnitsPerItemUnit)/(amount+aAmount);
		   Ca_content		= (Ca_content*amount + aAmount*aCa_content)/(amount+aAmount);
		   Na_content		= (Na_content*amount + aAmount*aNa_content)/(amount+aAmount);
		   Mg_content		= (Mg_content*amount + aAmount*aMg_content)/(amount+aAmount);
		   Cu_content		= (Cu_content*amount + aAmount*aCu_content)/(amount+aAmount);
		   Methionin   	= (Methionin*amount  + aAmount*aMethionin)/(amount+aAmount);
		   Threonin  	 	= (Threonin*amount   + aAmount*aThreonin)/(amount+aAmount);
		   Lysin     		= (Lysin*amount      + aAmount*aLysin)/(amount+aAmount);
		   fat       		= (fat*amount        + aAmount*afat)/(amount+aAmount);
		   sugar     		= (sugar*amount      + aAmount*asugar)/(amount+aAmount);
		   starch    		= (starch*amount     + aAmount*astarch)/(amount+aAmount);
		   chew      		= (chew*amount       + aAmount*achew)/(amount+aAmount);
		   aat       		= (aat*amount        + aAmount*aaat)/(amount+aAmount);
		   pbv       		= (pbv*amount        + aAmount*apbv)/(amount+aAmount);
		   cell      		= (cell*amount       + aAmount*acell)/(amount+aAmount);
		   fill      		= (fill*amount       + aAmount*afill)/(amount+aAmount);
		   fillU   	 		= (fillU*amount      + aAmount*afillU)/(amount+aAmount);
         pigFeedUnitsPerItemUnit = (pigFeedUnitsPerItemUnit*amount + aAmount*apigFeedUnitsPerItemUnit)/(amount+aAmount);
	     	MaxPercentage  = (MaxPercentage*amount + aAmount*aMaxPercentage)/(amount+aAmount);

		   if (aP_content*aAmount>0)
			   P_digest	 	 = (P_digest*P_content*amount + aP_content*aP_digest*aAmount)
		                      /(P_content*amount+aP_content*aAmount);
         if ((organic*amount+aorganic*aAmount)>0.0)
			   OMD				= (OMD*amount*organic + aAmount*aorganic* aOMD)/(organic*amount+aorganic*aAmount);
         else
         	OMD = 0.0;
         double amountProtein = orgN_content.n* amount;
         double aAmountProtein = aprotein_N* aAmount;
         if ((amountProtein+aAmountProtein)>0.0)
         {
            double amountUDP = amountProtein * (1-propRDP);
            double aAmountUDP = aAmountProtein * (1-apropRDP);
			   propAAinUDP				= (propAAinUDP*amountUDP + aAmountUDP*apropAAinUDP)/(amountUDP+aAmountUDP);
			   propUDPdigestible   		= (propUDPdigestible*amountUDP  + aAmountUDP*apropUDPdigestible)/(amountUDP+aAmountUDP);
			   propRDP					= (propRDP*amountProtein + aAmountProtein*apropRDP)/(amountProtein + aAmountProtein);
         }
         else
         {
         	propAAinUDP=0.0;
			   propUDPdigestible=0.0;
			   propRDP			 = 0.0;
         }

         if ((amount+aAmount)>0.0)
         {


            ME       		= (ME*amount        + aAmount*aME)/(amount+aAmount);
            microAAFactor   		= (microAAFactor*amount  + aAmount*amicroAAFactor)/(amount+aAmount);
            CHO_content  	 		= (CHO_content*amount   + aAmount*aCHO_content)/(amount+aAmount);
         }
         else
         {

            ME       		= 0.0;
            microAAFactor   		= 0.0;
            CHO_content  	 		= 0.0;
            if ((feedCategory)||(afeedCategory==1))
            	feedCategory			= 1;
         }
         double amountCHO= CHO_content * amount;
         double aAmountCHO = aCHO_content * aAmount;
         if(amountCHO+aAmountCHO>0)
         {
        	 propDigestCHO     	= (propDigestCHO*amountCHO + aAmountCHO*apropDigestCHO)/(amountCHO+aAmountCHO);
         }
         else
         {
        	 propDigestCHO     	= 0.0;
         }
		}
	   else
	   {
	      if (aFeedItem->GetObjType()==feedItemObj)
	      {
		   	proteinN_digestibility	= aproteinN_digestibility;
			   feedUnitsPerItemUnit = afeedUnitsPerItemUnit;
			   Ca_content		= aCa_content;
			   Na_content		= aNa_content;
			   Mg_content		= aMg_content;
			   Cu_content		= aCu_content;
			   Methionin   	= aMethionin;
			   Threonin  	 	= aThreonin;
			   Lysin     		= aLysin;
			   fat       		= afat;
			   sugar     		= asugar;
			   starch    		= astarch;
			   chew      		= achew;
			   aat       		= aaat;
			   pbv       		= apbv;
			   cell      		= acell;
			   fill      		= afill;
			   fillU   	 		= afillU;
			   P_digest	 		= aP_digest;
		      MaxPercentage	= aMaxPercentage;
			   plantName 		= aFeedItem->GetPlantName();
			   OMD				= aOMD;
            propRDP					= apropRDP;
            propAAinUDP				= apropAAinUDP;
            propUDPdigestible   		= apropUDPdigestible;
            microAAFactor   		= amicroAAFactor;
            CHO_content  	 		= aCHO_content;
            propDigestCHO     	= apropDigestCHO;
            ME       				= aME;
            pigFeedUnitsPerItemUnit=apigFeedUnitsPerItemUnit;
            feedCategory			= afeedCategory;
	      }
	   }
   }
	liveMatter::operator+(someFeedItem);

	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& feedItem::operator-(const product& someFeedItem)
{
	feedItem * aFeedItem;
	aFeedItem = (feedItem*)(&someFeedItem);

	if(proteinN_digestibility         != aFeedItem->GetproteinN_digestibility()||
		feedUnitsPerItemUnit != aFeedItem->GetfeedUnitsPerItemUnit()||
		Ca_content             != aFeedItem->GetCa_content()||
		Na_content             != aFeedItem->GetNa_content()||
		Mg_content             != aFeedItem->GetMg_content()||
		Cu_content             != aFeedItem->GetCu_content()||
		Threonin               != aFeedItem->GetThreonin()||
		Methionin              != aFeedItem->GetMethionin()||
		Lysin                  != aFeedItem->GetLysin()||
		fat                    != aFeedItem->Getfat()||
		sugar                  != aFeedItem->Getsugar()||
		starch                 != aFeedItem->Getstarch()||
		chew                   != aFeedItem->Getchew()||
		aat                    != aFeedItem->Getaat()||
		pbv                    != aFeedItem->Getpbv()||
		cell                   != aFeedItem->Getcell()||
		fill                   != aFeedItem->Getfill()||
		fillU                  != aFeedItem->GetfillU()||
		plantName				  != aFeedItem->GetPlantName()||
		P_digest					  != aFeedItem->GetP_digest()||
      pigFeedUnitsPerItemUnit!= aFeedItem->GetpigFeedUnitsPerItemUnit())
   {
      theMessage->Warning("feedItem::operator- - feedItems with different composition subtracted");
   }
   liveMatter::operator-(someFeedItem);

	return *this;
}

/****************************************************************************\
\****************************************************************************/
product* feedItem::clone() const
{
	feedItem* aProduct= new feedItem(*this);
   return (product*) aProduct;
}

/**
 * Reads feed information from file. File is generated as tab delineated ascii file from
 * Excel version of Danish feed standards for cattle. NJH June 2000
 */
bool feedItem::ReadParams(fstream * file)
{
   //code below was used to read from text file output from Excel version of feed tables
   char buffer[1000];
   buffer[0]  = '\0';
   int counter=0;
   string feedName="", DKfeedName="";
//   char feedName[MAX_TXT]="";;
//   feedName = ""; DKfeedName = ""; plantName = "";
   string outstring,testchar;
   double inarray[200];
   for (int I=0;I<200;I++)
      inarray[I]=-999.0;
   bool isastring=false;	//true if the current record is a string
   file->getline(buffer,1000);//,'\n');  // get a line
   string instring = string(buffer);                //read into instring
   if (instring.length()>0)
   {
      isastring=false;
   /*   cout << "string length " << instring.length() << endl;
      cout << buffer << endl;
      cout << instring << endl;*/
      int J=0;
      outstring="";
      while ((int (instring[J])==9)==false)
      {
         DKfeedName+=instring[J];
         J++;
      }
      J++;
      while ((int (instring[J])==9)==false)
      {
         feedName+=instring[J];
         J++;
      }
      J++;

      while ((int (instring[J])==9)==false)
      {
         plantName+=instring[J];
         J++;
      }
      J++;


      for (int I=J;I<int(instring.length());I++)    //parse line
      {
         testchar = instring[I];
         if ((int (instring[I])==9)==false)    //if not tab
         {
            if ((testchar==".") || (testchar=="-") || (testchar==" ") || (isdigit(instring[I])))  //accept numerics only
               outstring+=testchar;
         }

         if (((int (instring[I])==9) || (I==int (instring.length()-1))))  //if tab or end of line
          {
             if (!(isastring))  //ignore strings
             {
               if ((outstring.length()>0) && !(outstring == "  "))
               {
                  inarray[counter] = atof(outstring.c_str());
                  }
               else
                  inarray[counter] = -999.0;
               counter++;
             }
            outstring="";
            isastring=false;
          }
      } // end of parsing this line
      strcpy(Name,feedName.c_str());
      code							=  int(inarray[0]);
      dryMatter 					= inarray[1]/100.0;
      organic 						= dryMatter * (1-(inarray[3]/100.0));
      double protein_content			= (inarray[4]/100.0) * dryMatter;
      orgN_content      			= protein_content/N_to_protein();
      aat							= inarray[6] * dryMatter/100.0;	// aat
      pbv							= inarray[7] * dryMatter/100.0;	// pbv
      fat							= inarray[8] * dryMatter/100.0;	// fat
      CHO_content 				= inarray[12] * dryMatter/100.0;	// carbohydrate
      cell			 				= inarray[16] * dryMatter/100.0;	// cell wall carbohydrate
      sugar			 				= inarray[14] * dryMatter/100.0;
      starch		 				= inarray[15] * dryMatter/100.0;
      propRDP						= inarray[20]/100.0;	// proportion of crude protein that is rumen degradable protein
      propUDPdigestible          = inarray[22]/100.0;	// proportion of amino acid from UDP that is digestible
      propDigestCHO				= inarray[24]/100.0;	// proportion of carbohydrate that is digestible
      OMD							= inarray[25]/100.0;
      C_content					= GetC(); //C content is calculated from fat, protein, cell and starch, so needs no adjustment for dry matter content
      P_content 					= inarray[28] * dryMatter/1000.0;
      K_content 					= inarray[30] * dryMatter/1000.0;
      Na_content 					= inarray[31] * dryMatter/1000.0;
      fill							= inarray[48] * 1000.0 * dryMatter; //per tonne FW
      fillU							= inarray[49] * 1000.0 * dryMatter; //per tonne FW
      ME								= inarray[51] * 1000.0 * dryMatter; //per tonne FW
      feedUnitsPerItemUnit 	= inarray[52] * 1000.0 * dryMatter; //per tonne FW
      pigFeedUnitsPerItemUnit	= inarray[66] * 1000.0 * dryMatter; //per tonne FW;
      if ((inarray[4]>0.0)&& (inarray[5]>0.0)) // Changed 67 to 5 - BMP 2007 !!!
	      proteinN_digestibility	= inarray[5]/inarray[4];    //for pigs. Changed from (inarray[67]/10.0)/inarray[4] BMP 2007 !!!
      else
      {
      	if (inarray[4]>0.0)
		      proteinN_digestibility	= -999.0;    //missing a value
	      else
		      proteinN_digestibility	= 0.0;    //there is no protein in the feed
      }
      if (proteinN_digestibility>1.0) // Added by BMP 2007
      {
         proteinN_digestibility=0.99;
         theMessage->WarningWithDisplay("feedItem::ReadParams - protein digestibility above 100%. Set to 99%");
      }
      Lysin                   = inarray[68] * dryMatter;
      Methionin               = inarray[69] * dryMatter;
      Threonin                = inarray[70] * dryMatter;
      feedCategory				= int(inarray[71]);
      // t microbial amino acid per t digestible carbohydrate
      if ((code >= 401) && (code <= 469))
         microAAFactor	= 0.135;
      else
         microAAFactor = 0.125;
      // see if this feed is a concentrate and set proportion amino acid in UDP accordingly
      if (((code >= 2) && (code <= 270)) || ((code >= 295) && (code <= 297)) ||
            ((code >= 301) && (code <= 350)) || ((code >= 908) && (code <= 911)))
         propAAinUDP		= 0.85;
      else
         propAAinUDP		= 0.65;
//    CalcAAT_PBV();  Use table values until further notice
		mainUnitName=t;
   return true;
   }
 else return false;
}

/****************************************************************************\
\****************************************************************************/
double feedItem::GetProteinContent()
{
 return orgN_content.n * N_to_protein();
}

/**
 * Copies common ancestor info. Used when you want a feedItem but a plantItem is in storage
 */
void feedItem::AddPlantItem(const plantItem& somePlantItem, int feedCat)
{
	liveMatter::operator+(somePlantItem);
	string nameOfPlant = somePlantItem.GetName();
	plantName = nameOfPlant;
	CalcDigestibility();
	CalcFE();
	CalcME();
	CalcFill(feedCat);
}

void feedItem::CalcDigestibility()
{
   // temp fudge to make sure OMD and CP do not get out of balance
   OMD = 0.96 * ((orgN_content.n * N_to_protein()/dryMatter) - 0.0625) + 0.6;
   if (OMD>0.75)
      OMD=0.75;
   if (OMD<0.65)
      OMD=0.65;
}

/****************************************************************************\
Calculate the nitrogen free extract content of the feed
\****************************************************************************/
double feedItem::CalcNFE()
{
   double ash = 1.0-organic;
   double protein = orgN_content.n * N_to_protein();
   double NFE = 1.0 - (ash + fat + protein + cell);
   return NFE;
}

/****************************************************************************\
Calculate the C content of the feed
\****************************************************************************/
double feedItem::GetC()
{
   double protein = orgN_content.n * N_to_protein();
   double C;
   if ((fat==0.0)&&(cell==0.0)&&(sugar==0.0)&&(starch==0.0))
   	C = 0.46 * organic;
   else
      C = 0.76 * fat + 0.52 * protein + 0.48 * cell + 0.421 * sugar
          + 0.444 *starch;
   return C;
}

/****************************************************************************\
Calculate the gross energy content of the feed (MJ)
\****************************************************************************/
double feedItem::GetGrossEnergy()
{
   double grossEnergy =  18.4 * 1000.0 * amount * dryMatter;
   return grossEnergy;
}

/****************************************************************************\
\****************************************************************************/
void feedItem::CalcFE()
{
 //feedUnitsPerItemUnit = 1000.0 * (2.17 * OMD - 0.766) * dryMatter;
 feedUnitsPerItemUnit = 1000.0 * N_to_FE(orgN_content.n/dryMatter) * dryMatter;
 if (feedUnitsPerItemUnit<0.0001)
   	theMessage->FatalError("feedItem: digestibility too low");
}

/****************************************************************************\
Calculate the ME of a feed (MJ/tonne FW)
\****************************************************************************/
void feedItem::CalcME()
{
	double MEPerKg = 0.16*OMD*100.0 - 1.8;  // from SCA 1990
   SetME(1000.0 * MEPerKg * dryMatter);  //convert to tonnes
}
/****************************************************************************\
Calculate the fill of a feed
Normally only used for fresh crops
Not fully implemented for young animals
\****************************************************************************/
void feedItem::CalcFill(int type)
{
   switch (type)
   {
   	//concentrates
		case 1: fill=220.0*dryMatter;   //fill per tonne FW
      			fillU = 1.05*dryMatter;
               break;
		//fresh crops (not whole-crop), hay
		case 2: fill=1000.0 * (0.85 - 0.44 * feedUnitsPerItemUnit/(1000.0 * dryMatter))*dryMatter;   //fill per tonne FW
//      			fillU = 1.05*dryMatter;
               break;
		//silage
		case 3: fill=1000.0 * (0.85 - 0.44 * feedUnitsPerItemUnit/(1000.0 * dryMatter))*dryMatter;   //fill per tonne FW
      			if (dryMatter<0.03)
                fill = fill/(1 - 0.015 * (30 - 100.0 * dryMatter));
               break;
		//silage
		case 4: fill=1000.0 * (0.79 - 0.44 * feedUnitsPerItemUnit/(1000.0 * dryMatter))*dryMatter;   //fill per tonne FW
               break;
      default:    	theMessage->FatalError("pfeedit:CalcFill - type not found");

   }

}

/****************************************************************************\
\****************************************************************************/
double feedItem::GetdigestibleEnergy()
{
	double MJperkgDOM =17.33 + 0.0124 * 1000.0 * GetProteinContent()/GetdryMatter();
   double digestibleEnergy = amount * 1000.0* organic * OMD *  MJperkgDOM;
   return  digestibleEnergy;
}

/**
 * Calc AAT and PBV
 */
void feedItem::CalcAAT_PBV()
{
	double crudeProtein = orgN_content.n * N_to_protein();
   crudeProtein /=dryMatter;
   CHO_content /=dryMatter;

                      //A           B        -------------- F -----------------------
   pbv = 1000*(crudeProtein *  propRDP - propDigestCHO * CHO_content                      //PBV in g/kg DM
                                  // E           H
                               * microAAFactor/0.70);
                      // A           B/100        C              D/100
   aat = (1000*crudeProtein * (1 - propRDP) * propAAinUDP * propUDPdigestible
                              // E      ----------- F ------------        G
                   + 100.0*microAAFactor * propDigestCHO * CHO_content * 0.85);
   CHO_content *=dryMatter;
   pbv *=dryMatter;
   aat *=dryMatter;
//   cout << "PBV " << PBV << " AAT " << AAT << endl;
}

/**
 * Checks to see if all variables required have been set.
 * Add to this function with time (and increase in variables used)
 */
int feedItem::GotItAll()
{
	int ret_val = 0;
	if (GetNa_content()<=0.0)
      	ret_val = 1;
   if (ret_val!=0)
   {
		theMessage->WarningWithDisplay("feedItem:GotItAll(), code = ",ret_val);
	   theMessage->FatalError("feedItem: variable missing in ", GetName());
   }
   return ret_val;
}
