/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "../../base/commonData.h"
#include "phenology.h"
#include "../../base/climate.h"

#include "../../base/bstime.h"
#include "../../base/message.h"
#include "../../base/settings.h"
/****************************************************************************\
\****************************************************************************/
phenology::phenology()
{
   // state variables
   DSIncrease = 0.0;
	DS = -2.0;             // -1: sown, 0: emerged, 1: anthesis, 2: end grain fill, 3: ripe
   TempSumForLeaf = 0.0;  // Temperature sum for linear leafarea expansion
   TempSumAfterFirstMarch = 0.0;
   
	// constant definitions variables
   commonData * data=globalSettings[id]->CropInformation;

   data->FindItem("TS0",&TS0);                 // Emergence temperature sum
   data->FindItem("TS1",&TS1);             // Temperature sum for Anthesis
   data->FindItem("TS2",&TS2);                 // Temperature sum for end grain fill
   data->FindItem("TS3",&TS3);                 // Temperature sum for ripeness
   data->FindItem("TB0",&TB0);                   // Base temperature until emergence
   data->FindItem("TB1",&TB1);  				   // Base temperature until flowering
   data->FindItem("TB2",&TB2);  						// Base temperature after flowering
   data->FindItem("TB3",&TB3);                   // Base temperature for ripening
   data->FindItem("DB",&DB);  						// Daylength response   (Weir et al.)
   data->FindItem("DMAX",&DMAX);		            // Daylength response
   data->FindItem("DS_Flagligule",&DS_Flagligule);				// DS scale for start of flag ligule (BBCH 39)
   data->FindItem("DS_StartFill",&DS_StartFill);         // DS scale for start of grainfill

   data->FindItem("LinearLeafPhase",&LinearLeafPhase);                 // Note this phase relates to TempSumForLeaf!!
   DateOfEmergence.SetTime(1,1,1900);     // Date of emergence of the crop
     DateOfFlowering.SetTime(1,1,1900);     // Date of Flowering of the crop
     DateOfEndGrainFill.SetTime(1,1,1900);  // Date of end of grain filling of the crop
     DateOfRipeness.SetTime(1,1,1900);     // Date of ripeness of the crop

     EmergencePassed = false;
     FloweringPassed = false;
     EndOfGrainFillingPassed = false;
     RipenessPassed = false;
}

/****************************************************************************\
\****************************************************************************/
phenology::~phenology()
{
}

/****************************************************************************\
\****************************************************************************/
phenology::phenology(const phenology& c)
{
	DS             = c.DS;
   DSIncrease		= c.DSIncrease;
   TempSumForLeaf = c.TempSumForLeaf;
   TempSumAfterFirstMarch = c.TempSumAfterFirstMarch;

	DS_Flagligule  = c.DS_Flagligule;
   DS_StartFill   = c.DS_StartFill;
   TS0            = c.TS0;
   TS1            = c.TS1;
   TS2            = c.TS2;
   TS3            = c.TS3;
   TB0            = c.TB0;
   TB1            = c.TB1;
   TB2            = c.TB2;
   TB3            = c.TB3;
   DB             = c.DB;
   DMAX           = c.DMAX;
   LinearLeafPhase= c.LinearLeafPhase;
}
/**
 * Add 2 phenology classes together if they are not to different in the Developing stage
 */
void phenology::Add(phenology* aPhenology, double fraction)
{
   if (fabs(DS-aPhenology->DS)>0.1) 
   	theMessage[id]->FatalError("Phenology::Add - To large difference in development stage");

	DS      			= (1-fraction)*DS + fraction*aPhenology->DS;
   TempSumForLeaf = (1-fraction)*TempSumForLeaf + fraction*aPhenology->TempSumForLeaf;
   TempSumAfterFirstMarch = (1-fraction)*TempSumAfterFirstMarch + fraction*aPhenology->TempSumAfterFirstMarch;
}
/**
 * Update DS, DSIncrease, TempSumForLeaf and TempSumAfterFirstMarch.
 */
void phenology::Update(double AirTemp,double SoilTemp,double DayLength)
{
   if (DS<0.0)       											// Not emerged
   	DS += max(SoilTemp-TB0,0.0)/TS0;
   else                              						// Emerged
   {
   	TempSumForLeaf += max(0.0,AirTemp);
	if (theTime[id].GetMonth()==3 && theTime[id].GetDay()==1)
      	TempSumAfterFirstMarch = 0.0;
    TempSumAfterFirstMarch += max(0.0,AirTemp);
      
    double DL=max(0.0,min(1.0,(DayLength-DB)/(DMAX-DB)));
    DSIncrease = DS;

    if (DS<1.0)                                     // Until flowering
    	DS += DL*max(AirTemp-TB1,0.0)/TS1;
    else if (DS<2.0)                                  // Until end of grain filling
    	DS += max(AirTemp-TB2,0.0)/TS2;
    else if (TS3>0 && DS<3.0)                                // Until ripe
    	DS += max(AirTemp-TB3,0.0)/TS3;
    else
    	DS = 3.0;                              // Ripe
    if (DS>3.0)
      	DS = 3.0;

      DSIncrease = DS-DSIncrease;                     // Increase in DS this day
   }
   //log dates for critical growth developmental stages
      if (DS > 0 && EmergencePassed == false)
   	{
      	DateOfEmergence.SetTime(theTime[id].GetDay(),theTime[id].GetMonth(),theTime[id].GetYear());
         EmergencePassed = true;
      }
      if (DS > 1 && FloweringPassed == false)
   	{
      	DateOfFlowering.SetTime(theTime[id].GetDay(),theTime[id].GetMonth(),theTime[id].GetYear());
         FloweringPassed = true;
      }
      if (DS > 2 && EndOfGrainFillingPassed == false)
   	{
      	DateOfEndGrainFill.SetTime(theTime[id].GetDay(),theTime[id].GetMonth(),theTime[id].GetYear());
         EndOfGrainFillingPassed = true;
      }
      if (DS > 3 && RipenessPassed == false)
   	{
      	DateOfRipeness.SetTime(theTime[id].GetDay(),theTime[id].GetMonth(),theTime[id].GetYear());
         RipenessPassed = true;
      }
}
/****************************************************************************\
\****************************************************************************/
void phenology::Sow()
{
	DS = -1.0;
   TempSumForLeaf = 0.0;
   TempSumAfterFirstMarch = 0.0;
}
/****************************************************************************\
\****************************************************************************/
void phenology::SetDSAfterCut()
{
	DS = 0.05;
}
/**
 * function returns the DS remaining before anthesis
 */
double phenology::GetFractionToAnthesis()
{
	double fraction = 1.0 - DS;
   return max(min(fraction,1.0),0.0);
}
/**
 * function returns the DS remaining before flag ligule
 */
double phenology::GetFractionToFlagLigule()
{
	if (DS<DS_Flagligule)
   	return max(0.0,DS/DS_Flagligule);
   else
   	return min(2.0,1.0+(DS-DS_Flagligule)/(2.0-DS_Flagligule));
}
/**
 * function returns the DS fraction between start and end of grain fill
 */
double phenology::GetFractionOfGrainFill()
{
	double fraction = 0.0;
   if (DS<=2.0) fraction = DSIncrease/(2.0-DS_StartFill);
   return max(min(fraction,1.0),0.0);
}
/**
 * function returns leaf scenesence factor
 */
double phenology::LeafSenesence()
{
	return DSIncrease/(2.0-1.0);                   // 2.0 as assuming that crop is yellow at end of grain filling.
}


