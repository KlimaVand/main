//============================================================================
// Name        : checkStandartBuilding.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "base/commonData.h"
#include "tools/convert.h"
#include <vector>
using namespace std;
void check(string name)
{
	commonData standartBuilding;
		vector<string> alias;
		standartBuilding.readFile("StandardBuildings.dat");
		standartBuilding.setCritical(false);
		int small, large;

		standartBuilding.getSectionsNumbers(name,&small,&large);

		for(int i=small;i<=large;i++)
		{
			standartBuilding.setCritical(true);
			standartBuilding.FindSection(name,i);
			string item;

			standartBuilding.FindItem("Alias",&item);
			standartBuilding.setCritical(false);
			alias.push_back(item);
		}
		for(int i=0;i<alias.size();i++)
		{

			for(int j=0;j<alias.size();j++)
			{
				if(j!=i)
				{
					if(alias.at(i).compare(alias.at(j))==0)
					{
						cout<<"there is 2 of "<<alias.at(j)<<" in "<<name<<endl;
						exit(99);
					}
				}
			}
		}
}
int main() {

	check("surface");
	check("Slurrystore");
	check("Store");
	check("BiogasReactor");
	check("DigestateStore");
	check("Dungheap");
	check("Pigstable");
	commonData standartBuilding;
	standartBuilding.readFile("StandardBuildings.dat");
	standartBuilding.setCritical(false);
	int small, large;
	standartBuilding.getSectionsNumbers("Pigstable",&small,&large);
	convert conv;
	for(int i=small;i<=large;i++)
	{
		string name="Pigstable("+conv.IntToString(i)+").inhouse";
		check(name);
		name="Pigstable("+conv.IntToString(i)+").Section";
		check(name);
		int small1, large1;
		standartBuilding.getSectionsNumbers(name,&small1,&large1);
		for(int j=small1;j<=large1;j++)
		{
			name+="("+conv.IntToString(j)+").floor";
			check(name);
		}
	}
	check("CattleHousing");
	standartBuilding.getSectionsNumbers("CattleHousing",&small,&large);

	for(int i=small;i<=large;i++)
	{
		string name="CattleHousing("+conv.IntToString(i)+").inhouse";
		check(name);
		name="CattleHousing("+conv.IntToString(i)+").Section";
		check(name);
		int small1, large1;

		standartBuilding.getSectionsNumbers(name,&small1,&large1);
		for(int j=small1;j<large1;j++)
		{
			name+="("+conv.IntToString(j)+").floor";
			check(name);
		}
	}
	cout<<"no dublicated Alias detected"<<endl;
	return 0;
}
