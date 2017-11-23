/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// =============================================================================
// Soil water retension curve
// (c) J�rgen E. Olesen, Statens Planteavlsfors�g
// E-mail: jeo@pvf.sp.dk
// =============================================================================

#include "../base/common.h"
#include "pFCurveImplementation.h"
#include "../base/message.h"
#include "../base/nixcbits.h"


/****************************************************************************\
\****************************************************************************/
pF_Curve_Implementation::pF_Curve_Implementation(int Intpol)
  : base()
{
   InitVars(Intpol);
}



/****************************************************************************\
\****************************************************************************/
void pF_Curve_Implementation::InitVars(int Intpol)
{
   firstW=NULL,
   firstC=NULL;
   IntpolMethod = Intpol;
   NumberOfPFCurves = 0;
   numExternalConduc=0;
   tabulated=false;
   minpF=0;
   maxpF=0;
}

/****************************************************************************\
\****************************************************************************/
pF_Curve_Implementation::~pF_Curve_Implementation()
{
  pF_WaterData * pW;
 pF_ConductionData * pC;
 while (firstW)
 {
  pW = firstW;
  firstW = firstW->next;
  delete pW;
 }
 while (firstC)
 {
  pC = firstC;
  firstC = firstC->next;
  delete pC;
 }
}


/****************************************************************************\
\****************************************************************************/
void pF_Curve_Implementation::AddWaterItem(pF_WaterData data)
{
 pF_WaterData * newlink = new pF_WaterData;
 pF_WaterData * p;
 pF_WaterData * l;
 int Ok = 1;
 newlink->relativeWaterContent = data.relativeWaterContent;
 newlink->pF = data.pF;
 p = firstW;
 while (p)
 {
  if (p->relativeWaterContent==data.relativeWaterContent) Ok = 0;
  p = p->next;
 }
 if (Ok)
 {
  if (firstW==NULL || data.relativeWaterContent<firstW->relativeWaterContent)
  {
	newlink->next = firstW;
	firstW = newlink;
  }
  else
  {
	p = firstW;
	l = p;
	while (p->next!=NULL &&
			 data.relativeWaterContent>p->relativeWaterContent)
	{
	 l = p;
	 p = p->next;
	}
	if (data.relativeWaterContent<p->relativeWaterContent) p = l;
	newlink->next = p->next;
	p->next = newlink;
  }
 }
 else
  delete newlink;
}

/****************************************************************************\
\****************************************************************************/
void pF_Curve_Implementation::AddWaterData(double RelativeWater, double pFValue)
{
 pF_WaterData p;
 p.relativeWaterContent = RelativeWater;
 p.pF = pFValue;
 AddWaterItem(p);
}

/****************************************************************************\
\****************************************************************************/
void pF_Curve_Implementation::AddConductionItem(pF_ConductionData data)
{
 pF_ConductionData * newlink = new pF_ConductionData;
 pF_ConductionData * p;
 pF_ConductionData * l;
 int Ok = 1;
 newlink->pF = data.pF;
 newlink->logConduc = data.logConduc;
 newlink->fluxPotential = data.fluxPotential;
 p = firstC;
 while (p)
 {
  if (p->pF==data.pF)
   Ok = 0;
  p = p->next;
 }
 if (Ok)
 {
  if (firstC==NULL || data.pF<firstC->pF)
  {
	newlink->next = firstC;
	firstC = newlink;
  }
  else
  {
	p = firstC;
	l = p;
	while (p->next!=NULL && data.pF>p->pF)
	{
	 l = p;
	 p = p->next;
	}
	if (data.pF<p->pF) p = l;
	newlink->next = p->next;
	p->next = newlink;
  }
 }
}

/****************************************************************************\
\****************************************************************************/
void pF_Curve_Implementation::AddConductionData(double pFValue, double logCond, double fluxPot)    // units: pf, log10(m/s), m2 s-1
{
 pF_ConductionData p;
 p.pF = pFValue;
 p.logConduc = logCond;
 p.fluxPotential = fluxPot;
 AddConductionItem(p);
}



/****************************************************************************\
\****************************************************************************/
double pF_Curve_Implementation::GetExternalConductivity(double pF)
{
   double x[maxSplinePoints];
   double y[maxSplinePoints];
   double y2[maxSplinePoints];
   double yp;

   // Bubblesort
   bool unSorted;
   do
   {
      unSorted=false;
      for (int i=0;i<numExternalConduc-1;i++)
      {
         if (ExternalConduc[i+1].pF<ExternalConduc[i].pF)
         {  // Swap
            unSorted=true;
            double p=ExternalConduc[i].pF;
            double lC=ExternalConduc[i].logConduc;
            ExternalConduc[i].pF=ExternalConduc[i+1].pF;
            ExternalConduc[i].logConduc=ExternalConduc[i+1].logConduc;
            ExternalConduc[i+1].pF=p;
            ExternalConduc[i+1].logConduc=lC;
         }
      }
   }
   while (unSorted);

   // Transfer
   for (int i=0;i<numExternalConduc;i++)
   {
      x[i]=ExternalConduc[i].pF;
      y[i]=ExternalConduc[i].logConduc;
   }

   // Spline
   Spline(x,y,numExternalConduc,y2);

   // Get spline interpolated value and return
   yp = Splint(x,y,y2,numExternalConduc,pF);
   return yp;
}

/****************************************************************************\
Method from Kunze et al (1968)
\****************************************************************************/
void pF_Curve_Implementation::MakeHydraulicConductivity(double FieldConductivity)
{
 double flux = 0;
 int Intpol = IntpolMethod;
 IntpolMethod = 0;           // Use spline interpolation here
 pF_ConductionData * pC;
 while (firstC)
 {
  pC = firstC;
  firstC = firstC->next;
  delete pC;
 }
 double pF;
 double relW;
 double Con[81],cond[81];
 int i;

 if (numExternalConduc>1)
 {
  for (i=-10; i<=70; i++)
   cond[i+10] = GetExternalConductivity(0.1*i);
 }
 else
 {
  for (i=-10; i<=70; i++)
  {
   pF = i*0.1;
   relW = CalcRelativeWater(pF);
   double sum = 0;
   for (int j=i; j<=70; j++) sum += (2*j+1-2*i)/(pow(10.,2*i*0.1)/10000.);
   Con[i+10] = 2.7E-4*sum*relW/(81.*81.);
  }
  for (i=-10; i<=70; i++)
   cond[i+10] = Con[i+10]*pow(10,-FieldConductivity)/Con[30];
// OLD   cond[i+10] = Con[i+10]*FieldConductivity/Con[30];
 }

 for (i=-10; i<=70; i++)
 {
  if (i>0)
   flux += (pow(10.,i*0.1)-pow(10.,(i-1)*0.1))/100.0*cond[i+10];                // unit m2 s-1  (divides by 100 to convert cm to m)
  AddConductionData(i*0.1,cond[i+10],flux);
 }

 IntpolMethod = Intpol;
}

/****************************************************************************\
\****************************************************************************/
double pF_Curve_Implementation::CalcpF(double RelativeWater)
{
 double x[maxSplinePoints];
 double y[maxSplinePoints];
 double yp;
 double minpF=7;
 double maxWat=0.0;
 pF_WaterData * p;
 p = firstW;

 x[0]= 1E-5;
 y[0]= 7.0;

 int n = 1;
 while (p)
 {
  minpF=min(minpF,p->pF);
  maxWat=max(maxWat,p->relativeWaterContent);
  x[n] = p->relativeWaterContent;
  y[n] = p->pF;
  p = p->next;
  if (n>=maxSplinePoints)
   theMessage->FatalError("pF_Curve_Implementation::CalcpF - the maximum of pF-curve points exceeded");
  n++;
 };

 x[n]= 1.0;
 y[n]=-2.0;
 n++;

 yp = LinearInterpolation(x,y,n,RelativeWater);
 //Spline(x,y,n,y2);
 //yp = Splint(x,y,y2,n,RelativeWater);  // perform spline interpolation
 return min(7.0,max(-1.0,yp));
}

double pF_Curve_Implementation::CalcRelativeWater(double pFValue)
{
 double x[maxSplinePoints],xx[maxSplinePoints];
 double y[maxSplinePoints],yy[maxSplinePoints];
 double yp;
 double minpF=7;
 double maxWat=0.0;
 pF_WaterData * p;

 yy[0]= 7.0;
 xx[0]= 1E-5;

 int n = 1;
 p = firstW;
 while (p)
 {
  minpF=min(minpF,p->pF);
  maxWat=max(maxWat,p->relativeWaterContent);
  yy[n] = p->pF;
  xx[n] = p->relativeWaterContent;
  p = p->next;
  if (n>=maxSplinePoints)
   theMessage->FatalError("pF_Curve_Implementation::CalcRelativeWater - the maximum of pF-curve points exceeded");
  n++;
 };

 xx[n]= 1.0;
 yy[n]=-2.0;
 n++;

 for (int i=0; i<n; i++)
 {
  y[i] = xx[n-i-1];
  x[i] = yy[n-i-1];
 };
 yp = LinearInterpolation(x,y,n,pFValue);
 //Spline(x,y,n,y2);
 //yp = Splint(x,y,y2,n,pFValue);
 if (yp<=0)
  yp = 1e-50;
 if (yp>1)
  theMessage->FatalError("pF_Curve_Implementation::CalcRelativeWater - relative water content can not be above 1");
 return min(1.0,yp);
}

/****************************************************************************\
\****************************************************************************/
double pF_Curve_Implementation::CalcConductivity(double pFValue)
{
 double pFLow = -1;
 double pFHigh = -1;
 double condLow = -1;
 double condHigh=0;
 pF_ConductionData * p;
 p = firstC;
 if (p)
 {
  condLow = condHigh = p->logConduc;
  pFLow = pFHigh = p->pF;
 };
 while (p)
 {
  if (pFHigh<pFValue)
  {
	condLow = condHigh;
	pFLow = pFHigh;
	condHigh = p->logConduc;
	pFHigh = p->pF;
  };
  p = p->next;
 };
 if (pFValue<pFLow)
  return condLow;     // set to watLow outside low boundary
 else if (pFValue>pFHigh)
  return 0;     // set to 0 outside high boundary
 else                                   // Linear interpolation
 {
  if (pFHigh==pFLow)
   return pow(10.,condLow);
  else
   return pow(10.,condLow+(condHigh-condLow)*(pFValue-pFLow)/(pFHigh-pFLow));
 }
}

/****************************************************************************\
\****************************************************************************/
double pF_Curve_Implementation::CalcFluxPotential(double pFValue)
{
 double pFLow = -1;
 double pFHigh = -1;
 double fluxLow = -1;
 double fluxHigh=0;
 pF_ConductionData * p;
 p = firstC;
 if (p)
 {
  fluxLow = fluxHigh = p->fluxPotential;
  pFLow = pFHigh = p->pF;
 };
 while (p)
 {
  if (pFHigh<pFValue)
  {
	fluxLow = fluxHigh;
	pFLow = pFHigh;
	fluxHigh = p->fluxPotential;
	pFHigh = p->pF;
  };
  p = p->next;
 };
 if (pFValue<pFLow)
  return fluxLow;     // set to watLow outside low boundary
 else
  if (pFValue>pFHigh)
   return 0;     // set to 0 outside high boundary
  else                                   // Linear interpolation
  {
   if (pFHigh==pFLow)
    return fluxLow;
   else
    return fluxLow+(fluxHigh-fluxLow)*(pFValue-pFLow)/(pFHigh-pFLow);
  }
}

/****************************************************************************\
\****************************************************************************/
void pF_Curve_Implementation::MakeTables()
{
 minpF = -1.0;
 maxpF = 7.0;
 int i;
 double pFValue;
 for (i=0;i<=maxTablePoints;i++)
 {
  pFTable[i] = CalcpF((double) i/maxTablePoints/2.0);  // interval 0-0.5
  pFValue = minpF+i*(maxpF-minpF)/maxTablePoints;
  relativeWaterTable[i] = CalcRelativeWater(pFValue);
  conductivityTable[i] = CalcConductivity(pFValue);
  fluxPotentialTable[i] = CalcFluxPotential(pFValue);
 }
 tabulated = true;
}

/****************************************************************************\
\****************************************************************************/
double pF_Curve_Implementation::GetRelativeWater(double pFValue)
{
   if (!tabulated)
   	return CalcRelativeWater(pFValue);                    // no table made yet
   double index = (pFValue-minpF)/(maxpF-minpF)*maxTablePoints;
   int index1 = (int) floor(index);
   double low = relativeWaterTable[index1];
   double high = relativeWaterTable[index1+1];
   return low + (high-low)*(index-index1);
}

/****************************************************************************\
\****************************************************************************/
double pF_Curve_Implementation::GetFluxPotential(double pFValue)
{
 if (!tabulated)
  return CalcFluxPotential(pFValue);                    // no table made yet
 double index = (pFValue-minpF)/(maxpF-minpF)*maxTablePoints;
 int index1 = (int) floor(index);
 double low = fluxPotentialTable[index1];
 double high = fluxPotentialTable[index1+1];
 return low + (high-low)*(index-index1);
}


/****************************************************************************\
\****************************************************************************/
void pF_Curve_Implementation::ReadParameters(fstream * file,int index,double dryBulkDensity,double clayContent,double siltContent,double carbon, bool TopSoil)
{
 Setfile(file);
 // --------- Conductivity data
 int first,f1,num,n1;
 /* Excluded for now !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 char section[80];
 sprintf(section,"%s(%d).ConductionCurve","SoilLayer",index);
 GetSectionNumbers(section,&first,&num);
 if (num>0)
 {
  for (int i=first;i<(first+num);i++)
   if (FindSection(section,i))
   {
 	 double conduc = 0.0;
	 double pFValue = -2;
	 GetParameter("Conductivity",&conduc);
	 GetParameter("PFValue",&pFValue);
	 if (conduc!=0 && pFValue>=-1 && pFValue<7)
     AddExternalConductionData(pFValue,conduc);
    else
     Terminate("pF_Curve_Implementation::ReadParameters - Invalid conductivity point");
   }
   else
    Terminate("pF_Curve_Implementation::ReadParameters - section not found");
 }
 */
   double conductivityFC;
   FindSection("SoilLayer",index);
   UnsetCritical();

   int Mualem = 0;
   int vanGenuchten = 0;
   GetParameter("MualemVanGenucten",&Mualem);
   GetParameter("vanGenuchten",&vanGenuchten);
   if (Mualem || vanGenuchten)
   {
      double fis, fir, KS, N, L, alpha;
      if (vanGenuchten)
      {
      	SetCritical();
         GetParameter("KS",&KS);    // cm d-1
         GetParameter("fis",&fis);
         GetParameter("fir",&fir);
         GetParameter("L",&L);
         GetParameter("alpha",&alpha);
         GetParameter("n",&N);
   		UnsetCritical();
         KS = KS*24.0;
      }
      else
      {
	      /*Literature:
	       van Genuchten, M. Th. 1980. A closed-form equetion for predicting the hyraulic conductivity of unsaturated soils. Soil Sci. Soc. Am. J. 44: 892-898.
	       Wosten, J. H. M., A. Lilly, A. Nemes, C. Le Bas. 1999. Development and use of a database of hydraulic properties of European soils. Geoderma. 90: 169-185.
	       Schaap, M. G., F. J. Leij. 2000. Improved prediction of unsaturated hydraulic conductivity with the Mualem-van Genuchten model. Soil Sci. Soc. Am. J. 64: 843-851.
	       Garnier, P., C. Neel, B. Mary, F. Lafolie. 2001. Evaluation of a nitrogen transport and transformation model in a bare soil. European Journal of Soil Science. 52: 253-268.
	       Acutis, M., M. Donatelli. 2003. SOILPAR 2.00: software to estimate soil hydrological parameters and functions. European J. Agronomy. 18: 373-377.*/
	      double BD   = dryBulkDensity/1e6;               // unit g/cm3
	      double orgmat   = carbon/0.58;                      // Unit % // Not C but total org. mat. - BMP !
	      double clay = clayContent*100;                  // unit %
	      double silt = siltContent*100;                  // Unit % // Remember silt in the USDA sense - not Danish sense - BMP !!!
	      clay=clay/(1-0.01*orgmat);                          // Texture here calculated to sum to 100 WITHOUT orgmat, minor fault though - BMP !!!
	      silt=silt/(1-0.01*orgmat);                          // Texture here calculated to sum to 100 WITHOUT orgmat, minor fault though - BMP !!!

	      fis=0.7919+(0.001691*clay)-(0.29619*BD)-(0.000001491*silt*silt)+(0.0000821*orgmat*orgmat)+(0.02427/clay)
            +(0.01113/silt)+(0.01472*log(silt))-(0.0000733*orgmat*clay)-(0.000619*BD*clay)-(0.001183*BD*orgmat)-(0.0001664*TopSoil*silt);
	      fir = 0.01;
	      double sand = 100.0-clay-silt;
	      if (sand>65 && clay<18) fir = 0.025;         // if sandy soil
	      if (orgmat<=0)
	      	theMessage->FatalError("pF-Curve:: algoritm does not work for OM = 0");


	      alpha = -14.96+(0.03135*clay)+(0.0351*silt)+(0.646*orgmat)+(15.29*BD)-(0.192*TopSoil)-(4.671*BD*BD)
                     -(0.000781*clay*clay)-(0.00687*orgmat*orgmat)+(0.0449/orgmat)+(0.0663*log(silt))+0.1482*log(orgmat)-(0.0456*BD*silt)
                     -(0.4852*BD*orgmat)+(0.00673*TopSoil*clay);
	      alpha = exp(alpha);
	      N = -25.23-(0.02195*clay)+(0.0074*silt)-(0.194*orgmat)+(45.5*BD)-(7.24*BD*BD)+(0.0003658*clay*clay)
              +(0.002885*orgmat*orgmat)-(12.81/BD)-(0.1524/silt)-(0.01958/orgmat)-(0.2876*log(silt))
              -(0.0709*log(orgmat))-(44.6*log(BD))-(0.02264*BD*clay)+(0.0896*BD*orgmat)+(0.00718*TopSoil*clay);
	      L = 0.0202+(0.0006193*clay*clay)-(0.001136*orgmat*orgmat)-(0.2316*log(orgmat))-(0.03544*BD*clay)+(0.00283*BD*silt)+(0.0488*BD*orgmat);
	      L = 10.0*((exp(L)-1.0))/(1.0+exp(L));
	      KS = 7.755+(0.0352*silt)+(0.93*TopSoil)-(0.967*BD*BD)-(0.000484*clay*clay)-(0.000322*silt*silt)+(0.001/silt)-(0.0748/orgmat)
                 -(0.643*log(silt))-(0.01398*BD*clay)-(0.1673*BD*orgmat)+(0.02986*TopSoil*clay)-(0.03305*TopSoil*silt);

	      N=exp(N)+1.0;
	      KS=exp(KS);
      }

      double M = 1.0-1.0/N;
      double SE = 100;                       // water column

      // Equations below taken from Van Genuchten 1991, RETC description - BMP
      double a=pow(alpha*SE,M*N);
      double b=pow(1.0+pow(alpha*SE,N),-M);
      double c=pow(1.0+pow(alpha*SE,N),M*L);
      double K=KS*pow(1.0-a*b,2.0)/c;

      double conducField = -log10(K/(100.0*60*60*24));                                      // Unit m/s. OK provided K is correct - BMP
      MakeHydraulicConductivity(conducField);
		double pFValue = 0.0;
      while (pFValue<=7.0)
      {
      	double h = pow(10.0,pFValue);     // Calculate water potential from PF value - OK BMP
         double fi=fir+(fis-fir)/pow((1.0+pow(alpha*h,N)),M); // BMP solution
         AddWaterData(fi,pFValue);
         pFValue = pFValue + 0.2;

      }
      MakeTables();
   }
   else
   {
	   SetCritical();
      GetVectorInfo("RelativeWater",&first,&num);
      GetVectorInfo("PFValue",&f1,&n1);
      if ((first!=f1) || (num!=n1))
      	Terminate("pF_Curve_Implementation::ReadParameters-error in numbering of pF curve vector elements");

      if (num>0)
      {
      	for (int i=first;i<(first+num);i++)
         {
            double relWater = -2;
            double pFValue = -2;
            GetParameter("RelativeWater",&relWater,i);
            GetParameter("PFValue",&pFValue,i);
         	if (relWater>0 && relWater<1 && pFValue>=-1 && pFValue<7)
         		AddWaterData(relWater,pFValue);
         	else
         		if (pFValue>7 || relWater<0)                                         // pF = 7 and low relative water are ignored but no warning are given (point is inserted anyhow)
         			theMessage->WarningWithDisplay("soilLayerDescriptor::Initialize-pF point outside boundaries ignored");
         }
      }
      GetParameter("ConductivityFC",&conductivityFC);                  // Unit log(m/s)  ?
      MakeHydraulicConductivity(-conductivityFC);
      MakeTables();
   }
 Setfile(NULL);
}

