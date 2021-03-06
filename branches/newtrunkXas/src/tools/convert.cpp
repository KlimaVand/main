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
string convert::binary( unsigned long n )
  {
  char     result[ (sizeof( unsigned long ) * 8) + 1 ];
  unsigned index  = sizeof( unsigned long ) * 8;
  result[ index ] = '\0';

  do result[ --index ] = '0' + (n & 1);
  while (n >>= 1);

  return string( result + index );
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

