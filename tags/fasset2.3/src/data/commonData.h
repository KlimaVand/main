/*
 * commonData.h
 *
 *  Created on: 16/02/2010
 *      Author: beer
 */

#include "systemData.h"
#include "../tools/fileAccess.h"
#include "../tools/convert.h"
#include "../base/common.h"
using namespace std;
#ifndef COMMONDATA_H_
#define COMMONDATA_H_

class commonData {
private:
	int sectionPlace;
	convert conv;
	bool critical;
	string sectionName;
	string fileNameInUse;
protected:
  std::vector<string> items;
  std::vector<bool> used;
public:
  commonData();
  ~commonData();
  void setCritical(bool aCritical){critical=aCritical;};
  int findSectionThatContains(string common,string item, string section);
  int FindSection(string aSectionName, int sectionNumber=-1);
  bool FindVectorSize(string VectorName,int * output);
  bool FindItem(string ItemName,double * output, int inVector=-1);
  bool FindItem(string ItemName,bool * output, int inVector=-1);
  bool FindItem(string ItemName,int * output, int inVector=-1);
  bool FindItem(string ItemName,string * output, int inVector=-1);
  bool readFileWithExit(string fileName);
  bool readFileWithoutExit(string fileName);
  bool loadSection(string sectionName, int &sectionLevel, string nameOfItem);
  bool  getSectionsNumbers(string sectionName, int * small, int * large);
  bool findSection(string org,int base, string name,string Alias,string * path, int * Index);

};

#endif /* COMMONDATA_H_ */
