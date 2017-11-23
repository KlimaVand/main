/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
   Author   Lars Noboe Andersen & Poul Lassen
     Date
 Revision   Bjørn Molt Petersen, May-November 1997
\****************************************************************************/

#include "../base/common.h"
#include "technics.h"
#include "operation.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
operation::operation(const char * aName, const int aIndex, const base * aOwner)
   : base(aName, aIndex, aOwner)
{
   cost = 0.0;
   oldCost= 0.0;
   techEquipList = new linkList <string>;
   aGang = nullptr;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
operation::~operation()
{
   if (techEquipList)
      delete techEquipList;
   if (aGang)
      delete aGang;
}

/****************************************************************************\
 Function:   CreateGang
 Purpose:    Calls the gangs initializing function
\****************************************************************************/
int operation::CreateGang(linkList <techEquip> * aTechFarmList)
{
   aGang = new gang("gang",0,this);
   return aGang->Initialize(techEquipList,aTechFarmList);
}

/****************************************************************************\
\****************************************************************************/
void operation::DeleteGang()
{
   delete aGang;
   aGang=nullptr;
}

/****************************************************************************\
\****************************************************************************/
void operation::ReadParameters(commonData * &file)
{
   string * techEquipName;
   const int MAX_TXT    = 81;
   char section[MAX_TXT];
   section[0] = '\0';

   file->setCritical(true);

  file->FindSection(Name,Index); // Top of hierarcy

   file->FindItem("operationName",&operationName);

   int i=0;
   int j=-1;
   while (j<0 && i<numberOfOperations)
   {
      if (operationNameList[i]==operationName)
         j=i;
      i++;
   }
   if (j<0)
      theMessage->FatalError("operation::ReadOperationParameters - unknown operation/operation not found");
   operationNumber=j;
   sprintf(section,"%s(%d).mainTechEquip",Name,Index);
   file->FindSection(section,1);
   techEquipName = new string;
   file->FindItem("techEquipName",techEquipName);
   techEquipList->InsertLast(techEquipName);

   sprintf(section,"%s(%d).techEquip",Name,Index);
   int first,num;
   file->getSectionsNumbers(section,&first,&num);
   for(int index=first;index<(first+num);index++)
   {
	  file->FindSection(section,index);
      techEquipName = new string;
      file->FindItem("techEquipName",techEquipName);
      techEquipList->InsertLast(techEquipName);
   }
   file->FindSection(Name,Index);  // The reading of equipment parameters nescisitates this

}


/****************************************************************************\
\****************************************************************************/
void operation::CalcGangElectricity()
{

}

/****************************************************************************\
\****************************************************************************/
void operation::Update(linkList <techEquip> * aTechFarmList)
{
   CalcGangHours();

   CalcGangElectricity();
   UpdateFarmList(aTechFarmList);
   UpdateProducts();
   theTechnics->UpdateFieldLabour(aGang->GetgangHours());
   aGang->OutputGang(operationName);
}

/****************************************************************************\
 Function:   UpdateFarmList
 Purpose:    Updates the farms technical equipment list
\****************************************************************************/
void operation::UpdateFarmList(linkList <techEquip> * aTechFarmList)
{
   linkList <techEquip> *   aTechEquipListGang;
   linkList <techEquip>::PS aTechEquipInGang;
   linkList <techEquip>::PS  aTechEquipInFarmList;

   aTechEquipListGang=aGang->GetTechEquipList();
   aTechEquipListGang->PeekHead(aTechEquipInGang);
   while (aTechEquipInGang!=nullptr)
   {
      aTechFarmList->PeekHead(aTechEquipInFarmList);
      while (aTechEquipInFarmList!=nullptr)
      {
         if(aTechEquipInGang->element->GettechEquipName()==
            aTechEquipInFarmList->element->GettechEquipName())
	            aTechEquipInFarmList->element->AddhoursUltimo(aGang->GetgangHours());
         aTechEquipInFarmList=aTechEquipInFarmList->next;
      }
      aTechEquipInGang=aTechEquipInGang->next;
   }
}

/****************************************************************************\
\****************************************************************************/
int operation::GetoperationNumber()
{
	return operationNumber;
}

