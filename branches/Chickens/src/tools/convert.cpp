/*
 * converters.cpp
 *
 *  Created on: Aug 21, 2009
 *      Author: JOVE
 */

#include "../tools/convert.h"
#include "../base/message.h"
#include "../base/unix_util.h"
#include "../base/typer.h"


convert::convert()
{

}

string convert::IntToString(int conv) {
#ifdef __BCplusplus__
	 char *myBuff;
  string strRetVal;

  // Create a new char array
  myBuff = new char[100];

  // Set it to empty
  memset(myBuff,'\0',100);

  // Convert to string
  itoa(conv,myBuff,10);

  // Copy the buffer into the string object
  strRetVal = myBuff;
  
  // Delete the buffer
  delete[] myBuff;

  return(strRetVal);
#else

     std::stringstream Num;

     std::string str;

     Num << conv;

     str = Num.str();
     return str;
#endif

}

double convert::StringToDouble(string conv) {

	return atof(conv.c_str());
}
int convert::StringToInt(string conv) {
  int intReturn;
  intReturn = atoi(conv.c_str());
  return	intReturn;
}
string convert::DoubleToChar(double conv) {
	char output[200];
	sprintf(output,"%f",conv);
  return	output;
}


string convert::EatSpaces(string s)
{
   int source=0;
   int dest=0;
   while (s[source]!='\0')
   {
      while (s[source]==' ')
         source++;
      s[dest]=s[source];
      dest++;
      source++;
   }
   s[dest]='\0';
   return s;
}
bool convert::StringToBool(string input)
{
	bool output;
	if(input.compare("0")==0)
		output=false;
	if(input.compare("FALSE")==0)
		output=false;
	if(input.compare("False")==0)
		output=false;
	if(input.compare("false")==0)
		output=false;
	if(input.compare("F")==0)
		output=false;
	if(input.compare("f")==0)
		output=false;
	if(input.compare("NO")==0)
		output=false;
	if(input.compare("No")==0)
		output=false;
	if(input.compare("no")==0)
		output=false;
	if(input.compare("N")==0)
		output=false;
	if(input.compare("n")==0)
		output=false;
	if(input.compare("1")==0)
		output=true;
	if(input.compare("TRUE")==0)
		output=true;
	if(input.compare("True")==0)
		output=true;
	if(input.compare("true")==0)
		output=true;
	if(input.compare("T")==0)
		output=true;
	if(input.compare("t")==0)
		output=true;
	if(input.compare("YES")==0)
		output=true;
	if(input.compare("Yes")==0)
		output=true;
	if(input.compare("yes")==0)
		output=true;
	if(input.compare("Y")==0)
		output=true;
	if(input.compare("y")==0)
		output=true;
   return output;
}

int convert::GetUnitTypeStringToInt(string aMainUnitType)
{
	bool found = false;
	int typeRes=-1;
	if (aMainUnitType=="kwh")
     	{found=true;typeRes=kwh;}
	if (aMainUnitType=="tfi")
     	{found=true;typeRes=tfi;}
	if (aMainUnitType=="t")
     	{found=true;typeRes=t;}
	if (aMainUnitType=="kg")
    	{found=true;typeRes=kg;}
	if (aMainUnitType=="g")
     	{found=true;typeRes=g;}
	if (aMainUnitType=="m3")
     	{found=true;typeRes=m3;}
	if (aMainUnitType=="l")
     	{found=true;typeRes=l;}
	if (aMainUnitType=="ha")
     	{found=true;typeRes=ha;}
	if (aMainUnitType=="m2")
     	{found=true;typeRes=m2;}
	if (aMainUnitType=="m")
     	{found=true;typeRes=m;}
	if (aMainUnitType=="mm")
     	{found=true;typeRes=mm;}
	if (aMainUnitType=="sfu")
     	{found=true;typeRes=sfu;}
	if (aMainUnitType=="day")
     	{found=true;typeRes=day;}
	if (aMainUnitType=="hour")
    	{found=true;typeRes=hour;}
	if (aMainUnitType=="kr")
     	{found=true;typeRes=kr;}
	if (aMainUnitType=="fraction")
      {found=true;typeRes=fraction;}
	if (aMainUnitType=="pcs")
      {found=true;typeRes=pcs;}
	if(!found)
   {
   	theMessage->WarningWithDisplay("product::GetUnitType - the unit type <", (char*)aMainUnitType.c_str(), "> does not exist");
      return -1;
   }
	else
   	return typeRes;
}
string convert::GetUnitTypeIntToString(int aMainUnit)
{
	if (aMainUnit==kwh)
      return "kwh";
	if (aMainUnit==tfi)
    	return "tfi";
	if (aMainUnit==t)
    	return "t";
	if (aMainUnit==kg)
    	return "kg";
	if (aMainUnit==g)
    	return "g";
	if (aMainUnit==m3)
    	return "m3";
	if (aMainUnit==l)
    	return "l";
	if (aMainUnit==ha)
    	return "ha";
	if (aMainUnit==m2)
    	return "m2";
	if (aMainUnit==m)
    	return "m";
	if (aMainUnit==mm)
    	return "mm";
	if (aMainUnit==sfu)
    	return "sfu";
	if (aMainUnit==day)
   	return "day";
   if (aMainUnit==hour)
    	return "hour";
	if (aMainUnit==kr)
    	return "kr";
	if (aMainUnit==fraction)
    	return "fraction";
	if (aMainUnit==pcs)
    	return "pcs";
   return "illegal unit";
}
int convert::GetChemType(string aChemType)
{
	int typeRes=-1;
	if (aChemType=="herbicide")
   	typeRes=herbicide;
	if (aChemType=="fungicide")
   	typeRes=fungicide;
   if (aChemType=="pesticide")
   	typeRes=pesticide;
   if (aChemType=="microfertilizer")
   	typeRes=microfertilizer;
   if (aChemType=="insecticide")
   	typeRes=insecticide;
   if(typeRes==-1)
   {
 		theMessage->WarningWithDisplay("chemical::GetChemType - the chemical unit type <",(char*)aChemType.c_str(),"> does not exist");
      return -1;
   }
	else
		return typeRes;
}
string convert::partOfAString(string input, int number)
{
	#ifndef __BCplusplus__
		return partOfAStringC(input, number);
	#endif
	#ifdef __BCplusplus__
		return partOfAStringBorland(input, number);
	#endif
}
#ifndef __BCplusplus__
string convert::partOfAStringC(string input, int number)
{
  stringstream totalSString( input );
  string nameOfItem;

   for(int i=0;i<number;i++)
	   totalSString >>nameOfItem;
   return nameOfItem;
}
#endif
#ifdef __BCplusplus__
string convert::partOfAStringBorland(string input, int number)
{
   char fin[200];
   strcpy(fin,input.c_str());
   istrstream ins(fin, strlen(fin));
   string text1;
   for(int i=0;i<number;i++)
  		ins>>text1;
   return text1;
}
#endif

void convert::BlendProducts(linkList<plantItem>* HarvestList, int feedCode)
{
	linkList <plantItem>::PS aNode;
	HarvestList->PeekHead(aNode);
	plantItem *NewHarvestItem = aNode->element;

   while (HarvestList->NumOfNodes()>1)
   {
   	HarvestList->PeekTail(aNode);
      plantItem *HarvestItem = aNode->element;
      *NewHarvestItem + *HarvestItem;
      HarvestList->Release(aNode);
   }
   NewHarvestItem->SetCode(feedCode);
}
/**********************************************************************
Intepret standard feed code from crop or crop mixture
*******************************************************************/
int convert::GetHarvestProductCode(linkList<plantItem>* HarvestList)
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
      string cid = HarvestItem->GetName();
      if ((cid.compare("WHEAT")==0)||(cid.compare("WNWHT")==0)||(cid.compare("SPWHT")==0))     //SPWHT added MEL 2009
         wheat=true;
      if ((cid.compare("BARLEY")==0)||(cid.compare("WNBRL")==0))
         barley = true;
      if ((cid.compare("RAPE")==0)||(cid.compare("WNRPE")==0) || (cid.compare("SPRAPE")==0))
         rape = true;
      if (cid.compare("OAT")==0)
         oat = true;
      if (cid.compare("RYE")==0)
         rye = true;
      if (cid.compare("PEA")==0)  //pea crop (not whole-crop)
         pea=true;
      if (cid.compare("MAIZE")==0)
         maize = true;
      if (cid.compare("BEET")==0)
         beet=true;
      if (cid.compare("POTATO")==0)
         potatoes=true;
      if (cid.compare("CLOVER")==0)
         clover = true;
      if (cid.compare("GRASS")==0 ||cid.compare("RYEGRASS")==0 ||cid.compare("ITALRYEG")==0)
         grass=true;
      if (cid.compare("CLOVERGRASS")==0)
      {
         grass=true;
         clover=true;
      }
      if (cid.compare("BEETTOP")==0)
      	beettop=true;
      if (cid.compare("WHEATSTRAW")==0)
      	wheatstraw=true;
      if (cid.compare("BARLEYSTRAW")==0)
         barleystraw=true;
      if (cid.compare("RAPESTRAW")==0)
         rapestraw=true;
      if (cid.compare("OATSTRAW")==0)
         oatstraw=true;
      if (cid.compare("RYESTRAW")==0)
         ryestraw=true;
      if (cid.compare("PEASTRAW")==0)
         peastraw=true;
      if (cid.compare("MAIZESTRAW")==0)
         maizestraw=true;
      if (cid.compare("POTATOTOP")==0)
         potatotop=true;
      if (cid.compare("BARLEYPEA")==0)
         barleypea=true;
      if (cid.compare("SOYBEAN")==0)
         soybean=true;
      if (cid.compare("SOYBEANSTRAW")==0)
         soybeanstraw=true;
      if (cid.compare("SUNFLOWER")==0)
         sunflower=true;
      if (cid.compare("SUNFLOWERSTRAW")==0)
         sunflowerstraw=true;
      if (cid.compare("LUCERNE")==0)
         lucerne=true;
      if (cid.compare("LUCERNESTRAW")==0)
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
