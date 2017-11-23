// WP8Sensitivity.cpp : main project file.

#include <fstream>
#include "Slurry.h"
#include "SlurryTank.h"
#include "linkList.h"
#include "fileAccess.h"
//#include <direct.h>
using namespace std;

linkList <Slurry> theList;

int GetData(string dataFilename)
{
	fileAccess afileAccess;
	afileAccess.openFile(dataFilename);
	string astring;
	astring=afileAccess.getLineFile();
	cout << astring << endl;
	astring=afileAccess.getLineFile();
	cout << astring << endl;
	while (!afileAccess.endOfFile())
	{
		int anint;
		double adouble;
		int dietNo=0;
		fstream * filePtr=	afileAccess.getFilePointer();
//		*filePtr >> anint >> anint >> anint >> dietNo;
		*filePtr >> anint >> anint >> adouble;
		*filePtr>>dietNo;
		if (dietNo==0)
			break;
		cout << "Reading diet no " << dietNo << endl;
		for (int i=0; i<8; i++)
			*filePtr>>adouble;
		Slurry *aslurry = new Slurry();
		double amount;//mass in kg/d
		*filePtr >> amount;
		aslurry->setAmount(amount);
		double water ;//kg/d
		*filePtr >> water;
		double DM = amount-water;//DM is in kg/d
		aslurry->setDM(DM/amount); //convert to kg/litre for slurry
		double TAN;
		*filePtr >> TAN;
		TAN/=amount;//convert to kg/litre
		aslurry->setTan(TAN);
		double dumdouble;
		*filePtr >> dumdouble;
		double Ash;
		*filePtr >> Ash;
		double totS;
		*filePtr >> totS;
		totS/=amount;
		aslurry->setSulphateS(0.65*totS);
		aslurry->setTotalS(totS);
		double NDF;
		*filePtr >> NDF;
		double ADL;
		*filePtr >> ADL;
		double VFA;
		*filePtr >> VFA;
		double RL;
		*filePtr >> RL;
		for (int i=0; i<11; i++)
			*filePtr>>adouble;
		double faecalN;//in g/d
		*filePtr >> faecalN;
		double RP=6.25 * faecalN/1000;//convert from N to RP and from g to kg
		double urineN;//in gN/d
		*filePtr >> urineN;
		RP/=DM;//convert to kg/kg DM
		Ash/=DM;//convert to kg/kg DM
		NDF/=DM;//convert to kg/kg DM
		VFA/=DM;//convert to kg/kg DM
		ADL/=DM;//convert to kg/kg DM
		RL/=DM;//convert to kg/kg DM
		urineN/=amount * 1000;//convert to kg/litre
		aslurry->setADL(ADL);
		aslurry->setVFA(VFA);
		aslurry->setNDF(NDF);
		aslurry->setRP(RP);
		aslurry->setRL(RL);
		aslurry->setAsh(Ash);
		double housingNH3_N=0.17*(aslurry->getTan()+urineN);
		aslurry->setTan(aslurry->getTan()+urineN-housingNH3_N);
	
		astring=afileAccess.getLineFile();
	
		theList.InsertLast(aslurry);
		aslurry->calcRem();
	}
	afileAccess.closeFile();
	return 0;
}

int main() 
{
	bool batch=false;
	fileAccess systemFile = fileAccess();

	cout << "Looking for system.dat in directory " << systemFile.getCurrentPath() << endl;
	systemFile.openFile("system.dat",false);
	string inputDir = systemFile.getLineFile();
	systemFile.changeDir(inputDir);
	cout << "Reading data from directory " << systemFile.getCurrentPath() << endl;
	string dataFilename = "excreta8.txt"; //systemFile.getLineFile();
	systemFile.closeFile();
	GetData(dataFilename);
	Slurry *theSlurry;
	int daysInStorage=180;
	fstream *summaryFile = new fstream("summary.txt",fstream::out);
	*summaryFile << "Diet" << "\t" << "stockNo" << "\t" << "totalVS" << "\t" << "totalCInput" << "\t" << "C_CH4M" << "\t" << "CCH4S" << "\t" << "C_CO2M" << "\t" 
			<< "CCO2_S" << "\t" << "ENH3" << "\t" << "NN2O" << "\t" << "totalLigninCinput" << "\t" << "totalFastCinput"
			<< "\t" << "totalSlowCinput" << "\t" << "totalVFACinput" << "\t" << endl;
	for (int j=0; j<theList.NumOfNodes(); j++)
	{
		double C_CH4M=0.0; 
		double CCH4S=0.0;
		double C_CO2M=0.0;
		double CCO2_S=0.0;
		double ENH3=0.0;
		double NN2O=0.0;
		if (j==5)
			daysInStorage=7;
		SlurryTank tank;
		string s1; 
		s1=(char) (48+j);
		tank.setOutputFile("outfile" + s1 + ".txt");
		tank.openOutputFile();
		theSlurry =theList.ElementAtNumber(j);
		double stockNo=0;
		double mass = theSlurry->getAmount();
		if (!batch)
		{
			stockNo=100;
			theSlurry->setAmount(theSlurry->getAmount()*stockNo);
		}
		else
		{
			stockNo = tank.getCapacity()*1000.0/mass;
			theSlurry->setAmount(tank.getCapacity()*1000.0);
		}
		if (batch)
			tank.addSlurry(*theSlurry);
		for(int m=0;m<daysInStorage;m++)
		{
			if (!batch)
				tank.addSlurry(*theSlurry);
			tank.dailyUpdate(m+1);
			C_CH4M+=tank.getC_CH4M();
			CCH4S+=tank.getCCH4S();
			C_CO2M+=tank.getC_CO2M();
			CCO2_S+=tank.getCCO2_S(); 
			ENH3+=tank.getENH3();
			NN2O+=tank.getNN2O();
			if (m==daysInStorage-1)
				cout << "";
		}
		double totalCInput = tank.gettotalCinput();
		cout << j << " " << (0.46*theSlurry->getAmount()*theSlurry->getOM()) << " " << totalCInput << " " << totalCInput/daysInStorage << endl;
		*summaryFile << j << "\t" << stockNo << "\t" << theSlurry->getAmount()*theSlurry->getOM()<< "\t" << totalCInput << "\t" << C_CH4M << "\t" << CCH4S << "\t" << C_CO2M << "\t" 
			<< CCO2_S << "\t" << ENH3 << "\t" << NN2O << "\t" << tank.gettotalLigninCinput() << "\t" << tank.gettotalFastCinput()
			<< "\t" << tank.gettotalSlowCinput()<< "\t" << tank.gettotalVFACinput() << "\t" << endl;
		tank.endSimulation();
	}
	summaryFile->close();
	return 0;
}
