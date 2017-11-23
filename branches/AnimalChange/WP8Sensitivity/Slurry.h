/*
 * Slurry.h
 *
 *  Created on: Nov 22, 2011
 *      Author: JOVE
 */

#ifndef SLURRY_H_
#define SLURRY_H_

class Slurry {
private:
	//! volume of slurry (cubic metres)
	double amount;
	//! Concentration of dry matter in the slurry (kilograms per kilogram fresh weight)
	double DM;
	//! Concentrationof ash in the tank (kilograms per kilogram DM)
	double Ash;
	//! Concentrationof total ammoniacal nitrogen in the tank (kilograms per kilogram fresh weight)
	double Tan;
	//! Concentration of raw protein in the tank (kilograms per kilogram DM)
	double RP;
	//! Fraction of the organic nitrogen that is inert under anaerobic conditions
	double fInert;
	//! Concentration of acid detergent fibre (kilograms per kilogram DM)
	double ADL;
	//! Concentration of Neutral detergent fibre  (kilograms per kilogram DM)
	double NDF;
	//! Concentration of Raw lipid (kilograms per kilogram DM)
	double RL;
	//! Concentration of Volatile fatty acids (kilograms per kilogram DM)
	double VFA;
	//! Concentration of Remaining organic matter (kilograms per kilogram DM)
	double Rem;
	//! Concentration of Total sulphur (kilograms per kilogram DM)
	double TotalS;
	//! Concentration of Sulphate sulphur (kilograms per kilogram DM)
	double SulphateS;
	//! Concentration of Sulphide sulphur (kilograms per kilogram DM)
	double SulphideS;
	//! pH of the slurry
	double pH;
	//! urine N (kilograms per kilogram fresh weight)
	double urineN;
public:
	Slurry();
	virtual ~Slurry();
	double getAmount(){return amount;};
	double getDM(){return DM;};
	double getAsh(){return Ash;};
	double getTan(){return Tan;};
	double getRP(){return RP;};
	double getfInert(){return fInert;};
	double getADL(){return ADL;};
	double getNDF(){return NDF;};
	double getRL(){return RL;};
	double getVFA(){return VFA;};
	double getRem(){return Rem;};
	double getTotalS(){return TotalS;};
	double getSulphateS(){return SulphateS;};
	double getSulphideS(){return SulphideS;};
	double getpH(){return pH;};
	double getOM(){return (DM-Ash/amount);}
	double geturineN(){return urineN;}

	void setAmount(double aamount){ amount=aamount;};
	void setDM(double aDM){ DM = aDM;};
	void setAsh(double aAsh){ Ash=aAsh;};
	void setTan(double aTan){ Tan=aTan;};
	void setRP(double aRP){ RP=aRP;};
	void setfInert(double afInert){ fInert=afInert;};
	void setADL(double aADL){ ADL=aADL;};
	void setNDF(double aNDF){ NDF=aNDF;};
	void setRL(double aRL){ RL=aRL;};
	void setVFA(double aVFA){ VFA=aVFA;};
	void setRem(double aRem){ Rem=aRem;};
	void setTotalS(double aTotalS){ TotalS=aTotalS;};
	void setSulphateS(double aSulphateS){ SulphateS=aSulphateS;};
	void setSulphideS(double aSulphideS){ SulphideS=aSulphideS;};
	void setpH(double apH){ pH=apH;};
	void seturineN(double aurineN){urineN=aurineN;}
	void calcRem();
};

#endif /* SLURRY_H_ */
