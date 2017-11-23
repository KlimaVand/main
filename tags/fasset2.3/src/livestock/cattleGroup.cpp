/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/***************************************************************************
Constructed by Nick Hutchings
Used to manage groups of cattle (includes management info + list of animals)
***************************************************************************/
#include "../base/common.h"
#include "cattleGroup.h"
#include "../base/nixcbits.h"

#include "../data/outputData.h"

/**
 *  Constructor with arguments. Also call base(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
 */
cattleGroup::cattleGroup(const char * aName, const int aIndex, const base * aOwner)
			:base(aName,aIndex,aOwner)
{
//   group= new linkList<cattle>;
   group= new linkList<cattle>;
   feedPlanList = new cloneList <diet>;
	theCurrentDiet = NULL;  //holds current menu - needed when buying cattle
   areYoung = false;

//	Alias;
	numberOfCohorts=0;
	numberPerCohort=0;
   maxAge=0;	// age at which animals move to the next group (in years)
   minAge=0;	// age at which animals move into the group (in years)
   code=0;
   LivestockUnits=0.0;
   areFemale=false;
   areYoung=false;		//true if animals are young (directs info flow in to indicators)
	StableSection=NULL;    //holds pointer to relevant animal section in housing
   animalModel = 0;
}
linkList<cattle> *cattleGroup::GetGroupClone()
{
	linkList<cattle> *newlist;
   newlist=new linkList<cattle>;
   for (int i=0;i<group->NumOfNodes();i++)
   {
	   cattle *p = new cattle(*(group->ElementAtNumber(i)));
      newlist->InsertLast(p);
   }
	return newlist;
}

cloneList <diet> *cattleGroup::GetFeedPlanListClone()
{
	cloneList <diet> *newlist;
   newlist=new cloneList <diet>;
   for (int i=0;i<feedPlanList->NumOfNodes();i++)
   {
	   diet *p = new diet(*(feedPlanList->ElementAtNumber(i)));
      newlist->InsertLast(p);
   }
	return newlist;
}
cattleGroup::cattleGroup(const cattleGroup& a)
{
	group= GetGroupClone();
	   feedPlanList = GetFeedPlanListClone();
		theCurrentDiet = new diet(*a.theCurrentDiet);  //holds current menu - needed when buying cattle
		StableSection=new animalSection(*a.StableSection);
		areYoung = a.areYoung;

	//	Alias;
		numberOfCohorts=a.numberOfCohorts;
		numberPerCohort=a.numberPerCohort;
	   maxAge=a.maxAge;	// age at which animals move to the next group (in years)
	   minAge=a.minAge;	// age at which animals move into the group (in years)
	   code=a.code;
	   LivestockUnits=a.LivestockUnits;
	   areFemale=a.areFemale;
	   areYoung=a.areYoung;		//true if animals are young (directs info flow in to indicators)
		   //holds pointer to relevant animal section in housing
	   animalModel = a.animalModel;
}
/**
 *  Destructor
 */
cattleGroup::~cattleGroup()
{
   group->Reset();
   delete feedPlanList;
   delete group;
}


/**
 *	DailyUpdate is called daily
 */
void cattleGroup::DailyUpdate(void)
{
   for(int i=0;i<group->NumOfNodes();i++)
   {
      cattle * tempcattle = group->ElementAtNumber(i);
      tempcattle->DailyUpdate();
      if (tempcattle->GetCalf())
			tempcattle->GetCalf()->DailyUpdate();
//      tempcattle->DoDailyIndicators();
		tempcattle->checkBalances(false);
   }
}


/**
 * Check N budget for the animals
 */

void cattleGroup::checkBalances(bool show)
{
   for(int i=0;i<group->NumOfNodes();i++)
   {
      cattle * tempcattle = group->ElementAtNumber(i);
		tempcattle->checkBalances(show);
   }
}


/**
 * Read parameters for group
 */
void cattleGroup::ReadParameters(fstream *file)
{
	Setfile(file);
   SetCritical();
   FindSection(GetName(),Index);
 	GetParameter("Alias",&Alias);
 	GetParameter("Code",&code);
	GetParameter("LivestockUnits",&LivestockUnits);
//   if (animalModel>1)
   {
      GetParameter("minAge",&minAge);
      GetParameter("maxAge",&maxAge);
   }
   int dum;
   GetParameter("Young",&dum);
   if (dum==1) areYoung = true;
   else areYoung = false;
   GetParameter("Sex",&dum);
   if (dum==1) areFemale = true;
   else areFemale = false;
   Setfile(NULL);
}

/**
 * Check to see if any calves are to be born
 */

void cattleGroup::CheckBirth()
{
   for(int i=0;i<group->NumOfNodes();i++)
   {
	   cattle * tempcattle = group->ElementAtNumber(i);
      if (tempcattle->GetisFemale())
      {
         tempcattle->CheckBirth();
      }
   }
}

/**
 * See if any animals have out-aged their group and if so, extract them
 */
void cattleGroup::CheckAge(linkList<cattle> * oldies)
{
   linkList <cattle>::PS aNode;
   group->PeekHead(aNode);
   while (aNode)
   {
//   	if (((aNode->element->Getage()/theTime.daysInYear(0))>maxAge)&&(!aNode->element->isLactating()))
   	if ((aNode->element->Getage()/theTime.daysInYear(0)>maxAge))
      {
      	oldies->InsertLast(aNode->element);//animals in oldies are in order oldest .... youngest
	      group->Release(aNode);
         group->PeekTail(aNode);
      }
      else {group->OneStep(aNode);}
   }
};

/**
 *  Adds an Cattle to the group
 */
void cattleGroup::AddAnimal(cattle * anAnimal)
{
	group->InsertLast(anAnimal);
};

/**
 * Creates an number of animals spaced at even age increments
 */
void cattleGroup::SeedGroup(double startAge, double endAge, int *IDnumber)//, fstream * CattlefileStream)
{
// open database file of cattle parameters
   char fileName[15];
   strcpy(fileName,"animals.dat");
   if(!OpenInputFile(fileName))
      theMessage->FatalError("cattleGroup::cattle database file not found");
   else
   {
    if(numberOfCohorts>0)
      {
         double increment = (endAge-startAge)* theTime.daysInYear(0)/numberOfCohorts;
         double currentAge = ((int) (startAge * theTime.daysInYear(0)))+1;
         cattle* tempcow;
         for(int i=0;i<numberOfCohorts;i++)
         {
				tempcow = buyNewAnimal(currentAge,IDnumber,false);

            group->InsertLast(tempcow);
            currentAge+=increment;
         }
      }
    CloseInputFile();
   }
};

/**
 * Culls animals (oldest go first)
 */
void cattleGroup::CullAnimals(int numberToCull)
{
 if (group->NumOfNodes()<numberToCull)
      theMessage->FatalError("cattlegroup: more animals to cull than are currently present");
 int i=numberToCull;
// linkList <cattle>::PS aNode;
 linkList <cattle>::PS aNode;
 while (i>0)
 {
 	group->PeekTail(aNode);
   cattle * tempcow = (cattle *) aNode->element;
	CullAnimalFromGroup(tempcow);
   i--;
 }
}


/**
 * Culls specific a animal. If animal is in this group then returns true, otherwise false
 */
bool cattleGroup::CullAnimalFromGroup(cattle *theVictim)
{
	linkList <cattle>::PS aNode;
   group->PeekHead(aNode);
   int i = group->NumOfNodes();
   while ((aNode)&&(i>0))
   {
   	if ((cattle *) aNode->element == theVictim)
      {
			CullAnimal(theVictim,false);
		   group->Remove(aNode);
         return true;
      }
      i--;
   }
	return false;
}

/**
 * Recruits animals to group if numbers are lacking. If recruitment list is not NULL
 * then it will attempt to recruit from this list, otherwise it will buy animals
 */
void cattleGroup::AutoRecruit(linkList<cattle> * recruitmentList, int *IDnumber)//, fstream *CattlefileStream)
{
	//see if need to recruit animals
   //int numToGet=GetnumberInstances(); njh removed mar 07
   //njh added mar 07
  	int numToGet=0;
   if (GetnumberInstances() < numberOfCohorts)
   	numToGet=numberOfCohorts-GetnumberInstances();

      //end of add
   if (numToGet>0)
   {
   // open database file of cattle parameters
      char fileName[120];
      strcpy(fileName,"animals.dat");
      if(!OpenInputFile(fileName))
         theMessage->FatalError("dairyStock::cattle database file not found");
   }
	while (numToGet>0)
   {
   	if (recruitmentList)  //only normally present for first age group (contains calves)
	   {
		   for(int i=0;i<recruitmentList->NumOfNodes();i++)
      	{
				linkList <cattle>::PS aNode;
            recruitmentList->PeekHead(aNode);
				if ((aNode->element->Getage()>GetminAge())&&(aNode->element->Getage()<=GetmaxAge()))
            {
               if (((GetareFemale())&& (aNode->element->GetisFemale()))||       //check if sex of candidate is ok for group
                   ((!GetareFemale())&& (!aNode->element->GetisFemale())))
                {
                  aNode->element->SetisYoung(areYoung);
                  *IDnumber=*IDnumber + 1;
                  aNode->element->SetIndex(*IDnumber);
                  if (StableSection) aNode->element->SetStableSection(StableSection);
                  else {theMessage->FatalError("cattle::No current animal section for recruited animals");}
                  AddAnimal(aNode->element);  //add animal to group
                  char outstring[200];
		            strcpy(outstring,GetAlias().c_str());
                  strcat(outstring," - animal recruited by cattle group");
                  theMessage->LogEventWithTime(outstring);
                  recruitmentList->Release(aNode);   //release node from recruitment list
                  numToGet--;
                }
            }
         }
      }
	   if (numToGet>0)  //cannot recruit enough - buy some
	   {

         double currentAge = minAge*365 + 1;
			cattle * tempcow = buyNewAnimal(currentAge,IDnumber,true);//, CattlefileStream);
         if (StableSection)
         {
         	tempcow->SetStableSection(StableSection);
            if (tempcow->GetCalf())
            	tempcow->GetCalf()->SetStableSection(StableSection);
         }
         else {theMessage->FatalError("cattlegroup::No current animal section for bought animals");}
		   group->InsertLast(tempcow);
         numToGet--;
  		}
   }

   if (file) CloseInputFile();
	if (numberOfCohorts<GetnumberInstances()) //Too many animals - time for the oldest to move on to pastures new......
   {
		int theNoOfVictims = GetnumberInstances()-numberOfCohorts;
      CullAnimals(theNoOfVictims);
   }
};


/**
 * Returns the total N contained in the group
 */
double cattleGroup::GetCattleGroupN()
{
	double ret_val=0.0;
   for(int i=0;i<group->NumOfNodes();i++)
   {
	   cattle * tempcattle = group->ElementAtNumber(i);
      ret_val+=tempcattle->GetCattleN();
   }
	return ret_val;
}

/**
 * Returns the total C contained in the group
 */
double cattleGroup::GetCattleGroupC()
{
	double ret_val=0.0;
   for(int i=0;i<group->NumOfNodes();i++)
   {
	   cattle * tempcattle = group->ElementAtNumber(i);
      ret_val+=tempcattle->GetCattleC();
   }
	return ret_val;
}



/**
 * Checks to see if any calves should be weaned and if so, punts them into pointycalves
 * Note: Up to another function (currently in grazing manager) to set suckling to false
 * and therefore prompt weaning.
 */
void cattleGroup::CheckWean(linkList<cattle> * calves, bool stopLactation)
{
   for(int i=0;i<group->NumOfNodes();i++)
   {
	   cattle * tempcattle = group->ElementAtNumber(i);
      if (tempcattle->GetisFemale())
      {
         cattle* aCalf = tempcattle->GetCalf();
         if (aCalf)
         {
         	if (!aCalf->GetisSuckling())
	         {
            	tempcattle->WeanCalf();
            	aCalf->Setmother(NULL);
               aCalf->SetfeedPlanList(NULL);  //calf uses same feedplanlist as mother, so set to NULL to avoid double delete
               CullAnimal(aCalf,true);
               if (stopLactation)
               	tempcattle->Setlactating(false);
            }
         }
      }
   }
}
/**
 * return the sum of all Cattle
 */
double cattleGroup::GetnumberOfAnimals()
{
	double numberOfAnimals = 0.0;
   for(int i=0;i<group->NumOfNodes();i++)
   {
	   cattle * tempcattle = group->ElementAtNumber(i);
      numberOfAnimals+=tempcattle->GetNumber();
   }

	return numberOfAnimals;
};

/**
 * return the sum of all Cattle
 */
double cattleGroup::GetDE()
{
	double aVal = 0.0;
   for(int i=0;i<group->NumOfNodes();i++)
   {
	   cattle * tempcattle = group->ElementAtNumber(i);
      aVal+=tempcattle->GetDE();
   }

	return aVal;
};
/**
 * if input is economicIndicator the indicat.xls will have values and number from the group
 */
void cattleGroup::GiveIndicator(int indicatorType)
{
 double Number=0.0;
   for(int i=0;i<group->NumOfNodes();i++)
   {
	   cattle * tempcattle = group->ElementAtNumber(i);
/*		if (indicatorType==environmentalIndicator)
	      tempcattle->GiveIndicator(indicatorType);  */
      Number+= tempcattle->GetNumber();
   }
 if (indicatorType==economicIndicator)
 {
   animalProduct *anAnimalProduct = new animalProduct(*(animalProduct*) theProducts->GetProductElement(Alias.c_str()));
   double value = anAnimalProduct->GetaPrice()->GetactualSellPrice() * Number;
   delete anAnimalProduct;
   if (areYoung)
   {
		theOutput->AddStateIndicator(economicIndicator,"03.50 Value young cattle","Dkr",value);
		theOutput->AddStateIndicator(economicIndicator,"11.50 Young cattle present","no",Number);
   }
   else
   {
		theOutput->AddStateIndicator(economicIndicator,"03.51 Value mature cattle","Dkr",value);
		theOutput->AddStateIndicator(economicIndicator,"11.51 Mature cattle present","no",Number);
   }
 }
};



/*void cattleGroup::FlushIndicator(int indicatorType)
{
 double Number=0.0;
   for(int i=0;i<group->NumOfNodes();i++)
   {
	   cattle * tempcattle = group->ElementAtNumber(i);
		if (indicatorType==environmentalIndicator)
	      tempcattle->FlushIndicator(indicatorType);
      Number+= tempcattle->GetNumber();
   }
}
*/
/**
 * Create a new Cattle
 */
cattle * cattleGroup::buyNewAnimal(double currentAge, int *IDnumber, bool logit)//, fstream *CattlefileStream)
{
	cattle * tempcow=new dk_cattle(GetAlias().c_str(),*IDnumber,this);
   *IDnumber=*IDnumber+1;
/*   if (animalModel==1)
      tempcow= new uk_cattle(GetName(),*IDnumber,this);   */
   if (animalModel!=2)
      theMessage->FatalError("current animal Model is not supported at this point");
//      tempcow= new dk_cattle(GetName(),*IDnumber,this);
   if (areFemale)
      tempcow->SetisFemale(true);
   else
      tempcow->SetisFemale(false);
   tempcow->ReadParameters(file);
   //these two factors will be later reset by operations manager but we need to make an initial guess
//if (tempcow->GetIndex()==29)
  //	cout <<"";
   if (tempcow->GetisFemale())
   {
     if (currentAge<2*365)  //1st year heifers + 2nd year heifers
     {
         tempcow->Setlactating(false);
         tempcow->SetlactationNumber(0);
        if (currentAge<=(2*365-tempcow->GetgestationPeriod()))  //1st year heifers + non-pregnant 2nd year heifers
        {
               tempcow->Setpregnant(false);
               tempcow->SetdaysFromConception(0.0);
               tempcow->SetdaysFromParturition(0.0);
        }
        else                                        //2nd year heifers, pregnant
        {
               tempcow->Setpregnant(true);
               // take one day off, otherwise there is a risk that time since conception = gestation period
               tempcow->SetdaysFromConception(currentAge-((2*365.0)-tempcow->GetgestationPeriod()));
               if (tempcow->GetdaysFromConception()==tempcow->GetgestationPeriod())
               	tempcow->SetdaysFromConception(tempcow->GetgestationPeriod()-1); //avoid immediate birth - causes problems
               tempcow->SetdaysFromParturition(0.0);
               tempcow->SetlactationNumber(0);
        }
     }
     else if ((GetAlias()=="DAIRY") || (GetAlias()=="SUCKLER"))    //only FBEEF3 fall out from the if tree
     {
        int lactationNumber = int((currentAge/365.0) - 1);
        tempcow->SetlactationNumber(lactationNumber);
        tempcow->SetCalf(NULL);

        double partOfYear = (int)(currentAge)%365;
        tempcow->SetdaysFromParturition(partOfYear);
		  double conceptionTime=0; //days between parturition and conception
        double lengthOfLactation=0;
        if (GetAlias()=="DAIRY")
        {
        	conceptionTime = 49;
         lengthOfLactation = 315;
        }
        else if (GetAlias()=="SUCKLER")
        {
         conceptionTime = 98;
         lengthOfLactation = 225;
        }
        else
        	theMessage->FatalError("cattleGroup::buyNewAnimal - ");

        if (partOfYear<=conceptionTime)                        //assume conception after 7 weeks
         {
            tempcow->Setpregnant(false);
            tempcow->Setlactating(true);
            tempcow->SetdaysFromConception(0.0);
            if (GetAlias()=="SUCKLER")
            {
               if (animalModel==2)
               {
                     cattle * tempcalf= new dk_cattle("CALF",0,this);
                     tempcalf->SetisFemale(true);
                     tempcow->SetCalf(tempcalf);
                     tempcalf->Setcode(2);
						   tempcalf->ReadParameters(file);
                     tempcalf->Initialise(code, int(partOfYear), LivestockUnits, numberPerCohort, true);//, CattlefileStream);
                     tempcalf->SetAlias("CALF");
               }
            }
         }
         else if (partOfYear>=lengthOfLactation)      //assume dry
         {
            tempcow->Setpregnant(false);
            tempcow->Setlactating(false);
            tempcow->SetdaysFromConception(partOfYear-conceptionTime);
         }
            else
            {
               tempcow->Setpregnant(true);
               tempcow->Setlactating(true);
               tempcow->SetdaysFromConception(partOfYear-conceptionTime);
               if (GetAlias()=="SUCKLER")
               {
                  if (animalModel==2)
                  {
                     cattle * tempcalf= new dk_cattle("CALF",0,this);
                     tempcalf->SetisFemale(true);
                     tempcow->SetCalf(tempcalf);
                     tempcalf->Setcode(2);
						   tempcalf->ReadParameters(file);
                     tempcalf->Initialise(code, int(partOfYear), LivestockUnits, numberPerCohort, true);//, CattlefileStream);
                     tempcalf->SetAlias("CALF");
                  }
               }
            }
     }
   }
   tempcow->SetAlias(Alias);
   tempcow->Initialise(code, int(currentAge), LivestockUnits, numberPerCohort, areYoung);//, CattlefileStream);
   if (tempcow->isPregnant() && !tempcow->GetFoetus())
   	theMessage->FatalError("cattleGroup::buyNewAnimal - logic error as there is a pregnant animal without a foetus");
   if (logit)
   {
      animalProduct *anAnimalProduct = new animalProduct(*(animalProduct*) theProducts->GetProductElement(Alias.c_str()));
      tempcow->loadAnimalProduct(anAnimalProduct);
      theProducts->SubtractProduct(anAnimalProduct);
      delete anAnimalProduct;
      if (theOutputData->DetailsData.getHerdDetails()==true)
      {
         char outstring[200];
         strcpy(outstring,GetAlias().c_str());
         strcat(outstring," - animal bought by cattle group");
         theMessage->LogEventWithTime(outstring);
      }
   }
   return tempcow;
}



/**
 * Cull an animal. Delete animal if deleteAnimal is true, otherwise it will be done elsewhere
 */
void cattleGroup::CullAnimal(cattle *theVictim, bool deleteAnimal)
{
   theVictim->checkBalances(false);
   animalProduct *anAnimalProduct;
   if (theOutputData->DetailsData.getHerdDetails()==true)
   {
      char outstring[200];
      strcpy(outstring,GetAlias().c_str());
      strcat(outstring," - calf sold by cattle group");
      theMessage->LogEventWithTime(outstring);
   }
   anAnimalProduct = new animalProduct(*(animalProduct*) theProducts->GetProductElement(theVictim->GetAlias().c_str()));
   theVictim->loadAnimalProduct(anAnimalProduct);
//   anAnimalProduct->Setamount(anAnimalProduct->GetAmount()*anAnimalProduct->GetEndWeight());  //convert from number to mass in kg
   theProducts->AddProduct(anAnimalProduct);
   delete anAnimalProduct;
   if (deleteAnimal)
	   delete theVictim;
}



