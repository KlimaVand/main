
#include "../../base/common.h"
#include "../../base/commonData.h"
#include "phenology.h"
#include "../../base/climate.h"
#include "../../base/bstime.h"
#include "../../base/message.h"
#include "../../base/settings.h"
#include <algorithm> 
phenology::phenology() {

    // state variables
    DSIncrease             = 0.0;
    DS                     = -2.0;    // -1: sown, 0: emerged, 1: anthesis, 2: end grain fill, 3: ripe
    TempSumForLeaf         = 0.0;     // Temperature sum for linear leafarea expansion
    TempSumAfterFirstMarch = 0.0;
	VernalIndex			   = 0.0;
    // constant definitions variables
    commonData * data = globalSettings -> CropInformation;

    data -> FindItem("TS0", &TS0);    // Emergence temperature sum
    data -> FindItem("TS1", &TS1);    // Temperature sum for Anthesis
    data -> FindItem("TS2", &TS2);    // Temperature sum for end grain fill
    data -> FindItem("TS3", &TS3);    // Temperature sum for ripeness
    data -> FindItem("TB0", &TB0);    // Base temperature until emergence
    data -> FindItem("TB1", &TB1);    // Base temperature until flowering
    data -> FindItem("TB2", &TB2);    // Base temperature after flowering
    data -> FindItem("TB3", &TB3);    // Base temperature for ripening
    data -> FindItem("DB", &DB);    // Daylength response (Weir et al.)
    data -> FindItem("DMAX", &DMAX);    // Daylength response
    data -> FindItem("DS_Flagligule", &DS_Flagligule);    // DS scale for start of flag ligule (BBCH 39)
    data -> FindItem("DS_StartFill", &DS_StartFill);    // DS scale for start of grainfill
    data -> FindItem("LinearLeafPhase", &LinearLeafPhase);    // Note this phase relates to TempSumForLeaf!!

	data -> FindItem("vernalReq", &vernalReq); 
    data -> FindItem("VernalOptTemp", &VernalOptTemp);    // DS scale for start of grainfill
    data -> FindItem("VernalBaseTmp", &VernalBaseTmp);    // Note this phase relates to TempSumForLeaf!!

    // DateOfEmergence;    // Date of emergence of the crop
    // DateOfFlowering;    // Date of Flowering of the crop
    // DateOfEndGrainFill; // Date of end of grain filling of the crop
    // DateOfRipeness;    // Date of ripeness of the crop
    EmergencePassed         = false;
    FloweringPassed         = false;
    EndOfGrainFillingPassed = false;
    RipenessPassed          = false;
}

/*
 * Update DS, DSIncrease, TempSumForLeaf and TempSumAfterFirstMarch.
 */
void phenology::Update(double plantTemp,double AirTemp,
                       double SoilTemp,
                       double DayLength) {
    if (DS < 0.0) {                              // Not emerged
        DS += max(SoilTemp - TB0, 0.0) / TS0;
    } else {                                     // Emerged
        TempSumForLeaf += max(0.0, AirTemp);

        if ((theTime.GetMonth() == 3) && (theTime.GetDay() == 1)) {
            TempSumAfterFirstMarch = 0.0;
        }

        TempSumAfterFirstMarch += max(0.0, AirTemp);

        double DL = max(0.0, min(1.0, (DayLength - DB) / (DMAX - DB)));

        DSIncrease = DS;
		if(vernalReq>0)
		{
			VernalIndex+=max(0.0,(VernalOptTemp-AirTemp)/(VernalOptTemp-VernalBaseTmp))/vernalReq;
		}
		else
			VernalIndex=1;
		VernalIndex=min(1.0,VernalIndex);
        if (DS < 1.0) {                          // Until flowering
			DS += DL * max(plantTemp - TB1, 0.0) / TS1*VernalIndex;
        } else if (DS < 2.0) {                   // Until end of grain filling
			DS += max(plantTemp - TB2, 0.0) / TS2;
        } else if ((TS3 > 0) && (DS < 3.0)) {    // Until ripe
			DS += max(plantTemp - TB3, 0.0) / TS3;
        } else {
            DS = 3.0;                            // Ripe
        }

        if (DS > 3.0) {
            DS = 3.0;
        }

        DSIncrease = DS - DSIncrease;            // Increase in DS this day
    }

    // log dates for critical growth developmental stages
    if ((DS > 0) && (!EmergencePassed)) {
        DateOfEmergence = bsTime(theTime);
        EmergencePassed = true;
    }

    if ((DS > 1) && (!FloweringPassed)) {
        DateOfFlowering = bsTime(theTime);
        FloweringPassed = true;
    }

    if ((DS > 2) && (!EndOfGrainFillingPassed)) {
        DateOfEndGrainFill      = bsTime(theTime);
        EndOfGrainFillingPassed = true;
    }

    if ((DS >= 3) && (!RipenessPassed)) {
        DateOfRipeness = bsTime(theTime);
        RipenessPassed = true;
    }
}

void phenology::Sow() {
    DS                     = -1.0;
    TempSumForLeaf         = 0.0;
    TempSumAfterFirstMarch = 0.0;
}

void phenology::SetDSAfterCut() {
    DS = 0.05;
}

/*
 * function returns the DS remaining before anthesis
 */
double phenology::GetFractionToAnthesis() {
    double fraction = 1.0 - DS;

    return max(min(fraction, 1.0), 0.0);
}

/*
 * function returns the DS remaining before flag ligule
 */
double phenology::GetFractionToFlagLigule() {
    if (DS < DS_Flagligule) {
        return max(0.0, DS / DS_Flagligule);
    } else {
        return min(2.0, 1.0 + (DS - DS_Flagligule) / (2.0 - DS_Flagligule));
    }
}

/*
 * function returns the DS fraction between start and end of grain fill
 */
double phenology::GetFractionOfGrainFill() {
    double fraction = 0.0;

    if (DS <= 2.0) {
        fraction = DSIncrease / (2.0 - DS_StartFill);
    }

    return max(min(fraction, 1.0), 0.0);
}

/*
 * function returns leaf scenesence factor
 */
double phenology::LeafSenesence() {
    return DSIncrease / (2.0 - 1.0);

    // 2.0 as assuming that crop is yellow at end of grain filling.
}

void phenology::Terminate() {
    DS                     = -2.0;
    DSIncrease             = 0.0;
    TempSumForLeaf         = 0.0;
    TempSumAfterFirstMarch = 0.0;
	VernalIndex			= 0;
}

phenology::~phenology() {}

phenology::phenology(const phenology & c) {
    EmergencePassed        = FloweringPassed = EndOfGrainFillingPassed = RipenessPassed = false;
    DS                     = c.DS;
    DSIncrease             = c.DSIncrease;
    TempSumForLeaf         = c.TempSumForLeaf;
    TempSumAfterFirstMarch = c.TempSumAfterFirstMarch;
    DS_Flagligule          = c.DS_Flagligule;
    DS_StartFill           = c.DS_StartFill;
    TS0                    = c.TS0;
    TS1                    = c.TS1;
    TS2                    = c.TS2;
    TS3                    = c.TS3;
    TB0                    = c.TB0;
    TB1                    = c.TB1;
    TB2                    = c.TB2;
    TB3                    = c.TB3;
    DB                     = c.DB;
    DMAX                   = c.DMAX;
    LinearLeafPhase        = c.LinearLeafPhase;

	vernalReq= c.vernalReq;
	VernalIndex= c.VernalIndex;
	VernalOptTemp= c.VernalOptTemp;
	VernalBaseTmp=c.VernalBaseTmp;
}
	
