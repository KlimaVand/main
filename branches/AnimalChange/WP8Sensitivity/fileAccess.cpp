/*
 * fileAccess.cpp
 *
 *  Created on: Aug 21, 2009
 *      Author: JOVE
 */

#include "fileAccess.h"
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
#include "convert.h"
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

fileAccess::~fileAccess(){
	if (files!=nullptr)
	{

		  delete files;
	}

	};
fileAccess::fileAccess()
{
	files=new fstream();
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
	  while (dir!=nullptr)
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
	cout<<"fileAccess::deleteEveryFileInFolder does not work when using visal studio"<<endl;
	exit(99);
	int             iCount          = 0;
  std::string     strFilePath;          // Filepath
  LPCWSTR    strPattern;           // Pattern
  std::string     strExtension;         // Extension
  HANDLE          hFile;                // Handle to file
  WIN32_FIND_DATA FileInformation;      // File information


  strPattern ;//= path + "\\*.*";
  hFile;// = ::FindFirstFile(strPattern.c_str(), &FileInformation);
  if(hFile != INVALID_HANDLE_VALUE)
  {
    do
    {
		string name;;//=FileInformation.cFileName;
		if(!name.compare(".svn")==0)
    	{
				
				if(!name.compare(".")==0)
				{
					cout<<"delete" <<name<<endl;
					if(!name.compare("..")==0)
					{
						char * cName=new char[200];
						strcpy(cName,path.c_str());
						strcat(cName,"\\");
						strcat(cName,name.c_str());
						cout<<cName<<endl;
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
  }
#endif
}
void fileAccess::openFile(string filename,bool seachback)
{

	char file[255];

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

   		char buffer[200];
         _getcwd(buffer, 200);
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
//	cout<<oldDir<<endl;
#ifdef VS2008
	_setmaxstdio(2048);
#endif
	while(true)
	{
		bool bad=true;
		bool fail=true;

		char * place=new char[400];
		strcpy(place,getCurrentPath().c_str());
#ifdef unix
		strcat(place,"/");
#else
		strcat(place,"\\");
#endif
		strcat(place,filename);

		files->open(place, fstream::in);

		if(files->bad())
		{
			bad= false;
		}
	    if(files->fail())
		{
	    	fail= false;
		}
	    if(bad==false || fail==false)
	    {
		
			if(seachback==true)
	    	{
				#ifdef unix
					if(getCurrentPath().compare("/")==0)
					{
						changeDir(oldDir);
						delete[] place;
						return false;
					}
				#endif

				#ifdef CYGWIN
					if(getCurrentPath().compare("/cygdrive")==0)
					{
								changeDir(oldDir);
								 delete[] place;
								return false;
					}
				#endif
				#ifndef unix
					
					if(getCurrentPath().size()<=3)
					{
						changeDir(oldDir);
						 delete[] place;
						return false;
					}
				#endif
					changeDir("..");
	    	}
	    	else
	    	{
	    		 delete[] place;
	    		return false;
	    	}
	    }
	    else
	    {
	    	changeDir(oldDir);
	    	 delete[] place;
	    	break;

	    }
	    delete[] place;
	}
   return true;
}
void fileAccess::openFileWriteString(string filename){

	char file[100];

	strcpy(file, filename.c_str());

	openFileWrite(file);

}

void fileAccess::openFileWrite(char * filename)
{
	files=new fstream();
	files->open(filename, fstream::out);
	files->precision(4);
	if(files->bad())
	{
			cout<<"could not open file "<< filename<<" and therefore halting"<<endl;
			exit(1);
	}
   if(files->fail())
		{
				cout<<"could not open file "<< filename<<" and therefore halting"<<endl;
				exit(1);
		}
}
void fileAccess::closeFile()
{
	files->close();
}

string fileAccess::getLineFile()
{

      char newline[9000]={'\0'};
      string line="nullptr";
      if(!files->eof())
      {
		  files->getline (newline, 9000);
		  line=newline;
      }
      else {
    	  string tmp="nullptr";
    	  return tmp;
      }


	#ifdef unix

      if(line[0]!='\0')
		  if(line[line.size() - 1] == '\r')
		  {
			  line.resize(line.size() - 1);
		  }
		  #endif

		  if(!line.empty())
			if(line[0]==9)
			  line.resize(line.size() - 1);
		  //if a line in a text file is empty cygwin returns false
	#ifdef CYGWIN
		  while(line.size()<=1)
	#else
		  while(line.empty())
	#endif
		  {
			  if(!files->eof())
			  {
				  files->getline (newline, 9000);
				  string string1(newline);
				  line=string1;

			  }
			  else{
				  string tmp="nullptr";
				  return tmp;
			  }
			#ifdef unix
			  if(line[line.size() - 1] == '\r')
			  {
				 line.resize(line.size() - 1);
			  }
			#endif



		  }
		  int i=line[line.size() - 1];
		  if(i==13)
			  line.resize(line.size() - 1);

      return line;
}
int fileAccess::changeDir(string dirName)
{
#ifndef VS2008
#ifdef __BCplusplus__
	return chdir(dirName.c_str());
#else
		 char tmp[200];
	strcpy(tmp,dirName.c_str());
	return chdir(tmp);
#endif
#else
			char * tmp=new char[80];
			strcpy(tmp,dirName.c_str());
			int returnvalue=_chdir(tmp);
			return returnvalue ;
#endif
}
string fileAccess::getCurrentPath()
{
	string CurrentPath=" ";
    char path[FILENAME_MAX];
#ifdef VS2008
	_getcwd(path,sizeof(path));
#else
	getcwd(path,sizeof(path));
#endif
    CurrentPath=path;
    return CurrentPath;
}


bool fileAccess::endOfFile()
{
	return files->eof();
}

void fileAccess::WriteS(string input)
{

	*files<<input<<'\n';
}
void fileAccess::Write(double input)
{
	*files<<input<<'\n';
}

void fileAccess::WriteWithTabS(string input)
{
	*files<<input<<'\t';
}
void fileAccess::WriteWithTab(double input)
{

	*files<<conv.DoubleToChar(input)+'\t';
}
void fileAccess::WriteNewLine()
{

	*files<<endl;
}

double fileAccess::GetDoubleFromFile()
{
       double val;
              string read=getLineFile();
              val=conv.StringToDouble(read);
       return val;
}

int fileAccess::GetIntFromFile()
{
       int val;

       string read=getLineFile();
          val=conv.StringToInt(read);
       return val;
}

int fileAccess::CreateDir(string directoryName)
{
       int retVal=-1;

#ifdef CYGWIN
             retVal=mkdir(directoryName.c_str(),'-p');
#endif
#ifdef unix
             retVal=mkdir(directoryName.c_str(),'-p');
#endif
#ifndef VS2008
#ifndef	unix
              retVal=mkdir(directoryName.c_str());
#endif
#endif

              if (retVal==-1)
                    cout << "Unable to create directory " << directoryName << endl;
       return retVal;
}

