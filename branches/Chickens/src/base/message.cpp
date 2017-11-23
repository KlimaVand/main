
/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "message.h"
#include "bstime.h"
#include <string.h>
//--------------- Initialize static members ------------------------------------
#ifndef NITROSCAPE
	ofstream * message::fw=NULL;
	ofstream * message::fl=NULL;
	ofstream * message::fd=NULL;
	ofstream * message::ff1=NULL;
	ofstream * message::ff2=NULL;
	char * message::WarnFileName=NULL;
	char * message::LogFileName=NULL;
	char * message::DebugFileName=NULL;
	char * message::FieldFile1Name=NULL;
	char * message::FieldFile2Name=NULL;
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
	fw=NULL;
	fl=NULL;
	fd=NULL;
	ff1=NULL;
	ff2=NULL;
	WarnFileName=NULL;
	LogFileName=NULL;
	DebugFileName=NULL;
	FieldFile1Name=NULL;
	FieldFile2Name=NULL;
	Warnings=0;
#endif
}

message::~message()
{
	fw->close();
	fl->close();
	fd->close();
	ff1->close();
	ff2->close();
	if (fw)
   {
		delete fw;
		fw=NULL;
		delete fl;
      fl=NULL;
		delete WarnFileName;
		delete LogFileName;
      delete DebugFileName;
	}
	Copies--;
   if (fd)
   {
   	delete fd;
      fd=NULL;
   }
   if (ff1)
   {
   	delete ff1;
      ff1=NULL;
      delete FieldFile1Name;
   }
   if (ff2)
   {
   	delete ff2;
      ff2=NULL;
      delete FieldFile2Name;
   }
}

char * message::AssignString(char * c)
{
	char * ch=(char*)::operator new(strlen(c)+1);
	strcpy(ch,c);
	return ch;
}

// Must be called by simulation start. Can only be called once.
void message::InitMessage(char * WarnFN,char * LogFN, char *DebugFN, char *FieldFile1FN, char *FieldFile2FN)
{
   Warnings = 0;
	if (!fw)
   {
		WarnFileName=AssignString(WarnFN);
		LogFileName=AssignString(LogFN);
		DebugFileName=AssignString(DebugFN);
		fw = new ofstream(WarnFileName,ios::out);
	    *fw << "FASSET v2.1" << endl;
			fl = new ofstream();
//			fl = new fstream(LogFileName,ios::out);
		fl->open(LogFileName,ios::out);
	//	cout << LogFileName << endl;
//		if (fstream::failbit!=0)
	//		cout << "help";
      *fl << "FASSET v2.1" << endl;
		fd = new ofstream(DebugFileName,ios::out);
		ff1 = new ofstream(FieldFile1FN,ios::out);
		ff2 = new ofstream(FieldFile2FN,ios::out);
//      *fd << "FASSET v2.1" << endl;
      *ff1 << "day" << "," << "month" << "," << "year" << "," << "fieldid" << "," << "drainage" <<
           "," << "leaching" << "," << "fieldarea" << endl;
      *ff2 << "day" << "," << "month" << "," << "year" << "," << "fieldid" << "," << "drainage" <<
           "," << "leaching" << "," << "fieldarea" << endl;
		fw->close();
		fl->close();
		fd->close();
		ff1->close();
		ff2->close();
	}
	else
		FatalError("message::InitMessage called twice");
}


void message::Warning(const char * cp1,const char * cp2,const char * cp3)
{
	if (fw)
   {
		fw->open(WarnFileName,ios::app);
		*fw << cp1 << cp2 << cp3 << " (" << theTime << ")" << endl;
		fw->close();
		Warnings++;
	}
	else
		FatalError("message::Warning called prior to initialization");
}

void message::Warning(const string st1, const string st2, const string st3)
{
	Warning((char*) st1.c_str(),(char*) st2.c_str(),(char*) st3.c_str());
}

void message::Warning(const char * cp1,const double aNumber,const char * cp3)
{
	char NumberString[40];
	sprintf(NumberString, " %f ", aNumber);
	Warning(cp1,NumberString,cp3);
}

void message::WarningWithDisplay(const char * cp1,const char * cp2,const char * cp3)
{
   Warning(cp1,cp2,cp3);
   cout << endl << cp1 << cp2 << cp3 << " (" << theTime << ")" << endl;
}

void message::WarningWithDisplay(const string st1, const string st2, const string st3)
{
	WarningWithDisplay((char*) st1.c_str(),(char*) st2.c_str(),(char*) st3.c_str());
}

void message::Warning(const string cp1,const int anint)
{
	if (fw)
   {
		fw->open(WarnFileName,ios::app);
		*fw << cp1 << anint << " (" << theTime << ")" << endl;
		fw->close();
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

void message::FatalError(const char * cp1,const char * cp2,const char * cp3)
{
	cout << endl << "Fatal error: " << cp1 << cp2 << cp3 << endl;
   cout << " (" << theTime << ")" << endl;
//	cout << "Program will terminate. Press any key." << endl;
	cout << "Press 'Y' to terminate, any other key to continue." << endl; // While debugging
#ifndef __BCplusplus__
	char ch=getchar();
#else
   char ch=getch();
#endif
   if (ch=='y' || ch=='Y' )
   {
		fd->close();
		fl->close();
      exit(99);
   }
}

void message::LogEvent(const char * cp)
{
	if (fl)
   {
		fl->open(LogFileName,ios::app);
		*fl << cp << endl;
		fl->close();
	}
	else
		FatalError("message::LogEvent called prior to initialization");
}

void message::LogEvent(const char * cp, double number)
{
	if (fl)
   {
		fl->open(LogFileName,ios::app);
		*fl << cp << number << endl;
		fl->close();
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
	if (fl)
		fl->open(LogFileName,ios::app);
	else
		FatalError("message::GiveHandle called prior to initialization");
	return fl;
}

// Must allways be called after a call to "GiveHandle"
void message::CloseHandle()
{
	if (fl)
   {
		fl->close();
	}
	else
		FatalError("message::CloseHandle called prior to initialization");
}

void message::LogEventWithTime(const char * cp1)
{
	if (fl)
   {
		fl->open(LogFileName,ios::app);
		*fl << theTime << " ";
		fl->close();
      LogEvent(cp1);
	}
	else
		FatalError("message::LogEvent called prior to initialization");
}

void message::LogEventWithTime(const char * cp, const double number)
{
	if (fl)
   {
		fl->open(LogFileName,ios::app);
		*fl << theTime << " ";
		fl->close();
      LogEvent(cp, number);
	}
	else
		FatalError("message::LogEvent called prior to initialization");
}




