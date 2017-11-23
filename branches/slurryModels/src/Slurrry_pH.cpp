#include "Slurry_pH.h"
#include "c:\Users\njh\workspace\main\trunk\src\base\nixcbits.h"

Slurry_pH::Slurry_pH()
{
	temperature	=298;

	surfaceArea = 1.0;
	pH=8.0;
	//concentrations in moles per litre
	Na_conc = 0.0009;
	K_conc	=0.0179;
	Ca_conc	= 0.0025;
	Mg_conc	=0.0023;
	
	TAN_conc	=0.0297;
	Cl_conc	=0.0035;
	TIC_conc	=0.0236;
	TS_conc	=0.000278;
	TAc_conc	=0.0088;
	SO4_conc  = 0.24/(10 * 96);

	H2O = 960.0;
	DM = 40.0;

	AirH2S	=0.000000294;
	AirCO2	=0.0143;
	
	AirNH3	=0.000000177;
	AirHAc	=0.000000683;
	AirH2O	=1.0217E-58;
	
	aerodynamicResistance= 60.0;	//s per metre

	Na=Na_conc * H2O;
	K=K_conc* H2O;
	Ca=Ca_conc* H2O;
	Mg=Mg_conc* H2O;
	TAN=TAN_conc* H2O;
	Cl=Cl_conc	* H2O;
	TIC=TIC_conc	* H2O;
	TS=TS_conc	* H2O;
	TAc=TAc_conc* H2O;
	SO4=SO4_conc * H2O;
	NH3_aq_conc = TAN_conc/(1+pow(10,-pH)/KHNH3(temperature));
	CO2_aq_conc= TIC_conc/(1+pow(10,-pH)/KHCO2(temperature));
	H2S_aq_conc =TS_conc/(1+pow(10,-pH)/KHH2S(temperature));
	HAc_aq_conc=TAc_conc/(1+pow(10,-pH)/KHHAc(temperature));
};	

double Slurry_pH::getHionConc()
{
	double ret_val = -1.0;
	double cubic_coeff = 1.0 + NH3_aq_conc/KHNH3(temperature);
	double quadratic_coeff = Na_conc + K_conc + 2*(Ca_conc + Mg_conc) - Cl_conc - 2 *SO4_conc;
	double linear_coeff = -(KNH2O(temperature) + CO2_aq_conc *KNHCO3(temperature) + H2S_aq_conc * KNH2S(temperature) + HAc_aq_conc * KNHAc(temperature));
	double constant_coeff = -2.0 *CO2_aq_conc * KNHCO3(temperature) * KNCO2(temperature);
	int num_solutions;
    double solutions[3];
	SolveCubic(cubic_coeff,quadratic_coeff,linear_coeff,constant_coeff,&num_solutions,solutions);
	for (int i=0; i<num_solutions; i++)
	{
		if (solutions[i]>0.0) //find only positive values
		{
			double test_pH = log10(1/solutions[i]);
			if ((test_pH>2.0)&&(test_pH<13.0))	//only accept values within reasonable range
				ret_val=solutions[i];
		}
	}
/*	if (ret_val<=0.0)
	{
		cout << "error";
		exit();
	}*/
	return ret_val;
}

void Slurry_pH::Daily() 
{
	pH=log10(1/getHionConc());
	NH3_aq_conc = TAN_conc/(1+pow(10,-pH)/KHNH3(temperature));
	CO2_aq_conc= TIC_conc/(1+pow(10,-pH)/KHCO2(temperature));
	H2S_aq_conc =TS_conc/(1+pow(10,-pH)/KHH2S(temperature));
	HAc_aq_conc=TAc_conc/(1+pow(10,-pH)/KHHAc(temperature));
	double NH3_volat =3600.0 * surfaceArea * NH3_aq_conc/aerodynamicResistance;
	TAN-=NH3_volat;
	TAN_conc=TAN/H2O;
}

