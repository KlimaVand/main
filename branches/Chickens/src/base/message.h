/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef MESSAGE_H
#define MESSAGE_H

#include "common.h"

class message
{
private:
#ifndef NITROSCAPE
	static ofstream * fw;
	static ofstream * fl;
	static ofstream * fd; //debug text file
	static ofstream * ff1; //field text file 1
	static ofstream * ff2; //field text file 2
	static char * WarnFileName;
	static char * LogFileName;
	static char * DebugFileName;
	static char * FieldFile1Name;
	static char * FieldFile2Name;
	static int Warnings;
	static int Copies;
#else
	 ofstream * fw;
	 ofstream * fl;
	 ofstream * fd; //debug text file
	 ofstream * ff1; //field text file 1
	 ofstream * ff2; //field text file 2
	 char * WarnFileName;
	 char * LogFileName;
	 char * DebugFileName;
	 char * FieldFile1Name;
	 char * FieldFile2Name;
	 int Warnings;
	 int Copies;
#endif

	message& operator=(const message& m); // Dissable the compilers generation of default assignment operator.
	message(const message& m);            // Dissable the compilers generation of default copy constructor.

	char * AssignString(char * c);

public:
	message();
	~message();
	void InitMessage(char * WarnFN,char * LogFN, char *DebugFN, char *FieldFile1FN, char *FieldFile2FN);

	void Warning(const char * cp1,const char * cp2="",const char * cp3="");
   void Warning(const string st1, const string st2="", const string st3="");
   void Warning(const string st1, int anint);
   void Warning(const char * cp1, const double aNumber, const char * cp3="");
	void WarningWithDisplay(const char * cp1,const char * cp2="",const char * cp3="");
   void WarningWithDisplay(const string st1, const string st2="", const string st3="");
	void WarningWithDisplay(const char * cp1, const double aNumber, const char * cp3="");
	void FatalError(const char * cp1="",const char * cp2="",const char * cp3="");
	void LogEvent(const char * cp1);
   void LogEvent(const char * cp, const double number);
	int NumOfWarnings();
	ofstream * GiveHandle();
	void CloseHandle();


	void LogEventWithTime(const char * cp1);
   void LogEventWithTime(const char * cp, const double number);


};

extern message * theMessage;

#endif
