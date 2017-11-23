/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "output.h"

/****************************************************************************\
  Constructor
\****************************************************************************/
OutputControl::OutputControl() : base()
{
   PlantDetails = false;
   SoilDetails = false;
   WaterDetails = false;
   CowDetails	= false;
   HerdDetails	= false;
   PastureDetails = false;
   OperationalDetails = false;
   ManureManageDetails = false;

   for (int i=0; i<NumberWaterProfiles; i++)
   	WaterStart[i] = WaterEnd[i] = NminStart[i] = NminEnd[i] = 0.0;
   f.open("scenarie.txt",ios::out);
}

/****************************************************************************\
  Destructor
\****************************************************************************/
OutputControl::~OutputControl()
{
	f.close();
}

/****************************************************************************\
  Log for scenaries
\****************************************************************************/
void OutputControl::ScenarieLog(string aLogString)
{
	f << aLogString << "\t" ;
}

/****************************************************************************\
\****************************************************************************/
void OutputControl::ScenarieLogNewLine()
{
	f << endl;
}

/****************************************************************************\
\****************************************************************************/
void OutputControl::ScenarieLog(double aNumber)
{
	f  << aNumber << "\t";
}

/****************************************************************************\
\****************************************************************************/
void OutputControl::Initialize(char* filename)
{
	WaterEnd[0] = 200.0;
   WaterEnd[1] = 500.0;
   WaterEnd[2] = 1000.0;
   WaterEnd[3] = 0.0;
   NminEnd[0] = 20.0;
   NminStart[1] = 20;
   NminEnd[1] = 200.0;
   NminEnd[2] = 1000.0;
   NminEnd[3] = 0.0;
   CupDepth[0] = 100.0;                  // default suction cup depth
   TemperatureDepth[0] = 0.0;
   TemperatureDepth[1] = 100;
   TemperatureDepth[2] = 300;
   TemperatureDepth[3] = 500;
   TemperatureDepth[4] = -1;

   int first,num;
   double start, end;

   if (OpenInputFile(filename))
   {
   	if (FindSection("Water(0)"))
	 	{
      	WaterEnd[0] = WaterEnd[1] = WaterEnd[2] = WaterEnd[3] = 0.0;
			GetSectionNumbers("Water",&first,&num);
	   	for(int index=first;index<(first+num);index++)
	   	{
	   		FindSection("Water",index);
	      	GetParameter("StartDepth",&start);
	      	GetParameter("EndDepth",&end);
	         WaterStart[index-first] = start*10;
	         WaterEnd[index-first] = end*10;
	   	}
	   	WaterEnd[first+num] = 0.0;
      }
      if (FindSection("Nmin(0)"))
	 	{
      	NminEnd[0] = NminEnd[1] = 0.0;
			GetSectionNumbers("Nmin",&first,&num);
	   	for(int index=first;index<(first+num);index++)
	   	{
	   		FindSection("Nmin",index);
	      	GetParameter("StartDepth",&start);
	      	GetParameter("EndDepth",&end);
	         NminStart[index-first] = start*10;
	         NminEnd[index-first] = end*10;
	   	}
	   	NminEnd[first+num] = 0.0;
      }
      if (FindSection("PoreWater(0)"))
	 	{
      	CupDepth[0] =  0.0;
			GetSectionNumbers("PoreWater",&first,&num);
	   	for(int index=first;index<(first+num);index++)
	   	{
	   		FindSection("PoreWater",index);
	      	GetParameter("Depth",&start);
	         CupDepth[index-first] = start*10;
	   	}
	   	CupDepth[first+num] = 0.0;
      }
      if (FindSection("SoilC(0)"))       //MEL:log soilC  10/08
	 	{
      	SoilCEnd[0] = SoilCEnd[1] = 0.0;
			GetSectionNumbers("SoilC",&first,&num);
	   	for(int index=first;index<(first+num);index++)
	   	{
	   		FindSection("SoilC",index);
	      	GetParameter("StartDepth",&start);
	      	GetParameter("EndDepth",&end);
	         SoilCStart[index-first] = start*10;
	         SoilCEnd[index-first] = end*10;
	   	}
	   	SoilCEnd[first+num] = 0.0;
      }

      if (FindSection("Temperature(0)"))
	 	{
      	TemperatureDepth[0] =  0.0;
			GetSectionNumbers("Temperature",&first,&num);
	   	for(int index=first;index<(first+num);index++)
	   	{
	   		FindSection("Temperature",index);
	      	GetParameter("Depth",&start);
	         TemperatureDepth[index-first] = start*10;
	   	}
	   	TemperatureDepth[first+num] = -1.0;
      }

      if (FindSection("Details"))
      {
      	GetParameter("PlantDetails",&PlantDetails);
         GetParameter("SoilDetails",&SoilDetails);
         GetParameter("WaterDetails",&WaterDetails);
         GetParameter("CowDetails",&CowDetails);
         GetParameter("HerdDetails",&HerdDetails);
         GetParameter("PastureDetails",&PastureDetails);
         GetParameter("OperationalDetails",&OperationalDetails);
         GetParameter("ManureManageDetails",&ManureManageDetails);
		}
      CloseInputFile();
 	}
}

