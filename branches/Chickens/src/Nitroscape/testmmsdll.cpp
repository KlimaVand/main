#include <bstime.h>
#include <message.h>

message * theMessage;
bsTime theTime;   //needed for theMessage

extern "C"
{
__declspec(dllimport)
void DoDaily_ECO();
__declspec(dllimport)
void Destroy_ECO();
__declspec(dllimport)
void initialise_ECO(int numEcosysts, char * direct, char *climateDir, char * climateFileName, char * outputDir, int day, int month, int year);
__declspec(dllimport)
void initialise_MMS(char * inputDirectory, char * climateDirectory, char * climateFileName, char * outputDirectory);
__declspec(dllimport)
void DoMMSDaily();
__declspec(dllimport)
void Destroy_MMS();
}

void main(int argc, char* argv[])
{
   theMessage = new message;
//   char * testdir;
/*   initialise_ECO(1,"c:\\Home\\FASSET_scenarios\\Cropfarm\\R1\\","c:\\Home\\FASSET_scenarios\\Climate\\","foulum.clm",
   					"c:\\Home\\FASSET_scenarios\\Cropfarm\\R1\\output", 1,8,1990);*/
   initialise_MMS("c:\\Home\\FASSET_scenarios\\Cropfarm\\R1\\","c:\\Home\\FASSET_scenarios\\Climate\\","foulum.clm",
   					"c:\\Home\\FASSET_scenarios\\Cropfarm\\R1\\output");
   bsTime currTime;
   currTime.SetTime(1,8,1990);
   theTime.SetTime(1,8,1990);
   bsTime stopTime;
   stopTime.SetTime(1,8,1991);
   while(currTime < stopTime)
      {
//      	DoDaily_ECO();
      	DoMMSDaily();
         theTime.AddOneDay();
         currTime.AddOneDay();
      }
//   Destroy_ECO();
   Destroy_MMS();
	delete theMessage;
}