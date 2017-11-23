/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/


#ifdef __BCplusplus__
#include "../base/settings.h"

#include "../base/common.h"
#include "../base/bstime.h"
#include "../products/products.h"
#include "../base/message.h"

#include "../building/thebuild.h"
#include "../base/climate.h"
#include "../event/eventControl.h"
#include "../base/controlParameters.h"
#include "../technics/technics.h"
#include "../manager/manureManager.h"
#include "../tools/fileAccess.h"

#include "../base/commonData.h"

#endif
#ifndef __BCplusplus__



#include "base/common.h"
#include "base/bstime.h"
#include "products/products.h"
#include "base/message.h"
#include "base/id.h"

#include "building/thebuild.h"
#include "base/climate.h"
#include "event/eventControl.h"

#include "base/controlParameters.h"
#include "technics/technics.h"
#include "manager/manureManager.h"
#include "tools/fileAccess.h"


#include "base/commonData.h"


#include "tools/compare.h"
#endif
#include <time.h>
using namespace std;
#ifndef NITROSCAPE
#include <thread>
#include <vector>
bsTime theTime[100];
products * theProducts[100];
message * theMessage[100];

IndicatorOutput * theOutput[100];

technics * theTechnics[100];
buildings * theBuildings[100];
climate * theClimate[100];
controlParameters * theControlParameters[100];
manureManager *  theManure_manager[100];

settings * globalSettings[100];
//static fstream * fs; //scenario log file @@@
ThreadID * threadID;
class copyClass
{
public:
	int index;
};
class SayHello
{
private:


public:
	copyClass hallos;
	SayHello( )
	{

	}
	void set(copyClass classchange)
	{
		hallos=classchange;
	}
	void print()
	{
		for(int i=0;i<100000;i++)
		cout<<"indez "<<hallos.index<<endl;
	}
	void operator()()
	{
		print();
	}
};

/****************************************************************************\
\****************************************************************************/


/****************************************************************************\
\****************************************************************************/

int modified()
{




	   int index;
	   cout << "Farm ASSEsment Tool v2.3 beta" << endl << endl;


	   threadID=new ThreadID;

	   // Used to read info that is common  for all runs in system.dat. Can be overwritten later in the individual run definitions

	   commonData data;
	   data.setOutputWarning(false);
	   data.setCritical(true);
	   data.readFile("system.dat");
	   int firstScenarie=0;
	   int num=0;
data.setCritical(false);
	   data.FindItem("JumpStart",&firstScenarie);

	   data.FindItem("JumpStop",&num);

	   bool remove=false;


	    eventControl contronl0;
	   eventControl contronl1;



	   contronl0.Settup(data, 0,remove);
	   thread td0(ref(contronl0));

	    contronl1.Settup(data, 1,remove);
	    thread td1(ref(contronl1));

	   td0.join();
	   td1.join();
	     contronl0.CleanUp();

	   contronl1.CleanUp();



	   return 0;
}
double diffclock(clock_t clock1,clock_t clock2)
{
	double diffticks=clock1-clock2;
	double diffms=(diffticks*1000)/CLOCKS_PER_SEC;
	return diffms;
}





int main(int argc,char *argv[])  //NJH, 28.2.07
{

	
	int times=1;
	fileAccess hd;
	string path=hd.getCurrentPath();
	clock_t begin=clock();
	for(int i=0;i<times;i++)
	{
		hd.changeDir(path);
		modified();
	}
	clock_t end=clock();
	cout << "Time elapsed: " << double(diffclock(end,begin))/times << " ms"<< endl;
	return true;
}
#endif

