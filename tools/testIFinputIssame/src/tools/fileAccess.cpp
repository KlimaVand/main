/*
 * fileAccess.cpp
 *
 *  Created on: Aug 21, 2009
 *      Author: JOVE
 */

#include "../tools/fileAccess.h"
#ifdef __BCplusplus__
#include <dir.h>
#endif
#ifdef BUILDER
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
#include "convert.h"
#include <direct.h>
#ifndef __BCplusplus__
	#ifndef BUILDER
		#ifndef VS2008
		#include <unistd.h>
#endif
	#endif
#endif


fileAccess::fileAccess()
{
}
void fileAccess::deleteEveryFileInFolder(string path)
{
#ifndef VS2008
	string oldPpath=getCurrentPath();
	changeDir(path);
	DIR           *d;
  struct dirent *dir;
  d = opendir(path.c_str());
  cout<<"deleting everything in" << path<<endl;
  if (d)
  {
	  dir = readdir(d);
	  while (dir!=NULL)
    {

    	string name=dir->d_name;

    	if(!name.compare(".svn")==0)
    	{

				if(!name.compare(".")==0)
				{

					if(!name.compare("..")==0)
					{
						remove(name.c_str( ));
					}
				}
       }
    	dir = readdir(d);
       }
   }

  changeDir(oldPpath);
#else
	/*int             iCount          = 0;
  std::string     strFilePath;          // Filepath
  std::string     strPattern;           // Pattern
  std::string     strExtension;         // Extension
  HANDLE          hFile;                // Handle to file
  WIN32_FIND_DATA FileInformation;      // File information


  strPattern = path + "\\*.*";
  hFile = ::FindFirstFile(strPattern.c_str(), &FileInformation);
  if(hFile != INVALID_HANDLE_VALUE)
  {
    do
    {
		string name=FileInformation.cFileName;
		if(!name.compare(".svn")==0)
    	{

				if(!name.compare(".")==0)
				{

					if(!name.compare("..")==0)
					{
						char * cName=new char[200];
						strcpy(cName,path.c_str());
						strcat(cName,"\\");
						strcat(cName,name.c_str());

						if( remove( cName) != 0 )
							cout<< "Error deleting file" <<endl;
						else
							cout<<"File successfully deleted" <<endl;
						delete cName;
					}
				}
       }


    } while(::FindNextFile(hFile, &FileInformation) == TRUE);

    // Close handle
    ::FindClose(hFile);
  }*/
#endif
}
void fileAccess::openFile(string filename,bool seachback)
{

	char file[100];

	strcpy(file, filename.c_str());

	openFile(file, seachback);

}

char * filenames;
void fileAccess::openFile(char * filename,bool seachback)
{
	filenames=filename;
	bool tmp=openFileWithoutExit(filename, seachback);

	if(!tmp)
	{

   		
#ifndef VS2008
		char buffer[200];
         getcwd(buffer, 200);
#else
		wchar_t   buffer[200];
		_wgetcwd(buffer, 200);
#endif
			cout<<"Could not open file "<< filename << endl;
         cout << "In directory " << buffer <<endl;
         cout << "Press any key to terminate." << endl;
         cin.get();
			exit(1);
	}


}
bool fileAccess::openFileWithoutExit(string filename, bool seachback)
{
	char file[100];

	strcpy(file, filename.c_str());

	return openFileWithoutExit(file, seachback);
}
bool fileAccess::openFileWithoutExit(char * filename,bool seachback)
{
	string oldDir=getCurrentPath();
	while(true)
	{
		bool bad=true;
		bool fail=true;

		char * place=new char[4000];
		strcpy(place,getCurrentPath().c_str());

		strcat(place,"/");

		strcat(place,filename);
		files.open(place, fstream::in);
		if(files.bad())
		{
			bad= false;
			cout<<"problem with open file: "<<filename<<endl;
		}
	    if(files.fail())
		{
	    	fail= false;
	    	cout<<"problem with open file: "<<filename<<endl;
		}
	    if(bad==false || fail==false)
	    {
	    	if(seachback==true)
	    	{
				#ifdef UNIX
					if(getCurrentPath().compare("/")==0)
					{
											changeDir(oldDir);
											return false;
					}
				#endif
				#ifndef UNIX

					if(getCurrentPath().size()<=3)
					{
						changeDir(oldDir);
						return false;
					}
				#endif
					changeDir("..");
	    	}
	    	else
	    	{
	    		return false;
	    	}
	    }
	    else
	    {
	    	changeDir(oldDir);
	    	break;

	    }
	}
   return true;
}
void fileAccess::openFileWrite(string filename){

	char file[100];

	strcpy(file, filename.c_str());

	openFileWrite(file);

}

void fileAccess::openFileWrite(char * filename)
{

	files.open(filename, fstream::out);
	files.precision(4);
	if(files.bad())
	{
			cout<<"could not open file "<< filename<<" and therefore halting"<<endl;
			exit(1);
	}
   if(files.fail())
		{
				cout<<"could not open file "<< filename<<" and therefore halting"<<endl;
				exit(1);
		}
}
void fileAccess::closeFile()
{
	files.close();
}

string fileAccess::getLineFile()
{

      char newline[70000];
      string line;
      if(!files.eof())
      {
		  files.getline (newline, 70000);
		  line=newline;
      }
      else {
    	  string tmp="NULL2";
    	  return tmp;
      }
	#ifdef UNIX
      if(line[line.size() - 1] == '\r')
      {
    	  line.resize(line.size() - 1);
      }
      #endif
      //if a line in a text file is empty cygwin returns false
#ifdef CYGWIN

      while(line.size()<=1)
#else

      while(line.empty())
#endif
      {
    	  if(!files.eof())
    	  {
			  files.getline (newline, 70000);
			  string string1(newline);
			  line=string1;


    	  }
    	  else{
    		  string tmp="NULL1";
    		  return tmp;
    	  }
        #ifdef UNIX
    	  if(line[line.size() - 1] == '\r')
    	  {
    	     line.resize(line.size() - 1);
    	  }
        #endif

      }
      return line;
}
int fileAccess::changeDir(string dirName)
{
#ifndef VS2008
		 char tmp[80];
	strcpy(tmp,dirName.c_str());
	return chdir(tmp);
#else
	wstring widestr = std::wstring(dirName.begin(), dirName.end());
	const wchar_t* widecstr = widestr.c_str();
	int returnvalue = _wchdir(widecstr);
			return returnvalue ;
#endif
}

string fileAccess::getCurrentPath()
{
	string CurrentPath;
    

	char cCurrentPath[FILENAME_MAX];

	_getcwd(cCurrentPath, sizeof(cCurrentPath));

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	return cCurrentPath;

}
void fileAccess::ignore2000()
{	char buffer[2000];
	files.getline(buffer,2000,'\n');
}

bool fileAccess::endOfFile()
{
	return files.eof();
}

void fileAccess::Write(string input)
{

	files<<input<<'\n';
}
void fileAccess::Write(double input)
{
	files<<input<<'\n';
}
void fileAccess::WriteNothing(string input)
{

	files<<input;
}
void fileAccess::WriteNothing(double input)
{

	files<<input;
}
void fileAccess::WriteWithTabS(string input)
{
	files<<input<<'\t';
}
void fileAccess::WriteWithTab(double input)
{

	files<<conv.DoubleToString(input)+'\t';
}
void fileAccess::WriteNewLine()
{

	files<<endl;
}
bool fileAccess::NextSymbolEOF()
{
	if(files.peek()==EOF)
   	return true;
   else return false;

}
