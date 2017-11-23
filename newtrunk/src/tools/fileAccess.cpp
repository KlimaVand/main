/*
 * fileAccess.cpp
 *
 *  Created on: Aug 21, 2009
 *      Author: JOVE
 */

#include "../tools/fileAccess.h"
#include "../base/settings.h"
#ifdef __BCplusplus__

#include <dir.h>

#endif

#include <vector>
#include <string>

#ifndef VS2008

#include <dirent.h>

#else

#include <windows.h>
#include <conio.h>

#endif

#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#ifdef VS2008

#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>

#endif

#ifndef __BCplusplus__

#ifndef VS2008

#include <unistd.h>

#endif

#endif

#ifndef __BCplusplus__

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#endif

fileAccess::~fileAccess() {
    if (files) {
        delete files;
    }
}

fileAccess::fileAccess() {
    files = new fstream();
}

void fileAccess::deleteEveryFileInFolder(string path) {

#ifndef VS2008
    string oldPpath = getCurrentPath();

    changeDir(path);

    DIR * d;

    struct dirent *dir;


    d = opendir(path.c_str());

    cout << "deleting everything in" << path << endl;

    if (d) {
        dir = readdir(d);

        while (dir) {
            string name = dir -> d_name;

            if ((name != ".svn") && (name != ".") && (name != "..")) {
                        remove(name.c_str());

            }

            dir = readdir(d);
        }
        closedir(d);

    }

    changeDir(oldPpath);
#else
    cout << "fileAccess::deleteEveryFileInFolder does not work when using visal studio" << endl;

    exit(99);

    // int iCount = 0;
    // std::string strFilePath;          // Filepath
    // LPCWSTR strPattern;// Pattern
    // std::string strExtension;// Extension
    HANDLE          hFile;              // Handle to file
    WIN32_FIND_DATA FileInformation;    // File information

    // unused string strPattern;//= path + "\\*.*";
    hFile;    // = ::FindFirstFile(strPattern.c_str(), &FileInformation);

    if (hFile != INVALID_HANDLE_VALUE) {
        do {
            string name;

            // =FileInformation.cFileName;
            if (name != ".svn" && name != ".") {
                    cout << "delete" << name << endl;

                    if (name != "..") {
                        string cName = path + "\\" + name;

                        cout << cName << endl;

                        if (remove(cName.c_str()) != 0) {
                            cout << "Error deleting file" << endl;
                        } else {
                            cout << "File successfully deleted" << endl;
                        }


                    }

            }
        } while (::FindNextFile(hFile, &FileInformation));

        // Close handle
        ::FindClose(hFile);
    }
#endif

}

string filenames;

void fileAccess::openFile(string filename,
                          bool   seachback) {
    filenames = filename;

    bool tmp = openFileWithoutExit(filename, seachback);

    if (!tmp) {
        char buffer[200];

#ifdef VS2008
        _getcwd(buffer, 200);
#else
        getcwd(buffer, 200);
#endif

        cout << "Could not open file " << filename << endl;
        cout << "In directory " << buffer << endl;
        cout << "Press any key to terminate." << endl;

        cin.get();
        exit(1);
    }
}

bool fileAccess::openFileWithoutExit(string filename,
	bool                                seachback, bool firstTime) {
    string oldDir = getCurrentPath();

#ifdef VS2008
    _setmaxstdio(2048);
#endif

    while (true) {
        bool   bad   = true;
        bool   fail  = true;
        string place = getCurrentPath() + "/" + filename;

        files -> open(place.c_str(), fstream::in);

        if (files -> bad()) {
            bad = false;
        }

        if (files -> fail()) {
            fail = false;
        }

        if ((!bad) || (!fail)) {
            if (seachback) {

#ifdef unix
                if (getCurrentPath() == "/")  {
                    changeDir(oldDir);

                    return false;
                }
#endif

#ifdef CYGWIN
                if (getCurrentPath() == "/cygdrive")  {
                    changeDir(oldDir);

                    return false;
                }
#endif

#ifndef unix
                if (getCurrentPath().size() <= 3) {
					bool found=false;

					if (firstTime == true)
					{

						changeDir(globalSettings->getInputDirAlternative());
						found = openFileWithoutExit(filename, seachback,false);
					}
					changeDir(oldDir);

					return found;

                }
#endif

                changeDir("..");
            } else {
                return false;
            }
        } else {
            changeDir(oldDir);

            break;
        }
    }

    return true;
}

void fileAccess::openFileWriteString(string filename) {
    openFileWrite(filename);
}

void fileAccess::openFileWrite(string filename) {
    files = new fstream();

    files -> open(filename.c_str(), fstream::out);
    files -> precision(4);

    if (files -> bad()) {
        cout << "could not open file " << filename << " and therefore halting" << endl;

        exit(1);
    }

    if (files -> fail()) {
        cout << "could not open file " << filename << " and therefore halting" << endl;

        exit(1);
    }
}

void fileAccess::closeFile() {
    files -> close();
}

string fileAccess::getLineFile() {
    char   newline[9000] = { '\0' };
    string line          = "nullptr";

    if (!files -> eof()) {
        files -> getline(newline, 9000);

        line = newline;
    } else {
        string tmp = "nullptr";

        return tmp;
    }

#ifdef unix
    if (line[0] != '\0') {
        if (line[line.size() - 1] == '\r') {
            line.resize(line.size() - 1);
        }
    }
#endif

    if (!line.empty()) {
        if (line[0] == 9) {
            line.resize(line.size() - 1);
        }
    }

    // if a line in a text file is empty cygwin returns false

#ifdef CYGWIN
    while (line.size() <= 1)
#else
    while (line.empty())
#endif

    {
        if (!files -> eof()) {
            files -> getline(newline, 9000);

            string string1(newline);

            line = string1;
        } else {
            string tmp = "nullptr";

            return tmp;
        }

#ifdef unix
        if (line[line.size() - 1] == '\r') {
            line.resize(line.size() - 1);
        }
#endif

    }

    int i = line[line.size() - 1];

    if (i == 13) {
        line.resize(line.size() - 1);
    }

    return line;
}

int fileAccess::changeDir(string dirName) {

#ifndef VS2008

#ifdef __BCplusplus__
    return chdir(dirName.c_str());
#else
    string tmp = dirName;

    return chdir(tmp.c_str());
#endif

#else
    string tmp         = dirName;
    int    returnvalue = _chdir(tmp.c_str());

    return returnvalue;
#endif

}

string fileAccess::getCurrentPath() {
    char path[FILENAME_MAX];

#ifdef VS2008
    _getcwd(path, sizeof(path));
#else
    getcwd(path, sizeof(path));
#endif

    return std::string(path);
}

bool fileAccess::endOfFile() {
    return files -> eof();
}

void fileAccess::WriteWithTabS(string input) {
    *files << input << '\t';
}

void fileAccess::WriteWithTab(double input) {
    *files << conv.DoubleToChar(input) + '\t';
}

void fileAccess::WriteNewLine() {
    *files << endl;
}
bool fileAccess::fileExists(string filename)
{
	   string oldDir = getCurrentPath();

#ifdef VS2008
    _setmaxstdio(2048);
#endif

    while (true) {
        bool   found   = true;
        string place = getCurrentPath() + "/" + filename;

        ifstream ifile(filename.c_str());
if (ifile) {
	found=true;
}
else
	found=false;


        if (!found) {
         

#ifdef unix
                if (getCurrentPath() == "/")  {
                    changeDir(oldDir);

                    return false;
                }
#endif

#ifdef CYGWIN
                if (getCurrentPath() == "/cygdrive")  {
                    changeDir(oldDir);

                    return false;
                }
#endif

#ifndef unix
                if (getCurrentPath().size() <= 3) {
                    changeDir(oldDir);

                    return false;
                }
#endif

                changeDir("..");
          
        } else {
            changeDir(oldDir);

            break;
        }
    }

    return true;
}
