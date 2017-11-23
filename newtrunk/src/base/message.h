
#ifndef MESSAGE_H
#define MESSAGE_H

#include "common.h"
#include "../tools/convert.h"

class message {
    private:

#ifndef NITROSCAPE
        static ofstream * fw;
        static ofstream * fl;
        static string     WarnFileName;
        static string     LogFileName;
        static int        Warnings;
        static int        Copies;
#else
        ofstream * fw;
        ofstream * fl;
        ofstream * fd;                 // debug text file
        ofstream * ff1;                // field text file 1
        ofstream * ff2;                // field text file 2
        string     WarnFileName;
        string     LogFileName;
        string     DebugFileName;
        string     FieldFile1Name;
        string     FieldFile2Name;
        int        Warnings;
        int        Copies;
#endif

        message & operator = (const message & m);    // Dissable the compilers generation of default assignment operator.

        message(const message & m);    // Dissable the compilers generation of default copy constructor.

        convert conv;

    public:
        message();

        ~message();

        void InitMessage(string WarnFN,
                         string LogFN);

        void Warning(const string st1,
                     const string st2 = "",
                     const string st3 = "");

        void Warning(const string st1,
                     int          anint);

        void WarningWithDisplay(const string st1,
                                const string st2 = "",
                                const string st3 = "");

        void WarningWithDisplay(string       cp1,
                                const double aNumber,
                                string       cp3 = "");

        void FatalError();

        void FatalError(string cp1 = "",
                        string cp2 = "",
                        string cp3 = "");

        void LogEvent(string cp1);

        void LogEvent(string       cp,
                      const double number);

        int NumOfWarnings();

        ofstream * GiveHandle();

        void LogEventWithTime(string cp1);

        void LogEventWithTime(string       cp,
                              const double number);
};


extern message * theMessage;
#endif

