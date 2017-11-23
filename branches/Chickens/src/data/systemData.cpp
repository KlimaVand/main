/*
 * system.cpp
 *
 *  Created on: Aug 21, 2009
 *      Author: JOVE
 */
#include "../data/systemData.h"
#include "../tools/convert.h"
#include "../base/message.h"


/*
systemData::systemData(const systemData& someSystemData){
	someSystemData
}*/


systemData::systemData(bool notNitroscape)
{

	if (notNitroscape)
	{
    	loaddata();
    	checkData();
   }
   else
   {

   	currentRun=0;
	 	addRun();
	}
};



 void systemData::loaddataNITROSCAPE()
 {
 	cout << "loading Nitroscape system" << endl;
	 currentRun=0;
	 addRun();
 }
 void systemData::addRun()
 {
	 //default values for keywords for every run
		 systemRunData newDataset;
		newDataset.setInputDirectory(" ");
		newDataset.setOutputDirectory(" ");
		newDataset.setClimateDirectory(" ");
		newDataset.setClimateFileName(" ");
		newDataset.setEnvironmentalIndicator("1/1");
		newDataset.setEconomicIndicatorDate("1/1");

		newDataset.setInputDirectoryFile(false);
		newDataset.setOutputDirectoryFile(false);
		newDataset.setClimateDirectoryFile(false);
		newDataset.setClimateFileNameFile(false);
		newDataset.setEnvironmentalIndicatorFile(false);
		newDataset.setEconomicIndicatorDateFile(false);

		newDataset.setClimateFileType( -1 );
		newDataset.setEpotCalculationMethod(0);
		newDataset.setfertiliserUnit(0);
		newDataset.setsingleField(-1);
		newDataset.setVolatModel(0);
		newDataset.setStartYear(0);
		newDataset.setStopYear(0);

		newDataset.setClimateFileTypeFile(false);
		newDataset.setEpotCalculationMethodFile(false);
		newDataset.setVolatModelFile(false);
		newDataset.setfertiliserUnitFile(false);
		newDataset.setsingleFieldFile(false);
		newDataset.setStartYearFile(false);
		newDataset.setStopYearFile(false);



		newDataset.setSimpleTemperatureModel(false);
		newDataset.setuniSizeFields(false);
		newDataset.setFieldsFlexSize(false);
		newDataset.setMeasuredSoilTemperature(false);
		newDataset.setWriteSoilTemperature(false);
		newDataset.setAirToSoilTemperature(false);
		newDataset.setForcedOperations(false);
		newDataset.setForcedHarvest(false);

		newDataset.setSimpleTemperatureModelFile(false);
		newDataset.setuniSizeFieldsFile(false);
		newDataset.setFieldsFlexSizeFile(false);
		newDataset.setMeasuredSoilTemperatureFile(false);
		newDataset.setAirToSoilTemperatureFile(false);
		newDataset.setWriteSoilTemperatureFile(false);
		newDataset.setForcedOperationsFile(false);
		newDataset.setForcedHarvestFile(false);

		newDataset.setHarvestWaste(0.05);
		newDataset.setStrawLeft(0.45);

		newDataset.setHarvestWasteFile(false);
		newDataset.setStrawLeftFile(false);
		runData.push_back(newDataset);


 }
/**
 * This function checks if the necessary data has been read and if it is correct. If something is wrong it either sends a  FatalError or closing fasset
 */

void systemData::checkData()
{
	for(int i=0;i<=RunLevel;i++)
	{
		if (runData[i].getHarvestWaste()>1.0 || runData[i].getHarvestWaste()<0.0)
			            {
                     cout<<"main - illegal value for harvest waste"<<endl;;
	cin.get();
                     exit(1);
                     }
		if(runData[i].getStartYearFile()==false)
		{
			cout<<"there is no start year for run "<<i<<endl;
			cin.get();
			exit(1);
		}
		if(runData[i].getStopYearFile()==false)
		{
			cout<<"there is no stop year for run "<<i<<endl;
			cin.get();
			exit(1);
		}
		if(runData[i].getInputDirectoryFile()==false)
		{
			cout<<"there is no InputDirectory for run "<<i<<endl;
			cin.get();
			exit(1);
		}
		if(runData[i].getOutputDirectoryFile()==false)
		{
			cout<<"there is no OutputDirectory for run "<<i<<endl;
			cin.get();
			exit(1);
		}
		if(runData[i].getClimateFileNameFile()==false)
				{
					cout<<"there is no ClimateFileName for run "<<i<<endl;
cin.get();
					exit(1);
				}
		if(runData[i].getClimateDirectoryFile()==false)
		{
			cout<<"warning: there is no ClimateDirectory for run "<<i<<endl;
			cout<<"trying to make it from ClimateFileName "<<endl;
			string instring = runData[i].getClimateFileName();
			//cout<<instring<<endl;
#ifndef UNIX
			int loc = instring.rfind("\\",instring.length());          //separate directory and file names
#endif
#ifdef UNIX
			int loc = instring.rfind("/",instring.length());          //separate directory and file names
#endif
			if (loc>0)
			{
				runData[i].setClimateDirectory(instring.substr(0,loc));
				cout<<instring.substr(0,loc)<<endl;
				cout<<instring.substr(loc+1)<<endl;
				runData[i].setClimateFileName(instring.substr(loc+1));
			}
			else
			{
				cout<<"could not make ClimateDirectory for run "<<i <<endl;
				cout<<instring<<endl;
				exit(99);
			}

		}
		if(runData[i].getClimateFileTypeFile()==false)
		{
			cout<<"warning: there is no ClimateFileType for run "<<i<<endl;
			cout<<"trying to make it from ClimateName"<<endl;
			int size=runData[i].getClimateFileName().size();

			if(runData[i].getClimateFileName().substr(size-3, size).compare("CLM"))
				runData[i].setClimateFileType(0);
			else if(runData[i].getClimateFileName().substr(size-3, size).compare("clm"))
				runData[i].setClimateFileType(0);
			else if(runData[i].getClimateFileName().substr(size-3, size).compare("KML"))
				runData[i].setClimateFileType(1);
			else if(runData[i].getClimateFileName().substr(size-3, size).compare("kml"))
				runData[i].setClimateFileType(1);



		}
		if(runData[i].getEpotCalculationMethod()==true)
		{
			if (runData[i].getEpotCalculationMethod()==0 || runData[i].getEpotCalculationMethod()==3)
			{
				cout<<"EpotCalculationMethod"<<endl;
				cout<<"EpotCalculationMethod is set although evapotranspiration is in the selected climate file type"<<endl;
				cin.get();
                     exit(1);

			}
		}
		if (runData[i].getClimateFileType()<0 || runData[i].getClimateFileType()>6)
		               {
			cout<<"ClimateFileType"<<endl;

			cout<<"Unknown climate file type"<<endl;
cin.get();
                     exit(99);

                     }

		if (runData[i].getuniSizeFields()==true && runData[i].getFieldsFlexSize()==true)
		               {
			cout << "main - uniSizeFields and fieldsFlexSize can not both be set to true at the same time"<<endl;

                     exit(99);
                     }//
	}
	if ((JumpStart<0)|| (JumpStart>RunLevel))
	{
		cout << "JumpStart less than zero or greater than number of possible runs" << endl;
cin.get();
		exit(99);
	}
	if (JumpStart>JumpStop)
	{
		cout << "JumpStart is greater than JumpStop" << endl;
cin.get();
		exit(99);
	}
   if (JumpStop>RunLevel)
	{
		cout << "JumpStop is greater than numbers of runs" << endl;
cin.get();
		exit(99);
	}
	if (RunLevel==-1)
	{
			cout << "There is no runs" << endl;
cin.get();
			exit(1);
	}
	if (JumpStartFile==false)
	{
		JumpStart=0;
	}
	if (JumpStopFile==false)
	{
		JumpStop=RunLevel;
	}
	cout <<"Finished checking system.dat"<<endl;
}
/**
 * This class load the data from system.dat. Note that system.dat should be placed in the default path or Fasset will exit
 * If the keywords is not found the default value is used. See system.xlsx for that information
 */

void systemData::loaddata()
{
	// Default values for global keywords
   RunLevel=1;

    string EconomicIndicatorDate="1/1";

    RunLevel=-1;
    JumpStart=0;
    JumpStop=0;
    currentRun=0;

    readFileWithExit("system.dat");

	convert conv;


	for(unsigned int i=0;i<items.size();i++)
	{
		string line=items.at(i);

		string nameOfItem=conv.partOfAString(line,1);

		string value=conv.partOfAString(line,2);

			if(nameOfItem.compare("JumpStart")==0)
						{
							JumpStart=conv.StringToInt(value);
							JumpStartFile=true;
						}
			else if(nameOfItem.compare("JumpStop")==0)
						{
							JumpStop=conv.StringToInt(value);
							JumpStopFile=true;
						}
			else if(nameOfItem.compare("removeOldData")==0)
			{
				removeOldData=conv.StringToBool(value);
				removeOldDataFile=true;
			}
			else if(nameOfItem.substr(1,3).compare("run")==0)
			{

				string substring;
				int times=0;
				int multiplier=1;
				substring= nameOfItem.substr(5,1);

				int number=0;
				while(substring.compare(")"))
				{

					number=number*multiplier+conv.StringToInt(substring);
					multiplier=multiplier*10;
					times++;
					substring= nameOfItem.substr(5+times,1);
				}
				if(!(RunLevel+1==number))
				{
					cout<<RunLevel+1<<" is missing the system.dat"<<endl;
					exit(1);
				}

				RunLevel=number;


				addRun();


				while(nameOfItem.compare("[end]")!=0)
				{
           		i++;
					line=items.at(i);

					nameOfItem=conv.partOfAString(line,1);
               value=conv.partOfAString(line,2);

					if(nameOfItem.compare("InputDirectory")==0)
					{
						runData[RunLevel].setInputDirectory(value);
						runData[RunLevel].setInputDirectoryFile(true);
					}
					else if(nameOfItem.compare("EconomicIndicatorDate")==0)
					{
						runData[RunLevel].setEconomicIndicatorDate(value);
						runData[RunLevel].setEconomicIndicatorDateFile(true);
					}
					else if(nameOfItem.compare("ClimateFileName")==0)
					{
						runData[RunLevel].setClimateFileName(value);
						runData[RunLevel].setClimateFileNameFile(true);

					}
					else if(nameOfItem.compare("ClimateDirectory")==0)
					{
						runData[RunLevel].setClimateDirectory(value);
						runData[RunLevel].setClimateDirectoryFile(true);
					}
					else if(nameOfItem.compare("EnvironmentalIndicator")==0)
					{
						runData[RunLevel].setEnvironmentalIndicator(value);
						runData[RunLevel].setEnvironmentalIndicatorFile(true);
					}
					else if(nameOfItem.compare("OutputDirectory")==0)
					{
		#ifndef UNIX
					if(value.substr(value.size()-1,1).compare("\\"))
						  value=value+"\\";
		#else
					if(value.substr(value.size()-1,1).compare("/"))
									  value=value+"/";
		#endif
						runData[RunLevel].setOutputDirectory(value);
						runData[RunLevel].setOutputDirectoryFile(true);

					}
					else if(nameOfItem.compare("ClimateFileType")==0)
					{
						runData[RunLevel].setClimateFileType(conv.StringToInt(value));
						runData[RunLevel].setClimateFileTypeFile(true);
					}
					else if(nameOfItem.compare("EpotCalculationMethod")==0)
					{
						runData[RunLevel].setEpotCalculationMethod(conv.StringToInt(value));
						runData[RunLevel].setEpotCalculationMethodFile(true);
					}
					else if(nameOfItem.compare("fertiliserUnit")==0)
					{
						runData[RunLevel].setfertiliserUnit(conv.StringToInt(value));
						runData[RunLevel].setfertiliserUnitFile(true);
					}
					else if(nameOfItem.compare("SingleField")==0)
					{
						runData[RunLevel].setsingleField(conv.StringToInt(value));
						runData[RunLevel].setsingleFieldFile(true);
					}
					else if(nameOfItem.compare("StartYear")==0)
					{
						runData[RunLevel].setStartYear(conv.StringToInt(value));
						runData[RunLevel].setStartYearFile(true);
					}
					else if(nameOfItem.compare("VolatModel")==0)
					{

						runData[RunLevel].setVolatModel(conv.StringToInt(value));
						runData[RunLevel].setVolatModelFile(true);



					}
					else if(nameOfItem.compare("StopYear")==0)
					{
						runData[RunLevel].setStopYear(conv.StringToInt(value));
						runData[RunLevel].setStopYearFile(true);

					}
					else if(nameOfItem.compare("SimpleTemperatureModel")==0)
					{
						runData[RunLevel].setSimpleTemperatureModel(conv.StringToBool(value));
						runData[RunLevel].setSimpleTemperatureModelFile(true);

					}
					else if(nameOfItem.compare("uniSizeFieldsFile")==0)
					{
						runData[RunLevel].setuniSizeFields(conv.StringToBool(value));
						runData[RunLevel].setuniSizeFieldsFile(true);

					}
					else if(nameOfItem.compare("uniSizeFields")==0)
					{
						runData[RunLevel].setuniSizeFields(conv.StringToBool(value));
						runData[RunLevel].setuniSizeFieldsFile(true);

					}
					else if(nameOfItem.compare("FieldsFlexSize")==0)
					{
						runData[RunLevel].setFieldsFlexSize(conv.StringToBool(value));
						runData[RunLevel].setFieldsFlexSizeFile(true);

					}
					else if(nameOfItem.compare("MeasuredSoilTemperature")==0)
					{
						runData[RunLevel].setMeasuredSoilTemperature(conv.StringToBool(value));
						runData[RunLevel].setMeasuredSoilTemperatureFile(true);

					}
					else if(nameOfItem.compare("AirToSoilTemperature")==0)
					{
						runData[RunLevel].setAirToSoilTemperature(conv.StringToBool(value));
						runData[RunLevel].setAirToSoilTemperatureFile(true);
					}
					else if(nameOfItem.compare("WriteSoilTemperature")==0)
					{

						runData[RunLevel].setWriteSoilTemperature(conv.StringToBool(value));
						runData[RunLevel].setWriteSoilTemperatureFile(true);
					}
					else if(nameOfItem.compare("ForcedOperations")==0)
					{
						runData[RunLevel].setForcedOperations(conv.StringToBool(value));
						runData[RunLevel].setForcedOperationsFile(true);
					}
					else if(nameOfItem.compare("ForcedHarvest")==0)
					{
						runData[RunLevel].setForcedHarvest(conv.StringToBool(value));
						runData[RunLevel].setForcedHarvestFile(true);
					}
					else if(nameOfItem.compare("HarvestWaste")==0)
					{
						runData[RunLevel].setHarvestWaste(conv.StringToDouble(value));
						runData[RunLevel].setHarvestWasteFile(true);
					}
					else if(nameOfItem.compare("StrawLeft")==0)
					{
						runData[RunLevel].setStrawLeft(conv.StringToDouble(value));
						runData[RunLevel].setStrawLeftFile(true);
					}
					else if(nameOfItem.substr(0,1)=="[" &&nameOfItem.substr(nameOfItem.size()-1,nameOfItem.size())=="]")
					{
						i--;
						break;
					}



				}

			}
			else if(line.compare("[end]")==0)
			{
				break;
			}
			else
			{
				string output="for system.dat is there a keyword: "+conv.partOfAString(line,1)+"that is not used ";
			  //	theMessage->Warning(output);
			}
	}

}

void systemData::problem()
{

	for(unsigned int i=0;i<problems.size();i++)
	{
		theMessage->Warning(problems[i]);
	}
}
void systemData::reload(string orgPath){
	fileAccess hd;
	hd.changeDir(orgPath);
	loaddata();
	checkData();
}

