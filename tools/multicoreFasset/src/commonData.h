/*
 * commonData.h
 *
 *  Created on: 16/02/2010
 *      Author: beer
 */


#include "fileAccess.h"
#include "convert.h"
#include <vector>
#include <sstream>
using namespace std;
#ifndef COMMONDATA_H_
#define COMMONDATA_H_

class commonData {
private:

	convert conv;

	string sectionName;
	string fileNameInUse;
	bool secnodaryFileInUse;
	int sectionPlace2;
	string fileNameInUse2;
protected:
	bool critical;

  std::vector<string> itemsName;
  std::vector<string> itemsValue;
  std::vector<bool> used;
  std::vector<string> itemsName2;
  std::vector<string> itemsValue2;
   std::vector<bool> used2;
public:
   int sectionPlace;
  commonData();
  ~commonData();
  void setCritical(bool aCritical){critical=aCritical;};
  int findSectionThatContains(string common,string item, string section);
  int FindSection(string aSectionName, int sectionNumber=-1);
  string getLine(int lineNumber);
  bool FindVectorSize(string VectorName,int *small,int * large);
  bool FindItem(string ItemName,double * output, int inVector=-1);
  bool FindItem(string ItemName,bool * output, int inVector=-1);
  bool FindItem(string ItemName,int * output, int inVector=-1);
  bool FindItem(string ItemName,string * output, int inVector=-1);
  bool readFile(string fileName);
  bool readSecondaryFile(string fileName);
  bool loadSection(string sectionName, int &sectionLevel, string nameOfItem);
  bool  getSectionsNumbers(string sectionName, int * small, int * large);
  bool findSection(string org,int base, string name,string Alias,string * path, int * Index);

};

#endif /* COMMONDATA_H_ */
