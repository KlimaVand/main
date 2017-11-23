// Slurry_pH.cpp : Defines the entry point for the console application.
//
#include "Slurry_pH.h";
#include "c:\Users\njh\workspace\main\trunk\src\base\nixcbits.h"
#include <iostream>
using namespace std;

int main()
{
	Slurry_pH aSlurry_pH;
	for (int i=0; i<200; i++)
		aSlurry_pH.Daily();
	return 0;
}

