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
commonData::commonData()
{
	critical=true;
	sectionPlace=-1;
}
commonData::~commonData()
{
	/*for(int i=0;i<used.size();i++)
	{
		if(used[i]==false)
		{
			string output= items[i]+" has not been used in file " +fileNameInUse;
				theMessage->Warning(output);
		}

	}*/
}
bool commonData::readFileWithoutExit(string fileName)
{
	fileNameInUse=fileName;
	while(!items.empty())
	{
		items.pop_back();
		used.pop_back();
	}
	fileAccess file;
	bool returnValue= file.openFileWithoutExit(fileName);
   if(returnValue==true)
   {
	while(!file.endOfFile())
	{
		string lineRead=file.getLineFile();
		items.push_back(lineRead);
		used.push_back(false);
	}
	file.closeFile();
   }


	return returnValue;
}
bool commonData::readFileWithExit(string fileName)
{
	fileNameInUse=fileName;
	while(!items.empty())
	{
			items.pop_back();
			used.pop_back();
	}

	fileAccess file;
	file.openFile(fileName);
	while(!file.endOfFile())
	{
		string lineRead=file.getLineFile();
		items.push_back(lineRead);
		used.push_back(false);
	}
	file.closeFile();
	return true;
}
bool commonData::FindItem(string ItemName, double * output, int inVector)
{
	string out;
	bool findIt=FindItem(ItemName,&out,inVector);
	if(findIt==true)
	{
		*output= conv.StringToDouble(out);
		return true;
	}
	return false;
}
bool commonData::FindItem(string ItemName, int * output, int inVector)
{
	string out;
	bool findIt=FindItem(ItemName,&out,inVector);
	if(findIt==true)
	{

		*output= conv.StringToInt(out);
		return true;
	}
	return false;
}
bool commonData::FindItem(string ItemName, bool * output, int inVector)
{
	string out;
	bool findIt=FindItem(ItemName,&out,inVector);
	if(findIt==true)
	{
		*output= conv.StringToBool(out);
		return true;
	}
	return false;
}
bool commonData::FindItem(string ItemName,string * output, int inVector)
{
	if(sectionPlace==-1)
	{
		string output="something has gone wrong with fining of section" +sectionName;
		theMessage->FatalError(output);
	}
	if(items[sectionPlace][0]!='['&&items[sectionPlace][items[sectionPlace].size()-1]!=']')
	{
		string output="something has gone wrong with fining of section" +sectionName;
		theMessage->FatalError(output);
	}
	if(inVector!=-1)
	{
		ItemName=ItemName+'('+conv.IntToString(inVector)+')';
	}
	for(unsigned int i=sectionPlace+1;i<items.size();i++)
	{
		string name=conv.partOfAString(items[i],1);

		if(ItemName.compare(name)==0)
		{
			*output=conv.partOfAString(items[i],2);
			used[i]=true;
			return true;
		}
		if(items[i][0]=='['&&items[i][items[i].size()-1]==']')
		{
				break;
		}

	}
	string outputForMessage="could not find "+ItemName + " in " +sectionName+" in file "+fileNameInUse;
	if(critical==true)
	{

		theMessage->FatalError(outputForMessage);
	}

	return false;

}
int commonData::FindSection(string aSectionName, int sectionNumber)
{
	sectionName=aSectionName;
	if(sectionNumber!=-1)
	{
		aSectionName=aSectionName+'('+conv.IntToString(sectionNumber)+')';
	}
	aSectionName='['+aSectionName+']';

	for(unsigned int i=0;i<items.size();i++)
	{

		string tmp= conv.partOfAString(items[i],1);

		if(aSectionName.compare(tmp)==0)
		{
			sectionPlace=i;
			used[i]=true;
			return i;

		}
	}


	string output="could not find "+sectionName;
	theMessage->FatalError(output);


	return -1;
}
bool commonData::FindVectorSize(string VectorName,int * output)
{
	int size=VectorName.size();
	for(unsigned int i=sectionPlace+1;i<items.size();i++)
	{
		string itemName=items[i];

		string itemNameShort=itemName.substr(0,size);
		if(itemNameShort.compare(VectorName)==0)
		{
			*output=conv.StringToInt(itemName.substr(size+1,itemName.size()-2));
			*output+=1;
		}
		if(itemName[0]=='['&&itemName[itemName.size()]==']')
			break;
	}
	return false;
}
int commonData::findSectionThatContains(string common,string item, string section)
{
	int small=9999;
	int large=0;
	getSectionsNumbers(section, &small, &large);

	string result="NULL";
	for(int i=small;i<=large;i++)
	{
		FindSection(section, i);
		FindItem(common,&result);

		if(item.compare(result)==0)
			return i;
	}
	if(critical==true)
	{
		string output="could not find "+common + " in " +section;
		theMessage->FatalError(output);
	}
	return -1;

}
bool  commonData::getSectionsNumbers(string sectionName, int * small, int * large)
{
	*small=-1;
	*large=-2;
	bool found=false;
	string forCompare="["+sectionName;
	int size=forCompare.size();

	for(unsigned int i=0;i<items.size();i++)
	{

		if(items[i].substr(0,size).compare(forCompare)==0)
		{
			int last=items[i].find("]",size)-size;
			int tmp =conv.StringToInt(items[i].substr(size+1,last-2));
			if(found==false)
			{
				*small=tmp;
				*large=tmp;
				found=true;
			}
			else
			{
				if(*large<tmp)
				{
					*large=tmp;
				}
				if(*small>tmp)
				{
					*small=tmp;
				}
			}


		}

	}
	if(found==false&&critical==true)
	{
		theMessage->FatalError("cannot find section "+ sectionName);
	}
	return found;
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

			string output=conv.IntToString(sectionLevel+1)+" is missing for "+sectionName;
			theMessage->FatalError(output);
		}
		sectionLevel=number;
		return true;
	}
	return false;
}

//! Will try to find a section where name has the value of a Alias
  /*!
   * \param org contains base like PigHousing(0).Section(0).floor(0)
   * \param base should start with first section that should be include eg 0 for PigHousing
   * \param name of the given keyword
   * \param Alias is the value of that keyword
   * */

bool commonData::findSection(string org,int base, string name,string Alias, string * path, int * Index)
{

	convert conv;
	unsigned int first=org.find('(', base+1);
	if (first==string::npos)
	{
		bool tmp=critical;
		critical=false;
		string test=org;
		int place=findSectionThatContains(name,Alias,test);
		*path=org;
		*Index=place;

		critical=tmp;
		return true;
	}
	string SectionInOld=org.substr(0,first);
	int endbrakst=org.find(')',first);
	int largest=0;
	int small=0;

	getSectionsNumbers(SectionInOld, &small, &largest);
	bool returnVal=false;
	for(int i=small;i<=largest;i++)
	{

		string Newstring=SectionInOld+"("+conv.IntToString(i)+org.substr(endbrakst,org.size());

		returnVal=findSection(Newstring,first+1, name, Alias, path, Index);

		return returnVal;

	}
	return returnVal;

}

