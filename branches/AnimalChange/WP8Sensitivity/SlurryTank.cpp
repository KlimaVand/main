/*
 * SlurryTank.cpp
 *
 *  Created on: Nov 22, 2011
 *      Author: JOVE
 */

#include "SlurryTank.h"
#include "commonData.h"
#include <math.h>
SlurryTank::SlurryTank() {
	Water=0;
	Ash=0;
	Tan=0;
	orgN=0;
	N_Inert=0;
	NFast=0;
	C_Inert=0;
//	CRP=0;
	CSlow=0;
//	CRL=0;
	CVFA=0.0;
	CFast=0;
	CLignin=0.0;
//	CStarch=0.0;
	HFast=0;
	HSlow=0;
	OFast=0;
	OSlow=0;
	SFast=0;
	S_S04=0;
	S2_S=0.0;

	//const
	E=2.718281828459045;
	RA=1000.0;
	//from Files
	commonData slurryTank;
	slurryTank.readFile("slurryTank.txt");

	slurryTank.FindItem("ThetaA",&ThetaA);
	slurryTank.FindItem("ThetaB",&ThetaB);
	slurryTank.FindItem("ThetaC",&ThetaC);
	slurryTank.FindItem("ThetaAM",&ThetaAM);
	slurryTank.FindItem("ThetaBM",&ThetaBM);
	slurryTank.FindItem("ThetaCM",&ThetaCM);
	slurryTank.FindItem("temperatureInKelvin",&temperatureInKelvin);
	slurryTank.FindItem("GInert",&GInert);
	slurryTank.FindItem("b",&b);
	slurryTank.FindItem("surfaceArea",&surfaceArea);

	//gas
	CCH4S=0;
	CCO2_S=0;
	CGas=0;
	ENH3=0;

	HInert=0;
	OInert=0;
	HVFA = 0.0;
	OVFA=0.0;
	//gas
	C_CO2M=0;
	CCH4=0;

	//water
	CrustHoldingCap=0;
	CH4EM=0;
	NN2O=0;

	//from File

	slurryTank.FindItem("k1",&k1);
	slurryTank.FindItem("k2",&k2);
	slurryTank.FindItem("k3",&k3);
	slurryTank.FindItem("k4",&k4);
	slurryTank.FindItem("CrustTickness",&CrustTickness);
	slurryTank.FindItem("CrustCapasitet",&CrustCapasitet);
	slurryTank.FindItem("WaterWick",&WaterWick);
	slurryTank.FindItem("WaterRem",&WaterRem);
	slurryTank.FindItem("WaterDrained",&WaterDrained);
	slurryTank.FindItem("WaterCrust",&WaterCrust);
	slurryTank.FindItem("WaterPresibitation",&WaterPresibitation);
	slurryTank.FindItem("WaterEvap",&WaterEvap);


	slurryTank.FindItem("Epot",&Epot);

	slurryTank.FindItem("Epsalon1",&Epsalon1);
	slurryTank.FindItem("Epsalon2",&Epsalon2);
	slurryTank.FindItem("Epsalon3",&Epsalon3);
	slurryTank.FindItem("Epsalon4",&Epsalon4);
	slurryTank.FindItem("S",&S);
	slurryTank.FindItem("Ycrust",&Ycrust);
	slurryTank.FindItem("CCH4",&CCH4);
	slurryTank.FindItem("n",&n);
	slurryTank.FindItem("depth",&depth);
	capacity = surfaceArea * depth;
	totalCinput=0;
	totalLigninCinput=0;
	totalFastCinput=0;
	totalSlowCinput=0;
	totalVFACinput=0;
}

SlurryTank::~SlurryTank() {
}

void SlurryTank::openOutputFile()
{
	DailyOutput.open(outputFileName,fstream::out);
		DailyOutput <<"Day" <<'\t'<< "CLignin" <<'\t' << "C_Inert" <<'\t' << "CSlow" << '\t' << "CFast" << '\t' << "CVFA" << '\t' 
			<< "NFast" << '\t' << "Tan" << '\t' << "ENH3" << '\t' << "N_Inert" << '\t' 
			<< "C_CO2M" << '\t' << "C_CH4M" << '\t' << "CCH4S" << '\t' << "CCO2_S" << '\t'
			<< "SFast" << '\t' << "S_S04" << '\t' << "S2_S" << endl;
		DailyOutput <<"-" <<'\t'<< "kgC" <<'\t' << "kgC" <<'\t' << "kgC" << '\t' << "kgC" << '\t' << "kgC" << '\t' 
			<< "kgN" << '\t' << "kgN" << '\t' << "kgN/day" << '\t' << "kgN" << '\t' 
			<< "kgC/day" << '\t' << "kgC/day" << '\t' << "kgC/day" << '\t' << "kgC/day" << '\t'
			<< "kgS" << '\t' << "kgS"<< '\t' << "kgS" << endl;
		DailyOutput.precision(10);
		DailyOutput << "0" <<'\t'<< CLignin <<'\t' << C_Inert <<'\t' << CSlow << '\t' << CFast << '\t' << CVFA << '\t' 
			<< NFast << '\t' << Tan << '\t' << "0" << '\t' << N_Inert << '\t' 
			<< "0" << '\t' << "0"<< '\t' << "0"<< '\t' << "0" << '\t' << SFast << '\t' << S_S04 << '\t' << S2_S << endl;
}

void SlurryTank::addSlurry(Slurry input)
{
	slurrypH=input.getpH();
	double DM = input.getAmount() * input.getDM();
	Ash+=DM *input.getAsh(); // formel 1.2 AshContent  //kg
	double TanAdded=input.getAmount()*input.getTan(); //1.3  // kg
	Tan+=TanAdded;
	double RP = DM * input.getRP();
	//org N is obtained by dividing CP by 6.25
	double orgNAdded=RP/6.25; // 1.5	//kg
	orgN+=orgNAdded; //kg
	//Partition org N between inert and Fast pools
	double NinertAdded=input.getfInert()*orgNAdded;//1.4 //kg //fInert should be between 0 and 1
	N_Inert+=NinertAdded; 
	double NFastAdded=(1-input.getfInert())*orgNAdded; //1.5 /kg
	NFast+=NFastAdded;
	//! calculate the carbon content of the inert pool
	double C_InertAdded =10*input.getfInert()*orgNAdded;
	C_Inert+=C_InertAdded; //1.6 //kg
	totalCinput+=C_InertAdded;
	HInert+=0.055*C_InertAdded;//1.34
	OInert+=0.444*C_InertAdded;// 1.35
	//! Calculate the carbon content of the raw protein
	double C_CPAdded=4.28*NFastAdded;//1.6 //kg
	totalCinput+=C_CPAdded;
	//! Calculate the carbon added in lignin
	double C_ligninAdded=DM *0.55*input.getADL();
	totalCinput+=C_ligninAdded;
	CLignin+=C_ligninAdded; //1.8 //kg
	totalLigninCinput+=C_ligninAdded;
	//! Calculate the carbon content of the slow pool
	double C_slowAdded=DM *0.44*(input.getNDF()-input.getADL());
	totalCinput+=C_slowAdded;
	totalSlowCinput+=C_slowAdded;
	CSlow+=C_slowAdded; //1.9 //kg
	//! Calculate the carbon content of the raw lipid
	double C_lipidAdded=DM*0.77*input.getRL();//1.10 //kg
	totalCinput+=C_lipidAdded;
	//! calculate the carbon content of the volatile fatty acids
	double C_VFAadded=DM *0.4*input.getVFA(); //1.11 //kg
	totalCinput+=C_VFAadded;
	totalVFACinput+=C_VFAadded;
	CVFA+=C_VFAadded;
	HVFA+=DM *0.167 * input.getVFA();//1.16 // kg
	OVFA+=DM *0.889 * input.getVFA();//1.19 // kg
	//! Calculate the carbon content of the starch and sugar
	double C_StarchAdded=DM *0.44*input.getRem();//1.12 //kg
	totalCinput+=C_StarchAdded;
	//! Calculate the carbon content of the Fast pool
	CFast+=C_CPAdded+C_StarchAdded+C_lipidAdded; //1.13
	totalFastCinput+=C_CPAdded+C_StarchAdded+C_lipidAdded;
	//! Calculate the hydrogen in the Fast and Slow pools
	HFast=0.117*C_CPAdded+0.152*C_lipidAdded+0.139*C_StarchAdded;//1.14
	HSlow=0.139*CSlow;//1.15
	//! Calculate the oxygen in the Fast and Slow
	OFast=0.533*C_CPAdded+0.14*C_lipidAdded+1.111*C_StarchAdded;//1.17
	OSlow=1.111*CSlow;//1.18
	//! Calculate the sulphur in the Fast and Sulphate pools
	SFast+=input.getAmount()*(input.getTotalS()-(input.getSulphateS()+input.getSulphideS()));//1.20
	S_S04+=input.getAmount()*input.getSulphateS();//1.21
	S2_S+=input.getAmount()*input.getSulphideS(); // 1.22
	Water+=input.getAmount()*(1-input.getDM()); // 1.1 //kg
}

void SlurryTank::endSimulation()
{
	if (outputFileName.length()>0)
		DailyOutput.close();
	Water=0;
	Ash=0;
	Tan=0;
	orgN=0;
	N_Inert=0;
	NFast=0;
	C_Inert=0;
//	CRP=0;
	CSlow=0;
//	CRL=0;
	CVFA=0.0;
	CFast=0;
	CLignin=0.0;
//	CStarch=0.0;
	HFast=0;
	HSlow=0;
	OFast=0;
	OSlow=0;
	SFast=0;
	S_S04=0;
	S2_S=0.0;
	CCH4S=0;
	CCO2_S=0;
	CGas=0;
	ENH3=0;

	HInert=0;
	OInert=0;
	HVFA = 0.0;
	OVFA=0.0;
	//gas
	C_CO2M=0;
	CCH4=0;

	//water
	CH4EM=0;
	NN2O=0;
};


void SlurryTank::dailyUpdate(int dayNo)
{
	double temperatureInCelsius = temperatureInKelvin - 273.15;
	//! Calculate the normalised temperature effect
	double FTheta=pow(E,ThetaA+ThetaB*temperatureInCelsius *(1-0.5*(temperatureInCelsius /ThetaC)));  //1.26
	double FpH=1.0; //need to implement this
	//! Calculate the degradation rates of the Fast and Slow pools
	double k1act=FpH*FTheta*k1;//1.25 //should be 0 to 1
	double k2act=FpH*FTheta*k2;//1.25

	double hydrolysedCpool =k1act*CSlow+k2act*CFast;
	double hydrolysedHpool =k1act*HSlow+k2act*HFast;
	double hydrolysedOpool =k1act*OSlow+k2act*OFast;

	double FS=pow(E,-b*(S_S04/Water));	//1.31
	double FThetaM=pow(E,ThetaAM+ThetaBM*temperatureInCelsius*(1-0.5*(temperatureInCelsius/ThetaCM)));
	double k3act=FThetaM*FpH*FS*k3; //1.32
	double k4act=FTheta*FpH*(1-FS)*k4; //1.33

	//calculate C in CH4S. This will be instantaneously lost as CH4-C
	CCH4S=0.375*k2act*SFast; //1.28
	//calculate the H in CH4S
	double HCH4S=0.333*CCH4S;//1.39

	CGas=k3act*CVFA;//1.35	//anaerobic degradation of VFA by methanogens

	double Hgas=k3act * HVFA; //1.44
	double Ogas=k3act * OVFA;//1.45
	C_CH4M=12*(CGas/24+Hgas/8-Ogas/64); //1.46

	CCO2_S=k4act*CVFA; //1.34  //CO2-C from oxidation by SO4
//calculate the H utilised during oxidation of VFA by SO4. Oxygen is not budgetted here.
	double HSO4=0.167*CCO2_S;//1.37

	C_CO2M=CGas-C_CH4M;//1.47	
	CCH4=CCH4S+C_CH4M; //1.48	

	double KN=pow(10,-0.09018-(2729.92/temperatureInKelvin)); //1.53
	double KH=pow(10,-1.69+1447.7/temperatureInKelvin); //1.55

	ENH3=24 * 60 * 60 * surfaceArea * Tan/(Water*KH*(1+pow(10,-slurrypH)/KN)*RA); //1.57

//update C state variables
	//! Update the carbon in the Inert pool
	//GInert=0.0;
	C_Inert+=GInert*hydrolysedCpool; //1.27

	//! Update the values of the carbon in the Fast and Slow pools is
	CSlow*=(1-k1act); //1.24
	CFast*=(1-k2act); //1.24 
	double CAddVFA=(1-GInert)*hydrolysedCpool-CCH4S;//1.29
	if (CAddVFA<0.0)
	{
		if (outputFileName.length()>0)
		DailyOutput << "Negative VFA addition" << endl;
		exit(0);
	}
	CVFA=(1-(k3act+k4act))*CVFA+CAddVFA; //1.30

	//update the H and O
	HFast*=(1-k2act);//1.36
	HSlow*=(1-k1act);//1.36
	OFast*=(1-k2act);//1.36
	OSlow*=(1-k1act);//1.36
	double HAddInert=0.055*GInert*hydrolysedHpool;
	HInert+=HAddInert; //1.37
	double OAddInert=0.044*GInert*hydrolysedOpool;
	OInert+=OAddInert;//1.38
	HVFA+=hydrolysedHpool-(HAddInert+HCH4S+HSO4+Hgas); //1.42
	OVFA+=hydrolysedOpool-(OAddInert+Ogas);//1.43

	//update S in Fast	
	SFast*=(1-k2act);//1.59
	S_S04-=2.667 * CCO2_S; //1.60
	double SAddS2 = 2.667 * (CCO2_S + CCH4S);
	S2_S+=SAddS2; //1.61

	//update the N
	double NAddInert=0.1*GInert*hydrolysedCpool;
	N_Inert+=NAddInert;//1.51
	Tan+=k2act*NFast-(NAddInert+ENH3);// 1.52
	NFast*=(1-k2act); //1.50

	if (Tan<0.0)
	{
		if (outputFileName.length()>0)
			DailyOutput << "Not enough TAN to enable the inert to be created or too much volatilisation" << endl;
		exit(0);
	}
	

	if (outputFileName.length()>0)
		DailyOutput << dayNo <<'\t'<< CLignin <<'\t' << C_Inert <<'\t' << CSlow << '\t' << CFast << '\t' << CVFA << '\t' 
			<< NFast << '\t' << Tan << '\t' << ENH3 << '\t' << N_Inert << '\t' 
			<< C_CO2M << '\t' << C_CH4M<< '\t' << CCH4S << '\t'  << CCO2_S << '\t'  
			<< SFast << '\t' << S_S04 << '\t' << S2_S << endl;

	/*
	CrustHoldingCap=CrustTickness*CrustCapasitet;//1.54
	double WaterMeth=18*(CGas/12+Hgas/4+Ogas/32); //1.52
	Water=Water+WaterDrained-(WaterMeth+WaterRem+WaterWick); //1.53

	WaterCrust=WaterCrust+WaterPresibitation-WaterDrained-WaterEvap; //1.55
	if((WaterCrust+WaterPresibitation-Epot)<=0) //1.56
	{
		WaterEvap=WaterPresibitation+WaterCrust;
		WaterCrust=WaterDrained=0;

		WaterWick=Epot-WaterEvap;
	}
	else if((WaterCrust+WaterPresibitation-Epot)>CrustHoldingCap) //1.57
	{

		WaterDrained=WaterCrust+WaterPresibitation-Epot;
		WaterEvap=Epot;
		WaterWick=0;
		WaterCrust=CrustHoldingCap;
	}
	else//1.5.8
	{
		WaterEvap=Epot;
		WaterDrained=WaterWick=0;
		WaterCrust=WaterCrust+WaterPresibitation-WaterEvap;
	}

	double FW=WaterCrust/CrustHoldingCap; //1.60

	CH4EM+=n*(1-FW)*FTheta*CCH4; //1.59
	double Finnoc=Epsalon1*pow(E,Epsalon2*pow(E,Epsalon3*(1-Epsalon4)));//1.62

	NN2O+=S*Ycrust*(1-FW)*FTheta*Finnoc;//1.61
	*/
	NN2O=0.0;
//	CH4EM=CCH4;
	//DailyOutput<<'\n';
}
