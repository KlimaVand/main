#ifndef Slurry_pH_H
   #define Slurry_pH_H
#include <math.h>
class Slurry_pH
{
	double pH;

//masses in mols
	double Na;
	double K;
	double Ca;
	double Mg;
	
	double TAN;
	
	double Cl;
	double TIC;
	double TS;
	double TAc;
	double SO4;
	//these masses are in kg per tonne
	double H2O;
	double DM;
	//concentrations in moles per litre

	double Na_conc;
	double K_conc;
	double Ca_conc;
	double Mg_conc;

	double TAN_conc;
	double Cl_conc;
	double TIC_conc;
	double TS_conc;
	double TAc_conc;
	double SO4_conc;
	
	double AirH2S;
	double AirCO2;
	double AirNH3;	
	double AirHAc;
	double AirH2O;
	
	double NH3_aq_conc;
	double CO2_aq_conc;
	double H2S_aq_conc;
	double HAc_aq_conc;

	double temperature;

	double aerodynamicResistance;//massTransCoeff;
	double surfaceArea;
public:
	Slurry_pH();
	void setpH(double aVal);
	//!dissociation constants

	double KNH2S(double temper){return pow(10,(-3448.7/temper)+47.479-(7.5227*log(temper)));}
	double KNCO2(double temper){return exp((-80063.5/temper)+(0.714984*temper)-(478.653*log(temper))+2767.92);}
	double KNHCO3(double temper){return  exp((-6286.89/temper)-0.050628*temper+12.405);}
	double KNNH3(double temper){return exp((-1843.22/temper)-(0.0544943*temper)+(31.4335*log(temper))-177.95292);}
	double KNHAc(double temper){return exp(-406.6*(3+exp(temper/184.6)/temper));}
	double KNH2O(double temper){return exp((-10294.83/temper)-0.039282*temper+14.01708);}
	//! Henry's constants
	double KHH2S(double temper) {return pow(10,(5.703-884.94/temper));}
	double KHHAc(double temper) {return pow(10,(3.65-2596/temper));}
	double KHNH3(double temper) {return exp(160.559-(8621.06/temper)-25.6767*log(temper)+0.035388*temper);}
	double KHCO2(double temper) {return exp(60.538-(5160.4/temper)-7.61*log(temper));}
	double getHionConc();
	void Daily() ;

};

#endif