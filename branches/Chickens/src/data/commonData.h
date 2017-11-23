/*
 * commonData.h
 *
 *  Created on: 16/02/2010
 *      Author: beer
 */

#include "systemData.h"
#include "../tools/fileAccess.h"
#include "../base/common.h"
using namespace std;
#ifndef COMMONDATA_H_
#define COMMONDATA_H_

class commonData {
protected:
  std::vector<string> items;
public:
  int FindSection(string fileName);
  bool FindItem(string ItemName,int start, double & output);
  bool FindItem(string ItemName,int start, bool & output);
  bool FindItem(string ItemName,int start, int & output);
  bool FindItem(string ItemName,int start, string & output);
  bool readFileWithExit(string fileName);
  bool readFileWithoutExit(string fileName);
  bool loadSection(string sectionName, int &sectionLevel, string nameOfItem);

};

#endif /* COMMONDATA_H_ */
