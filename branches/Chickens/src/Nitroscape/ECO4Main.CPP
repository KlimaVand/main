#include "..\base\bstime.h"
#include "..\base\message.h"
#include "..\Nitroscape\ECOMainClass.h"


message * theMessage;
bsTime theTime;   //needed for theMessage

/*
THIS PROGRAM COMPILES AND RUNS FOR A SHORT TIME, BEFORE FAULTING WITH A CONTROLLED PROGRAM TERMINATION IN THE SOIL
*/ 
void main(int argc, char* argv[])
{
  	string inputDirectory;
	string outputDirectory;
	string climateDirectory;
	string climateFileName;
	string scenarioFileName;
	string baseOutputDir;
//   char * testdir;
//NJH call to this class constructor leads to input data being read from system.dat (must be in current working directory
	theSystemData =new systemData(true);
//NJH read number of farms - could be replaced by Palm variable
   int numOfFarms=theSystemData->getRunLevel()+1;
	inputDirectory = theSystemData->getInputDirectory();
   climateDirectory = theSystemData->getClimateDirectory();
   climateFileName = theSystemData->getClimateFileName();
    outputDirectory = theSystemData->getOutputDirectory();
    int startYear=0;
    int stopYear=0;
    ECOMainClass * theECOMainClass = new ECOMainClass();
   theECOMainClass->initialise_ECO(1,theSystemData->getSystemRunData(0),&startYear, &stopYear);


   bsTime currTime;
   currTime.SetTime(1,8,startYear);
   theTime.SetTime(1,8,startYear);
   bsTime stopTime;
   stopTime.SetTime(1,8,stopYear);
   while(currTime < stopTime)
      {
         if((theTime.GetDay()==17) && (theTime.GetMonth()==9) && (theTime.GetYear()==1987))
         cout << " ";
         cout << theTime << endl;;
      	theECOMainClass->DoDaily_ECO();
         theTime.AddOneDay();
         currTime.AddOneDay();
      }
   theECOMainClass->Destroy_ECOs();
   delete theECOMainClass;
	delete theMessage;
}
