/*
 * fileAccess.h
 *
 *  Created on: Aug 21, 2009
 *      Author: JOVE
 */

#ifndef FILEACCESS_H_
#define FILEACCESS_H_

#include "convert.h"
#include <fstream>

using namespace std;

class fileAccess {
    private:
        fstream * files;
        convert   conv;

    public:
        fileAccess();

        ~fileAccess();

        fileAccess(string filename);

        void deleteEveryFileInFolder(string path);

        void openFile(string filename,
                      bool   seachback = true);

        bool openFileWithoutExit(string filename,
			bool   seachback = true, bool firstTime=true);

        void openFileWrite(string filename);

        void openFileWriteString(string filename);

        void closeFile();

        string getLineFile();

        int changeDir(string dirName);

        string getCurrentPath();

        bool endOfFile();

        void WriteWithTabS(string input);
		bool fileExists(string filename);

        void WriteWithTab(double input);

        void WriteNewLine();

        fstream * getFilePointer() {
            return files;
        }
};
#endif /* FILEACCESS_H_ */

