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
   aGang = NULL;
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
   aGang=NULL;
}

/****************************************************************************\
\****************************************************************************/
void operation::ReadParameters(fstream * file)
{
   string * techEquipName;
   char       charVar[80];
   const int MAX_TXT    = 81;
   char section[MAX_TXT];
   section[0] = '\0';

   Setfile(file);
   SetCritical();
   FindSection(Name,Index); // Top of hierarcy

   GetParameter("operationName",charVar);
   operationName=charVar;
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
   FindSection(section,1);
   techEquipName = new string;
   GetParameter("techEquipName",techEquipName);
   techEquipList->InsertLast(techEquipName);

   sprintf(section,"%s(%d).techEquip",Name,Index);
   int first,num;
   GetSectionNumbers(section,&first,&num);
   for(int index=first;index<(first+num);index++)
   {
      FindSection(section,index);
      techEquipName = new string;
      GetParameter("techEquipName",techEquipName);
      techEquipList->InsertLast(techEquipName);
   }
   FindSection(Name,Index);  // The reading of equipment parameters nescisitates this
   Setfile(NULL);
}


/****************************************************************************\
\****************************************************************************/
void operation::CalcGangElectricity()
{
   double       aGangElectricity=0.0;
   installation aInstallation("installation",0,this);

   linkList <techEquip> * aTechEquipList;
   linkList <techEquip>::PS aTechEquipNode;

   aTechEquipList=aGang->GetTechEquipList();
   aTechEquipList->PeekHead(aTechEquipNode);

   while (aTechEquipNode!=NULL)
   {
      if (strcmp(aTechEquipNode->element->GetName(),"installation")==0)
      {
         aInstallation= *(installation *)(aTechEquipNode->element);
         aGangElectricity+= aInstallation.GetelectricityConsumption();
      }
      aTechEquipNode=aTechEquipNode->next;
   }

   aGangElectricity*=aGang->GetgangHours();
   aGang->SetgangElectricity(int(aGangElectricity));
}

/****************************************************************************\
\****************************************************************************/
void operation::Update(linkList <techEquip> * aTechFarmList)
{
   CalcGangHours();
   CalcGangFuel();
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
   while (aTechEquipInGang!=NULL)
   {
      aTechFarmList->PeekHead(aTechEquipInFarmList);
      while (aTechEquipInFarmList!=NULL)
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

