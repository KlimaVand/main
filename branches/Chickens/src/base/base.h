/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __BASE_H
   #define __BASE_H

#include "typer.h"
#include "common.h"
//inserted by NJH Feb 2003



class base
{
   protected:

   	enum {MAX_TXT=255};

      char         Name[MAX_TXT];
      int          Index;
      const base * Owner;
      long         cur_pos;
      fstream    * file;
      bool         error;
      bool         critical;


      char fileNme[MAX_TXT];
      void EatSpaces(char * s);

   public:
      // Default Constructor
      base();
      // Constructor with arguments
      base(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      base(const base& aBase);
      // Destructor
      virtual ~base();
      // Assignment operator
      base& operator = (const base& aBase);

      void Terminate(const char * c1="", const char * c2="", const char * c3="", const char * c4="");
      void InitVariables();
      void Setfile(fstream * somefile) {file = somefile;};
      void Setcur_pos(long acur_pos) {cur_pos = acur_pos;};
      void SetOwner(const base* aOwner) {Owner = aOwner;};
      const char * GetName(void) const {return Name;};

      const base* GetOwner() {return Owner;};

      void SetCritical();
      void UnsetCritical();

      bool OpenInputFileString(string fileName);
      bool OpenInputFile(char * fileName); // �ndre til void ???
      void CloseInputFile();
      void DeleteInputFile();
      void GetSectionNumbers(const char * sectionName, int * firstNum, int * numOfSections);
      void GetVectorInfo(const char * parameter,int * firstNum,int * vectorSize);
      bool FindSection(const char * sectionName);
      bool FindSection(const string sectionName);
      bool FindSection(const char * sectionName, int index);
      bool GetParameter(const char * parameter, char   * p_string, int index = -1);
      bool GetParameter(const char * parameter, int    * p_value,  int index = -1);
      bool GetParameter(const char * parameter, double * p_value,  int index = -1);
      bool GetParameter(const char * parameter, bool   * p_value,  int index = -1);
      bool GetParameter(const char * parameter, string * aString,  int index = -1);
      void ReadLine(fstream * f,char * s,int maxlen);
      string GetLongName();
      string GetLongNameInNumbers();
      int GetIndex() {return Index;};
      void SetSignPositive();
      void SetSignNegative();
      void UnsetSign();

      void SetIndex(int anIndex) {Index=anIndex;};
      bool FileExists(string strFilename);
};

#endif

