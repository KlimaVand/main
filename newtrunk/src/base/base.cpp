
#include "base.h"
#include "message.h"
#include "common.h"
#include <sys/stat.h>
#include "../tools/fileAccess.h"
#include <string>
#include <cstring> 
fileAccess filesAcceess;

base::base() {
    InitVariables();
}

base::base(string       aName,
           const int    aIndex,
           const base * aOwner) {
    InitVariables();

    Name  = aName;
    Index = aIndex;
    Owner = aOwner;
}

void base::InitVariables() {
    Name  = "";
    Index = -1;
    Owner = nullptr;
}

/*
 * Copy Constructor
 * Does NOT copy "file", as there is no copy constructor for "fstream"
 */
base::base(const base & aBase) {
    Name  = aBase.Name;
    Index = aBase.Index;
    Owner = aBase.Owner;
}

base::~base() {}

/*
 *
 * @param aBase
 * @return
 */
base & base::operator = (const base & aBase) {
    if (this == &aBase) {
        return *this;
    }

    Name  = aBase.Name;
    Index = aBase.Index;
    Owner = aBase.Owner;

    return *this;
}

string base::GetLongName() {
    base * p = this;
    string s1="empty";

    s1[0] = '\0';

    string s;

    while (p) {
        string s2="empty";

        s2[0] = '\0';
        s2    += p -> GetName();

        if (s2[0] != '\0') {
            s1 = s2 + "(" + conv.IntToString(p -> GetIndex()) + ")";
            s  = "/" + (string) s1 + s;
            p  = (base *) p -> Owner;
        } else {
            p = nullptr;
        }
    }

    return s;
}

string base::GetLongNameInNumbers() {
   base * p=this;
   char s1[255];
	s1[0] = '\0';
   string s;
   while (p)
   {
      char s2[255];
      s2[0]='\0';
	  strcat(s2,p->GetName().c_str());
      if (s2[0]!='\0')
      {
         sprintf(s1,"%s%d","_",p->GetIndex());
         s=(string)s1+s;
         p=(base*)p->Owner;
      }
      else
         p=nullptr;
   }

   if(s.size()!=0)
   s=s.substr(1,s.size()-1);

   return s;
}

