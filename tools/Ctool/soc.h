#ifndef SOC_H
   #define SOC_H

#include <stdlib.h>
#include <math.h>
#include <complex>
using namespace std;
class soc
{
   protected:

   	soc& operator=(const soc& o); // Dissable the compilers automatic generation of default assignment operator

      complex<double> fom,hum,rom;

      double
         clayParameter1,
         clayParameter2,
         clayParameter3,
         fomK,
         humK,
         romK,
         fomT,
         humT,
         romT,
         romF,
         humQ,
         Nmineralised;

      int
         index;

   public:
      soc();
      void Terminate(char * c);
      complex<double> Update(double f,soc * below);
      double HumificationQ(double clay);
	  void AddFOM(double r,double i){
		  fom.real(r+fom.real());
		fom.imag(i+fom.imag());
	  };
      void AddHUM(double r,double i){hum.real(hum.real()+r);hum.imag(hum.imag()+i);};
      void AddROM(double r,double i){rom.real(rom.real()+r);rom.imag(rom.imag()+i);};
      void SetClayFraction(double clay);
      void Import(char * poolName,complex<double>  c);
      complex<double> Decay14C(double halfTime);
      complex<double> GetSOC(){
		  return fom+hum+rom;
	  };
      complex<double> GetFOM(){return fom;};
      complex<double> GetHUM(){return hum;};
      complex<double> GetROM(){return rom;};
      void SetIndex(int idx){index=idx;};
      void PutFOM(complex<double> c){fom=c;};
      void PutHUM(complex<double> c){hum=c;};
      void PutROM(complex<double> c){rom=c;};
      double FlushNmineralised();
};

#endif
