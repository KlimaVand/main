#include <stdlib.h>
#include <iostream>
#include <string>

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

	bool StringToBool(string input);

};

#endif /* FILEACCESS_H_ */
