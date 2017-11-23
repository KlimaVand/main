
/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "message.h"

#include <string.h>
#include "../tools/fileAccess.h"
#include "../base/settings.h"
//--------------- Initialize static members ------------------------------------
#ifndef NITROSCAPE
	ofstream * message::fw=nullptr;
	ofstream * message::fl=nullptr;
	char * message::WarnFileName=nullptr;
	char * message::LogFileName=nullptr;

	int message::Warnings=0;
	int message::Copies=0;
#endif
//------------------------------------------------------------------------------

message::message()
{
	Copies++;
#ifndef NITROSCAPE
	if (Copies!=1)
		FatalError("message::message - Only one instance of this class is allowed");
#else
	fw=nullptr;
	fl=nullptr;
	fd=nullptr;
	ff1=nullptr;
	ff2=nullptr;
	WarnFileName=nullptr;
	LogFileName=nullptr;
	DebugFileName=nullptr;
	FieldFile1Name=nullptr;
	FieldFile2Name=nullptr;
	Warnings=0;
#endif
}

message::~message()
{
	fw->close();
	fl->close();


	if (fw)
   {
		delete fw;
		fw=nullptr;
		delete fl;
      fl=nullptr;
		delete WarnFileName;
		delete LogFileName;

	}
	Copies--;


}

char * message::AssignString(char * c)
{
	char * ch=(char*)::operator new(strlen(c)+1);
	strcpy(ch,c);
	return ch;
}

// Must be called by simulation start. Can only be called once.
void message::InitMessage(char * WarnFN,char * LogFN)
{
   Warnings = 0;
	if (!fw)
   {
		WarnFileName=AssignString(WarnFN);
		LogFileName=AssignString(LogFN);

		fw = new ofstream(WarnFileName,ios::out);
	    *fw << globalSettings->version << endl;
			fl = new ofstream();
//			fl = new fstream(LogFileName,ios::out);
		fl->open(LogFileName,ios::out);
	//	cout << LogFileName << endl;
//		if (fstream::failbit!=0)
	//		cout << "help";
      *fl <<globalSettings->version <<  endl;

//      *fd << "FASSET v2.1" << endl;



	}
	else
		FatalError("message::InitMessage called twice");
}


void message::Warning(const char * cp1,const char * cp2,const char * cp3)
{
	if (fw)
   {

		*fw << cp1 << cp2 << cp3 << " (" <<  ")" << endl;

		Warnings++;
	}
	else
		FatalError("message::Warning called prior to initialization");
}

void message::Warning(const string st1, const string st2, const string st3)
{
	Warning((char*) st1.c_str(),(char*) st2.c_str(),(char*) st3.c_str());
}



void message::WarningWithDisplay(const char * cp1,const char * cp2,const char * cp3)
{
   Warning(cp1,cp2,cp3);
   cout << endl << cp1 << cp2 << cp3 << " (" << ")" << endl;
}

void message::WarningWithDisplay(const string st1, const string st2, const string st3)
{
	WarningWithDisplay((char*) st1.c_str(),(char*) st2.c_str(),(char*) st3.c_str());
}

void message::Warning(const string cp1,const int anint)
{
	if (fw)
   {

		*fw << cp1 << anint << " (" <<  ")" << endl;

		Warnings++;
	}
	else
		FatalError("message::Warning called prior to initialization");
}
void message::WarningWithDisplay(const char * cp1,const double aNumber,const char * cp3)
{
	char NumberString[40];
	sprintf(NumberString, " %f ", aNumber);
	WarningWithDisplay(cp1,NumberString,cp3);
}
void message::FatalError(string cp1,string cp2,string cp3)
{
	FatalError((char*) cp1.c_str(),(char*) cp2.c_str(),(char*) cp3.c_str());
}

void message::FatalError(const char * cp1,const char * cp2,const char * cp3)
{

	cout << endl << "Fatal error: " << cp1 << cp2 << cp3 << endl;
   cout << " (" << ")" << endl;
//	cout << "Program will terminate. Press any key." << endl;
	cout << "Press 'Y' to terminate, any other key to continue." << endl; // While debugging
#ifndef __BCplusplus__
	char ch=getchar();
#else
   char ch=getch();
#endif
   if (ch=='y' || ch=='Y' )
   {

		fl->close();
		fw->close();

      exit(99);
   }
}

void message::LogEvent(const char * cp)
{
	if (fl)
   {
		*fl << cp << endl;
	}
	else
		FatalError("message::LogEvent called prior to initialization");
}

void message::LogEvent(const char * cp, double number)
{
	if (fl)
   {

		*fl << cp << number << endl;

	}
	else
		FatalError("message::LogEvent called prior to initialization");
}

int message::NumOfWarnings()
{
	return Warnings;
}

// Allows "streaming" of logfile-output.
ofstream * message::GiveHandle()
{
	if (!fl)
		FatalError("message::GiveHandle called prior to initialization");
	return fl;
}



void message::LogEventWithTime(const char * cp1)
{
	if (fl)
   {

		*fl <<  " ";

      LogEvent(cp1);
	}
	else
		FatalError("message::LogEvent called prior to initialization");
}

void message::LogEventWithTime(const char * cp, const double number)
{
	if (fl)
   {

		*fl <<  " ";
      LogEvent(cp, number);
	}
	else
		FatalError("message::LogEvent called prior to initialization");
}




