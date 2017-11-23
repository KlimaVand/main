

#include <iostream>
#include <math.h>
#include "tools\commonData.h"
#include "base\settings.h"

using namespace std;
settings * globalSettings;
message * theMessage;

struct layer {
	double FOMC14;
	double FOMC13;
	double HUMC14;
	double HUMC13;
	double ROMC14;
	double ROMC13;
} ;
double CalcDampingDepth(double k,double rho)
{
   return sqrt(2.0*k/rho);
}
/****************************************************************************\
OBS: depth in m
\****************************************************************************/
double Temperature(double avgTemperature,int day,double depth,double amplitude,double offset)
{
	 double rho=3.1415926*2.0/(365.0*24.0*3600.0);
	 double Th_diff=0.35E-6;
	double dampingDepth=CalcDampingDepth(Th_diff,rho);
   double retVal=avgTemperature+amplitude*exp(-depth/dampingDepth)*sin(rho*(day+offset)*24.0*3600.0-depth/dampingDepth);
   //need outpiut of reval
 
   return retVal;
}


double temperatureCoefficent(double temperature)
{
	double Coefficent=7.24*exp(-3.432+0.168*temperature*(1-0.5*temperature/36.9)); //2
	return Coefficent;
}
double R(double Clayfraction)
{
	double Coefficent=1.67*(1.85+1.6*exp(-7.86*Clayfraction)); //4
	return Coefficent;
}

int sumulation()
{
	double input,avgTemperature,Clayfraction,FOMdecompositionrate,HUMdecompositionrate,ROMdecompositionrate,tF,HumificationCoefficient,HUMfraction,ROMfraction,HUMfCO2,RUMfCO2,RomificationCoefficient;
	double amplitude,offset,depth;
	int NumberOfMonths;
	ROMfraction=0.0;
	HUMfraction=0.0;
	//input
	fileAccess hd;
	hd.changeDir(globalSettings->inputDir);

	commonData inputInformation;

	inputInformation.readFile("input.txt");
	inputInformation.setCritical(true);
	cout<<globalSettings->inputDir<<endl;
	inputInformation.FindSection("Parameters");
	inputInformation.FindItem("NumberOfMonths",&NumberOfMonths);

	inputInformation.FindItem("amount",&input);
	inputInformation.FindItem("temperature",&avgTemperature);
	inputInformation.FindItem("amplitude",&amplitude);
	inputInformation.FindItem("offset",&offset);
	inputInformation.FindItem("depth",&depth);

	inputInformation.FindSection("FOM");
	inputInformation.FindItem("HumificationCoefficient",&HumificationCoefficient);
	inputInformation.FindItem("FOMdecompositionrate",&FOMdecompositionrate);
	inputInformation.FindItem("clayfraction",&Clayfraction);
	inputInformation.FindItem("tF",&tF);
	inputInformation.FindSection("HUM");
	inputInformation.FindItem("HUMdecompositionrate",&HUMdecompositionrate);
	inputInformation.FindItem("HUMfraction",&HUMfraction);
	inputInformation.FindSection("ROM");
	inputInformation.FindItem("ROMdecompositionrate",&ROMdecompositionrate);
	inputInformation.FindItem("ROMfraction",&ROMfraction);
	


	//constants
	const int numberofLayers=100;	
	layer allLayers[numberofLayers];

	//init
	fileAccess initC;
	initC.openFile("CContent1.txt");
	initC.getLineFile();
	convert conv;
	for(int j=0;j<numberofLayers;j++)
	{
		string line=initC.getLineFile();
		allLayers[j].FOMC14=conv.StringToDouble(conv.partOfAString(line,2));
		allLayers[j].HUMC14=conv.StringToDouble(conv.partOfAString(line,3));
		allLayers[j].ROMC14=conv.StringToDouble(conv.partOfAString(line,4));
		allLayers[j].FOMC13=conv.StringToDouble(conv.partOfAString(line,5));
		allLayers[j].HUMC13=conv.StringToDouble(conv.partOfAString(line,6));
		allLayers[j].ROMC13=conv.StringToDouble(conv.partOfAString(line,7));
	}
	initC.closeFile();
	//open output files
	hd.changeDir(globalSettings->outputDir);
	ofstream transportFOM;
	transportFOM.open ("transportFOM.xls");
	//write header

	for (int j=0; j<numberofLayers;j++)
	{

		transportFOM << j<< " FOMC13" <<'\t';
	}
	transportFOM<<endl;
	ofstream transportHUM;
	transportHUM.open ("transportHUM.xls");
	ofstream transportROM;
	transportROM.open ("transportROM.xls");
	ofstream CO2FOM;
	CO2FOM.open ("CO2FOM.xls");
	ofstream CO2HUM;
	CO2HUM.open ("CO2HUM.xls");
	ofstream CO2ROM;
	CO2ROM.open ("CO2ROM.xls");
	ofstream totalC02;
	totalC02.open ("totalC02.xls");
	ofstream amount;
	amount.open ("amount.xls");
	fileAccess CarbonInput;
	CarbonInput.openFile("CarbonInput.txt");
	CarbonInput.getLineFile();
	CarbonInput.getLineFile();
	//loop through months
	int i=0;
	while(i<NumberOfMonths)
	{
		double total13CO2=0;
		double total14CO2=0;
		/*string tmp=CarbonInput.getLineFile();
		double yield=conv.StringToDouble(conv.partOfAString(tmp,3));
		double pCrop=conv.StringToDouble(conv.partOfAString(tmp,4));
		double ResidueReturn =conv.StringToDouble(conv.partOfAString(tmp,5));
		double ManureC =conv.StringToDouble(conv.partOfAString(tmp,6));
		input=yield*(pCrop+ResidueReturn)+ManureC; */
		//input
		allLayers[0].FOMC14=allLayers[0].FOMC14+input*(1-HUMfraction);
	

		allLayers[0].FOMC13=allLayers[0].FOMC13+input*(1-HUMfraction);
			
		
		//allLayers[0].HUMC14=allLayers[0].HUMC14+input*HUMfraction;
		//allLayers[0].HUMC13=allLayers[0].HUMC13+input*HUMfraction;

		//decay of FOM
		for(int j=0;j<numberofLayers;j++)
		{
			double depthInLayer=depth*j+(depth)/2;
			double tempCofficent=temperatureCoefficent(Temperature(avgTemperature,i,depthInLayer,amplitude,offset));
			allLayers[j].FOMC13=allLayers[j].FOMC13-allLayers[j].FOMC13*FOMdecompositionrate*tempCofficent; //1
			allLayers[j].FOMC14=allLayers[j].FOMC14-allLayers[j].FOMC14*FOMdecompositionrate*tempCofficent; //1

		}
		//transport
		transportFOM<<i<<'\t';
		for(int j=0;j<(numberofLayers-1);j++)
		{
			allLayers[j+1].FOMC13=allLayers[j+1].FOMC13+allLayers[j].FOMC13*tF;
			transportFOM<<allLayers[j].FOMC13*tF<<'\t';
			allLayers[j].FOMC13=allLayers[j].FOMC13*(1-tF);

			allLayers[j+1].FOMC14=allLayers[j+1].FOMC14+allLayers[j].FOMC14*tF;
			allLayers[j].FOMC14=allLayers[j].FOMC14*(1-tF);
		}
		transportFOM<<endl;
		
		//C02

		double Rfraction=R(Clayfraction);
		double humification=1/(Rfraction+1); //5

		for(int j=0;j<numberofLayers;j++)
		{
	
			allLayers[j].FOMC13=allLayers[j].FOMC13-allLayers[j].FOMC13*(1-humification);
			total13CO2=total13CO2+allLayers[j].FOMC13*(1-humification);
			allLayers[j].FOMC14=allLayers[j].FOMC14-allLayers[j].FOMC14*(1-humification);
			total14CO2=total14CO2+allLayers[j].FOMC14*(1-humification);
		}
		CO2FOM<<i<<'\t';
		//humifictation
		for(int j=0;j<numberofLayers;j++)
		{
			
		
			allLayers[j].HUMC13=allLayers[j].FOMC13*humification+allLayers[j].HUMC13;
			CO2FOM<<allLayers[j].FOMC13*humification<<'\t';


			allLayers[j].HUMC14=allLayers[j].FOMC14*humification+allLayers[j].HUMC14;

		}
		CO2FOM<<endl;
		//decay of HUM
		for(int j=0;j<numberofLayers;j++)
		{
			double depthInLayer=depth*j+(depth)/2;
			double tempCofficent=temperatureCoefficent(Temperature(avgTemperature,i,depthInLayer,amplitude,offset));
			allLayers[j].HUMC13=allLayers[j].HUMC13-allLayers[j].HUMC13*HUMdecompositionrate*tempCofficent; //1

			allLayers[j].HUMC14=allLayers[j].HUMC14-allLayers[j].HUMC14*HUMdecompositionrate*tempCofficent; //1
		}
				
		//CO2
		CO2HUM<<i<<'\t';
		for(int j=0;j<(numberofLayers);j++)
		{
			CO2HUM<<Rfraction*allLayers[j].HUMC13<<'\t';
			double depthInLayer=depth*j+(depth)/2;
			allLayers[j].HUMC13=allLayers[j].HUMC13-allLayers[j].HUMC13*Rfraction/(1-Rfraction);
			total13CO2=total13CO2+allLayers[j].HUMC13*Rfraction/(1-Rfraction);
		total14CO2=total14CO2+allLayers[j].HUMC14*Rfraction/(1-Rfraction);
			allLayers[j].HUMC14=allLayers[j].HUMC14-allLayers[j].HUMC14*Rfraction/(1-Rfraction);
			
		}
		CO2HUM<<endl;
		//transport
		transportHUM<<i<<'\t';
		for(int j=0;j<(numberofLayers-1);j++)
		{
			double depthInLayer=depth*j+(depth)/2;
			//we are not sure about we should use Rfraction or CO2lost/lost to other pools
			allLayers[j+1].HUMC13=allLayers[j+1].HUMC13+allLayers[j].HUMC13*(1-(Rfraction/(1-Rfraction))-ROMfraction);
			transportHUM<<allLayers[j].HUMC13*(1-Rfraction-ROMfraction)<<'\t';
			allLayers[j].HUMC13=allLayers[j].HUMC13*(ROMfraction);

			allLayers[j+1].HUMC14=allLayers[j+1].HUMC14+allLayers[j].HUMC14*(1-(Rfraction/(1-Rfraction))-ROMfraction);

			allLayers[j].HUMC14=allLayers[j].HUMC14*(ROMfraction);
		}
		transportHUM<<endl;
		//romification
		for(int j=0;j<(numberofLayers-1);j++)
		{
			allLayers[j].ROMC13=allLayers[j].HUMC13*ROMfraction;


			allLayers[j].ROMC14=allLayers[j].HUMC14*ROMfraction;
	
		}
		//decay of ROM
		for(int j=0;j<numberofLayers;j++)
		{
			double depthInLayer=depth/100*j+(depth/100)/2;
			double tempCofficent=temperatureCoefficent(Temperature(avgTemperature,i,depthInLayer,amplitude,offset));
			allLayers[j].ROMC13=allLayers[j].ROMC13-allLayers[j].ROMC13*ROMdecompositionrate*tempCofficent; //1

			allLayers[j].ROMC14=allLayers[j].ROMC14-allLayers[j].ROMC14*ROMdecompositionrate*tempCofficent; //1
		}
		//CO2 ROM
		CO2ROM<<i<<'\t';
		for(int j=0;j<(numberofLayers);j++)
		{			
			double depthInLayer=depth/100*j+(depth/100)/2;
			allLayers[j].ROMC13=allLayers[j].ROMC13-allLayers[j].ROMC13*(1-(Rfraction/(1-Rfraction))-ROMfraction);
			CO2ROM<<Rfraction*allLayers[j].ROMC13<<'\t';
			total14CO2=total14CO2+allLayers[j].ROMC14*Rfraction/(1-Rfraction);
			total13CO2=total13CO2+allLayers[j].ROMC13*Rfraction/(1-Rfraction);
			allLayers[j].ROMC14=allLayers[j].ROMC14-allLayers[j].ROMC14*(1-(Rfraction/(1-Rfraction))-ROMfraction);
		}
		CO2ROM<<endl;
		//transport of Rom
		transportROM<<i<<'\t';
		for(int j=0;j<(numberofLayers-1);j++)
		{
			double depthInLayer=depth/100*j+(depth/100)/2;
			double temperature= Temperature(avgTemperature,i,depthInLayer,amplitude,offset);
			allLayers[j+1].ROMC13=allLayers[j+1].ROMC13+allLayers[j].ROMC13*(1-(Rfraction/(1-Rfraction)));
			transportROM<<(1-Rfraction)*allLayers[j].ROMC13<<'\t';

			allLayers[j+1].ROMC14=allLayers[j+1].ROMC14+allLayers[j].ROMC14*(1-(Rfraction/(1-Rfraction)));

		}
		transportROM<<endl;
		amount<<i<<'\t';
		for(int j=0;j<(numberofLayers);j++)
		{
			double result=allLayers[j].FOMC13+allLayers[j].HUMC13+allLayers[j].ROMC13;
			double result2=allLayers[j].FOMC14+allLayers[j].HUMC14+allLayers[j].ROMC14;
			amount<<result<<'\t'<<result2;

		}
		amount<<endl;
		totalC02<<i<<'\t'<<total14CO2<<'\t'<<total13CO2<<endl;
		i++;
	}
	totalC02.close();
	transportFOM.close();
	transportHUM.close();
	transportROM.close();
	amount.close();
	CO2FOM.close();
	CO2HUM.close();
	CO2ROM.close();
  return 0;
}
int main()
{
	commonData  data;
	data.readFile("system.txt");
	int firstScenarie=0;
	int num=0;
	data.setCritical(false);
	data.FindItem("JumpStart",&firstScenarie);
	data.FindItem("JumpStop",&num);
	for(int index=firstScenarie;index<=num;index++)
	{
		string InputDirectory,OutputDirectory;
		data.setCritical(true);
		data.FindSection("run",index);
		data.FindItem("InputDirectory",&InputDirectory);
		data.FindItem("OutputDirectory",&OutputDirectory );
#ifndef unix
		if(OutputDirectory.substr(OutputDirectory.size()-1,1).compare("\\"))
			OutputDirectory=OutputDirectory+"\\";
#else
		if(OutputDirectory.substr(OutputDirectory.size()-1,1).compare("/"))
			OutputDirectory=OutputDirectory+"/";
#endif
#ifndef unix
		if(InputDirectory.substr(InputDirectory.size()-1,1).compare("\\"))
			InputDirectory=InputDirectory+"\\";
#else
		if(OutputDirectory.substr(OutputDirectory.size()-1,1).compare("/"))
			OutputDirectory=OutputDirectory+"/";
#endif
		globalSettings=new settings();
	    globalSettings->setInputDir(InputDirectory);
	    globalSettings->setOutputDir(OutputDirectory);
	    globalSettings->run=index;
		theMessage                   = new message;
		char FN1[200],FN2[200];
	    strcpy(FN1,OutputDirectory.c_str());
	    strcat(FN1,"warnings.txt");
	    strcpy(FN2,OutputDirectory.c_str());
	    strcat(FN2,"logfile.txt");
		theMessage->InitMessage(FN1,FN2);
		sumulation();
		delete theMessage;
		delete globalSettings;
	}

}