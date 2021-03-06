/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
   Author   Lars Noboe Andersen & Poul Lassen
     Date
 Revision   Bj�rn Molt Petersen, May-June 1997
\****************************************************************************/

#include "../base/common.h"
#include "gang.h"
#include <string.h>
/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
gang::gang(const char * aName, const int aIndex, const base * aOwner)
   : base(aName, aIndex, aOwner)
{
   gangHours = 0.0;
   gangFuel = 0.0;
   gangElectricity = 0;

   techEquipList = new linkList <techEquip>;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
gang::~gang()
{
   if(techEquipList)
      delete techEquipList;
}

/****************************************************************************\
 Function:   Initialize
 Purpose:    Creates gangs technical equipment list from the operations
             teEqHead list and the technical equipment list
\****************************************************************************/
int gang::Initialize(linkList <string> * aStringList, linkList <techEquip> * aTechFarmList)
{
   linkList <string>::PS     aTechString;
   linkList <techEquip>::PS  aTechFarm;
   int retVal = 1;

   aStringList->PeekHead(aTechString);
   while(aTechString!=nullptr)
   {
      aTechFarmList->PeekHead(aTechFarm);
      while(aTechFarm!=nullptr)
      {
         if (aTechFarm->element->GettechEquipName()==*aTechString->element)
            break;
         aTechFarm=aTechFarm->next;
      }
      if(aTechFarm!=nullptr)
         AppendTechEquipList(aTechFarm->element);
      else
         retVal=0;
   aTechString=aTechString->next;
   }
	return retVal;
}

/****************************************************************************\
 Function:   GetMainTechEquip
 Purpose:    Returns a pointer to the main technical equipment (first element
             in the list)
\****************************************************************************/
techEquip * gang::GetMainTechEquip()
{
// ADVARSEL: Pointer returneres til mainTechEquip i listen.
// Aendrer man derfor paa den returnerede vaerdi, vil aendringen
// ogsaa ske i listen!

   linkList <techEquip>::PS aMainTechEquipNode;
   techEquipList->PeekHead(aMainTechEquipNode);
   return aMainTechEquipNode->element;
}


/****************************************************************************\
\****************************************************************************/
linkList <techEquip> * gang::GetTechEquipList()
{
   return techEquipList;
}

/****************************************************************************\
 Function:   AppendTechEquipList
 Purpose:    Appends a technical equipment to the gangs technical equipment
             list
\****************************************************************************/
void gang::AppendTechEquipList(techEquip * aTechEquip)
{
   if(strcmp(aTechEquip->GetName(),"installation")==0)
   {
      installation * installationInstance;
      installationInstance = new installation("installation",0,this);
      *installationInstance= *(installation*)aTechEquip;
      techEquipList->InsertLast(installationInstance);
   }

   if(strcmp(aTechEquip->GetName(),"manureApplicator")==0)
   {
      manureApplic * manureApplicInstance = new manureApplic("manureApplicator",0,this);
      *manureApplicInstance= *(manureApplic*)aTechEquip;
      techEquipList->InsertLast(manureApplicInstance);
   }


}

/****************************************************************************\
 Function:   OutputGang
 Purpose:    Action for outputting data concerning the consumption of labour,
             machinery and energy
\****************************************************************************/
void gang::OutputGang(string aOperationName)
{
/*	fstream * file=theMessage->GiveHandle();
   *file
      << theTime
      << " "
      << setfill(' ')
      << setiosflags(ios::left)
      << setw(20)
      << aOperationName
      << " "
      << setiosflags(ios::right)
      << setprecision(2)
      << setw(8)
      << gangHours
      << " hours "
      << setprecision(2)
      << setw(8)
      << gangFuel
      << " l diesel"
      << endl ;
	*/
}

