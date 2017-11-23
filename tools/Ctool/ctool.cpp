//BUILDER #include <vcl.h>
//BUILDER #pragma hdrstop
#ifdef borland
#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include <dir.h>
#include <math.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip.h>
#include <strstrea.h>
#include <cstring.h>
#include <soc.h>
#endif
#ifndef borland
#include <iostream>
#include <fstream>
#include <string.h>
//#include <dir.h>
#include <math.h>
//#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
//#include <iomanip.h>
//#include <strstrea.h>
//#include <cstring.h>
#include "soc.h"
#include "fileAccess.h"
#include "convert.h"
#include "compare.h"
using namespace std;
#endif
int mode=2; // See below

bool writeinput= true;
bool writesoilC= true;
bool writesoilCN=true;
/****************************************************************************\
Skal debugges for fejl i høbjergning for frø (Søren Kolind Hviid).
Skal udvides med disse modes:
1) www-mode (default)
2) Indlæsning af environmentstreng(e) fra fil. Disse strege er fx genereret af
   en modificeret udgave af html versionen
3) Indlæsning af følgende fra fil {Postnummer; Kvæg Ja/Nej; JB-nummer;
   C fra planterester; C fra husdyrgødning}
4) Indlæsning af følgende fra fil {Postnummer; Kvæg Ja/Nej; JB-nummer;
   Afgrødenavn; Udbytte; % halm bjerget; N fra husdyrgødning; Type af husdyrgødning}
Modes 2 - 4 skal gemme resultaterne for år 0, 10, 25, 50 og 100 i en fil.
Der skal tilføjes en funktion der ud fra samme oplysninger som mode 4 foretager
den forenklede (FarmN) beregning af jordpuljeændringen i år 0 og 10. Funktionen
skal samtidig kunne generere afgrødeparametre til FarmN. Hermed skal antallet
af afgrøder også udvides.
\****************************************************************************/

//BUILDER using namespace std;
int postnr=8830; // Only used for mode 3 and 4
int type=-1;     // Only used for mode 3 and 4
double c1,c2,c3,c4;
double cn1,cn2,cn3,cn4;

enum {slen=255,profilenum=4};
enum {maxPoints=1500};
enum {firstYear=0,lastYear=105};

const double defaultClay=0.1;
const int maxLayers=100;
const int ploughday=90; // 1. April
int ploughLayers=5;
const bool useSpringob=true;
const double Kirschbaum1=-3.432;
const double Kirschbaum2=0.168;
const double KirschbaumTopt=36.9;
const double temperatureAdjust=7.240153; // To give unity output at 10 deg.
const double fym_hum=0.08;               // Fraction of FYM that enters the HUM pool directly
const double beginApplyDay=111; // 1st May
const double endApplyDay=305;  // 1st October
const double f_root=0.35; // Depth in m above which 63 % of the root matter is deposited - Gerwitz & Page
const double cnRatioPigManure=4.73;
const double cnRatioCattleManure=7.57;
double cnRatioROM;

int haulmObs=0;

fstream infile;

soc soclist[maxLayers];

struct locationStruct
{
   int
      nr;
   double
      E,N,zone;
};

locationStruct locationArray[maxPoints];

struct simStruct
{
   double
      C_Tot,N_tot,Nmineralised;
   complex<double>
      FOM,HUM,ROM;
};

simStruct simArray[lastYear];

struct mailAreaStruct
{
   int
      postCode;
   double
      E,N,zone; // "zone" presently unused
};

mailAreaStruct mailDistrictArray[maxPoints];

struct soilStruct
{
   double
      amp,
      start,
      mean;
   long int
      UTM_E,
      UTM_N;
};

soilStruct soilArray[maxPoints];

double meanT=8.0;
double amplitude=8.0;
double offset=250.0;
double Th_diff=0.35E-6; // Thermal diffusivity. Unit: m2/s
double layerthickness=0.05;
int numOfLayers=21; // One additional layer to keep budget of downflux
complex<double> decay14C;
complex<double>
   rootC,
   topC,
   secC,
   manureC,
   initialC,
   totalCO2,
   totalAddedC;

double
   rho,
   dampingDepth, // Unit: m
   LERTOP,
   LERSUB,
   CTOP,
   NTOP,
   CSUB,
   NSUB;

double clay;
int mailObs = 0;
int pointObs = 0;
int JB;

char* qe;

bool simulateC = true;

char buff[slen];

//BUILDER using namespace std;

/****************************************************************************\
Interface to access html-input
\****************************************************************************/
float ParVal(char* qe,char* name)
{
   char result[50];
   char* prt = strstr(qe,name);           // points to first character
   if (prt!=NULL)
   {
      int i=strlen(name)+1;
      int NewPos = 0;
      while (prt[i]!='&' && i<20)
      {
         if (prt[i]!='%')                // To correct for danish , = %2C
            result[NewPos] = prt[i];
         else
         {
            result[NewPos] = '.';
            i = i + 2;
         }
         i++;
         NewPos++;
      }
      result[NewPos] = NULL;
      float val = atof(result);
      return val;
   }
   return -99;
}

/****************************************************************************\
\****************************************************************************/
double CalcDampingDepth(double k,double rho)
{
   return sqrt(2.0*k/rho);
}

/****************************************************************************\
\****************************************************************************/
void InitVariables()
{
   initialC.real(0);
   initialC.imag(0);
   totalCO2.real(0);
   totalCO2.imag(0);
   totalAddedC.real(0);
   totalAddedC.imag(0);
   rho=3.1415926*2.0/(365.0*24.0*3600.0);
   dampingDepth=CalcDampingDepth(Th_diff,rho);
   decay14C.real(0);
   decay14C.imag(0);
   clay = 0.025;
   if (1==mode)
      JB=ParVal(qe,"jb");
   else
   {
	   cout<<"Warning: no JP selected. using 1"<<endl;
	   JB=1;
   }
   if (JB>2)
      clay = 0.075;
   if (JB>4)
      clay = 0.125;
   if (JB>6)
      clay = 0.2;
   if (JB>7)
      clay = 0.35;
}

/****************************************************************************\
\****************************************************************************/
void Terminate(char * c)
{
   if (strlen(c)>0)
      cout << "<br>" << c;
   cout << "<br>Program terminated.";
   exit(99);
}

/****************************************************************************\
\****************************************************************************/
double GiveDensity(int JB,int nr)
{
   if (JB<0 || JB>7 || nr<0 || nr>4)
   {
      cout << "Illegal input values";
      cin.get();
   }
   double densarr[7][4] =
   	{{1.424,	1.480,	1.527,	1.566},  //JB1
   	{1.403,	1.437,	1.487,	1.583},  //JB2
   	{1.445,	1.479,	1.571,	1.572},  //JB3
   	{1.425,	1.561,	1.614,	1.663},  //JB4
   	{1.539,	1.559,	1.594,	1.601},  //JB5
   	{1.507,	1.570,	1.578,	1.634},  //JB6
   	{1.531,	1.597,	1.605,	1.633}}; //JB7
   return densarr[JB-1][nr-1];
}

/****************************************************************************\
\****************************************************************************/
void Soil(int jb, int postnummer, int bedrifttype)
{
	double ctopArray[7] = {2.35,1.74,2.30,2.15,1.89,1.83,1.87};
	double c12[7]       = {0.54,0.48,0.58,0.54,0.69,0.68,0.79};
	double c13[7]       = {0.25,0.26,0.29,0.20,0.30,0.32,0.46};
	double c14[7]       = {0.17,0.18,0.18,0.16,0.27,0.24,0.36};
	c1 = ctopArray[jb-1];
	if (postnummer>=6000)
      c1 = c1 + 0.29;

   else
      c1 = c1 - 0.43;                        // Kvæg uændret

	c2 = c1*c12[jb-1];
	c3 = c1*c13[jb-1];
	c4 = c1*c14[jb-1];

	double cnvalArray[8] = {16.7,13.0,15.4,12.5,10.8,10.7,10.3,10.2};
	cn1 = cnvalArray[jb-1];
	if (postnummer>=6000)
      cn1 = cn1 + 0.88;
	cn2 = 1.02*cn1;
	cn3 = 0.91*cn1;
	cn4 = 0.63*cn1;
   /* TEST
  	cn1 = 17.6;
	cn2 = 17.825;
	cn3 = 16.0976;
	cn4 = 10.9512;
   */
}

/****************************************************************************\
Returns the effect of temperature on processes
temp - Soil temperature [°C]
\****************************************************************************/
double TemperatureEffect(double t)
{
   return temperatureAdjust*exp(Kirschbaum1+Kirschbaum2*t*(1.0-0.5*t/KirschbaumTopt));
}

/****************************************************************************\
\****************************************************************************/
void InitPointLocation(char * filename)
{
   pointObs=0;
   fstream f;
   f.open(filename,ios::in);
   f.getline(buff,slen);
   int p = 0;
   int e,n,z;
   while (f && p>=0)
   {
      p=-1;
      f >> p >> e >> n >> z;
      f.getline(buff,slen);
      if (f && p>0)
      {
         locationArray[pointObs].nr=p;
         locationArray[pointObs].E=e;
         locationArray[pointObs].N=n;
         locationArray[pointObs].zone=z;
      }
      pointObs++;
   }
   f.close();
}

/****************************************************************************\
Later: include UTM zone (32 or 33) in file !!!
\****************************************************************************/
void InitMailDistricts(char * filename)
{
   mailObs=0;
   fstream f;
   f.open(filename,ios::in);
   f.getline(buff,slen);
   int pc = 0;
   int e,n;
   while (f && pc>=0)
   {
      pc=-1;
      f >> pc >> e >> n;
      f.getline(buff,slen);
      if (f && pc>0)
      {
         mailDistrictArray[mailObs].postCode=pc;
         mailDistrictArray[mailObs].E=e;
         mailDistrictArray[mailObs].N=n;
         mailDistrictArray[mailObs].zone=32; // !!!
      }
      mailObs++;
   }
   f.close();
}

/****************************************************************************\
\****************************************************************************/
void GetMailCoordinates(int pC, double * E, double * N)
{
   bool found=false;
   for(int i=0;i<mailObs;i++)
   {
      if (mailDistrictArray[i].postCode==pC)
      {
         found=true;
         *E =mailDistrictArray[i].E;
         *N =mailDistrictArray[i].N;
         break;
      }
   }
   if (!found)
   {
      cout << mailObs << " " << pC <<  " " << mailDistrictArray[1052].postCode << "<br>";
      Terminate("Mail coordinates not found");
   }
}

/****************************************************************************\
\****************************************************************************/
int FindNearestPoint(double E,double N)
{
   double minDist=1E10;
   double dist;
   int point=-1;
   for(int i=0;i<pointObs;i++)
   {
      dist=sqrt((locationArray[i].E-E)*(locationArray[i].E-E)
               +(locationArray[i].N-N)*(locationArray[i].N-N));
      if (dist<minDist)
      {
         minDist=dist;
         point=locationArray[i].nr;
      }
   }
   return point;
}

/****************************************************************************\
Check assumed water content for these products!
\****************************************************************************/
double CarbonContentSecondary(string s)
{
   double retVal=0.0;
   if (s=="SpringCereals")
      retVal=0.45*0.85;
   if (s=="WinterCereals")
      retVal=0.45*0.85;
   if (s=="Rape")
      retVal=0.45*0.85;
   if (s=="GrassCloverSeed")
      retVal=0.45*0.85;
   if (s=="Legume")
      retVal=0.45*0.85;
   if (s=="RootCrop")
      retVal=0.45*0.12;
   if (retVal<1E-20)
   {
      cout << "Secondary product for: " << s << " is either not found, or gives incorrect C content" << endl;
      Terminate("Product must be defined correctly");
   }
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
double GiveHaulmFactor(string s)
{
   const bool yearspecific=false;
   double retVal=0;
   if (s=="SpringCereals")
   {
      retVal=0.63;
   }
   if (s=="WinterCereals")
   {
      retVal=0.60;
   }
   if (s=="Legume")
   {
      retVal=0.42;
   }
   if (s=="Rape")
   {
      retVal=1.00;
   }
   if (s=="GrassCloverSeed")
   {
      retVal=0.0;
   }
   if (s=="RootCrop")
   {
      retVal=0.0;
   }
   if (s=="Maize")
   {
      retVal=0.0;
   }
   if (s=="WholeCrop")
   {
      retVal=0.0;
   }
   if (s=="GrazedGrass")
   {
      retVal=0.0;
   }
   if (s=="CutGrass")
   {
      retVal=0.0;
   }
   if (s=="SetAside")
   {
      retVal=0.0;
   }
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
void CarbonPerAreaUnit(string s,double yield,double * root,double * top,double * fym)
{
  *top=0.0;
  *root=0.0;
  *fym=0.0;
   const double Ccont = 0.45;
   double DM=0.85;
   double HI=0.4;
   double rootalloc=0.16;
   double secondary=0;
   double total;

   if (s.compare("SpringCereals")==0)
   {
      HI=0.45;
      rootalloc=0.14;
      secondary=GiveHaulmFactor(s)*yield*CarbonContentSecondary(s);
      total=yield*DM*Ccont/HI; // Only top total here
      *top=total*(1.0-HI)-secondary;
      total=total*(1.0+rootalloc/(1.0-rootalloc)); // Now calculate total net C production
      *root=total*rootalloc;
   }
   if (s.compare("WinterCereals")==0)
   {
      if (yield>10 || yield<1)
         cout << "";
      secondary=GiveHaulmFactor(s)*yield*CarbonContentSecondary(s);
      total=yield*DM*Ccont/HI; // Only top total here
      *top=total*(1.0-HI)-secondary;
      total=total*(1.0+rootalloc/(1.0-rootalloc)); // Now calculate total net C production
      *root=total*rootalloc;
   }
   if (s.compare("Legume")==0)
   {
      HI=0.5;
      rootalloc=0.1;
      secondary=GiveHaulmFactor(s)*yield*CarbonContentSecondary(s);
      total=yield*DM*Ccont/HI; // Only top total here
      *top=total*(1.0-HI)-secondary;
      total=total*(1.0+rootalloc/(1.0-rootalloc)); // Now calculate total net C production
      *root=total*rootalloc;
   }
   if (s.compare("Rape")==0)
   {
      DM=0.92;
      HI=0.37;
      rootalloc=0.2;
      secondary=GiveHaulmFactor(s)*yield*CarbonContentSecondary(s);
      total=yield*DM*Ccont/HI; // Only top total here
      *top=total*(1.0-HI)-secondary;
      total=total*(1.0+rootalloc/(1.0-rootalloc)); // Now calculate total net C production
      *root=total*rootalloc;
   }
   if (s.compare("GrassCloverSeed")==0)
   {
      *top=2.0;
      *root=1.25;
   }
   if (s.compare("RootCrop")==0)
   {
      DM=0.22;
      HI=0.75;
      rootalloc=0.1;
      secondary=GiveHaulmFactor(s)*yield*CarbonContentSecondary(s);
      total=yield*DM*Ccont/HI; // Only top total here
      *top=total*(1.0-HI)-secondary;
      total=total*(1.0+rootalloc/(1.0-rootalloc)); // Now calculate total net C production
      *root=total*rootalloc;
   }
   if (s.compare("Maize")==0)
   {
      DM=0.31;
      HI=0.9;
      rootalloc=0.1;
      total=yield*DM*Ccont/HI; // Only top total here
      *top=total*(1.0-HI)-secondary;
      total=total*(1.0+rootalloc/(1.0-rootalloc)); // Now calculate total net C production
      *root=total*rootalloc;
   }
   if (s.compare("WholeCrop")==0)
   {
      DM=0.36;
      HI=0.7;
      rootalloc=0.14;
      total=yield*DM*Ccont/HI; // Only top total here
      *top=total*(1.0-HI)-secondary;
      total=total*(1.0+rootalloc/(1.0-rootalloc)); // Now calculate total net C production
      *root=total*rootalloc;
   }
   if (s.compare("GrazedGrass")==0)
   {
      *top=3.0;
      *root=3.0;
      *fym=1.5;
   }
   if (s.compare("CutGrass")==0)
   {
      *top=3.0;
      *root=3.0;
   }
   if (s.compare("SetAside")==0)
   {
      *root=2.0;
      *top=2.0;
   }
   if ((*root<0.05) || (*top<0.1))
   {
      cout << "Crop: <" << s << "> is either not found, or gives incorrect C translocation" << endl;
         Terminate("Crop must be defined correctly");
   }
}

/****************************************************************************\
\****************************************************************************/
double CarbonPerManureUnit(string s)
{
   return 0.39; // 0.38 - 0.40
}


/****************************************************************************\
\****************************************************************************/
double CNfactor(double c,double n)
{
   double retVal=61.651*pow(c/n,-1.7314);
   retVal=min(1.0,retVal);
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
complex<double> GetTotalCarbon()
{
   complex<double> retVal;
   retVal.real(0);
   retVal.imag(0);
   for(int i=0;i<numOfLayers-1;i++)
      retVal+=soclist[i].GetSOC();
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
void TrackBalance()
{
   complex<double> totC=GetTotalCarbon();
   complex<double> borderFlux = soclist[numOfLayers-1].GetSOC();
   complex<double> cbalance=initialC-totalCO2-decay14C+totalAddedC-totC-borderFlux;
  
   if (fabs(cbalance.real())>fabs(0.01))
   {

      cout << endl << "Warning - carbon unbalanced by " <<fabs( cbalance.real()) << "<br>";
   }
   if (fabs(cbalance.imag())>fabs(0.01))
   {
      cout << "<br>" << "Warning - isotope unbalanced by "
           <<fabs(  cbalance.imag()) << " relative units<br>";
   }

}

/****************************************************************************\
\****************************************************************************/
void SaveStatus(char * filename)
{
   fstream fout;
   fout.open(filename,ios::out);
   fout << "FOM.C\tFOM.ISO\tHUM.C\tHUM.ISO\tROM.C\tROM.ISO" << endl;
   for(int i=0;i<numOfLayers-1;i++)
   {
      fout << soclist[i].GetFOM().real() << "\t" << soclist[i].GetFOM().imag() << "\t"
           << soclist[i].GetHUM().real() << "\t" << soclist[i].GetHUM().imag() << "\t"
           << soclist[i].GetROM().real() << "\t" << soclist[i].GetROM().imag() << "\t"
           << endl;
   }
   fout.close();
}

/****************************************************************************\
\****************************************************************************/
void ReadStatus(char * filename)
{
	fileAccess hd;
	hd.openFile(filename);
	hd.getLineFile();
   int i=0;
   complex<double>
      fom,hum,rom;
   fom.real(1);

     
	  while (!hd.endOfFile() && fom.real()>=0)
   {
      fom.real(-1);
	  double ting=0;
	  string info=hd.getLineFile();
	  convert conv;
	  string lol=conv.partOfAString(info,1);
	  fom.real(conv.StringToDouble(conv.partOfAString(info,1))); 
	 fom.imag(conv.StringToDouble(conv.partOfAString(info,2))); 
	 hum.real(conv.StringToDouble(conv.partOfAString(info,3)));  
	 hum.imag(conv.StringToDouble(conv.partOfAString(info,4))); 
	 rom.real(conv.StringToDouble(conv.partOfAString(info,5)));   
	 rom.imag(conv.StringToDouble(conv.partOfAString(info,6))); 

      if (fom.real()>=0)
      {
         soclist[i].PutFOM(fom);
         soclist[i].PutHUM(hum);
         soclist[i].PutROM(rom);
      }
      i++;
   }

   if (i!=numOfLayers)
   {
      cout << "Read " << i << " layers<br>";
      Terminate("ReadStatus - inconsistent number of layers");
   }
 
}

/****************************************************************************\
name: Danish crop group name
c1: area (%)
c2: yield ("relevant unit")
c3: haulm (% of possible removed)
\****************************************************************************/
void CalcPlantInput(char * name,char* c1,char* c2,char* c3,double ConvertFactor)
{
   double area;
   double yield;                   // convert hkg to t or FE to t (wet weight)
  
		area=0.01*ParVal(qe,c1);
		yield=ConvertFactor*ParVal(qe,c2); 

   double haulmRemoved=0;
  
	   if (c3[0]!=0)
		  haulmRemoved = ParVal(qe,c3);
  
   double root,top,fym;
   CarbonPerAreaUnit(name,yield,&root,&top,&fym);
   topC.real( topC.real()+area*top);
   rootC.real( rootC.real()+area*root);
   manureC.real(  manureC.real()+area*fym); // To account for assumed deposit during grazing
   double hf=GiveHaulmFactor(name);
   if (hf>0)
   {
      double ccs=CarbonContentSecondary(name);
      secC.real(   secC.real()+area*(1.0-haulmRemoved/100.0)*hf*yield*ccs);
   }
}
void CalcPlantInput(string name,double c1,double c2,double c3,double ConvertFactor)
{
   double area;
   double yield;                   // convert hkg to t or FE to t (wet weight)
  
		area=0.01*c1;
		yield=ConvertFactor*c2; 

   double haulmRemoved=0;
  
	   if (c3!=0)
		  haulmRemoved = c3;
  
   double root,top,fym;
   CarbonPerAreaUnit(name,yield,&root,&top,&fym);
   topC.real( topC.real()+area*top);
   rootC.real( rootC.real()+area*root);
   manureC.real(  manureC.real()+area*fym); // To account for assumed deposit during grazing
   double hf=GiveHaulmFactor(name);
   if (hf>0)
   {
      double ccs=CarbonContentSecondary(name);
      secC.real(   secC.real()+area*(1.0-haulmRemoved/100.0)*hf*yield*ccs);
   }
}

/****************************************************************************\
\****************************************************************************/
void CalcInputs()
{
	if(mode==1)
	{
	   CalcPlantInput("Vårkorn","a1","u1","h1",0.1);
	   CalcPlantInput("Vinterkorn","a2","u2","h2",0.1);
	   CalcPlantInput("Frø (græs/kløver)","a3","u3","h3",0.1);
	   CalcPlantInput("Roer/Kartofler","a4","u4","h4",0.1);
	   CalcPlantInput("Helsæd","a5","u5","",0.001/(0.77*0.35));           // Fra fodermiddel tabel
	   CalcPlantInput("Majs","a6","u6","",0.001/(0.85*0.31));           // Fra fodermiddel tabel
	   CalcPlantInput("Bælgplanter","a7","u7","h7",0.1);
	   CalcPlantInput("Raps","a8","u8","h8",0.1);
	   CalcPlantInput("Afgræsset","a9","u9","",0);                      // Ikke brugt
	   CalcPlantInput("Sletmarker","a10","u10","",0);
	   CalcPlantInput("Brak","a11","u11","",0);
	}
	else
	{
		fileAccess input;
		input.openFile("cropYields.txt");
		convert conv;
		string items=input.getLineFile();
		while(!input.endOfFile())
		{
			items=input.getLineFile();
			if(items!="nullptr")
			{
				double area=conv.StringToDouble(conv.partOfAString(items,2));
				double yield=conv.StringToDouble(conv.partOfAString(items,3));
				double haulm=conv.StringToDouble(conv.partOfAString(items,4));
				double ConvertFactor=conv.StringToDouble(conv.partOfAString(items,5));
				string name=conv.partOfAString(items,1);
				CalcPlantInput(name,area,yield,haulm,ConvertFactor);
			}
		
		}
	}
   double pigManureN=0;
   double cattleManureN=0;
   if(mode==1)
   {
	  pigManureN= ParVal(qe,"gn2");
	 cattleManureN=ParVal(qe,"gn1");
   }
   manureC.real(manureC.real()+0.001*pigManureN*cnRatioPigManure+0.001*cattleManureN*cnRatioCattleManure);
/*   cout << "<br>" << "manure " << manureC.re;
   cout << "<br>" << "Sec " << secC.re;
   cout << "<br>" << "top " << topC.re;
   cout << "<br>" << "root " << rootC.re;*/
}

/****************************************************************************\
\****************************************************************************/
void InitProfile(char * filename,int point=0)
{
   Soil(JB,postnr,type);
   ReadStatus(filename);
   for(int i=0;i<numOfLayers-1;i++)
   {
      soclist[i].SetIndex(i);
      soclist[i].SetClayFraction(defaultClay);
   }
   double CN,C,N;
   for(int i=0;i<20;i++)
   {
      if (i==0)
      {
         if (1==mode)
         {
            C=ParVal(qe,"c1");
            N=ParVal(qe,"n1");
            CN=C/N;
         }
         else
            CN=cn1;
      }
      if (i==5)
      {
         if (1==mode)
         {
            C=ParVal(qe,"c2");
            N=ParVal(qe,"n2");
            CN=C/N;
         }
         else
            CN=cn2;
      }
      if (i==10)
      {
         if (1==mode)
         {
            C=ParVal(qe,"c3");
            N=ParVal(qe,"n3");
            CN=C/N;
         }
         else
            CN=cn3;
      }
      if (i==15)
      {
         if (1==mode)
         {
            C=ParVal(qe,"c4");
            N=ParVal(qe,"n4");
            CN=C/N;
         }
         else
            CN=cn4;
      }
      soclist[i].SetClayFraction(clay);
      complex<double> fom=soclist[i].GetFOM();
      fom.imag(fom.real());
      complex<double> hum=soclist[i].GetHUM();
      hum.imag(hum.real());
      complex<double> rom=soclist[i].GetROM();
      rom.imag(rom.real());
      complex<double> sum=soclist[i].GetSOC();
      sum.imag(sum.real());
      if (useSpringob)
      {
         fom=fom*CNfactor(CN,1);
         hum=hum*CNfactor(CN,1);
         rom=sum-fom-hum;
      }
      // Set C/N ratio of ROM pool. This is done by altering the isotope content relative to the N/C (sic!) ratio
      double N=sum.real()/CN;
      double Npools=0.1*(fom.real()+hum.real()+rom.real());
      double Ndiff=min(Npools-N,rom.real()*0.099);
      double Nrom=rom.real()*0.1-Ndiff;
      rom.imag(10.0*Nrom);
      soclist[i].PutFOM(fom);
      soclist[i].PutHUM(hum);
      soclist[i].PutROM(rom);
   }

   double cTot=0;
   double c=0;
   for(int i=0;i<5;i++)
      cTot+=soclist[i].GetSOC().real();
   if (1==mode)
      c=ParVal(qe,"c1");
   else
      c=c1;
   double q=c*25.0*GiveDensity(JB,1)/cTot;

   for(int i=0;i<5;i++)
   {
      soclist[i].PutFOM(soclist[i].GetFOM()*q);
      soclist[i].PutHUM(soclist[i].GetHUM()*q);
      soclist[i].PutROM(soclist[i].GetROM()*q);
   }
   cTot=0;
   for(int i=5;i<10;i++)
      cTot+=soclist[i].GetSOC().real();
   if (1==mode)
      c=ParVal(qe,"c2");
   else
      c=c2;
   q=c*25.0*GiveDensity(JB,2)/cTot;
   for(int i=5;i<10;i++)
   {
      soclist[i].PutFOM(soclist[i].GetFOM()*q);
      soclist[i].PutHUM(soclist[i].GetHUM()*q);
      soclist[i].PutROM(soclist[i].GetROM()*q);
   }
   cTot=0;
   for(int i=10;i<15;i++)
      cTot+=soclist[i].GetSOC().real();
   if (1==mode)
      c=ParVal(qe,"c3");
   else
      c=c3;
   q=c*25.0*GiveDensity(JB,3)/cTot;
   for(int i=10;i<15;i++)
   {
      soclist[i].PutFOM(soclist[i].GetFOM()*q);
      soclist[i].PutHUM(soclist[i].GetHUM()*q);
      soclist[i].PutROM(soclist[i].GetROM()*q);
   }
   cTot=0;
   for(int i=15;i<20;i++)
      cTot+=soclist[i].GetSOC().real();
   if (1==mode)
      c=ParVal(qe,"c4");
   else
      c=c4;
   q=c*25.0*GiveDensity(JB,4)/cTot;
   for(int i=15;i<20;i++)
   {
      soclist[i].PutFOM(soclist[i].GetFOM()*q);
      soclist[i].PutHUM(soclist[i].GetHUM()*q);
      soclist[i].PutROM(soclist[i].GetROM()*q);
   }
}

/****************************************************************************\
OBS: depth in m
\****************************************************************************/
double Temperature(int day,double depth)
{
   double retVal=meanT+amplitude*exp(-depth/dampingDepth)*sin(rho*(day+offset)*24.0*3600.0-depth/dampingDepth);
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
double RootPresence(int layer)
{
   double topF = 1.0-exp(-(1.0/f_root)*layerthickness*layer);
   double bottF = 1.0-exp(-(1.0/f_root)*layerthickness*(1.0+layer));
   return bottF-topF;
}

/****************************************************************************\
The bottom layer is reserved to contain cumulated downflux. This layer thus
has no turnover.
\****************************************************************************/
void Simulation()
{
	
   if (mode<3)
      CalcInputs();
   double applyDays=endApplyDay-beginApplyDay;
   //fstream f;
   //f.open("c:\\kvadratnet\\testoutput.xls",ios::out);
   double depth;
   double C14=1.0; // 14C ignored. Codeline aims to keep as much as possible of the code unchanged.
   for(long int y=firstYear;y<lastYear;y++)
   {
	  
      topC.imag(C14*topC.real());
      simArray[y].FOM.real(0.0);
      simArray[y].FOM.imag(0.0);
      simArray[y].HUM.real(0.0);
      simArray[y].HUM.imag(0.0);
      simArray[y].ROM.real(0.0);
      simArray[y].ROM.imag(0.0);
	   ofstream * d2f=new ofstream;

	
      for(int i=0;i<numOfLayers-1;i++)
      {
         simArray[y].FOM=simArray[y].FOM+soclist[i].GetFOM();
         simArray[y].HUM=simArray[y].HUM+soclist[i].GetHUM();
         simArray[y].ROM=simArray[y].ROM+soclist[i].GetROM();
      }
	

      simArray[y].N_tot=100.0*(simArray[y].FOM.real()+simArray[y].HUM.real()+simArray[y].ROM.imag());
	


      simArray[y].C_Tot=simArray[y].FOM.real()+simArray[y].HUM.real()+simArray[y].ROM.real();
	

      topC.imag(C14*topC.real());
      rootC.imag(C14*rootC.real());
      manureC.imag(C14*manureC.real());
      secC.imag(C14*secC.real());
      for(int day=1;day<=365;day++)
      {
         int month=-1;
         if (day==1)
            month=1;
         if ((day % 30)==0)
            month=1+day/30;
         if (month>12)
            month=-1;
         //if (day==1)
         //   f << point << "\t" << y << "\t" << month << "\t" << day << "\t";
         for(int i=0;i<numOfLayers-1;i++)
         {
            complex<double> pool=soclist[i].GetSOC();
            //if (day==1)
            //   f << pool.re << "\t";
            depth=i*layerthickness+layerthickness/2.0;
            double t=Temperature(day,depth);
            soc * socp = NULL;
            if ((i+1)<numOfLayers)
               socp=&soclist[i+1];
            totalCO2+=soclist[i].Update(TemperatureEffect(t),socp);
            if(day>=beginApplyDay && day<endApplyDay)
            {
               soclist[i].AddFOM(RootPresence(i)*rootC.real()/applyDays,RootPresence(i)*rootC.imag()/applyDays);
               totalAddedC+=RootPresence(i)*rootC/applyDays;
            }
         }
         TrackBalance();
         if(day>=beginApplyDay && day<endApplyDay)
         {

            soclist[0].AddFOM((topC.real()+secC.real())/applyDays,(topC.imag()+secC.imag())/applyDays);
            totalAddedC+=(topC+secC)/applyDays;
            soclist[0].AddFOM(manureC.real()*(1.0-fym_hum)/applyDays,manureC.imag()*(1.0-fym_hum)/applyDays);
            soclist[0].AddHUM(manureC.real()*fym_hum/applyDays,manureC.imag()*fym_hum/applyDays);
            totalAddedC+=manureC/applyDays;
         }
         if(day==ploughday) // Add a test of crop type, as not all crops are ploughed!!!
         {
            complex<double> topFOM;
            complex<double> topHUM;
            complex<double> topROM;
            topFOM.real(0);
            topFOM.imag(0);
            topHUM.real(0);
            topHUM.imag(0);
            topROM.real(0);
            topROM.imag(0);
            for(int i=0;i<ploughLayers;i++)
            {
               topFOM+=soclist[i].GetFOM();
               topHUM+=soclist[i].GetHUM();
               topROM+=soclist[i].GetROM();
            }
            for(int i=0;i<ploughLayers;i++)
            {

            	 complex<double> topFOMtmp(topFOM.real()/ploughLayers,topFOM.imag()/ploughLayers);
            	 complex<double> topHUMtmp(topHUM.real()/ploughLayers,topHUM.imag()/ploughLayers);
            	 complex<double> topROMtmp(topROM.real()/ploughLayers,topROM.imag()/ploughLayers);
               soclist[i].PutFOM(topFOMtmp);
               soclist[i].PutHUM(topHUMtmp);
               soclist[i].PutROM(topROMtmp);
            }
         }
         //if (day==365)
         //   f << endl;
      }

      simArray[y].Nmineralised=0.0;
      for(int i=0;i<numOfLayers-1;i++)
         simArray[y].Nmineralised+=soclist[i].FlushNmineralised();
   }

//   f.close();
}
/****************************************************************************\
\****************************************************************************/
void InitTemperatures(char * filename)
{
   int PUNKTNO;
   double
      amp,
      start,
      mean;
   fstream fin;

   fin.open(filename,ios::in);

   fin.getline(buff,slen);
   PUNKTNO=999;
   int Count = 0;
   while (fin && PUNKTNO>0 && Count<5000)
   {
      Count++;
      PUNKTNO=-1;
      fin >> PUNKTNO	>> amp >> start >> mean;
      fin.getline(buff,slen);
      if (PUNKTNO>0 && PUNKTNO<maxPoints && fin)
      {
         soilArray[PUNKTNO].amp=amp;
         soilArray[PUNKTNO].start=start;
         soilArray[PUNKTNO].mean=mean;
      }
   }
   fin.close();
}

/****************************************************************************\
\****************************************************************************/
void Singlerun()
{

   double E,N;
   if (1==mode)
   {
      puts("Content-type: text/html\n");
      puts("<html><head>");
      puts("<link rel=\"stylesheet\" href=\"styles/style.css\" type=\"text/css\">");
      puts("<title>C-TOOL resultater </title></head>");
      puts("<body><form action=\"http://web.agrsci.dk/jbe/bin/ctoolwww.exe\" method=get>");
      //puts("<body bgcolor=\"#66CCFF\"><form action=\"http://web.agrsci.dk/jbe/bin/ctoolwww.exe\" method=get>");
      //cout << "<center><H1>C-TOOL Resultat</H1></center>";
      //cout << "<HR WIDTH=\"100%\" SIZE=\"3\" NOSHADE>";
   }
   if (1==mode)
   {
      qe = getenv("QUERY_STRING");
	  puts(qe);
   }
   if (!qe && mode==1)
      cout << "Program error: no query string" << endl;
   else
   {
      /*char buffer[128];
      getcwd(buffer, 128);
      cout << buffer << "<br>";*/
//#define NJFJORD
#ifdef NJFJORD
      if (1==mode)
         chdir("CTool");
#else
      if (1==mode)
      {
		 fileAccess hd;
		  hd.changeDir("jbe");
         hd.changeDir("bin");
      }
#endif

      double top=-999;
      double rod=-999;
      double halm=-999;
      double hug=-999;
      if (3==mode)
      {
         infile >> postnr >> type >> JB >> top >> rod >> halm >> hug;
         topC.real(  top);
         rootC.real(  rod);
         manureC.real(  hug);
         secC.real( halm);
      }

      string crop;
      double yield=-999;
      double pct_haulm_remov=-999;
      double hug_n=-999;
      string hug_type;
      if (4==mode)
      {
         infile >> postnr >> type >> JB >> crop >> yield >> pct_haulm_remov >> hug_n >> hug_type;
         double area=1.0;
         //double yield=ConvertFactor*ParVal(qe,c2);                    // convert hkg to t or FE to t (wet weight)
         double root,top,fym;
         CarbonPerAreaUnit(crop,yield,&root,&top,&fym);
         topC.real( topC.real()+area*top);
         rootC.real(  rootC.real()+area*root);
         manureC.real( manureC.real()+area*fym); // To account for assumed deposit during grazing
         double hf=GiveHaulmFactor(crop);
         if (hf>0)
         {
            double ccs=CarbonContentSecondary(crop);
            secC.real(secC.real()+area*(1.0-pct_haulm_remov/100.0)*hf*yield*ccs);
         }
         double cnRatioManure=-1;
         if (hug_type=="pigslurry")
            cnRatioManure=cnRatioPigManure;
         if (hug_type=="cattleslurry")
            cnRatioManure=cnRatioCattleManure;
         manureC.real(( manureC.real()+0.001*hug_n*cnRatioManure));
      }

      InitVariables();
      string CurrentPath=" ";
  fileAccess hd;
  CurrentPath=hd.getCurrentPath();
      CurrentPath+="\\c-tool\\";

	  cout<<CurrentPath<<endl;
	hd.changeDir(CurrentPath);
      InitProfile("initialprofile.txt");
      InitMailDistricts("postdistrikter.txt");
      InitTemperatures("temperature_parms.txt");
      InitPointLocation("pointlocation.txt");

      if (1==mode)
         ParVal(qe,"po");

      GetMailCoordinates(postnr,&E,&N);
      int punktnr=FindNearestPoint(E,N);
      meanT=soilArray[punktnr].mean;
      amplitude=soilArray[punktnr].amp;
      offset=soilArray[punktnr].start;
      initialC=GetTotalCarbon();
      if (1==mode)
         cout << "<center><h3>Resultat</h3><br>";
	  	
      Simulation();

	 
      complex<double> endC = GetTotalCarbon();
      //cout << "Initial C: " << initialC.re << "<br>End C: " << endC.re;

      if (1==mode)
      {
         cout << "<table border=\"3\" cellpadding=\"5\" cellspacing=\"0\">";
         cout << "<tr><td>År</td><td>Jord N<br>kg total N/ha</td><td>Ændring<br>kg total N/ha/år</td><td>Basis mineralisering<br>kg N/ha/år </td><td>Jord C<br>t C/ha</td></tr>";
      }

      int ShowYears[8] = {3,8,13,23,38,53,78,103};
      //int ShowYears[8] = {0,5,10,20,35,50,75,100};
      double N_Offset = simArray[ShowYears[0]].N_tot-simArray[0].N_tot;
      double C_Offset = simArray[ShowYears[0]].C_Tot-simArray[0].C_Tot;
     // ofstream  tablefile;

	   ofstream tablefile;
      if (1!=mode)
      {
		  fileAccess hd;
		 
		  string outputfile=hd.getCurrentPath();
		  outputfile+="\\output\\output.txt";
		 // hd.changeDir(outputfile);
		  cout<<endl;
		 cout<<outputfile<<endl;
        tablefile.open(outputfile);
		 if (tablefile.bad()||tablefile.fail())
            Terminate("Could not open outputfile");
         tablefile << "År\tJord_N\tÆndring\tMineralisering\tJord_C" << endl ;
      }
      for (int i=0;i<8;i++)
      {
         if (1==mode)
         {
            cout << "<tr><td>" << (ShowYears[i]-3) << "</td>";
            cout << "<td>" << (int) (simArray[ShowYears[i]].N_tot-N_Offset) << "</td>";
            cout << "<td>" << 0.1*((int) (10*(simArray[ShowYears[i]+1].N_tot-simArray[ShowYears[i]].N_tot))) << "</td>";
            cout << "<td>" << 0.1*((int) (10000*simArray[ShowYears[i]].Nmineralised)) << "</td>";
            cout << "<td>" << 0.1*((int) (10*(simArray[ShowYears[i]].C_Tot-C_Offset))) << "</td></tr>";
         }
         if (1!=mode)
         {
            tablefile << (ShowYears[i]-3);
            tablefile << "\t" << (int) (simArray[ShowYears[i]].N_tot-N_Offset);
            tablefile << "\t" << 0.1*((int) (10*(simArray[ShowYears[i]+1].N_tot-simArray[ShowYears[i]].N_tot)));
            tablefile << "\t" << 0.1*((int) (10000*simArray[ShowYears[i]].Nmineralised));
            tablefile << "\t" << 0.1*((int) (10*(simArray[ShowYears[i]].C_Tot-C_Offset))) << endl;

            cout << (ShowYears[i]-3);
            cout << "\t" << (int) (simArray[ShowYears[i]].N_tot-N_Offset);
            cout << "\t" << 0.1*((int) (10*(simArray[ShowYears[i]+1].N_tot-simArray[ShowYears[i]].N_tot)));
            cout << "\t" << 0.1*((int) (10000*simArray[ShowYears[i]].Nmineralised));
            cout << "\t" << 0.1*((int) (10*(simArray[ShowYears[i]].C_Tot-C_Offset))) << endl;
         }
      }
      if (writeinput)
      {
         if (1==mode)
         {
            cout << "<tr><td>Input</td>";
            cout << "<td>" << topC.real() << "</td>";
            cout << "<td>" << rootC.real() << "</td>";
            cout << "<td>" << manureC.real() << "</td>";
            cout << "<td>" << secC.real() << "</td></tr>";
         }

         if (1!=mode)
         {
            tablefile << "Input";
            tablefile << "\t" << topC.real();
            tablefile << "\t" << rootC.real();
            tablefile << "\t" << manureC.real();
            tablefile << "\t" << secC.real() << endl;

            cout << "Input";
            cout << "\t" << topC.real();
            cout << "\t" << rootC.real();
            cout << "\t" << manureC.real();
            cout << "\t" << secC.real() << endl;
         }
      }
      if (writesoilC)
      {
         if (1==mode)
         {
            cout << "<tr><td>Soil C</td>";
            cout << "<td>" << ParVal(qe,"c1")*25.0*GiveDensity(JB,1) << "</td>";
            cout << "<td>" << ParVal(qe,"c2")*25.0*GiveDensity(JB,2) << "</td>";
            cout << "<td>" << ParVal(qe,"c3")*25.0*GiveDensity(JB,3) << "</td>";
            cout << "<td>" << ParVal(qe,"c4")*25.0*GiveDensity(JB,4) << "</td></tr>";
         }

         if (1!=mode)
         {
            tablefile << "Soil_C";
            tablefile << "\t" << c1*25.0*GiveDensity(JB,1);
            tablefile << "\t" << c2*25.0*GiveDensity(JB,2);
            tablefile << "\t" << c3*25.0*GiveDensity(JB,3);
            tablefile << "\t" << c4*25.0*GiveDensity(JB,4) << endl;

            cout << "Soil C";
            cout << "\t" << c1*25.0*GiveDensity(JB,1);
            cout << "\t" << c2*25.0*GiveDensity(JB,2);
            cout << "\t" << c3*25.0*GiveDensity(JB,3);
            cout << "\t" << c4*25.0*GiveDensity(JB,4) << endl;
         }
      }
      if (writesoilCN)
      {
         if (1==mode)
         {
            cout << "<tr><td>Soil C/N</td>";
            cout << "<td>" << ParVal(qe,"c1")/ParVal(qe,"n1") << "</td>";
            cout << "<td>" << ParVal(qe,"c2")/ParVal(qe,"n2") << "</td>";
            cout << "<td>" << ParVal(qe,"c3")/ParVal(qe,"n3") << "</td>";
            cout << "<td>" << ParVal(qe,"c4")/ParVal(qe,"n4") << "</td></tr>";
         }

         if (1!=mode)
         {
            tablefile << "Soil_C/N";
            tablefile << "\t" << cn1;
            tablefile << "\t" << cn2;
            tablefile << "\t" << cn3;
            tablefile << "\t" << cn4 << endl;

            cout << "Soil C/N";
            cout << "\t" << cn1;
            cout << "\t" << cn2;
            cout << "\t" << cn3;
            cout << "\t" << cn4 << endl;
         }
      }
      if (1!=mode)
         tablefile.close();
      if (1==mode)
      {
         cout << "</table>";
         cout << "<br><h3>Bedrift beskrivelse</h3><br>";
         cout << "<table border=\"3\" cellpadding=\"5\" cellspacing=\"0\">";
         cout << "<tr><td>JB nummer</td><td>" <<  ParVal(qe,"jb") << "</td></tr>";
      }
      int BedriftType = -1;
      if (1==mode)
         BedriftType = ParVal(qe,"bedrift");
      else
         BedriftType = type;
      if (BedriftType == -1)
         cout << "Programfejl, bedriftstype ikke initialiseret" << endl;
      string Type = "Andet";
      //AnsiString Type = "Andet";
      if (BedriftType==1)
         Type = "Kvæg";
      if (1==mode)
      {
         cout << "<tr><td>bedrift</td><td>" << Type.c_str()  << "</td></tr>";
         cout << "<tr><td>vanding</td><td>" <<  (ParVal(qe,"vanding")==0?"Uvandet":"Vandet") << "</td></tr>";
         cout << "<tr><td>Postnummer</td><td>" <<  ParVal(qe,"po") << "</td></tr>";
         cout << "</table>";
      }
   }
   /*fstream f;
   f.open("ctool.log",ios::out);
   f << Now().DateString().c_str() << " " << Now().TimeString().c_str() << "Query: " << qe << endl;
   f.close();
   cout << "Slut";*/
   cout << "</body></html>";
}

/****************************************************************************\
\****************************************************************************/
int main(int argc, char *argv[],char *env[])
{
   if (1==mode)
      Singlerun();
   else
   {
      if (3== mode)
         infile.open("input_3.txt",ios::in);
      if (4== mode)
         infile.open("input_4.txt",ios::in);
      Singlerun();
      infile.close();
      //getch();
   }
   return 0;
}

