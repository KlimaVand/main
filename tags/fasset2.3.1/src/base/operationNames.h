/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
  Archive
   Author
     Date
 Revision
 ----------------------------------------------------------------------------
      Log
\****************************************************************************/

#ifndef __OPNAMES_H
  #define __OPNAMES_H

/****************************************************************************\
 enum: operationNames
\****************************************************************************/

enum operationNames
{
	Ploughing,                 //  0 DK: Pl�jning
   StubHarrowing,             //  1 DK: Stubharvning
   SeedbedHarrowing,          //  2 DK: S�bedsharvning
   WeedHarrowing,             //  3 DK: Ukrudsharvning
   RowCultivating,            //  4 DK: Ukrudtsharvning, r�kkeafgr�der
   Sowing,                    //  5 DK: S�ning
   SowingBeets,               //  6 DK: S�ning af roer
   SpreadingLiquidManure,     //  7 DK: Udbringning af flydende husdyrg�dning
   ClosedSlotInjectingLiquidManure,     //  8 DK: Nedf�lding af flydende husdyrg�dning
   SpreadingSolidManure,      //  9 DK: Udbringning af fast husdyrg�dning
   MineralFertilizing,        // 10 DK: Udbringning af handelsg�dning
   SpreadingLime,             // 11 DK: Udbringning af kalk
   CombineHarvesting,         // 12 DK: Mejet�rskning
   CombineHarvestingShred,    // 13 DK: Mejet�rskning med halmsnitning
   ForageHarvesting,          // 14 DK: Finsnitning
   BeetHarvesting,            // 15 DK: Roeoptagning
   BeetTopHarvesting,         // 16 DK: H�stning af roetoppe
   BeetTopChopping,           // 17 DK: Snitning af roetoppe
   Mowing,                    // 18 DK: Sk�rl�gning
   CutSetAside,               // 19 DK: Afpudsning af brak
   GatheringBales,            // 20 DK: Hjemk�rsel af halmballer
   Spraying,                  // 21 DK: Spr�jtning
   HandWeedingBeets,          // 22 DK: H�ndhakning af roer
   Irrigating,                // 23 DK: Vanding
   MonitoringCrop,            // 24 DK: Plantev�rnsregistrering
   Fencing,                   // 25 DK: Ops�tning af hegn
   RemoveFencing,             // 26 DK: Nedtagning af hegn
   MakeGrazable,					// 27 DK: G�re klar til afgr�sning
   MakeSilage,						// 28 DK: Lave ensilage
   EmptyOperation,            // 29 DK: Tom operation
   StartGrazing,              // 30
   EndGrazing,                // 31
   GatheringCereal,           // 32
   GatheringSilage,            // 33
   GatheringHay,					// 34
   OpenSlotInjectingLiquidManure,	//35
   TrailingHoseSpreadingLiquidManure, //36
   TrailingShoeSpreadingLiquidManure  //37
};

/****************************************************************************\
 const: numberOfOperations and const: operationNamelist
\****************************************************************************/

const int numberOfOperations=38;
const string operationNameList[numberOfOperations]=
{
   "Ploughing",						//0
   "StubHarrowing",              //1
   "SeedbedHarrowing",           //2
   "WeedHarrowing",              //3
   "RowCultivating",             //4
   "Sowing",                     //5
   "SowingBeets",                //6
   "SpreadingLiquidManure",      //7
   "ClosedSlotInjectingLiquidManure",      //8
   "SpreadingSolidManure",       //9
   "MineralFertilizing",         //10
   "SpreadingLime",              //11
   "CombineHarvesting",          //12
   "CombineHarvestingShred",     //13
   "ForageHarvesting",           //14
   "BeetHarvesting",             //15
   "BeetTopHarvesting",          //16
   "BeetTopChopping",            //17
   "Mowing",                     //18
   "CutSetAside",                //19
   "GatheringBales",             //20
   "Spraying",                   //21
   "HandWeedingBeets",           //22
   "Irrigating",                 //23
   "MonitoringCrop",             //24
   "Fencing",                    //25
   "RemoveFencing",              //26
   "MakeGrazable",               //27
   "MakeSilage",                 //28
   "EmptyOperation",              //29
   "StartGrazing",               //30
   "EndGrazing",                 //31
   "GatheringCereal",            //32
   "GatheringSilage",            //33
   "GatheringHay",                //34
   "OpenSlotInjectingLiquidManure",	//35
   "TrailingHoseSpreadingLiquidManure", //36
   "TrailingShoeSpreadingLiquidManure"  //37
};
#endif

