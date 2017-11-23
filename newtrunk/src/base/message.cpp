
#include "message.h"
#include "bstime.h"
#include <string.h>
#include "../tools/fileAccess.h"
#include "../base/settings.h"

// --------------- Initialize static members ------------------------------------

#ifndef NITROSCAPE
ofstream * message::fw           = nullptr;
ofstream * message::fl           = nullptr;
string     message::WarnFileName = "";
string     message::LogFileName  = "";
int        message::Warnings     = 0;
int        message::Copies       = 0;
#endif

message::message() {
    Copies++;

#ifndef NITROSCAPE
    if (Copies != 1) {
        FatalError("message::message - Only one instance of this class is allowed");
    }
#else
    fw             = nullptr;
    fl             = nullptr;
    fd             = nullptr;
    ff1            = nullptr;
    ff2            = nullptr;
    WarnFileName   = "";
    LogFileName    = "";
    DebugFileName  = "";
    FieldFile1Name = "";
    FieldFile2Name = "";
    Warnings       = 0;
#endif

}

message::~message() {
    fw -> close();
    fl -> close();

    if (fw) {
        delete fw;

        fw = nullptr;

        delete fl;

        fl = nullptr;
    }

    Copies--;
}

// Must be called by simulation start. Can only be called once.
void message::InitMessage(string WarnFN,
                          string LogFN) {
    Warnings = 0;

    if (!fw) {
        WarnFileName = WarnFN;
        LogFileName  = LogFN;
        fw           = new ofstream(WarnFileName, ios::out);

        *fw << globalSettings -> version << endl;

        fl = new ofstream();

        fl -> open(LogFileName.c_str(), ios::out);

        *fl << globalSettings -> version << endl;
    } else {
        FatalError("message::InitMessage called twice");
    }
}

void message::Warning(string cp1,
                      string cp2,
                      string cp3) {
    if (fw) {
        *fw << cp1 << cp2 << cp3 << " (" << theTime << ")" << endl;

        Warnings++;
    } else {
        FatalError("message::Warning called prior to initialization");
    }
}

void message::WarningWithDisplay(string cp1,
                                 string cp2,
                                 string cp3) {
    Warning(cp1, cp2, cp3);

 //   cout << endl << cp1 << cp2 << cp3 << " (" << theTime << ")" << endl;
}

void message::Warning(const string cp1,
                      const int    anint) {
    if (fw) {
        *fw << cp1 << anint << " (" << theTime << ")" << endl;

        Warnings++;
    } else {
        FatalError("message::Warning called prior to initialization");
    }
}

void message::WarningWithDisplay(string       cp1,
                                 const double aNumber,
                                 string       cp3) {
    WarningWithDisplay(cp1, conv.DoubleToChar(aNumber), cp3);
}

void message::FatalError() {
    FatalError("", "", "");
}

void message::FatalError(string cp1,
                         string cp2,
                         string cp3) {    cout << endl << "Fatal error: " << cp1 << cp2 << cp3 << endl;
    cout << " (" << theTime << ")" << endl;
    cout << "Press 'Y' to terminate, any other key to continue." << endl;    // While debugging

#ifndef __BCplusplus__
    char ch = getchar();
#else
    char ch = getch();
#endif

    if ((ch == 'y') || (ch == 'Y')) {
        fl -> close();
        fw -> close();
        exit(99);
    }
}

void message::LogEvent(string cp) {
    if (fl) {
        *fl << cp << endl;
    } else {
        FatalError("message::LogEvent called prior to initialization");
    }
}

void message::LogEvent(string cp,
                       double number) {
    if (fl) {
        *fl << cp << number << endl;
    } else {
        FatalError("message::LogEvent called prior to initialization");
    }
}

int message::NumOfWarnings() {
    return Warnings;
}

// Allows "streaming" of logfile-output.
ofstream * message::GiveHandle() {
    if (!fl) {
        FatalError("message::GiveHandle called prior to initialization");
    }

    return fl;
}

void message::LogEventWithTime(string cp1) {
    if (fl) {
        *fl << theTime << " ";

        LogEvent(cp1);
    } else {
        FatalError("message::LogEvent called prior to initialization");
    }
}

void message::LogEventWithTime(string       cp,
                               const double number) {
    if (fl) {
        *fl << theTime << " ";

        LogEvent(cp, number);
    } else {
        FatalError("message::LogEvent called prior to initialization");
    }
}
