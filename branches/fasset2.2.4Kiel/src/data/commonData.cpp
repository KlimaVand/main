/*
 * commonData.cpp
 *
 *  Created on: 16/02/2010
 *      Author: beer
 */
#include "../tools/fileAccess.h"
#include "../tools/convert.h"
#include "commonData.h"
convert conv;
bool commonData::readFileWithoutExit(string fileName)
{
	fileAccess file;
	bool returnValue= file.openFileWithoutExit(fileName);
   if(returnValue==true)
   {
	while(!file.endOfFile())
	{
		string lineRead=file.getLineFile();

		//CK cout << lineRead << endl;

		items.push_back(lineRead);
	}
	file.closeFile();
   }

	return returnValue;
}
bool commonData::readFileWithExit(string fileName)
{
	fileAccess file;
	file.openFile(fileName);
	while(!file.endOfFile())
	{
		string lineRead=file.getLineFile();
		items.push_back(lineRead);
	}
	file.closeFile();
	return true;
}
bool commonData::FindItem(string ItemName,int start, double & output)
{
	string out;
	bool findIt=FindItem(ItemName,start,out);
	if(findIt==true)
	{
		convert conv;
		output= conv.StringToDouble(out);
		return true;
	}
	return false;
}
bool commonData::FindItem(string ItemName,int start, int & output)
{
	string out;
	bool findIt=FindItem(ItemName,start,out);
	if(findIt==true)
	{
		convert conv;
		output= conv.StringToInt(out);
		return true;
	}
	return false;
}
bool commonData::FindItem(string ItemName,int start, bool & output)
{
	string out;
	bool findIt=FindItem(ItemName,start,out);
	if(findIt==true)
	{
		convert conv;
		output= conv.StringToBool(out);
		return true;
	}
	return false;
}
bool commonData::FindItem(string ItemName,int start, string & output)
{
	convert conv;
    start++;
	for(unsigned int i=start;i<items.size();i++)
	{
		string name=conv.partOfAString(items[i],1);

		if(name[0]!='['&&name[name.size()-1]!=']')
		{
			if(ItemName.compare(name)==0)
			{

				output=conv.partOfAString(items[i],2);
				return true;
			}
		}
		else {

			cout <<  "not found: " <<  ItemName << " " << start << " " << output << endl;

			return false;
		}
	}

	cout <<  "not found: " <<  ItemName << " " << start << " " << output << endl;

	return false;

}
int commonData::FindSection(string fileName)
{
	for(unsigned int i=0;i<items.size();i++)
	{
		size_t found=-1;
		found=items[i].find(fileName);
		if (found!=string::npos)
		{

			if(items[i][found+fileName.size()]=='('||items[i][found+fileName.size()]==']')
				return i;
		}
	}
	return -1;
}



bool commonData::loadSection(string sectionName, int &sectionLevel, string nameOfItem)
{
	convert conv;
	int size=sectionName.size();
	if(nameOfItem.substr(1,size).compare(sectionName)==0)
	{
		string substring;
		int times=0;
		int multiplier=1;
		substring= nameOfItem.substr(size+2,1);
		int number=0;
		while(substring.compare(")"))
		{
			number=number*multiplier+conv.StringToInt(substring);
			multiplier=multiplier*10;
			times++;
			substring= nameOfItem.substr(size+2+times,1);

		}
		if(!(sectionLevel+1==number))
		{
			cout<<sectionLevel<< " d "<<number<<endl;
			cout<<sectionLevel+1<<" is missing the output.dat for "<<sectionName<<endl;
			exit(1);
		}
		sectionLevel=number;
		return true;
	}
	return false;
}
