#include<iostream>
#include <fstream>
#include "base/common.h"
#include "base/bstime.h"
#include "base/message.h"
#include "base/settings.h"
#include "base/IndicatorOutput.h"
#include "base/climate.h"
#include "base/controlParameters.h"
#include "tools/fileAccess.h"
#include "base/commonData.h"
#include "tools/compare.h"
#include "separator.h"
#include "products/liveMatter.h"










using namespace std;


bsTime theTime;

message * theMessage;




climate * theClimate;
controlParameters * theControlParameters;

IndicatorOutput * theOutput;
settings * globalSettings;


int main(){
	liveMatter slurry;
	slurry.Setamount(176);
	nitrogen TAN;
	TAN.n=176*3.7/100;
	slurry.SetNH4_content (TAN);
	
	int i;
	Separation ting;
	ting.receive(slurry);
	ting.output();
	for (i=1; i <= 10; i++);
}
