/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include <common.h>
#include <livestoc.h>
#include <swinestk.h>
#include <dairystk.h>
#include <indicat.h>

/****************************************************************************\
  Default Constructor
\****************************************************************************/
livestock::livestock()
   : base()
{
	theStockList = new linkList <animalStock>;
   strcpy(Name,"livestock");
}

/****************************************************************************\
  Destructor
\****************************************************************************/
livestock::~livestock()
{
	delete theStockList;
}

/****************************************************************************\
\****************************************************************************/
void livestock::Initialize(char* filename, bool pigfarm, bool dairyfarm, bool beeffarm, buildings * theBuildings, char *filenameCattle)
{
   int animalModel,aggregation;
   SetCritical();
   OpenInputFile(filename);
   Setcur_pos(0);
   GetParameter("AnimalModel",&animalModel);
   GetParameter("Aggregation",&aggregation);
   if (pigfarm)
   {

     char * InstanceName="                 ";
     int first,num;
      UnsetCritical();
      InstanceName="SwineStock";
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
         aDairyStock->InitCattle(file,animalModel,aggregation, filenameCattle);
         theStockList->InsertLast((animalStock*) aDairyStock);
      }
   }
   if (beeffarm)
   {
      if (FindSection("BeefStock"))
      {
         dairyStock* aDairyStock = new dairyStock("BeefStock",0,this);
         aDairyStock->InitCattle(file,animalModel,aggregation, filenameCattle);
         theStockList->InsertLast((animalStock*) aDairyStock);
      }
   }
   UnsetCritical(); // Adjust placing of Set/UnsetCritical() !!!
  	CloseInputFile();
}

/****************************************************************************\
\****************************************************************************/
void livestock::ReceivePlan(char* fileExtension)
{
   for (int i=0;i<theStockList->NumOfNodes();i++)
		theStockList->ElementAtNumber(i)->ReceivePlan(fileExtension);
}

/****************************************************************************\
\****************************************************************************/
double livestock::GetDE()
{
	double animalUnits = 0.0;
   for (int i=0;i<theStockList->NumOfNodes();i++)
   	animalUnits +=theStockList->ElementAtNumber(i)->GetDE();
   return animalUnits;
}

/****************************************************************************\
 Function:   CalcLP
\****************************************************************************/
void livestock::CalcLP(int period)
{
   writeLivestockPigs("LPIGS.INC");
	writeNotProduceableItems("CONC.INC");
	writeFeedComponentsList("FECOMP.INC");
   writeVariableCosts("VCL.INC");
   writeReplacementRate("RPRSOWS.INC");

   if (theStockList->NumOfNodes()==0)
   {
		writeEmptyFile("CROPCONT.INC");
	   writeEmptyFile("CONCCONT.INC");
	   writeEmptyFile("CROPPER.INC");
	   writeEmptyFile("CONCPER.INC");
	   writeEmptyFile("FECONEED.INC");
	   writeEmptyFile("FECOMAX.INC");
	   writeEmptyFile("YL.INC");
	   writeEmptyFile("MANUPROD.INC");
	   writeEmptyFile("MANUNPK.INC");
	   writeEmptyFile("LxCONC.INC");
	   writeEmptyFile("PCONC.INC");
	   writeEmptyFile("PIGLAB.INC");
   }
   else
	   if (period>1)
	   	theStockList->ForEach(CalcLP3);
	   else
	      theStockList->ForEach(CalcLP1);
}

/****************************************************************************\
\****************************************************************************/
void livestock::DailyUpdate()
{
   theStockList->ForEach(UpdateAnimals);
}

/****************************************************************************\
	writeFeedComponentsList write the list of feed components
\****************************************************************************/
void livestock::writeFeedComponentsList(char* filename)
{
	fstream fileStream;
	fileStream.open(filename,ios::out);
	fileStream << "FE \nLYSIN \nMETHI \nTREON \nPROT \nFPROT \nPHOS \n";
   fileStream.close();
}

/****************************************************************************\
	writeLivestockPigs write the list of feed components
\****************************************************************************/
void livestock::writeLivestockPigs(char* filename)
{
	fstream fileStream;
	fileStream.open(filename,ios::out);
	fileStream << "SOWS \nBACONERS \n";
   fileStream.close();
}

/****************************************************************************\
	writeNotProduceableItems defines not produceable items
\****************************************************************************/
void livestock::writeNotProduceableItems(char * filename)
{
	fstream fileStream;
	fileStream.open(filename,ios::out);

   feedItem* aFeedItem;
   string aPlantName, prefix;
	linkList <product>* feedItemList;
	feedItemList = theProducts->GetSubsetofProductList(feedItemObj);
   for (int i=0; i<2; i++)
   {
   	prefix = (i==0) ? "S_" : "B_";
   	for (int j=0; (j<feedItemList->NumOfNodes()); j++)
   	{
   		aFeedItem 	= (feedItem*) feedItemList->ElementAtNumber(j);
      	aPlantName 	=  aFeedItem->GetPlantName();
      	if (aPlantName=="NONE")
      		fileStream << (prefix + aFeedItem->Getname()) << " \n";
   	}
   }
   fileStream.close();
   delete feedItemList;
}

/****************************************************************************\
writeEmptyFile writes empty files so gams do not crash if there
is no livestock
\****************************************************************************/
void livestock::writeEmptyFile(char * filename)
{
	fstream fileStream;
	fileStream.open(filename,ios::out);
	fileStream  << " \n";
   fileStream.close();
}

/****************************************************************************\
writeVariableCosts writes the variable costs Dkr/unit
\****************************************************************************/
void livestock::writeVariableCosts(char * filename)
{
	fstream fileStream;
	fileStream.open(filename,ios::out);
   swine aSwine;
   slaughterSwine aSlaughterSwine;
   fileStream  << "SOWS      -" << aSwine.GetVariableCosts() << endl;
	fileStream  << "BACONERS  -" << aSlaughterSwine.GetVariableCosts() << endl;
	fileStream  << "COWS      -1500   " << endl;
	fileStream  << "HEIFERS    -100   " << endl;
	fileStream  << "YBULLS     -100   " << endl;
   fileStream.close();
}

/****************************************************************************\
	writeSwineYield writes the number of pigs pr swine
\****************************************************************************/
void livestock::writeReplacementRate(char * filename)
{
	fstream fileStream;
	fileStream.open(filename,ios::out);

   fileStream << "RPRSOWS Replacement rate for sows\n / 0.4 /" << endl;
	fileStream.close();
}

/****************************************************************************\
\****************************************************************************/
void livestock::checkBalances(bool show)
{
   for (int i=0;i<theStockList->NumOfNodes();i++)
   {
	 (theStockList->ElementAtNumber(i))->checkBalances(show);
	}
}


/****************************************************************************\
\****************************************************************************/
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

