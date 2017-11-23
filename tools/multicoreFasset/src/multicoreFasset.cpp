//============================================================================
// Name        : multicoreFasset.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "commonData.h"
#ifndef VS2008
#include <dirent.h>
#else
#include <windows.h>
#include <conio.h>
#endif
#ifdef VS2008
#include <windows.h>
#include <tchar.h>
#include <shellapi.h>
#include <direct.h>
#include <stdlib.h>
#include <stdio.h>
#endif
using namespace std;

int main() {
	commonData data;
	data.readFile("system.dat");
	int min;
	int large;
	data.getSectionsNumbers("run",&min, &large);
	
	int cores;
	data.setCritical(true);
	data.FindItem("cores",&cores);
	int start=data.FindSection("run",min);
	fileAccess ting;
	ofstream myfile;
	myfile.open("cores.txt");
	myfile<<cores;
	myfile.close();
	int numberOfruns=(large-min)/cores;

	for(int i=0;i<cores;i++)
	{
		fileAccess system;
		system.openFile("system.dat");
		convert conv;
		string name=system.getCurrentPath()+"\\fasset"+conv.IntToString(i);
		system.CreateDir(name);
		ofstream myfile;
		name+="\\system.dat";
		myfile.open(name);
		myfile<<"JumpStart\t"<<(min+numberOfruns*i)<<'\n';
		if((i+1)!=cores)
			myfile<<"JumpStop\t"<<(numberOfruns+i*numberOfruns-1)<<'\n';
		else
			myfile<<"JumpStop\t"<<large<<'\n';
		for(int j=0;j<start;j++)
		{
			string line=system.getLineFile();
			myfile<<line+'\n';
			
		}
		
		int firstRun=data.FindSection("run",min+numberOfruns*i);
		int stop=data.FindSection("run",numberOfruns+i*numberOfruns);
		for(int k=firstRun;k<stop;k++)
		{
			string line=data.getLine(k);
			myfile<<line+'\n';
	
		}
		if((i+1)==cores)
		{
			int first=data.FindSection("run",numberOfruns+i*numberOfruns);
			int lastStop=data.FindSection("end");
			for(int k=first;k<lastStop;k++)
			{
				string line=data.getLine(k);
				myfile<<line+'\n';
	
			}
		}
		myfile<<"[end]";
		system.closeFile();
		myfile.close();

	}
	
	return 0;
}
