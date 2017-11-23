/*
 * SlurryTank.h
 *
 *  Created on: Nov 22, 2011
 *      Author: JOVE
 */

#ifndef SLURRYTANK_H_
#define SLURRYTANK_H_
#include "Slurry.h"
#include <fstream>
using namespace std;
class SlurryTank {
private:
	//surface area of tank in square metres
	double surfaceArea;
	//! depth of tank in metres
	double depth;
	//! capacity of tank in cubic meters
	double capacity;
	//! Amount of water in the slurry tank (kilograms). 
	double Water;
	//! Amount of ash in the slurry tank (kilograms).
	double Ash;
	//! Amount of total ammoniacal nitrogen in the slurry tank (kilograms).
	double Tan;
	//! Amount of organic nitrogen in the slurry tank (kilograms)
	double orgN;
	//! Amount of nitrogen in the inert organic matter pool in the slurry tank (kilograms) .
	double N_Inert;
	//! Amount of nitrogen in the fast organic matter pool in the slurry tank (kilograms) . 
	double NFast;
	//! Amount of carbon in the inert organic matter organic matter pool in the slurry tank (kilograms). 
	double C_Inert;
	//! Amount of carbon in crude protein crude protein in the slurry tank (kilograms). 
	//double CRP;
	//! Amount of carbon in the lignin pool in the slurry tank (kilograms) .
	double CLignin;
	//! Amount of carbon in the slow organic matter pool in the slurry tank (kilograms). 
	double CSlow;
	//! Amount of carbon in the raw lipid in the slurry tank (kilograms). 
	//double CRL;
	//! Amount of carbon in the volatile fatty acids in the slurry tank (kilograms).
	double CVFA;
	//! Amount of carbon in the starch in the slurry tank (kilograms). 
	//double CStarch;
	//! Amount of carbon in the fast organic matter pool in the slurry tank (kilograms)
	double CFast;
	//! Amount of hydrogen in the fast organic matter pools in the slurry tank (kilograms). 
	double HFast;
	//! Amount of hydrogen in the slow organic matter pools in the slurry tank (kilograms). 
	double HSlow;
	//! Amount of hydrogen in the VFA pool in the slurry tank (kilograms). 
	double HVFA;
	//! Amount of oxygen in the fast organic matter pools in the slurry tank (kilograms). 
	double OFast;
	//! Amount of oxygen in the slow organic matter pools in the slurry tank (kilograms). 
	double OSlow;
	//! Amount of oxygen in the VFA pool in the slurry tank (kilograms). 
	double OVFA;
	//! Amount of sulphur in the fast organic matter pools in the slurry tank (kilograms). 
	double SFast;
	//! Amount of sulphate sulphur in the slurry tank (kilograms). 
	double S_S04;
	//! Amount of sulphide  sulphur in the slurry tank (kilograms). 
	double S2_S;
	//! slurry pH
	double slurrypH;
	//!total C input to tank
	double totalCinput;
	double totalLigninCinput;
	double totalFastCinput;
	double totalSlowCinput;
	double totalVFACinput;
	//const
	//! real number representation of the natural logarithm
	double E;
	//!is a model input that varies dynamically
	double RA;
	//from Files
	//! ThetaA are constant used in 1.23
	double ThetaA;
	//! ThetaB are constant used in 1.23
	double ThetaB;
	//! ThetaC are constant used in 1.23
	double ThetaC;
	//! ThetaAM are constant used in 1.23/1.29
	double ThetaAM;
	//! ThetaBM are constant used in 1.23/1.29
	double ThetaBM;
	//! ThetaCM are constant used in 1.23/1.29
	double ThetaCM;
	//! the temprattur in the slurry in Kelvin
	double temperatureInKelvin;
	//! is not defined in the text
	double GInert;
	//! is a constant used in 1.28
	double b;

	//gas
	//! the total amount of CO2 produced by methanogenics (kg) defined in 1.42
	double C_CO2M;
	//! the total amount of CH4 produced (kg)  defined in 1.43
	double CCH4;
	//! the amount of CH4 C produced by CH3SH degradation. defined at 1.25 (kg)
	double CCH4S;
	//! the amount of CO2 C produced by SO4 oxidation. defined at 1.31 (kg)
	double CCO2_S;
	//! the amount of biogas C produced. defined at 1.32 (kg)
	double CGas;
	//! amount of CH4 C produced by methanogenics (kg)
	double C_CH4M;

	//! the amount of CNH3 produced. Combining Equations 1.47 to 1.51 (kg)
	double ENH3;

	// other
	//! K value for slow organic matter pool
	double k1;
	//! K value for fast organic matter pool
	double k2;
	//! k value used for 1.32
	double k3;
	//! k value used for 1.31
	double k4;
	//! the hydrogen inert pool as defined in 1.34
	double HInert;
	//! the oxygen inert pool as defined in 1.35
	double OInert;

	//water
	// the Water Holding capacity (dimensionless)
	double CrustHoldingCap;
	//! total amount of CH4EM (kg). Defined in 1.61
	double CH4EM;
	//! total amount of CH4EM (kg). Defined in 1.60
	double NN2O;

	//from File
	//! the Tickness of the crust of the slurry
	double CrustTickness;
	//! the amount of water in the crust
	double CrustCapasitet;
	//! Water removed because of removel of slurry (kg pr day)
	double WaterWick;
	//! removel of water because of removel of slurry (kg pr day)
	double WaterRem;
	//! the water that is drained. first used in 1.55
	double WaterDrained;
	//! the water in the crust. Defined in 1.55
	double WaterCrust;
	//! water precipitation.
	double WaterPresibitation;
	//! water evaporation.
	double WaterEvap;
	//! weather information
	double Epot;
	//! constant used in 1.62
	double Epsalon1;
	//! constant used in 1.62
	double Epsalon2;
	//! constant used in 1.62
	double Epsalon3;
	//! constant used in 1.62
	double Epsalon4;
	//! constant used in 1.60
	double S;
	//! Ycrust is constant based upon crust type that is used in 1.60
	double Ycrust;
	//! constant used in 1.59
	double n;
	//! Daily output
	fstream DailyOutput;
	//! name of output file
	string outputFileName;

public:
	SlurryTank();
	double getCapacity(){return capacity;}

	//! return the total amount of CO2 produced by methanogenics (kg) defined in 1.42
	double getC_CO2M(){return C_CO2M;};
	//! return the total amount of CH4 produced (kg)  defined in 1.43
	double getCCH4(){return CCH4;};
	//! return the amount of CH4 C produced by CH3SH degradation. defined at 1.25 (kg)
	double getCCH4S(){return CCH4S;};
	//! return the amount of CO2 C produced by SO4 oxidation. defined at 1.31 (kg)
	double getCCO2_S(){return CCO2_S;};
	//! return the amount of biogas C produced. defined at 1.32 (kg)
	double getC_CH4M(){return C_CH4M;};
	//! return the amount of NH3 emitted. Combining Equations 1.47 to 1.51 (kg)
	double getENH3(){return ENH3;};

	double getNN2O(){return NN2O;}

	double getTotalC(){return (CFast + CSlow + CLignin + C_Inert + CVFA);}

	//double getTotalOM(){return (DM-Ash);}
	void setOutputFile(string afilename) {outputFileName=afilename;}
	void openOutputFile();
	void setCapacity(double volume){capacity=volume;}
	void addSlurry(Slurry input);
	void dailyUpdate(int dayNo);
	void endSimulation();
	virtual ~SlurryTank();
	double gettotalCinput(){return totalCinput;}
	double gettotalLigninCinput() {return totalLigninCinput;}
	double gettotalFastCinput(){return totalFastCinput;}
	double gettotalSlowCinput(){return totalSlowCinput;}
	double gettotalVFACinput(){return totalVFACinput;}
};

#endif /* SLURRYTANK_H_ */
