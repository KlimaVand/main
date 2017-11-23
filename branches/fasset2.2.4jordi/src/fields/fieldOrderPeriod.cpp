/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "fieldOrderPeriod.h"
#include <string.h>
/****************************************************************************\
\****************************************************************************/
fieldOrderPeriod::fieldOrderPeriod(const char * aName, const int aIndex, const base * aOwner = NULL)
   :base(aName,aIndex,aOwner)
{
   TSum = -99.0;
   soilMoisture=100.0;
   upper=true;
}

/****************************************************************************\
\****************************************************************************/
void fieldOrderPeriod::ReadPeriod(fstream *aFile)
{
	Setfile(aFile);
	char variableString[MAX_TXT];
   char * tagString=new char[10];
   strcpy(tagString,"StartDate");
   string * s = new string;     //the 's' strings are copied to date strings in this class and are deleted when the instance is deleted
   strcpy(variableString,GetName());
   strcat(variableString,tagString);
   SetCritical();
   if (GetParameter(variableString,s,Index))
   {
      if (s->length()==0)
     	 theMessage->FatalError("Format for start date incorrect");
      StartDate=s;
      strcpy(variableString,GetName());
      strcat(variableString,"EndDate");
	   s = new string;
      GetParameter(variableString,s,Index);
      if (s->length()==0)
     	 theMessage->FatalError("Format for end date incorrect");
      EndDate=s;
      strcpy(variableString,GetName());
      strcat(variableString,"TSum");
      GetParameter(variableString,&TSum,Index);
      strcpy(variableString,GetName());
      strcat(variableString,"Moist");
      GetParameter(variableString,&soilMoisture,Index);
      UnsetCritical();
      strcpy(variableString,GetName());
      strcat(variableString,"Upper");
      bool anUpper;
      upper = true;
      if (GetParameter(variableString,&anUpper,Index))
	      upper=anUpper;

   }
//   delete s;
	Setfile(NULL);
};

bool fieldOrderPeriod::operator!= (const fieldOrderPeriod& somePeriod) const
{
	bool ret_val=false;
   if ((*StartDate != *somePeriod.StartDate)||
   		(*EndDate != *somePeriod.EndDate) ||
		   (TSum != somePeriod.TSum) ||
         (soilMoisture != somePeriod.soilMoisture)||
   		(upper != somePeriod.upper))
         ret_val=true;

    return ret_val;
}

