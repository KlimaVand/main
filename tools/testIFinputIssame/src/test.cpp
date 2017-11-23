#include <vector>
#include <string>

#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include "tools/fileAccess.h"
#include "tools/compare.h"
#include "tools/convert.h"
#ifndef __BCplusplus__
	#include <sstream>
#endif
#ifdef __BCplusplus__
	#include <strstream>
#endif
using namespace std;
fileAccess output;
#include <io.h>
#include <stdio.h>
#define NUM_THREADS     5

string EatUnsedChars(string oldS, bool ignoreLettes=false, bool ignoreTab=false, bool ignoreSpace=false)
{

   unsigned int source=0;
   int dest=0;
   string newS;
   newS=oldS;


   while (oldS.size()>source)
   {

	   bool canBeUsed=false;

	   if(ignoreLettes==false)
	   {
		   //small letters

		   for(int i=97;i<=122;i++)
		   {
			   int tmp=oldS[source];

			   if(tmp==i)
			   {
				   canBeUsed=true;
				   break;
			   }
		   }
		   //large letters
		   for(int i=65;i<=90;i++)
			   {
				   int tmp=oldS[source];

				   if(tmp==i)
				   {
					   canBeUsed=true;
					   break;
				   }
			   }
	   }
	   // numbers
	   for(int i=48;i<=57;i++)
	   	  	   {
	   	  		   int tmp=oldS[source];

	   	  		   if(tmp==i)
	   	  		   {
	   	  			   canBeUsed=true;
	   	  			   break;
	   	  		   }
	   	  	   }

	   int tmp=oldS[source];
	   // for /
	   if(tmp==47)
	   {
		   canBeUsed=true;
	   }
	   //space
	   if(ignoreSpace==false)
	   {   if(tmp==32)
	   	   {
	   		   canBeUsed=true;
	   	   }
	   }
	   // .
	   if(tmp==46)
	   {
		   canBeUsed=true;
	  }
	   if (ignoreTab==false)
	   {
		   if(tmp==9)
		   	   {
		   		   canBeUsed=true;
		   	  }

	   }
	   if(canBeUsed==true)
	   {


		   newS[dest]=oldS[source];
		  dest++;
		  source++;
	   }
	   else
	   {
		   source++;
	   }

   }
   newS[dest]='\0';
   string output=newS.substr(0, dest);

   return output;
}
string partOfAString(string input, int number)
{
  stringstream totalSString( input );
  string nameOfItem;

   for(int i=0;i<number;i++)
   {
	   totalSString >>nameOfItem;


   }

   return nameOfItem;
}

void openAndCompareInicat(string name)
{
	vector<string> avoid;
	avoid.push_back("-Farm");
	avoid.push_back("-Capital-");
	avoid.push_back("-Gross");
	avoid.push_back("-Store-");
	avoid.push_back("-Labour-");
	avoid.push_back("-Areas-");
	avoid.push_back("-Yields-");
	avoid.push_back("-Livestock");
	avoid.push_back("-Energy-");
	avoid.push_back("-Nitrogen-");
	avoid.push_back("-Carbon-");
	avoid.push_back("-Phosporus-");
	avoid.push_back("-Potassium-");
	avoid.push_back("-Water-");
	avoid.push_back("-Pesticides-");
	avoid.push_back("-Net");
	avoid.push_back("01");
	fileAccess wantedOutput;
	wantedOutput.openFile("wantedoutput/"+name);
	fileAccess actuallyOutput;

	if(actuallyOutput.openFileWithoutExit("output/"+name)==true)
	{
		int line=0;
		while(!wantedOutput.endOfFile())
		{
			line++;
			string wantedString=wantedOutput.getLineFile();
			string theWantedDouble=partOfAString(EatUnsedChars(wantedString,true),8);
			string actuallyString=actuallyOutput.getLineFile();
			string theActuallyDouble=partOfAString(EatUnsedChars(actuallyString,true),8);
			istringstream i(theWantedDouble);
			double wanted=-1;
			if (!(i >> wanted))
				wanted=0;
			istringstream k(theActuallyDouble);
			double actually;
			if (!(k >> actually))
				actually=0;

			convert conv;
			if(doubleEquals(wanted,0,0.0001)==1)
			  {

				  if(doubleEquals(wanted,actually,0.0001)!=1)
				  {
					  double dif=wanted-actually;

					  output.Write("In "+name+" at line"+conv.IntToString(line)+" the value do not match.The different are"+conv.DoubleToString(dif));

				  }
			  }
			  else
			  {
				  double realDif=wanted-actually;
				  double dif=(realDif)/actually;

				  if(doubleEquals(dif,0,0.0001)!=1)
				  {

					  output.Write("In "+name+" at line"+conv.IntToString(line)+" the value do not match.The different are"+conv.DoubleToString(dif)+" % and the real dif is "+conv.DoubleToString(realDif));

				  }

			  }



		}
	}
	else
	{
		output.Write("cannot find file "+name+" in the output dir");
	}


	wantedOutput.closeFile();
	actuallyOutput.closeFile();
}
string previusWanted;
string previusActually;
void openAndCompareLogfile(string name)
{

	fileAccess wantedOutput;
	wantedOutput.openFile("wantedoutput/"+name);
	fileAccess actuallyOutput;
	convert conv;
	previusWanted="nothing";
	previusActually="nothing";
	if(actuallyOutput.openFileWithoutExit("output/"+name)==true)
	{
		int line=0;
		while(!wantedOutput.endOfFile())
		{
			line++;
			string actuallyLine=actuallyOutput.getLineFile();
			string wantedLine=wantedOutput.getLineFile();
			string sortActuallyLine=EatUnsedChars(actuallyLine,false,true, true);
			string sortWantedLine=EatUnsedChars(wantedLine,false,true,true);

			if(sortActuallyLine.empty())
			{
				actuallyLine=actuallyOutput.getLineFile();
				sortActuallyLine=EatUnsedChars(actuallyLine,false,true, true);
			}
			if(sortWantedLine.empty())
			{

				wantedLine=actuallyOutput.getLineFile();
				sortWantedLine=EatUnsedChars(wantedLine,false,true, true);
			}

			if(sortWantedLine.compare(sortActuallyLine)!=0)
			{
				convert conv;

				if(sortWantedLine.compare(previusActually)==0)
				{

					output.Write("in logfile "+name+" at line "+conv.IntToString(line)+" the standard output looks like yours but on previous line. The lines are: ");
					output.WriteNewLine();
					output.Write(actuallyLine);
					output.WriteNewLine();
					output.Write(wantedLine);
				}
				if(sortActuallyLine.compare(previusWanted)==0)
				{

					output.Write("in logfile "+name+" at line "+conv.IntToString(line)+" your output looks like standard but on previous line. The lines are: ");
					output.WriteNewLine();
					output.Write(actuallyLine);
					output.WriteNewLine();
					output.Write(wantedLine);
				}
				else
				{
					output.Write("in logfile "+name+" line number "+conv.IntToString(line)+" is different. The lines are: ");
					output.WriteNewLine();
					output.Write(actuallyLine);
					output.WriteNewLine();
					output.Write(wantedLine);
				}
			}
			previusWanted=sortWantedLine;
			previusActually=sortActuallyLine;
		}
	}


}

void openAndCompareFieldFile(string name)
{

	fileAccess wantedOutput;
	wantedOutput.openFile("wantedoutput/"+name);
	fileAccess actuallyOutput;
	convert conv;
	if(actuallyOutput.openFileWithoutExit("output/"+name)==true)
	{

		string wantedString=wantedOutput.getLineFile();
		string actuallyString=actuallyOutput.getLineFile();
		int line=0;

		while(!wantedOutput.endOfFile())
		{
			line++;

			wantedString=wantedOutput.getLineFile();
			actuallyString=actuallyOutput.getLineFile();
			stringstream streamWanted (wantedString);
			stringstream streamActually (actuallyString);

			string partOfWantedString="NULL";
			string partOfActuallyString="NULL";
			int items=0;
			while(!partOfWantedString.empty())
			{

				items++;
				partOfWantedString.clear();
				streamWanted>>partOfWantedString;
				streamActually>>partOfActuallyString;
				if(partOfWantedString.empty())
				{
					break;
				}
				switch ( items ) {
				  case 1 :
					  if(partOfWantedString.compare(partOfActuallyString)!=0)
					  {
						  string out="In "+name+" at "+conv.IntToString(line)+ " the dates do not match. The dates are:"+partOfWantedString+" and "+partOfActuallyString;
						  output.Write(out);

					  }
					  break;
				  case 4 :
					  if(partOfWantedString.compare(partOfActuallyString)!=0)
					  {
						  output.Write("In "+name+" at "+conv.IntToString(line)+ " the crop do not match"+"The crop are:"+partOfWantedString+" and "+partOfActuallyString);
					  }
					  break;
				  default :
					  convert conv;
					  double wantedDouble=conv.StringToDouble(partOfWantedString);
					  double actuallyDouble=conv.StringToDouble(partOfActuallyString);
					  if(doubleEquals(wantedDouble,0,0.0001)==1)
					  {

						  if(doubleEquals(wantedDouble,actuallyDouble,0.0001)!=1)
						  {
							  double dif=wantedDouble-actuallyDouble;
							  output.Write("In "+name+" at line"+conv.IntToString(line)+" and cell "+ conv.IntToString(items)+" the value do not match.The different are"+conv.DoubleToString(dif));

						  }
					  }
					  else
					  {
						  double realDif=wantedDouble-actuallyDouble;
						  double dif=(wantedDouble-actuallyDouble)/wantedDouble;

						  if(doubleEquals(dif,0,0.0001)!=1)
						  {

							  output.Write("In "+name+" at line"+conv.IntToString(line)+" and cell "+ conv.IntToString(items)+" the value do not match.The different are"+conv.DoubleToString(dif)+" % and the real dif is "+conv.DoubleToString(realDif ));

						  }

					  }
					  break;
				}

			}
		}

	}
	else
	{
		cout<<"cannot find file "+name+" in the output dir"<<endl;
		output.Write("cannot find file "+name+" in the output dir");
	}



}
void openAndCompareSoilFile(string name)
{
	fileAccess wantedOutput;
	wantedOutput.openFile("wantedoutput/"+name);
	fileAccess actuallyOutput;
	convert conv;

	if(actuallyOutput.openFileWithoutExit("output/"+name)==true)
	{
		string wantedString=wantedOutput.getLineFile();
		string actuallyString=actuallyOutput.getLineFile();
		int line=1;
		while(!wantedOutput.endOfFile())
		{
			line++;
			wantedString=wantedOutput.getLineFile();
			actuallyString=actuallyOutput.getLineFile();
			stringstream streamWanted (wantedString);
			stringstream streamActually (actuallyString);
			string partOfWantedString="NULL";
			string partOfActuallyString="NULL";
			int items=0;

			while(!partOfWantedString.empty())
			{

				items++;
				//cout<<"items"<<items<<endl;
				partOfWantedString.clear();
				streamWanted>>partOfWantedString;
				streamActually>>partOfActuallyString;
				if(partOfWantedString.empty())
				{
					break;
				}
				switch ( items ) {
				  case 1 :
					  if(partOfWantedString.compare(partOfActuallyString)!=0)
					  {
						  string out="In "+name+" at "+conv.IntToString(line)+ " the dates do not match. The dates are:"+partOfWantedString+" and "+partOfActuallyString;
						  output.Write(out);

					  }
					  break;
				  default :
					  convert conv;
					  double wantedDouble=conv.StringToDouble(partOfWantedString);
					  double actuallyDouble=conv.StringToDouble(partOfActuallyString);
					  if(doubleEquals(wantedDouble,0,0.0001)==1)
					  {

						  if(doubleEquals(wantedDouble,actuallyDouble,0.0001)!=1)
						  {
							  double dif=wantedDouble-actuallyDouble;

							  output.Write("In "+name+" at line"+conv.IntToString(line)+" and cell "+ conv.IntToString(items)+" the value do not match.The different are"+conv.DoubleToString(dif));

						  }
					  }
					  else
					  {
						  double dif=(wantedDouble-actuallyDouble)/wantedDouble;

						  if(doubleEquals(dif,0,0.0001)!=1)
						  {

							  output.Write("In "+name+" at line"+conv.IntToString(line)+" and cell "+ conv.IntToString(items)+" the value do not match.The different are"+conv.DoubleToString(dif));

						  }
					  }
					  break;
				}
			}
		}
	}
	else
	{
		output.Write("cannot find file "+name+" in the output dir");
	}
}
void openAndCompareHerd(string name)
{
	fileAccess wantedOutput;
	wantedOutput.openFile("wantedoutput/"+name);
	fileAccess actuallyOutput;
	convert conv;

	if(actuallyOutput.openFileWithoutExit("output/"+name)==true)
	{
		string wantedString=wantedOutput.getLineFile();
		string actuallyString=actuallyOutput.getLineFile();
		wantedString=wantedOutput.getLineFile();
		actuallyString=actuallyOutput.getLineFile();
		int line=2;
		while(!wantedOutput.endOfFile())
		{
			line++;
			wantedString=wantedOutput.getLineFile();
			actuallyString=actuallyOutput.getLineFile();
			stringstream streamWanted (wantedString);
			stringstream streamActually (actuallyString);
			string partOfWantedString="NULL";
			string partOfActuallyString="NULL";
			int items=0;
			while(!partOfWantedString.empty())
			{

				items++;
				//cout<<"items"<<items<<endl;
				partOfWantedString.clear();
				streamWanted>>partOfWantedString;
				streamActually>>partOfActuallyString;
				if(partOfWantedString.empty())
				{
					break;
				}
				switch ( items ) {
				  case 1 :
					  if(partOfWantedString.compare(partOfActuallyString)!=0)
					  {
						  string out="In "+name+" at "+conv.IntToString(line)+ " the dates do not match. The dates are:"+partOfWantedString+" and "+partOfActuallyString;
						  output.Write(out);

					  }
					  break;
				  default :
					  convert conv;
					  double wantedDouble=conv.StringToDouble(partOfWantedString);
					  double actuallyDouble=conv.StringToDouble(partOfActuallyString);
					  if(doubleEquals(wantedDouble,0,0.0001)==1)
					  {

						  if(doubleEquals(wantedDouble,actuallyDouble,0.0001)!=1)
						  {
							  double dif=wantedDouble-actuallyDouble;

							  output.Write("In "+name+" at line"+conv.IntToString(line)+" and cell "+ conv.IntToString(items)+" the value do not match.The different are"+conv.DoubleToString(dif));

						  }
					  }
					  else
					  {
						  double dif=(wantedDouble-actuallyDouble)/wantedDouble;

						  if(doubleEquals(dif,0,0.0001)!=1)
						  {

							  output.Write("In "+name+" at line"+conv.IntToString(line)+" and cell "+ conv.IntToString(items)+" the value do not match.The different are"+conv.DoubleToString(dif));

						  }
					  }
					  break;
				}
			}
		}
	}
	else
	{
		output.Write("cannot find file "+name+" in the output dir");
	}
}
void openAndCompareDAIRY(string name)
{
	fileAccess wantedOutput;
	wantedOutput.openFile("wantedoutput/"+name);
	fileAccess actuallyOutput;
	convert conv;

	if(actuallyOutput.openFileWithoutExit("output/"+name)==true)
	{
		string wantedString=wantedOutput.getLineFile();
		string actuallyString=actuallyOutput.getLineFile();
		int line=1;
		while(!wantedOutput.endOfFile())
		{
			line++;
			wantedString=wantedOutput.getLineFile();
			actuallyString=actuallyOutput.getLineFile();
			stringstream streamWanted (wantedString);
			stringstream streamActually (actuallyString);
			string partOfWantedString="NULL";
			string partOfActuallyString="NULL";
			int items=0;
			while(!partOfWantedString.empty())
			{

				items++;
				//cout<<"items"<<items<<endl;
				partOfWantedString.clear();
				streamWanted>>partOfWantedString;
				streamActually>>partOfActuallyString;
				if(partOfWantedString.empty())
				{
					break;
				}
				switch ( items ) {
				  case 1 :
					  if(partOfWantedString.compare(partOfActuallyString)!=0)
					  {
						  string out="In "+name+" at "+conv.IntToString(line)+ " the dates do not match. The dates are:"+partOfWantedString+" and "+partOfActuallyString;
						  output.Write(out);

					  }
					  break;
				  default :
					  convert conv;
					  double wantedDouble=conv.StringToDouble(partOfWantedString);
					  double actuallyDouble=conv.StringToDouble(partOfActuallyString);
					  if(doubleEquals(wantedDouble,0,0.0001)==1)
					  {

						  if(doubleEquals(wantedDouble,actuallyDouble,0.0001)!=1)
						  {
							  double dif=wantedDouble-actuallyDouble;

							  output.Write("In "+name+" at line"+conv.IntToString(line)+" and cell "+ conv.IntToString(items)+" the value do not match.The different are"+conv.DoubleToString(dif));

						  }
					  }
					  else
					  {
						  double dif=(wantedDouble-actuallyDouble)/wantedDouble;

						  if(doubleEquals(dif,0,0.0001)!=1)
						  {

							  output.Write("In "+name+" at line"+conv.IntToString(line)+" and cell "+ conv.IntToString(items)+" the value do not match.The different are"+conv.DoubleToString(dif));

						  }
					  }
					  break;
				}
			}
		}
	}
	else
	{
		output.Write("cannot find file "+name+" in the output dir");
	}
}
void openAndCompareScenarie(string name)
{
	fileAccess wantedOutput;
	wantedOutput.openFile("wantedoutput/"+name);
	fileAccess actuallyOutput;
	convert conv;

	if(actuallyOutput.openFileWithoutExit("output/"+name)==true)
	{

		int line=0;
		while(!wantedOutput.endOfFile())
		{
			line++;
			string wantedString=wantedOutput.getLineFile();
			string actuallyString=actuallyOutput.getLineFile();
			string wantedName=partOfAString(wantedString,1);
			string actuallyName=partOfAString(actuallyString,1);
			if(wantedName.compare(actuallyName)!=0)
				output.Write("in "+name+" the name do not mach at"+ conv.IntToString(line));
			string wantedDay=partOfAString(wantedString,2);
			string actuallyDay=partOfAString(actuallyString,2);
			if(wantedDay.compare(actuallyDay)!=0)
				output.Write("in "+name+" the date do not mach at"+ conv.IntToString(line));
			for(int item=3;item<=6;item++)
			{
				string wantedData=partOfAString(wantedString,item);
				string actuallyData=partOfAString(actuallyString,item);
				double dif=conv.StringToDouble(wantedData)-conv.StringToDouble(actuallyData);
				if(dif!=0)
				{
					output.Write("in "+name+" there are a dif on"+conv.DoubleToString(dif)+" at "+conv.IntToString(line)+" and the item is " +conv.IntToString(item));
				}
			}

		}

	}
	else
	{
		output.Write("cannot find file "+name+" in the output dir");
	}

}
void openAndCompareWater(string name)
{
	fileAccess wantedOutput;
	wantedOutput.openFile("wantedoutput/"+name);
	fileAccess actuallyOutput;
	convert conv;
	if(actuallyOutput.openFileWithoutExit("output/"+name)==true)
	{
		int line=0;
		while(!wantedOutput.endOfFile())
		{
			line++;
			string wantedString=wantedOutput.getLineFile();
			string actuallyString=actuallyOutput.getLineFile();
			string wantedDay=partOfAString(wantedString,1);
			string actuallyDay=partOfAString(actuallyString,1);
			if(wantedDay.compare(actuallyDay)!=0)
				output.Write("in "+name+" the date do not mach at"+ conv.IntToString(line));
			for(int item=2;item<=7;item++)
			{
				string wantedData=partOfAString(wantedString,item);
				string actuallyData=partOfAString(actuallyString,item);
				double dif=conv.StringToDouble(wantedData)-conv.StringToDouble(actuallyData);
				if(dif!=0)
				{
					output.Write("in "+name+" there are a dif on"+conv.DoubleToString(dif)+" at "+conv.IntToString(line)+" and the item is " +conv.IntToString(item));
				}
				}
		}

	}
	else
	{
		output.Write("cannot find file "+name+" in the output dir");
	}

}
bool is_file_exist(string fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}
int function()
{

	output.openFileWrite("output.txt");



	if (_access("output/INDICATX.XLS", 0) == 0)
		openAndCompareInicat("INDICATX.XLS");
	for (int i = 0; i < 10; i++)
	{
		std::string s = std::to_string(i);
		string path = "output/soil" + s + ".xls";
		if (is_file_exist(path))
			openAndCompareSoilFile("soil.xls");
	}
	for (int i = 0; i < 10; i++)
	{
		std::string s = std::to_string(i);

		string path = "output/field" + s + ".xls";
		if (is_file_exist(path))
			openAndCompareSoilFile("field" + s + ".xls");
	}
	if (is_file_exist("output/scenarie.txt"))
		openAndCompareScenarie("scenarie.txt");
	if (is_file_exist("output/herd.txt"))
		openAndCompareHerd("herd.txt");
	if (is_file_exist("output/DAIRY.txt"))
		openAndCompareDAIRY("DAIRY.txt");
	if (is_file_exist("output/waterDetails.txt"))
		openAndCompareWater("waterDetails.txt");
	output.closeFile();
  return(0);
}
int main()
{
	cout<<"start"<<endl;

	function();
		cout<<"done"<<endl;
	    return 0;
}
