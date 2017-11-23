//============================================================================
// Name        : tests.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
using namespace std;
//userdefines
int NrOfTimes=20;
string fileName ="c:\\example.txt";
int sizeOfheader=4;
string headers[]={"ntotal","nmin", "nmax", "thing"};

//return a "random" double between low and high
double randomDouble(double low, double high)
{
  double range=(high-low);
  double num = static_cast<double>( rand() ) * range / static_cast<double>( RAND_MAX ) + low ;

  return(num);
}
//return a "random" int between low and high
int randomInt(int low, int high)
{
  return randomDouble(low,high);
}
//return a random string from a list
string randomString(string list[],int size)
{
	int placeInList=randomInt(0,size);
	return list[placeInList];
}
// return eiter true or false
bool randomBool()
{
	int returnValue=randomInt(0,2);
	if(returnValue==0)
		return false;
	else
		return true;
}
//test fuction. Should be deleted when done
double testFunction(int in1,int in2, double in3, double in4)
{
	return in1+in2+in3+in4;
}
int main() {

	srand((unsigned)time(0));
	ofstream myfile;
	myfile.open (fileName.c_str());

	for(int i=0;i<sizeOfheader;i++)
	{
		myfile<<headers[i]+'\t';
	}
	myfile<<endl;
	for(int i=0;i<NrOfTimes;i++)
	{
		//user defines input to function
		double input1 = randomDouble(1,20);
		double input2 = randomDouble(1,20);
		int input3 = randomInt(1,20);
		int input4 = randomInt(1,20);
		//user defines the function
		double result=testFunction(input1,input2,input3,input4);

		//user defines what goes to output
		myfile<<input1<<"\t"<<input2<<"\t"<<input3<<"\t"<<input4<<"\t"<<result<<endl;

	}

	myfile.close();

	return 0;
}
