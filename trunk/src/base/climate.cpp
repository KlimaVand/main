/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Modificed November 1997 JBE, included Makkink calculations and corrected some errors
Modified June 2002 JBE to an fit structure with an daily update followed by a direct
request of the parameters
Modified sept 2002 NJH to read data files downloaded from DIAS weather station database
\****************************************************************************/
#include "common.h"
#include "climate.h"
#include "message.h"
#include "controlParameters.h"

#include "nixcbits.h"
#include "../tools/fileAccess.h"
#include "../base/commonData.h"
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include "../base/bstime.h"
const int AccmonthLength[12]={0,31,59,90,120,151,181,212,243,273,304,334};   // accumulated days
const double OldAdjust[13]={0,1.21,1.22,1.22,1.18,1.15,1.14,1.12,1.11,1.13,1.14,1.16,1.19};   // accumulated days
const double NewAdjust[13]={0,1.41,1.42,1.35,1.24,1.13,1.11,1.10,1.10,1.11,1.14,1.23,1.37};   // accumulated days
fileAccess hd;

/****************************************************************************\
Destructor
\****************************************************************************/
climate::~climate()
{
   if (file)
   {
	file->close();

   }
   if (TemperatureFile)
   	TemperatureFile.close();
}

/****************************************************************************\
Reads a single line from the ascii-file
\****************************************************************************/
void climate::ReadLine()
{
   co2 = 0; // BMP added October 2007 to remove stochastic behaviour
	humidity=0.0;
	 std::string nameOfItem;
	  convert conv;
	 stringstream * totalSString=nullptr;
	 int date;
	 bsTime toJD;
   switch (filetyp)
   {
	   case 0:	 // CLM filer
			if (DailyCO2Values)
//				f1 >> yr >> mnth >> d >> tmin >> tmax >> rad >> precip >> epot >> co2;
			*file >> yr >> mnth >> d >> tmin >> tmax >> rad >> precip >> epot >> co2;
         else
//             f1 >> yr >> mnth >> d >> tmin >> tmax >> rad >> precip >> epot;
            *file >> yr >> mnth >> d >> tmin >> tmax >> rad >> precip >> epot;
	      tmean = (tmin+tmax)/2;
	      windVelocity = 4.0;
//			file->getline(buffer,strlength);
			file->getline(buffer,strlength);
         break;
		case 1:	// KLM filer (both Penman and Makkink)
      case 2:
			double vel, date;
//			f1 >> date >> tmin >> tmax >> tmean >> precip >> rad  >> humidity >> vel;
			*file >> date >> tmin >> tmax >> tmean >> precip >> rad  >> humidity >> vel;
	      d = (int) fmod(date,100);
	      mnth = (int) fmod(floor(date/100),100);
	      yr = 1900 + (int) fmod(floor(date/10000),100);
	      tmin = tmin/10;
	      tmax = tmax/10;
	      tmean = tmean/10;
	      precip = precip/10;
	      rad = rad/10;
	      humidity = humidity/100;
	      windVelocity = GetWindAtHeight(10.0,vel,2.0,0.01);                 // calculates the velocity at 2m (vel is at 10m)
			file->getline(buffer,strlength);
         break;
      case 3:	// DAISY climate files
         char c[20];
         file->get(c,3);
         yr = atoi(c)+1900;
         file->get(c,3);
         mnth = atoi(c);
         file->get(c,3);
         d = atoi(c);
         file->get(c,7);
         rad = atof(c);
         rad = rad*24.0*60.0*60.0/1000000.0; // Conversion Wm-2 to Jm-2. BMP added 4-6-1999
         file->get(c,7);
         tmean = tmax = tmin = atof(c);
         file->get(c,7);
         precip = atof(c);
         file->get(c,7);
         epot = atof(c);
         vel = 0.0;                          // Wind velocity
         file->getline(buffer,strlength);
         break;
      case 4:	// DAISY climate files but with year 2000 compatibility and extra column with windspeed
         char buf[20];
         file->get(buf,5);
         yr = atoi(buf);
         file->get(buf,3);
         mnth = atoi(buf);
         file->get(buf,3);
         d = atoi(buf);
         file->get(buf,7);
         rad = atof(buf);
         rad = rad*24*60*60/1000000; // Conversion Wm-2 to Jm-2. BMP added 4-6-1999
         file->get(buf,7);
         tmean = tmax = tmin = atof(buf);
         file->get(buf,7);
         precip = atof(buf);
         file->get(buf,7);
         epot = atof(buf);
         file->get(buf,7);
         vel = atof(buf);                  // wind velocity
	      windVelocity = GetWindAtHeight(10.0,vel,2.0,0.01);                 // calculates the velocity at 2m (vel is at 10m)
         file->getline(buffer,strlength);
         break;
      case 5:	// Download format from Intranet
		  
		
		  file->getline(buffer,strlength);
		
		//  std::string str=buffer;
		  yr=(buffer[12]-48)*1000+(buffer[13]-48)*100+(buffer[14]-48)*10+(buffer[15]-48);
		 mnth =(buffer[17]-48)*10+(buffer[18]-48);
		 d=(buffer[20]-48)*10+(buffer[21]-48);
		 totalSString=new stringstream( buffer );
	     *totalSString >>nameOfItem;
	     *totalSString >>nameOfItem;
		 *totalSString >>nameOfItem;
		
		 humidity=conv.StringToDouble(nameOfItem);
		 *totalSString >>nameOfItem;
		 tmin=conv.StringToDouble(nameOfItem);
		 *totalSString >>nameOfItem;
		 tmax=conv.StringToDouble(nameOfItem);
		 *totalSString >>nameOfItem;
		 tmean=conv.StringToDouble(nameOfItem);
		 *totalSString >>nameOfItem;
		 precip =conv.StringToDouble(nameOfItem);
		 *totalSString >>nameOfItem;
		 windVelocity =conv.StringToDouble(nameOfItem);
		 *totalSString >>nameOfItem;
		 rad=conv.StringToDouble(nameOfItem);
		 *totalSString >>nameOfItem;
		 epot=conv.StringToDouble(nameOfItem);
         break;
	   case 6:	 // files from MARS system
         double dum;
         *file >> yr >> mnth >> d >> rad >> tmax >> tmin >> precip >> dum >> windVelocity;
	      tmean = (tmin+tmax)/2;
         epot=MakkinkEvaporation(rad,tmean);
         file->getline(buffer,strlength);
         break;
		case 7:
		 totalSString=new stringstream( buffer );
	   *totalSString >>nameOfItem;
		 yr=conv.StringToDouble(nameOfItem);
		 *totalSString >>nameOfItem;
		 date=conv.StringToDouble(nameOfItem);
		 toJD.SetTime(yr,date);
		mnth= toJD.GetMonth();
		  *totalSString >>nameOfItem;
		  tmin=conv.StringToDouble(nameOfItem);
		 *totalSString >>nameOfItem;
		 tmax=conv.StringToDouble(nameOfItem);
		 *totalSString >>nameOfItem;
		 precip =conv.StringToDouble(nameOfItem);
		  *totalSString >>nameOfItem;
		 rad=conv.StringToDouble(nameOfItem);
	   file->getline(buffer,strlength);
		 break;
	}
   if(totalSString!=nullptr)
	   delete totalSString;
   if (rad<0.0)
   {
   	theMessage->WarningWithDisplay("climate::Readline - incoming radiation cannot be negative");
      rad=0.0;
   }
   if (precip<0.0)
   {
   	theMessage->WarningWithDisplay("climate::Readline - precipitation is negative");
      precip=0.0;
   }
   if ((d==1)&&(mnth==1))
      ResetdegreeDays();
   else
      degreeDays+=tmean;
}

/****************************************************************************\
Gets current soil temperature array (NOTE: this procedure assumes same number
of arrays in measurements and simulation)
\****************************************************************************/
void climate::UpdateMeasuredSoilTemperature()
{
   enum {RecordLength=1000};
	char buf[RecordLength];

	bsTime MeasureDate;
   string DateString;

	TemperatureFile >> DateString;
   MeasureDate.SetTime(DateString);
   while (TemperatureFile && MeasureDate<theTime)
   {
		TemperatureFile.getline(buf,RecordLength);
   	TemperatureFile >> DateString;
   	MeasureDate.SetTime(DateString);
	}
}

/****************************************************************************\
\****************************************************************************/
double climate::GetNextSoilTemperature()
{
	double temperature;
   TemperatureFile >> temperature;
   return temperature;
}

/****************************************************************************\
Initializes the climate object.
Can be used an arbitrary number of times in one simulation if needed.
Parameters:
   filename   name of ascii-file with climate data
   filetype   probably temporarely
   firstobs   states the number of headerlines without data in climate file
\****************************************************************************/
void climate::InitClimate(char * filename)
{
   Circular = false;
	CO2Concentration=377.0;
	latitude=56.5;

	NitrogenInRain=0.002;
   commonData data;
   data.setCritical(false);
   data.readFile("environ.dat");

   data.FindItem("NitrogenInRain",&NitrogenInRain);

  	DailyCO2Values = false;
   data.FindItem("CO2Concentration",&CO2Concentration);
   data.FindItem("latitude",&latitude);                                         // Denmark
	int correct=0;
	data.FindItem("PrecipitationCorrection",&correct);
   for (int i=0;i<13;i++)
   	CorrectionFactor[i] = 1.0;
    if (CO2Concentration==-1)
    DailyCO2Values = true;

    if (correct==1)
      	for (int i=0;i<13;i++)
   			CorrectionFactor[i] = NewAdjust[i];
    if (correct==2)
      	for (int i=0;i<13;i++)
   			CorrectionFactor[i] = OldAdjust[i];

   if (strlen(climateDirectory))
   hd.changeDir(climateDirectory);

   if(file!=nullptr)
   {
	   if(file->is_open())
		file->close();
   }
	strcpy(filenam,filename);
	hd.openFile(filename);
	file=hd.getFilePointer();
	if(filetyp==5)
	{
			file->getline(buffer,strlength);
	}
	else if(filetyp==7)
	{
		file->getline(buffer,strlength);
		file->getline(buffer,strlength);
	}


   bsTime TempDate;
   FirstDate.SetTime(1,1,2050);
   LastDate.SetTime(1,1,1900);
   YearlyAverage = 0.0;
   int i=0;
   while (!file->eof())
   {
		ReadLine();
      i++;
      YearlyAverage += tmean;
      TempDate.SetTime(d,mnth,yr);
   	if (yr>0 && FirstDate>TempDate)  FirstDate = TempDate;
      if (yr>0 && LastDate<TempDate)  LastDate = TempDate;
   }
   if (i>0) YearlyAverage = YearlyAverage/i;

   file->close();

   hd.openFile(filename);
   	file=hd.getFilePointer();
   ReadLine();

   if (TemperatureFile)
   	TemperatureFile.close();
#ifndef _STANDALONE
   if (theControlParameters->GetMeasuredSoilTemperature())
   	TemperatureFile.open("soiltemp.txt",ios::in);
#endif
}

/****************************************************************************\
Initializes the climate object.
Can be used an arbitrary number of times in one simulation if needed.
Parameters:
   filename   name of ascii-file with climate data
   filetype   probably temporarely
   firstobs   states the number of headerlines without data in climate file
\****************************************************************************/
void climate::InitClimate(char *aDirectory, char * filename)
{

  strcpy(climateDirectory,aDirectory);
  hd.changeDir(climateDirectory);


   	InitClimate(filename);         // CLM files
}

/****************************************************************************\
\****************************************************************************/
double climate::GetCO2Concentration()
{
   if (CO2Concentration > 2000 || CO2Concentration < 100)
      theMessage->FatalError("climate::GetCO2Concentration - CO2 concentration beyond reasonable borders");
   return CO2Concentration;
}

/****************************************************************************\
\****************************************************************************/
double climate::VaporPress(double temp)
{
	double ret_val;
   if (temp<0)
   	ret_val = 6.11*exp(log(10.)*9.5*temp/(temp+265.5));
   else
   	ret_val = 6.11*exp(log(10.)*7.5*temp/(temp+237.3));
   return ret_val;
}

/****************************************************************************\
\****************************************************************************/
double climate::DeltaVaporPress(double temp)
{
	double ret_val;
   ret_val = VaporPress(temp)*(4098.0259/((temp+237.3)*(temp+237.3)));
   return ret_val;
}

/****************************************************************************\
\****************************************************************************/
double climate::DayAngle()
{
   int DayNo=theTime.GetDayInYear();
	if (((theTime.GetYear() % 4)==0) && (DayNo>59))
      DayNo--;
	return 2*3.1415926*(DayNo-1)/theTime.daysInYear(0);
}

/****************************************************************************\
\****************************************************************************/
double climate::SunDeclination()
{
	double Omega=DayAngle();
	return 0.006918-0.399912*cos(Omega)+0.070257*sin(Omega)
		  -0.006758*cos(2*Omega)+0.000907*sin(2*Omega)
		  -0.002697*cos(3*Omega)+0.001480*sin(3*Omega);
}

/****************************************************************************\
 Returns length of day at a given latitude
\****************************************************************************/
double climate::PhotoPeriod()
{
   double phi = 3.1415926;
	double Dec=SunDeclination();
	double Lat=latitude*3.1415926/180;;
	return 24.0/phi*acos(sin(-4*phi/180)/(cos(Lat)*cos(Dec))-tan(Lat)*tan(Dec));
}

/****************************************************************************\
Calculates the potential evaporation after a modified Pennman method.
See: H.E. Mikkelsen & J.E. Olesen: Sammenligning af metoder til bestemmelse
af potentiel fordampning. Statens Planteavlsfors�g. Beretning nr. S 2157. 1991
globalRadiation = global radiation in MJ/m**2/day,
temp = air temperature in Celsius at 2m,
windSpeed = windspeed at 2m,
humidity =  relative humidity as a proportion
result = Evap in mm/day,
\****************************************************************************/
double climate::PenmanEvaporation(double globalRadiation,double temp,double windSpeed,double humidity)
{
   double angle=DayAngle();
   const double evpHtWat=2.465;   //MJ/kg
   const double psykrConst=0.667;  //hPa/Celsius
   double B0=0.114-0.0659*cos(angle)+0.0451*sin(angle)-0.0851*cos(2*angle)-0.0031*sin(2*angle);
   double B1=0.083-0.4629*cos(angle)+0.0354*sin(angle)-0.0914*cos(2*angle)+0.0286*sin(2*angle);
   double B2=0.963-0.1707*cos(angle)+0.0123*sin(angle)-0.0419*cos(2*angle)+0.0381*sin(2*angle);
   double result =B0+B1*(DeltaVaporPress(temp)*globalRadiation/(evpHtWat*(DeltaVaporPress(temp)+psykrConst)))
                  +B2*(psykrConst*0.263*(0.5+0.54*windSpeed)*VaporPress(temp)*(1-humidity)
                      /(DeltaVaporPress(temp)+psykrConst));
   return result;
}

/****************************************************************************\
Calculates the potential evaporation after the Makkink method.
See: H.E. Mikkelsen & J.E. Olesen: Sammenligning af metoder til bestemmelse
af potentiel fordampning. Statens Planteavlsfors�g. Beretning nr. S 2157. 1991
\****************************************************************************/
double climate::MakkinkEvaporation(double globalRadiation,double temp)
{
   const double evpHtWat=2.465;
   const double psykrConst=0.667;
   double result =0.7*(DeltaVaporPress(temp)*globalRadiation/
                      (evpHtWat*(DeltaVaporPress(temp)+psykrConst)));
   return result;
}

/****************************************************************************\
Open water evaporation
\****************************************************************************/
double climate::Evaporation(double radiation,double temp,double humidity, double transferCoefficient)
{
   //air density = (mole weight * atm. pressure)/(gas const. * Tair )
   double air_density = (28.96 * 101.3)/(8.314* (temp+273.0));   	// kg per cubic m
   const double specific_heat= 1010.0;                           	// J/kg/K
   const double evpHtWat=2465000.0;                              	// J per kg
   const double psykrConst=66.7;                                 	// Pa per K
   double Delta = DeltaVaporPress(temp) * 100.0;                 	// mult by 100 to convert from mbars/K to Pa/K
   double vp = VaporPress(temp) * 100.0;                          // mult by 100 to convert from mbars to Pa
   double result =(Delta*radiation + air_density * specific_heat* vp * (1-humidity) *
			   transferCoefficient)/((Delta + psykrConst) * evpHtWat);
   return result; 																// result is in kg per square metre per second
}

/****************************************************************************\
Open water evaporation without radiation input (used for indoor losses)
\****************************************************************************/
double climate::Evaporation(double temp,double humidity, double transferCoefficient)
{
	double result = Evaporation(0,temp,humidity, transferCoefficient);
   return result;
}

/****************************************************************************\
preciptation adjustments coefficients
\****************************************************************************/
double climate::AdjustPrecip(int month)
{
	return CorrectionFactor[month];
}


/****************************************************************************\
\****************************************************************************/
void climate::GetClimate(double &temp, double &min_temp,
						 double &max_temp, double &prec, double &radiation,
						 double &EPot, double &windspeed, double &relHumidity)
{
  if (found)
  {
   temp=tmean;
	min_temp=tmin;
	max_temp=tmax;
 	prec=precip;
	radiation=rad;
   EPot = epot;
   relHumidity = humidity;
   windspeed = windVelocity;
  }
  else
  {
   temp=8.0;
	min_temp=8.0;
	max_temp=8.0;
 	prec=0.0;
	radiation=0.0;
   EPot = 0.0;
   relHumidity = 0.0;
   windspeed = 0.0;
   theMessage->WarningWithDisplay("climate::GetClimate - today's weather has not been found yet");
   cout << "";
  }
}

/*
NJH/BMP version --- probably obsolete
Obtains climate data for the current day

void climate::Update()
{
   int year = theTime.GetYear();
   int month = theTime.GetMonth();
   int day = theTime.GetDay();

   if (theTime<FirstDate)
   	year = FirstDate.GetYear();
   if (theTime>LastDate)
   {
   	if (!Circular) theMessage->WarningWithDisplay("Climate::Current date greather than last date - using circular dates!");
      Circular = true;
   	year = FirstDate.GetYear() + ((year-FirstDate.GetYear()) % (LastDate.GetYear()-FirstDate.GetYear()));                 // ensures cirkular referencing if you run
   }
	long int idx=yr*512+mnth*32+d;
	long int searchindex=year*512+month*32+day;
	if (searchindex<idx) {
   	file->close();
		InitClimate(filenam,filetyp,firstob);
		idx=yr*512+mnth*32+d;
	}
	while ((f1) && (idx<searchindex)) {
		ReadLine();
		idx=yr*512+mnth*32+d;
	}

	precip = max(0.0,precip);              // this is nessesary because of potential precip = -1!
   if (filetyp!=3)
   	precip = precip*AdjustPrecip(month);

   switch (EpotCalculationMethod) {
   	case 0:
      case 3:
      	if (epot==-99)
         	epot = MakkinkEvaporation(rad,tmean);
         else
           epot = epot;
	     	break;
	   case 1:
	   	epot = PenmanEvaporation(rad,tmean,windVelocity,humidity);
         break;
      case 2:
      	epot = MakkinkEvaporation(rad,tmean);
         break;
   }
   if (co2>100)
   	CO2Concentration = co2;

   epot = max(epot,0.0);        // do not allow negativ epot

   found=(idx==searchindex);
   if (!found && Circular && month==2 && day>=29)
   	found = true;                 // a fix to ensure correct circular referencing
   if (!found)
   	theMessage->Warning("Climate::DailyUpdate - date not in climate-database: ");

#ifndef _STANDALONE
   if (theControlParameters->GetMeasuredSoilTemperature())
   	UpdateMeasuredSoilTemperature();
#endif
   if ((theTime.GetDay()==1)&&(theTime.GetMonth()==1))
   	degreeDays=0.0;
   if (tmean>0.0)
	   degreeDays+=tmean;
}
*/


/****************************************************************************\
Update climate to current day
Author: JBE
This function should probably facilitate reading of [CO2]
Presently it does not !!!
\****************************************************************************/
void climate::Update()
{
   char buffer[MAX_TXT];
   strcpy(buffer, hd.getCurrentPath().c_str()); //get current directory - this procedure will divert to climate directory
   int year = theTime.GetYear();
   int month = theTime.GetMonth();
   int day = theTime.GetDay();

   if (theTime<FirstDate)
   	year = FirstDate.GetYear()+1;
   if (theTime>LastDate)
   {
   	if (!Circular) theMessage->WarningWithDisplay("Climate::Current date greather than last date - using circular dates!");
      Circular = true;
   	year = FirstDate.GetYear() + ((year-FirstDate.GetYear()) % (LastDate.GetYear()-FirstDate.GetYear()));                   // ensures cirkular referencing if you run
   }
	long int idx=yr*512+mnth*32+d;
	long int searchindex=year*512+month*32+day;
	if (searchindex<idx)
   {
     	file->close();
		InitClimate(filenam);
		idx=yr*512+mnth*32+d;
	}
	while ((file) && (idx<searchindex))
   {
		ReadLine();
		idx=yr*512+mnth*32+d;
	}

	precip = max(0.0,precip);              // this is nessesary because of potential prcp = -1!
   if ((filetyp!=3) && (filetyp!=6))
   	precip = precip*AdjustPrecip(month);

   switch (EpotCalculationMethod)
   {
   	case 0:
      case 3:
      	if (epot==-99)
         	epot = MakkinkEvaporation(rad,tmean);

	     	break;
	   case 1:
	   	epot = PenmanEvaporation(rad,tmean,windVelocity,humidity);
         break;
      case 2:
      	epot = MakkinkEvaporation(rad,tmean);
         break;
   }
   if (co2>100)
   {
      if (co2<2000)
      	CO2Concentration = co2;
      else
         theMessage->FatalError("climate::Update - CO2 concentration at an unrealistic high level");
   }
   epot = max(epot,0.0);        // do not allow negative epot

   found=(idx==searchindex);
   if (!found && Circular && month==2 && day>=29)
   	found = true;                 // a fix to ensure correct circular referencing
   if (!found)
   	theMessage->WarningWithDisplay("Climate::DailyUpdate - date not in climate-database: ");

   if (theControlParameters->GetMeasuredSoilTemperature())
   	UpdateMeasuredSoilTemperature();
   hd.changeDir(buffer);   //revert to previous directory
}

/****************************************************************************\
This function should probably facilitate reading of [CO2]
Presently it does not !!!
\****************************************************************************/
void climate::Update(bsTime *aTime)
{
   char buffer[MAX_TXT];
   strcpy(buffer, hd.getCurrentPath().c_str()); //get current directory - this procedure will divert to climate directory
   int year = aTime->GetYear();
   int month = aTime->GetMonth();
   int day = aTime->GetDay();

   if (*aTime<FirstDate)
   	year = FirstDate.GetYear();
   if (*aTime>LastDate)
   {
   	if (!Circular) theMessage->WarningWithDisplay("Climate::Current date greather than last date - using circular dates!");
      Circular = true;
   	year = FirstDate.GetYear() + ((year-FirstDate.GetYear()) % (LastDate.GetYear()-FirstDate.GetYear()));                 // ensures cirkular referencing if you run
   }
	long int idx=yr*512+mnth*32+d;
	long int searchindex=year*512+month*32+day;
	if (searchindex<idx)
   {
		file->close();
		InitClimate(filenam);
		idx=yr*512+mnth*32+d;
	}
	while ((file) && (idx<searchindex))
   {
		ReadLine();
		idx=yr*512+mnth*32+d;
	}

	precip = max(0.0,precip);              // this is nessesary because of potential precip = -1!
   if ((filetyp>=3)&&(filetyp!=6))
   	precip = precip*AdjustPrecip(month);

   switch (EpotCalculationMethod)
   {
   	case 0:
      case 3:
      	if (epot==-99)
         	epot = MakkinkEvaporation(rad,tmean);

	     	break;
	   case 1:
	   	epot = PenmanEvaporation(rad,tmean,windVelocity,humidity);
         break;
      case 2:
      	epot = MakkinkEvaporation(rad,tmean);
         break;
   }
   if (co2>100)
   {
      if (co2<2000)
      	CO2Concentration = co2;
      else
         theMessage->FatalError("climate::Update - CO2 concentration at an unrealistic high level");
   }
   epot = max(epot,0.0);        // do not allow negativ epot

   found=(idx==searchindex);
   if (!found && Circular && month==2 && day>=29)
   	found = true;                 // a fix to ensure correct circular referencing
   if (!found)
   	theMessage->Warning("Climate::DailyUpdate - date not in climate-database: ");

#ifndef _STANDALONE
   if (theControlParameters->GetMeasuredSoilTemperature())
   	UpdateMeasuredSoilTemperature();
#endif
   if ((theTime.GetDay()==1)&&(theTime.GetMonth()==1))
   	degreeDays=0.0;
   if (tmean>0.0)
	   degreeDays+=tmean;
   hd.changeDir(buffer);   //revert to previous directory
}

/****************************************************************************\
\****************************************************************************/
void climate::CalcAverageClimate()
{
   char buffer[MAX_TXT];
   strcpy(buffer, hd.getCurrentPath().c_str()); //get current directory - this procedure will divert to climate directory
   for (int i=0; i<13; i++)
   {
      normRad[i]=0.0;
      normTemp[i]=0.0;
   }
	bsTime *theDate = new bsTime(FirstDate);
   if ((theDate->GetDay()!=1) && (theDate->GetMonth()!=1))
	   theDate->SetTime(1, 1,theDate->GetYear()+1);
	bsTime *endDate = new bsTime();
   endDate->SetTime(31, 12,theDate->GetYear()+10);
   if (*endDate>LastDate)
   	endDate->SetTime(31, 12,LastDate.GetYear()-1);
   if ((endDate->GetDay()!=31) && (endDate->GetMonth()!=12))
	   endDate->SetTime(31, 12,endDate->GetYear()-1);
   int numYears=endDate->GetYear()-theDate->GetYear();
   if (numYears==0)
	   theMessage->FatalError("Climate::Less than one year of data in climate database");
   if (numYears<5)
   	theMessage->Warning("Climate::average climate based on <5 years of data");
   while (*theDate<=*endDate)
   {
   	double sumRad=0.0, sumTemp=0.0;
      int currentMonth=0, count=0;
      currentMonth=theDate->GetMonth();
      while (theDate->GetMonth()==currentMonth)
      {
         theClimate->Update(theDate);
         sumTemp+=theClimate->tmean;
         sumRad+=theClimate->rad;
         theDate->AddOneDay();
         count++;
      }
      normRad[currentMonth]+=sumRad/count;
      normTemp[currentMonth]+=sumTemp/count;
   	sumRad=0.0, sumTemp=0.0;
      currentMonth=theDate->GetMonth();
	}
   for (int i=1; i<13; i++)
   {
      normRad[i]/=((double)numYears);
      normTemp[i]/=((double)numYears);
   }
	delete theDate;
   delete endDate;

  theClimate->Update();  //reset climate to current date
  hd.changeDir(buffer);   //revert to previous directory
}

/****************************************************************************\
\****************************************************************************/
void climate::CalcAverageClimateForPeriod(bsTime *startDate, int numOfDays, double *aveTemperature, double *aveWindspeed)
{
   char buffer[MAX_TXT];
   strcpy(buffer, hd.getCurrentPath().c_str()); //get current directory - this procedure will divert to climate directory
   int count=0;
	bsTime *theDate = new bsTime(*startDate);
   bsTime *endDate = new bsTime(*startDate);
   endDate->AddDays(numOfDays);
  	double sumWind=0.0, sumTemp=0.0;
   while (*theDate<*endDate)
   {
         theClimate->Update(theDate);
         sumTemp+=theClimate->tmean;
         sumWind+=theClimate->windVelocity;
         theDate->AddOneDay();
         count++;
	}
   *aveWindspeed=sumWind/count;
   *aveTemperature=sumTemp/count;
	delete theDate;
   delete endDate;
   theClimate->Update();  //reset climate to current date
   hd.changeDir(buffer);   //revert to previous directory
};

