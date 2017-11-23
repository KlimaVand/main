/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 Created by NJH - 2.6.99
  ----------------------------------------------------------------------------
      Log
 Describes a separate free-standing store)
\****************************************************************************/
#include <common.h>
#include <slurrystore.h>
#include <indicat.h>
#include <climclas.h>
#include <contrparm.h>
#include <nixcbits.h>
#include <output.h>

/****************************************************************************\
  Destructor
\****************************************************************************/
slurrystore::~slurrystore()
{
   external = true;
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
slurrystore::slurrystore(char *aName,const int aIndex,const base* aOwner)
 : manurestore(aName,aIndex,aOwner), building(aName,aIndex,aOwner)
{
   crustResistance=0;     		//crust resistance to volatization
   specificResistance=0;           //building spec. resistance to volatization
   coverResistance=0;       //Covers on slurrystore resistance --||--
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
slurrystore::slurrystore(slurrystore& s): manurestore(s),
 building(s)
{
   crustResistance=s.crustResistance;     		//crust resistance to volatization
   specificResistance=s.specificResistance;           //building spec. resistance to volatization
   coverResistance=s.coverResistance;       //Covers on slurrystore resistance --||--
   ALFAMtype=s.ALFAMtype;
}

/****************************************************************************\
	Init reads the relevant values from its owners file
\****************************************************************************/
void slurrystore::ReadParameters(fstream * file)
{
	manurestore::ReadParameters(file);
	building::ReadParameters(file);
   Setfile(file);
   FindSection(aBase->GetName(),aBase->GetIndex()); // perform this in the top of hierarchy - and only there
   GetParameter("AreaPrAnimal",&AreaPrAnimal);
   SetCritical();
   GetParameter("CrustResistance",&crustResistance);
   GetParameter("SpecificResistance",&specificResistance);
   GetParameter("CoverResistance",&coverResistance);
   Setfile(NULL);
   if (theOutputControl->GetManureManageDetails())
		OpenDetailsFile();
   else
   	detailsFile=NULL;
}

/****************************************************************************\
	Dailyactions
\****************************************************************************/
void slurrystore::DailyUpdate()
{
/*     fstream * filehandle = theMessage->GiveDebugHandle();
    *filehandle << " storeV " << GetAmountManure() << " storeN " << GetTotalNStored() << " ";
    theMessage->CloseDebugHandle();*/
	building::DailyUpdate();
   double volatAmmonia=0.0, N2Emission=0.0, N2OEmission=0.0, CH4Emission=0.0, CO2Emission=0.0;

	switch (theControlParameters->GetvolatModel())
   {
      case 0:{
               double Nlost,Clost;
               Volatize(&Nlost,&Clost,&volatAmmonia, &N2Emission, &N2OEmission, &CH4Emission, &CO2Emission);
               Nbudget.AddOutput(Nlost);
               Cbudget.AddOutput(Clost);
             }
         break;
      case 1: VolatizeSlurry(&volatAmmonia, &N2Emission, &N2OEmission, &CO2Emission, &CH4Emission);
      break;
   }
	manurestore::DailyUpdate(volatAmmonia, N2Emission, N2OEmission, CO2Emission, CH4Emission);
   theOutput->AddIndicator(environmentalIndicator,"35.43 NH3-N volatilisation from manure storage","kg N",volatAmmonia*1000.0);
   theOutput->AddIndicator(environmentalIndicator,"35.44 N2-N volatilisation from manure storage","kg N",N2Emission*1000.0);
   theOutput->AddIndicator(environmentalIndicator,"35.45 N2O-N volatilisation from manure storage","kg N",N2OEmission*1000.0);
   theOutput->AddIndicator(environmentalIndicator,"45.43 CO2-C volatilisation from manure storage","kg C",CO2Emission*1000.0);
   theOutput->AddIndicator(environmentalIndicator,"45.44 CH4-C volatilisation from manure storage","kg C",CH4Emission*1000.0);
   GiveIndicator(environmentalIndicator);


}

/****************************************************************************\
\****************************************************************************/
void slurrystore::VolatizeSlurry(double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CO2Emission, double *CH4Emission)
{
	if (GetAmountInManureStore()>0.0)
   {
      double waterEvap=0.0;
      double rb,ra,resistance;

      double Ameantemp,Amin_temp,Amax_temp,Aprecip,Aradiation,AEPot,Awindspeed, Ahumidity;

      theClimate->GetClimate(Ameantemp, Amin_temp,Amax_temp, Aprecip, Aradiation,AEPot, Awindspeed, Ahumidity);

      if(Awindspeed<0.0001) Awindspeed=0.0001;

      double height = 2.0;
      double displacementHt=0.6*height;
      double zeroPlane=0.001;
      double ustar=GetFrictionVelocity(height, displacementHt, zeroPlane, Awindspeed);
      rb=6.2*pow(ustar,-0.67);				//Olesen & Summer 94  boundary layer res.   86400 (s->day) inserted by JB

      ra=(log(height/zeroPlane))/(GetvonKarman()*ustar);		//------||--------    Aerodynamic res.
      resistance=rb+ra+coverResistance+crustResistance+specificResistance;                 		//rC is the crusts resistance to volatization

      manurestore::Volatize(Ameantemp, Ahumidity, Aprecip, surfaceArea, resistance,
                                 volatAmmonia, &waterEvap, N2Emission, N2OEmission, CO2Emission, CH4Emission);

   /*   *theMessage->GiveDebugHandle() << theTime.GetDay() << "," << theTime.GetMonth() << "," << theTime.GetYear() << ","
       << Ameantemp << "," << volat*1000.0 << endl;
      theMessage->CloseDebugHandle();*/
   }
}

/****************************************************************************\
\****************************************************************************/
void slurrystore::GiveIndicator(int indicatorType)
{
	if (indicatorType==economicIndicator)
   			building::GiveIndicator(indicatorType);
	if (indicatorType==environmentalIndicator)
   {
      double Vt=0,N=0, C=0;
      manure * aManure=(manure *) theStore->GetProductPointer();

      if (aManure)
      {
         Vt=aManure->GetAmount();           //The total amount of slurry in kg
         N=Vt*aManure->GetTotalN().n  ;            //The total N-fraction
         C=Vt*aManure->GetC_content()  ;
      }

      theOutput->AddStateIndicator(environmentalIndicator,"35.60 N bound in manure storage","kg N",N*1000.0);
      theOutput->AddStateIndicator(environmentalIndicator,"45.60 C bound in manure storage","kg C",C*1000.0);
      theOutput->AddStateIndicator(economicIndicator,"11.70 Slurry in slurrystore","t",Vt/1000);
   }
}

/****************************************************************************\
\****************************************************************************/
void slurrystore::CalcLP()
{
   manurestore::CalcLP();
   building::CalcLP();
}

/****************************************************************************\
Calls manurestore's routine - is defined in slurrystore to help in debugging
\****************************************************************************/
void slurrystore::RcvManure(const manure* someManure)
{
	manurestore::RcvManure(someManure);
	theOutput->AddIndicator(economicIndicator,"19.72 Slurry produced","t",someManure->GetAmount());
//   if (theStore->Getamountstored()>theStore->GetCapacity())
  // 	theMessage->FatalError(manurestore::GetName()," slurry storage capacity exceeded");
}

/****************************************************************************\
	Scale the manure storage that receives material from this instore  Note the cumulative effect on capacity
\****************************************************************************/
void slurrystore::Scale(double annualAmount)
{
   scalable = true;
   double newCapacity = annualAmount + GetCapacity();
   manurestore::Scale(newCapacity);  //scales capcity and area via manurestore
}

/****************************************************************************\
	Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os,slurrystore s)
{
/* os << setiosflags(ios::left) << setw(40) << s.rT << endl;
   os << setiosflags(ios::left) << setw(40) << "Cover resistance " << s.rCover << endl;*/
   return os;
}



