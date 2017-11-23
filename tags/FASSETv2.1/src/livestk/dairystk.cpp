/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/*****************************
 modified by NJH 6/3/00
*****************************/
#include <common.h>
#include "dairystk.h"
#include <indicat.h>
#include <thebuild.h>
#include <nixcbits.h>
#include <output.h>

//#include <panimal.h>

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
dairyStock::dairyStock(const char * aName, const int aIndex, const base * aOwner)
			:animalStock(aName,aIndex,aOwner)
{
   grouplist = new linkList<cattleGroup>;
   pointyCalves = new linkList<cattle>;
   oldies = new linkList<cattle>;
   wasFemale = false;
   delete theStock;
   theStock = NULL;
   IDnumber = 0;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
dairyStock::~dairyStock()
{
   if (pointyCalves->NumOfNodes())
   {
   		theMessage->FatalError("dairyStock::pointy calves should be empty.");
   }
   delete pointyCalves;  //should be empty
// call destructors for all groups and animals
   delete grouplist;
   if (oldies->NumOfNodes())
   {
   		theMessage->FatalError("dairyStock::oldies should be empty.");
   }
    delete oldies;
    CattlefileStream->close();
    delete CattlefileStream;
}

/****************************************************************************\
	ReceivePlan read the plan for all animals
\****************************************************************************/
void dairyStock::ReceivePlan(char* fileExtension)
//Calculates number of animals to be imported + herd structure
{
   char fileName[15];
   strcpy(fileName,"cattle.");
   strcat(fileName,fileExtension);
   UnsetCritical();
   if(!OpenInputFile(fileName))
   {
   	strcpy(fileName,"cattle.fnn");
      if (!OpenInputFile(fileName))
		   {theMessage->FatalError("dairystock::ReceivePlan - error in opening cattle.f??");}
   }

// Get number of animals produced per year
   double numAnimals;
   Setcur_pos(0);
	if (aggregation==0)
   	individual = true;
	else
   {
   	individual = false;
		aggregateNumber = aggregation;
	}
   double numberPerGroup = 0;

//Note that this next part breaks convention that a class should read its own parameters
   for(int i=0;i<grouplist->NumOfNodes();i++)
   {
      cattleGroup * agroup = grouplist->ElementAtNumber(i);
      string name = agroup->GetAlias();
      numAnimals=0;
      GetParameter(name.c_str(),&numAnimals);
      if (individual)
      {
        	agroup->SetnumberOfCohorts(int(numAnimals));
        	agroup->SetnumberPerCohort(1.0);
      }
      else
      {
      	if (numAnimals>0.0)
         {
            numberPerGroup = numAnimals/aggregateNumber;
            agroup->SetnumberOfCohorts(aggregateNumber);
            agroup->SetnumberPerCohort(numberPerGroup);
   //            cout << numberPerGroup << " " << extras << endl;
   		}
		   else
         {
            agroup->SetnumberOfCohorts(0);
            agroup->SetnumberPerCohort(0);
         }
      }
   }
   if (GetTotalAnimals()==0.0)  //this is initial call - no animals present
   {
      	SeedGroups();
   }
   else
   {
      for(int i=0;i<grouplist->NumOfNodes();i++)
      {
         cattleGroup * agroup = grouplist->ElementAtNumber(i);
         string name = agroup->GetAlias();
         GetParameter(name.c_str(),&numAnimals);
         int numberToCull;
         if (agroup->GetnumberInstances()<agroup->GetnumberOfCohorts())  //if too many animals then remove excess immediately
         {
         	numberToCull=agroup->GetnumberInstances()-agroup->GetnumberOfCohorts();
         	agroup->CullAnimals(numberToCull);
         }
//         agroup->SetNumber(numAnimals);
      }

   }
   UnsetCritical();
   CloseInputFile();
}


/****************************************************************************\
	DailyUpdate is the daily execution of cattle
\****************************************************************************/
void dairyStock::DailyUpdate(void)
{
   for(int i=0;i<grouplist->NumOfNodes();i++)
   {
      cattleGroup * agroup = grouplist->ElementAtNumber(i);
      agroup->DailyUpdate();
   }
   CheckBirth();
   GiveIndicator(environmentalIndicator);
}

/****************************************************************************\
	Initializes a new dairyStock
   NJH Mar 2000
\****************************************************************************/
void dairyStock::InitCattle(fstream * file, int anAnimalModel, int anAggregation, char *filenameCattle)
{
   aggregation=anAggregation;
   if (filenameCattle)
	 	CattlefileStream = new fstream(filenameCattle,ios::out);
	Setfile(file);
   Setcur_pos(0);
   SetCritical();
	if (FindSection(GetName()))
	{
      string namestr;
      // get info on groups
      Setcur_pos(0);
      namestr=(string) GetName()+"."+"Group";
      int first,num;
      GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of sections associated with this instance
	   UnsetCritical();
      double lastAge = 0.0;
      for(int inx=first;inx<(first+num);inx++)
      {
         cattleGroup * acattlegroup = new cattleGroup((char *)namestr.c_str(),inx,this);
         acattlegroup->SetanimalModel(anAnimalModel);
         acattlegroup->ReadParameters(file);
         lastAge = acattlegroup->GetmaxAge();
			grouplist->InsertLast(acattlegroup);
      }
   }
   wasFemale = false;
 	Setfile(NULL);
}

/****************************************************************************\

\****************************************************************************/

void dairyStock::checkBalances(bool show)
{
   for(int i=0;i<grouplist->NumOfNodes();i++)
   {
   	grouplist->ElementAtNumber(i)->checkBalances(show);
   }
}


/****************************************************************************\
 Check to see if any calves are to be born
\****************************************************************************/
void dairyStock::CheckBirth()
{
   for(int i=0;i<grouplist->NumOfNodes();i++)
   {
   	grouplist->ElementAtNumber(i)->CheckBirth();
   }
};

/****************************************************************************\
 Check to see if animals have out-aged their age class and move them if necessary
 Cast animals (those moving out of the oldest age class) are left in oldies linkList
\****************************************************************************/
void dairyStock::CheckAge()
{
   for(int i=0;i<grouplist->NumOfNodes();i++)
   {
      cattleGroup * agroup = grouplist->ElementAtNumber(i);
		if (agroup->GetnumberOfAnimals()>0)
     		agroup->CheckAge(oldies);

//this code enables animals to migrate upwards in age categories. Not implemented yet
      //pass information over to the next age category
/*      if (i<grouplist->NumOfNodes()-1) // hop over if this is the oldest age category
      {
      	cattleGroup * nextgroup = grouplist->ElementAtNumber(i+1);
         if (nextgroup->GetnumberPerCohort()>0)
         {
            for(int j=0;j<oldies->NumOfNodes();j++)
            {
         if (i==grouplist->NumOfNodes()-2)
         cout <<""  ;
               cattle * anAnimal = (cattle *) oldies->ElementAtNumber(j);
               //IMPORTANT - increment animal's Index to ensure they get the correct diet
               anAnimal->IncrementIndex();
               anAnimal->SetAlias(nextgroup->GetAlias());
               anAnimal->SetStableSection(nextgroup->GetanimalSection());
               linkList <cattle>::PS Head;
               linkList <cattle>::PS aNode;
               nextgroup->GetGroup()->PeekHead(Head);
               nextgroup->GetGroup()->InsertBefore(Head,aNode,anAnimal);
               if (theOutputControl->GetCowDetails())
               {
                  char outstring[200];
                  strcpy(outstring,agroup->GetAlias().c_str());
                  strcat(outstring," - animal promoted in dairystock");
                  theMessage->LogEventWithTime(outstring);
               }
            }
         }
      }
      else
      {
         for(int j=0;j<agroup->GetGroup()->NumOfNodes();j++)
   		{
         	if (j>=agroup->GetnumberOfCohorts())
            {
		   	   cattle * tempcattle = agroup->GetGroup()->ElementAtNumber(j);
            	oldies->InsertLast(tempcattle);
			   }
         }
      } */

      if (oldies->NumOfNodes()>0)
            Cull(oldies);
      oldies->ForgetAll();
   }
}

/****************************************************************************\
\****************************************************************************/
void dairyStock::SeedGroups()
{
	double startAge = 0.0;
   double endAge = 0.0;
   for(int i=0;i<grouplist->NumOfNodes();i++)
   {
      startAge = grouplist->ElementAtNumber(i)->GetminAge();
      endAge = grouplist->ElementAtNumber(i)->GetmaxAge();
   	if (grouplist->ElementAtNumber(i)->GetnumberOfCohorts()>0)
	      grouplist->ElementAtNumber(i)->SeedGroup(startAge,endAge, aggregation, &IDnumber);
   }

};
/****************************************************************************\
Sell cast animals, delete instances and record sale
\****************************************************************************/
void dairyStock::Cull(linkList<cattle> * victims)
{
	linkList <cattle>::PS aNode;
   victims->PeekHead(aNode);
   while (aNode)
   {
      cattle * tempcow = (cattle *) aNode->element;
      char outstring[200];
      strcpy(outstring,aNode->element->GetAlias().c_str());
      if (theOutputControl->GetHerdDetails())
      {
         if (!tempcow->GetisYoung())
            strcat(outstring," - animal sold by dairystock");
         else
            strcat(outstring," - calf sold by dairystock");
         theMessage->LogEventWithTime(outstring);
      }
      animalProduct *anAnimalProduct = new animalProduct(*(animalProduct*) theProducts->GetProductElement("DAIRY"));
      tempcow->loadAnimalProduct(anAnimalProduct);
      theProducts->AddProduct(anAnimalProduct);
      delete anAnimalProduct;
      if (tempcow->GetCalf())
      {
         linkList<cattle> * tempList = new linkList<cattle>;
         tempList->InsertLast(tempcow->GetCalf());
         Cull(tempList);
         tempcow->SetCalf(NULL);
         delete tempList;
      }
      victims->Remove(aNode);
      victims->PeekHead(aNode);
   }
   victims->ForgetAll();
}

/****************************************************************************\
Return total number of animals
\****************************************************************************/
double dairyStock::GetTotalAnimals()
{
	double ret_val = 0.0;
   for(int i=0;i<grouplist->NumOfNodes();i++)
   {
     	cattleGroup * aGroup = grouplist->ElementAtNumber(i);
   	ret_val+=aGroup->GetnumberOfAnimals();
   }
   return ret_val;
};

/****************************************************************************\
Recruit new animals if necessary - currently only works for calves
\****************************************************************************/
void dairyStock::AutoRecruit()
{
   for(int i=0;i<grouplist->NumOfNodes();i++)
   {
   	if (i==0) grouplist->ElementAtNumber(0)->AutoRecruit(pointyCalves,&IDnumber);//,CattlefileStream);
   	else grouplist->ElementAtNumber(i)->AutoRecruit(NULL,&IDnumber);//,CattlefileStream);
   }
   if (pointyCalves->NumOfNodes()>0)
   	{Cull(pointyCalves);}
};

/****************************************************************************\
\****************************************************************************/
void dairyStock::GiveIndicator(int indicatorType)
{
   if (indicatorType == environmentalIndicator)
   {
      double val=0.0;
      val = GetHerdN();
      theOutput->AddStateIndicator(environmentalIndicator,"33.60 N bound in cattle","kg N",val);
      val = GetHerdC();
      theOutput->AddStateIndicator(environmentalIndicator,"43.60 C bound in cattle","kg C",val);
      for(int i=0;i<grouplist->NumOfNodes();i++)
      {
         grouplist->ElementAtNumber(i)->GiveIndicator(indicatorType);
      }
   }
   if (indicatorType == economicIndicator)
   {
      for(int i=0;i<grouplist->NumOfNodes();i++)
      {
         grouplist->ElementAtNumber(i)->GiveIndicator(indicatorType);
      }
   //include these groups?
/*   for(int i=0;i<pointyCalves->NumOfNodes();i++)
   {
		val+=pointyCalves->ElementAtNumber(i)->GetDE();
   }
   for(int i=0;i<oldies->NumOfNodes();i++)
   {
		val+=oldies->ElementAtNumber(i)->GetDE();
   }*/
   }
};

/****************************************************************************\
 Check to see if any calves are to be weaned and put them into pointyCalves
\****************************************************************************/
void dairyStock::CheckWean(bool stopLactation)
{
   for(int i=0;i<grouplist->NumOfNodes();i++)
   {
   	grouplist->ElementAtNumber(i)->CheckWean(pointyCalves, stopLactation);
   }
};


double dairyStock::GetHerdN()
{
	double val=0.0;
   for(int i=0;i<grouplist->NumOfNodes();i++)
   {
		val+=grouplist->ElementAtNumber(i)->GetCattleGroupN();
   }
   for(int i=0;i<pointyCalves->NumOfNodes();i++)
   {
		val+=pointyCalves->ElementAtNumber(i)->GetCattleN();
   }
   for(int i=0;i<oldies->NumOfNodes();i++)
   {
		val+=oldies->ElementAtNumber(i)->GetCattleN();
   }
   return val;
}

double dairyStock::GetHerdC()
{
	double val=0.0;
   for(int i=0;i<grouplist->NumOfNodes();i++)
   {
		val+=grouplist->ElementAtNumber(i)->GetCattleGroupC();
   }
   for(int i=0;i<pointyCalves->NumOfNodes();i++)
   {
		val+=pointyCalves->ElementAtNumber(i)->GetCattleC();
   }
   for(int i=0;i<oldies->NumOfNodes();i++)
   {
		val+=oldies->ElementAtNumber(i)->GetCattleC();
   }
   return val;
}

double dairyStock::GetDE()
{
	double val=0.0;
   for(int i=0;i<grouplist->NumOfNodes();i++)
   {
		val+=grouplist->ElementAtNumber(i)->GetDE();
   }
   for(int i=0;i<pointyCalves->NumOfNodes();i++)
   {
		val+=pointyCalves->ElementAtNumber(i)->GetDE();
   }
   for(int i=0;i<oldies->NumOfNodes();i++)
   {
		val+=oldies->ElementAtNumber(i)->GetDE();
   }
   return val;
};

/*void dairyStock::FlushIndicator(int indicatorType)
{
   for(int i=0;i<grouplist->NumOfNodes();i++)
   {
		grouplist->ElementAtNumber(i)->FlushIndicator(indicatorType);
   }
} */

void dairyStock::theSorting(bool canCull)
{
//   GiveIndicator(environmentalIndicator);
	if (canCull)
   {
      CheckAge();
      Cull(oldies);
   }
   AutoRecruit();
}

