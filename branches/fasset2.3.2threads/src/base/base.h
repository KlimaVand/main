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
#include "id.h"
//inserted by NJH Feb 2003



class base
{
   protected:

   	enum {MAX_TXT=255};
int id;
      char         Name[MAX_TXT];
      int          Index;
      const base * Owner;
      long         cur_pos;
      bool         error;



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


      void InitVariables();

      void Setcur_pos(long acur_pos) {cur_pos = acur_pos;};
      void SetOwner(const base* aOwner) {Owner = aOwner;};
      const char * GetName(void) const {return Name;};

      const base* GetOwner() {return Owner;};

      string GetLongName();
      string GetLongNameInNumbers();
      int GetIndex() {return Index;};
      void SetSignPositive();
      void SetSignNegative();
      void UnsetSign();

      void SetIndex(int anIndex) {Index=anIndex;};

};

#endif

