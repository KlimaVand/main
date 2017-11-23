/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "base.h"
#include "message.h"
#include "common.h"
#include <sys/stat.h>
#include "../tools/fileAccess.h"
#include <string.h>

fileAccess filesAcceess;
/****************************************************************************\
 Default Constructor
\****************************************************************************/
base::base()
{
   InitVariables();
}

/****************************************************************************\
 Constructor with arguments
\****************************************************************************/
base::base(const char * aName, const int aIndex, const base * aOwner)
{
   InitVariables();
   strcpy(Name,aName);
   Index      = aIndex;
   Owner      = aOwner;
}

/****************************************************************************\
\****************************************************************************/
void base::InitVariables()
{
	id =threadID->getID(std::this_thread::get_id());
   strcpy(Name,"");
   Index      =-1;
   Owner      = NULL;
   cur_pos    = 0;

   fileNme[0] = '\0';

   error      = false;
}

/****************************************************************************\
 Copy Constructor
 Does NOT copy "file", as there is no copy constructor for "fstream"
\****************************************************************************/
base::base(const base& aBase)
{
	id=aBase.id;
   strcpy(Name,aBase.Name);
   Index      = aBase.Index;
   Owner      = aBase.Owner;
   cur_pos    = aBase.cur_pos;

   fileNme[0] = '\0';

   error      = false;
}

/****************************************************************************\
 Destructor
\****************************************************************************/
base::~base()
{


}

/****************************************************************************\
 Assignment operator
\****************************************************************************/
base& base::operator = (const base& aBase)
{
   if(this==&aBase)
      return *this;
   strcpy(Name,aBase.Name);
   Index      = aBase.Index;
   Owner      = aBase.Owner;
   cur_pos    = aBase.cur_pos;

   fileNme[0] = '\0';
   error      = false;

   return *this;
}


/****************************************************************************\
 Removes all spaces
\****************************************************************************/
void base::EatSpaces(char * s)
{
   int source=0;
   int dest=0;
   while (s[source]!='\0')
   {
      while (s[source]==' ')
         source++;
      s[dest]=s[source];
      dest++;
      source++;
   }
   s[dest]='\0';
}



/****************************************************************************\
\****************************************************************************/
string base::GetLongName()
{
   base * p=this;
   char s1[255];
	s1[0] = '\0';
   string s;
   while (p)
   {
      char s2[255];
      s2[0]='\0';
      strcat(s2,p->GetName());
      if (s2[0]!='\0')
      {
         sprintf(s1,"%s(%d)",s2,p->GetIndex());
         s="/"+(string)s1+s;
         p=(base*)p->Owner;
      }
      else
         p=NULL;
   }
   return s;
}
string base::GetLongNameInNumbers()
{
   base * p=this;
   char s1[255];
	s1[0] = '\0';
   string s;
   while (p)
   {
      char s2[255];
      s2[0]='\0';
      strcat(s2,p->GetName());
      if (s2[0]!='\0')
      {
         sprintf(s1,"%s%d","_",p->GetIndex());
         s=(string)s1+s;
         p=(base*)p->Owner;
      }
      else
         p=NULL;
   }

   if(s.size()!=0)
   s=s.substr(1,s.size()-1);

   return s;
}

