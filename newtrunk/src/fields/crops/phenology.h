
#ifndef PHEN_H
#define PHEN_H

#include "../../base/base.h"
#include "../../base/bstime.h"

// 0: emerged, 1: anthesis, 2: end grain fill, 3: ripe

/*
 * this class describe cycle for a plan, from sowing to ripping
 */
class phenology:
    public base
{
    protected:

    public:

        // Definition variables
        double DS_Flagligule,    // ! Developing System scale for start of flag ligule
               DS_StartFill,     // ! Developing System scale for start of grainfill (is it a scale??
               TS0,              // !  Temperature sum from sowing to emergence. Temperature is added when it is above TB0
               TS1,              // ! Temperature sum from emergence to Anthesis. Temperature is added when it is above TB0 Anthesis is when the flower is fully open and functional.
               TS2,                // ! Temperature sum from Anthesis to grain fill. Temperature is added when it is above TB1
               TS3,                // ! Temperature sum from grain fillto  ripeness Temperature is added when it is above TB2
               TB0,                // ! Base temperature until emergence
               TB1,                // ! Base temperature until flowering
               TB2,                // ! Base temperature after flowering
               TB3,                // ! Base temperature for ripening
               DB,                 // ! Daylength. The min daylight length where a plan can develop. Only used in winter times
               DMAX,               // ! Daylength. The max daylight length where a plan can developOnly used in winter times
               LinearLeafPhase;    // ! Growth phase for linear leaf growth (related to TempSumForLeaf!)

        // State variables
        bsTime DateOfEmergence,           // ! The date of emergence; used for indicator output
               DateOfFlowering,           // ! The date of flowering; used for indicator output
               DateOfEndGrainFill,        // ! The date of end of the grainfilling period; used for indicator output
               DateOfRipeness;            // ! The date of ripeness; used for indicator output
        bool   EmergencePassed, FloweringPassed, EndOfGrainFillingPassed, RipenessPassed;
        double DS,                        // ! development stage
               DSIncrease,                // ! Daily increase, only used for output
               TempSumAfterFirstMarch,    // ! Temperature sum after 1. march (used in some old crop models)
               TempSumForLeaf;            // ! Temperature sum for leaf expansion (used in linear growth)
		double vernalReq,VernalIndex,VernalOptTemp,VernalBaseTmp;
        phenology();

        virtual ~phenology();

        phenology(const phenology & aphenology);

        // ! return true if the crop is sown
        bool Sown() {
            return DS >= -1.0;
        }

        // ! return true if crop has emerged
        bool Emerged() {
            return DS >= 0.0;
        }

        // ! return true if crop has reached anthesis
        bool Anthesis() {
            return DS >= 1.0;
        }

        // ! return true if the grain has started to develop
        bool GrainFillStart() {
            return DS >= DS_StartFill;
        }

        // ! return true if the grains is fully developed
        bool GrainFillEnd() {
            return DS >= 2.0;
        }

        // ! return true if planed is ready to be ripped
        bool Ripe() {
            return DS >= 3.0;
        }

        bsTime GiveDateOfEmergence() {
            return DateOfEmergence;
        }

        bsTime GiveDateOfFlowering() {
            return DateOfFlowering;
        }

        bsTime GiveDateOfEndGrainFill() {
            return DateOfEndGrainFill;
        }

        bsTime GiveDateOfRipeness() {
            return DateOfRipeness;
        }

        double GetFractionToAnthesis();

        double GetFractionOfGrainFill();

        double GetFractionToFlagLigule();

        double LeafSenesence();

        void Sow();

        void SetDSAfterCut();

        void Update(double plantTemp,double AirTemp,
                    double soilTemp,
                    double DayLength);

        void Terminate();
};
#endif

