#include "soc.h"
#include <stdio.h>
#include <string.h>

#include <iostream>
using namespace std;
/****************************************************************************\
Constructor
\****************************************************************************/
soc::soc()
{
   fom.real(0);
   fom.imag(0);
   hum.real(0);
   hum.imag(0);
   rom.real(0);
   hum.imag(0);
   clayParameter1=3.0895;
   clayParameter2=2.672;
   clayParameter3=-7.86;
   humQ=-1;
   index=-1;
   fomK=0.00395;
   humK=4.76E-05;
   romK=8.815E-7;
   romF=0.02293;
   double factorT=2.81;
   humT=factorT*humK;
   fomT=humT;
   romT=factorT*romK;
//   defaultRomK=romK;
//   defaultRomF=romF;
}


/****************************************************************************\
\****************************************************************************/
void soc::Terminate(char * c)
{
  // if (strlen(c)>0)
    //  cout << endl << c << endl;
  // cout << endl << "Program terminated. Press any key." << endl;
 //  cin.get();
  // exit(99);
}


/****************************************************************************\
\****************************************************************************/
double soc::FlushNmineralised()
{
   double retVal=Nmineralised;
   Nmineralised=0;
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
double soc::HumificationQ(double clay)
{
   double R=clayParameter1+clayParameter2*exp(clayParameter3*clay);
   return 1.0/(1.0+R);
}

/****************************************************************************\
\****************************************************************************/
void soc::SetClayFraction(double clay)
{
   humQ=HumificationQ(clay);
}

/****************************************************************************\
\****************************************************************************/
void soc::Import(char * poolName,complex<double>  c)
{
   if (c.real()<0)
      Terminate("soc::Import - negative C downflux");
   if (c.imag()<0)
      Terminate("soc::Import - negative C isotope downflux");
   if (strcmp(poolName,"FOM")==0)
      fom+=c;
   if (strcmp(poolName,"HUM")==0)
      hum+=c;
   if (strcmp(poolName,"ROM")==0)
      rom+=c;
}

/****************************************************************************\
\****************************************************************************/
complex<double> soc::Decay14C(double halfTime)
{
   double f=exp(log(0.5)/halfTime);
   complex<double> decay=(0.0,0.0);
   decay.imag((fom.imag()+hum.imag()+rom.imag())*(1.0-f));
   fom.imag(fom.imag()*f);
   hum.imag(hum.imag()*f);
   rom.imag(rom.imag()*f);
   return decay;
}

/****************************************************************************\
\****************************************************************************/
complex<double> soc::Update(double f,soc * below)
{
   complex<double> downflux=fom*fomT;
   if (downflux.real()<0)
      Terminate("soc::Update - negative C downflux from 'FOM' pool");
   if (downflux.imag()<0)
      Terminate("soc::Update - negative C isotope downflux from 'FOM' pool");
   below->Import("FOM",downflux);
   complex<double> flow_fom=fom*fomK*f;
   fom-=flow_fom+downflux;

   if (fom.real()<0)
      Terminate("soc::Update - 'FOM' pool C content gone negative");
   if (fom.imag()<0)
      Terminate("soc::Update - 'FOM' pool C isotope content gone negative");
   complex<double> CO2=(1.0-humQ)*flow_fom;

   downflux=hum*humT;
   if (downflux.real()<0)
      Terminate("soc::Update - negative C downflux from 'HUM' pool");
   if (downflux.imag()<0)
      Terminate("soc::Update - negative C isotope downflux from 'HUM' pool");
   below->Import("HUM",downflux);
   complex<double> flow_hum=hum*humK*f;
   cout<<flow_hum<<" flow_hum"<<endl;
   cout<<humK<<" humK"<<endl;
	cout<<f<<" f"<<endl;
   hum-=flow_hum+downflux;
   if (hum.real()<0)
      Terminate("soc::Update - 'HUM' pool C content gone negative");
   if (hum.imag()<0)
      Terminate("soc::Update - 'HUM' pool C isotope content gone negative");
   hum+=flow_fom*humQ;
   Nmineralised += 0.1*flow_hum.real();
   CO2+=(1.0-romF)*flow_hum;

   downflux=rom*romT;
   if (downflux.real()<0)
      Terminate("soc::Update - negative C downflux from 'ROM' pool");
   if (downflux.imag()<0)
      Terminate("soc::Update - negative C isotope downflux from 'ROM' pool");
   below->Import("ROM",downflux);
   complex<double> flow_rom=rom*romK*f;
   rom-=flow_rom+downflux;
   if (rom.real()<0)
      Terminate("soc::Update - 'ROM' pool C content gone negative");
   if (rom.imag()<0)
      Terminate("soc::Update - 'ROM' pool C isotope content gone negative");
   rom+=romF*flow_hum;
   CO2+=flow_rom;

   return CO2;
}

