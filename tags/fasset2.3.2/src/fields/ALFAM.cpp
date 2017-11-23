/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "ALFAM.h"

#include "../base/message.h"

/****************************************************************************\
\****************************************************************************/
ALFAM::ALFAM()
{
maxTime	=168.0;
b_Nmx0  =	-6.5757 ;
b_sm1Nmx	=0.0971;
b_atNmx	=0.0221 ;
b_wsNmx	=0.0409  ;
b_mt1Nmx=	-0.156 ;
b_mdmNmx	=0.1024    ;
b_mtanNmx =	-0.1888  ;
b_ma0Nmx	=3.5691;
b_ma1Nmx	=3.0198 ;
b_ma2Nmx	=3.1592  ;
b_ma3Nmx	=2.2702   ;
b_ma4Nmx	= 2.9582   ;
b_mrNmx	=-0.00433   ;
b_mi0Nmx	=2.4291      ;
b_met1Nmx =	-0.6382    ;
b_met2Nmx  =	-0.5485  ;
b_Km0	=0.037;
b_sm1Km=	0.0974;
b_atKm  =	-0.0409;
b_wsKm  =	-0.0517 ;
b_mt1Km	=1.3567;
b_mdmKm	=0.1614;
b_mtanKm	=0.1011;
b_mrKm	=0.0175;
b_met1Km	=0.3888;
b_met2Km	=0.7024;

}

/****************************************************************************\
manureType: 0 = cattle, 1 = pig
\****************************************************************************/
void ALFAM::initialise(int soilWet, double aveAirTemp, double aveWindspeed, int manureType,
			double initDM, double initTAN, double appRate, int appMeth, double anexposureTime)
{
   TAN=initTAN;
   applicRate=appRate;
   exposureTime=anexposureTime;
   timeElapsed=0.0;
   switch (appMeth) {
   case 1:   //broadcast
	Nmax=exp(b_Nmx0 + b_sm1Nmx*soilWet + b_atNmx*aveAirTemp + b_wsNmx*aveWindspeed + b_mt1Nmx*manureType
   		 + b_mdmNmx*initDM + b_mtanNmx*TAN + b_ma0Nmx
          + b_mrNmx*appRate + b_mi0Nmx + b_met2Nmx);
	km= exp(b_Km0 + b_sm1Km*soilWet + b_atKm*aveAirTemp + b_wsKm*aveWindspeed + b_mt1Km*manureType
   		 + b_mdmKm*initDM + b_mtanKm*TAN
          + b_mrKm*appRate + b_met2Km);
          break;
   case 2:    //trailing hose
	Nmax=exp(b_Nmx0 + b_sm1Nmx*soilWet + b_atNmx*aveAirTemp + b_wsNmx*aveWindspeed + b_mt1Nmx*manureType
   		 + b_mdmNmx*initDM + b_mtanNmx*TAN + b_ma1Nmx
          + b_mrNmx*appRate + b_mi0Nmx + b_met2Nmx);
	km= exp(b_Km0 + b_sm1Km*soilWet + b_atKm*aveAirTemp + b_wsKm*aveWindspeed + b_mt1Km*manureType
   		 + b_mdmKm*initDM + b_mtanKm*TAN
          + b_mrKm*appRate + b_met2Km);
          break;
   case 3:   //trailing shoe
	Nmax=exp(b_Nmx0 + b_sm1Nmx*soilWet + b_atNmx*aveAirTemp + b_wsNmx*aveWindspeed + b_mt1Nmx*manureType
   		 + b_mdmNmx*initDM + b_mtanNmx*TAN + b_ma2Nmx
          + b_mrNmx*appRate + b_mi0Nmx + b_met2Nmx);
	km= exp(b_Km0 + b_sm1Km*soilWet + b_atKm*aveAirTemp + b_wsKm*aveWindspeed + b_mt1Km*manureType
   		 + b_mdmKm*initDM + b_mtanKm*TAN
          + b_mrKm*appRate + b_met2Km);
          break;
   case 4:  //open slot
	Nmax=exp(b_Nmx0 + b_sm1Nmx*soilWet + b_atNmx*aveAirTemp + b_wsNmx*aveWindspeed + b_mt1Nmx*manureType
   		 + b_mdmNmx*initDM + b_mtanNmx*TAN + b_ma3Nmx
          + b_mrNmx*appRate + b_mi0Nmx + b_met2Nmx);
	km= exp(b_Km0 + b_sm1Km*soilWet + b_atKm*aveAirTemp + b_wsKm*aveWindspeed + b_mt1Km*manureType
   		 + b_mdmKm*initDM + b_mtanKm*TAN
          + b_mrKm*appRate + b_met2Km);
          break;
   case 5:  //closed slot
	Nmax=exp(b_Nmx0 + b_sm1Nmx*soilWet + b_atNmx*aveAirTemp + b_wsNmx*aveWindspeed + b_mt1Nmx*manureType
   		 + b_mdmNmx*initDM + b_mtanNmx*TAN  + b_ma4Nmx
          + b_mrNmx*appRate + b_mi0Nmx + b_met2Nmx);
	km= exp(b_Km0 + b_sm1Km*soilWet + b_atKm*aveAirTemp + b_wsKm*aveWindspeed + b_mt1Km*manureType
   		 + b_mdmKm*initDM + b_mtanKm*TAN
          + b_mrKm*appRate + b_met2Km);
          break;
   };
};
double ALFAM::ALFAM_volatilisation(double duration)
{
   if (timeElapsed>=exposureTime)
   {
		timeElapsed+=duration;
   	return 0.0;
   }
   else
   {
      if ((timeElapsed+duration)>exposureTime)
         duration=exposureTime-timeElapsed;
      double ret_val = Nmax * ((timeElapsed+duration)/((timeElapsed+duration)+km)-timeElapsed/(timeElapsed+km));
      if (ret_val>1.0)
			ret_val=1.0;
      ret_val *=TAN * applicRate;
      timeElapsed+=duration;
      return ret_val;
   }
};

int ALFAM::GetALFAMApplicCode(int OpCode)
{
	switch (OpCode) {
   case 7://SpreadingLiquidManure
   		return 1;
   case 8:  //ClosedSlotInjectingLiquidManure
   		return 5;
   case 9://SpreadingSolidManure
   		return 1;
   case 35: //OpenSlotInjectingLiquidManure
   		return 4;
   case 36: //TrailingHoseSpreadingLiquidManure
   		return 2;
   case 37: //TrailingShoeSpreadingLiquidManure
   		return 3;
   default:
   	theMessage->FatalError("ALFAM: application method code not found");
   }
	return 0;
};

bool ALFAM::GetIsfinished()
{
   if (timeElapsed>=maxTime)
   	return true;
   else
   	return false;
}

