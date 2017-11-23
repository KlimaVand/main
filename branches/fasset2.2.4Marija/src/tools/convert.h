#include <stdlib.h>
#include <iostream>
#include <string>
#include "../base/linklist.h"
#include "../products/plantItem.h"
#ifndef CONVERT_H_
#define CONVERT_H_

using namespace std;
class convert{
private:


public:
	convert();
	int StringToInt(string convert);
	string IntToString(int conv);
	string DoubleToChar(double conv);
	double StringToDouble(string conv);
	string EatSpaces(string s);
	bool StringToBool(string input);

	string GetUnitTypeIntToString(int aMainUnit);
	int GetUnitTypeStringToInt(string aMainUnit);

	int GetChemType(string aChemType);
	string partOfAString(string input, int number);
	int GetHarvestProductCode(linkList<plantItem>* HarvestList);
private:
	string partOfAStringBorland(string input, int number);
	string partOfAStringC(string input, int number);
	void BlendProducts(linkList<plantItem>* HarvestList, int feedCode);

};

#endif /* FILEACCESS_H_ */
