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
class fileAccess
{
private:
	fstream * files;
	convert conv;

public:

	fileAccess();
	~fileAccess();
	fileAccess(char * filename);
   void deleteEveryFileInFolder(string path);
	void openFile(char * filename, bool seachback=true);
	void openFile(string filename, bool seachback=true);
	bool openFileWithoutExit(string filename, bool seachback=true);
	bool openFileWithoutExit(char * filename, bool seachback=true);
	void openFileWrite(char * filename);
	void openFileWriteString(string filename);

	void closeFile();
	string getLineFile();
	int changeDir(string dirName);
	string getCurrentPath();
	void ignore2000();
	bool endOfFile();
	bool NextSymbolEOF();
	void WriteWithTabS(string input);
	void WriteWithTab(double input);
	void WriteNewLine();
	void WriteS(string input);
	void Write(double input);
   void WriteNothing(string input);
	void WriteNothing(double input);
	fstream * getFilePointer(){return files;}
	double GetDoubleFromFile();
	int GetIntFromFile();
	int CreateDir(string directoryName);
};
#endif /* FILEACCESS_H_ */
