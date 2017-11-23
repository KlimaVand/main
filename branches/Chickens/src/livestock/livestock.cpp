/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "livestock.h"
#include "swineStock.h"
#include "dairyStock.h"
#include "../base/IndicatorOutput.h"

/**
 * Default Constructor
 */

livestock::livestock()
   : base()
{
	theStockList = new linkList <animalStock>;
   strcpy(Name,"livestock");
}

/**
  *  Destructor
 */
livestock::~livestock()
{
	delete theStockList;
}

/**
 * Initialize a live stock
 *  @param filename is the filename where is variable is stored
 *  @param pigfarm symbolize if SwineStock section should be read or not
 *  @param dairyfarm symbolize if DairyStock section should be read or not
 *  @param beeffarm symbolize if BeefStock section should be read or not
 *  @param beeffarm symbolize if BeefStock section should be read or not
 *  @param theBuildings is where the SwineStock is stored
 *  @param filenameCattle filename where there is extra cattle information
 */
void livestock::Initialize(string filename, bool pigfarm, bool dairyfarm, bool beeffarm, buildings * theBuildings, char *filenameCattle)
{
   int animalModel;
   SetCritical();
   OpenInputFileString(filename);
   Setcur_pos(0);
   GetParameter("AnimalModel",&animalModel);
   if (pigfarm)
   {

     char * InstanceName=new char[100];
     int first,num;
      UnsetCritical();
      strcpy(InstanceName,"SwineStock");
      GetSectionNumbers(InstanceName,&first,&num);
      Setcur_pos(0);
      for(int inx=first;inx<(first+num);inx++)
      {
         SetCritical();
         FindSection(InstanceName,inx);
         swineStock * aSwineStock = new swineStock("SwineStock",0,this);
         aSwineStock->InitSwines(file,theBuildings);
         theStockList->InsertLast((animalStock*) aSwineStock);
      }
      UnsetCritical();



/*
      if (FindSection("SwineStock"))
      {
            swineStock * aSwineStock = new swineStock("SwineStock",0,this);
            aSwineStock->InitSwines(file,theBuildings);
            theStockList->InsertLast((animalStock*) aSwineStock);
      }*/
   }
   if (dairyfarm)
   {
      if (FindSection("DairyStock"))
      {
         dairyStock* aDairyStock = new dairyStock("DairyStock",0,this);
         aDairyStock->InitCattle(file,animalModel,filenameCattle);
         theStockList->InsertLast((animalStock*) aDairyStock);
      }
      else theMessage->FatalError("livestock::Initialize - section DairyStock not found");
   }
   if (beeffarm)
   {
      if (FindSection("BeefStock"))
      {
         dairyStock* aDairyStock = new dairyStock("BeefStock",0,this);
         aDairyStock->InitCattle(file,animalModel, filenameCattle);
         theStockList->InsertLast((animalStock*) aDairyStock);
      }
   }
   UnsetCritical(); // Adjust placing of Set/UnsetCritical() !!!
  	CloseInputFile();
}

/**
 * update each animal stock in theStockList
 */

void livestock::ReceivePlan(char* fileExtension)
{
   for (int i=0;i<theStockList->NumOfNodes();i++)
		theStockList->ElementAtNumber(i)->ReceivePlan(fileExtension);
}

/**
 */
double livestock::GetDE()
{
	double animalUnits = 0.0;
   for (int i=0;i<theStockList->NumOfNodes();i++)
   	animalUnits +=theStockList->ElementAtNumber(i)->GetDE();
   return animalUnits;
}

/**
 */
void livestock::DailyUpdate()
{
   theStockList->ForEach(UpdateAnimals);
}

/**
 */
void livestock::GiveIndicator(int indicatorType)
{
   for (int i=0;i<theStockList->NumOfNodes();i++)
   {
	 (theStockList->ElementAtNumber(i))->GiveIndicator(indicatorType);
	}
}

void livestock::FlushIndicator(indicatorTypes indicatorType)
{
   for (int i=0;i<theStockList->NumOfNodes();i++)
   {
	 (theStockList->ElementAtNumber(i))->FlushIndicator(indicatorType);
	}
}

