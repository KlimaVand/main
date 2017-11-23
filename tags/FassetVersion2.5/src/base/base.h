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

     public:
      // Default Constructor
      base();
      // Constructor with arguments
      base(const char * aName, const int aIndex = -1, const base * aOwner = nullptr);
      // Copy Constructor
      base(const base& aBase);
      // Destructor
      virtual ~base();
      // Assignment operator
      base& operator = (const base& aBase);


      void InitVariables();

      void SetOwner(const base* aOwner) {Owner = aOwner;};
      const char * GetName(void) const {return Name;};

      const base* GetOwner() {return Owner;};

      string GetLongName();
      string GetLongNameInNumbers();
      int GetIndex() {return Index;};


      void SetIndex(int anIndex) {Index=anIndex;};

};

#endif

