/*
 * converters.cpp
 *
 *  Created on: Aug 21, 2009
 *      Author: JOVE
 */

#include "../tools/convert.h"


#include <iostream>
#include <string>
#include <sstream>

#include <stdio.h>
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



