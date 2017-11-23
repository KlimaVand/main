/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef ALFAM_H
   #define ALFAM_H
/**
This is a test
*/
class ALFAM
{
double maxTime;
///<Parameters of the ALFAM model
	double		b_Nmx0,	///< parameter in ALFAM model
				b_sm1Nmx,	///<parameter in ALFAM model
				b_atNmx,	///< parameter in ALFAM model
				b_wsNmx,	///< parameter in ALFAM model
				b_mt1Nmx,	///< parameter in ALFAM model
				b_mdmNmx,	///< parameter in ALFAM model
				b_mtanNmx,	///< parameter in ALFAM model
				b_ma0Nmx,	///< parameter in ALFAM model
				b_ma1Nmx,	///< parameter in ALFAM model
				b_ma2Nmx,	///< parameter in ALFAM model
				b_ma3Nmx,	///< parameter in ALFAM model
				b_ma4Nmx,	///< parameter in ALFAM model
				b_mrNmx,	///< parameter in ALFAM model
				b_mi0Nmx,	///< parameter in ALFAM model
				b_met1Nmx,///< parameter in ALFAM model
				b_met2Nmx,	///< parameter in ALFAM model
				b_Km0,	///< parameter in ALFAM model
				b_sm1Km,	///< parameter in ALFAM model
				b_atKm,	///< parameter in ALFAM model
				b_wsKm,	///< parameter in ALFAM model
				b_mt1Km,	///< parameter in ALFAM model
				b_mdmKm,	///< parameter in ALFAM model
				b_mtanKm,	///< parameter in ALFAM model
				b_mrKm,	///< parameter in ALFAM model
				b_met1Km,	///< parameter in ALFAM model
				b_met2Km;	///< parameter in ALFAM model


protected:

   double   TAN,					///<initial TAN content (gN/kg manure)
            applicRate;    	///<application rate (tonnes/ha)
   double km,			///< time for half of Nmax to have been emitted (hours)
	Nmax;			///<proportion of TAN that is emitted at time = infinity
   double timeElapsed, 		///<time since application occurred
		exposureTime;	///<time from application before ploughing or harrowing (hours)

public:
	ALFAM();	///< constructor
/// initialise emiting area
   void initialise(int soilWet, double aveAirTemp, double aveWindspeed, int manureType,
   					double initDM, double initTAN, double appRate, int appMeth, double anexposureTime);
///calculate emission during a period (hours)
   double ALFAM_volatilisation(double duration);

	int GetALFAMApplicCode(int OpCode); ///<Get type of application method used
   bool GetIsfinished();	///< returns true if area has finished emiting

};

#endif

