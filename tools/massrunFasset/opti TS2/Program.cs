using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace opti
{
    class Program
    {
        static void Main(string[] args)
        {


            string[] dir = Directory.GetDirectories("D:\\massRun\\");
            
  
            System.IO.StreamWriter biasFile = new System.IO.StreamWriter("D:\\massRun\\bias.txt");
  
           
            int oldbias = 99;
            int runs = 0;

            string[] clima = System.IO.File.ReadAllLines("WW_pheno_v3.csv");
            double harvestObs = 0;
            for (int t = 0; t < dir.Count() -3; t++)
            {
                double guess1 = 0;
                double guess2 = 1024;
                double lowerboundery = -1000;
                double upperboundery = 1000;
                double bias = 2;
                string[] sub = dir[t].Split('\\');
                for (int climaLine = 0; climaLine < clima.Count(); climaLine++)
                {
                    if (clima[climaLine].Contains(sub[2]))
                    {

                        harvestObs = double.Parse(clima[climaLine].Split(',')[8])-5;
                    }
                }

                System.IO.StreamWriter system = new System.IO.StreamWriter("D:\\massRun\\system.dat");
                system.WriteLine("[run(0)]");
                system.WriteLine("StartYear                     1980");
                system.WriteLine("StopYear                      2010");
                system.WriteLine("EnvironmentalIndicator        1/8");
                system.WriteLine("EconomicIndicatorDate	      1/8");

                system.WriteLine("InputDirectory " + dir[t]);
                system.WriteLine("OutputDirectory " + dir[t] + "/output/c1");

                system.WriteLine("ClimateFileName " + sub[0] + "\\" + sub[1] + "\\climate\\" + sub[2] + "c1.clm");

                system.WriteLine("ForcedOperations        1");
                system.WriteLine("[end]");
                system.Close();

                while (!(Math.Abs(guess1 - guess2) < 2)&&bias!=0)
                {
                    runs++;

                  

                    double newGuess = (guess1 + guess2) / 2;
                    string[] crop = System.IO.File.ReadAllLines(dir[t] + "\\RunParameters.dat");
                    System.IO.StreamWriter file = new System.IO.StreamWriter(dir[t] + "\\RunParameters.dat");
                   
                    for (int k = 0; k < crop.Count(); k++)
                    {
                        if (crop[k].Contains("TS2"))
                            file.WriteLine("TS2 " + newGuess.ToString());
                        else if (crop[k].Contains("TS3"))
                            file.WriteLine("TS3 " + (newGuess*0.38).ToString());

                        else
                        {
                            file.WriteLine(crop[k]);
                        }


                    }
                    file.Close();
                    Directory.SetCurrentDirectory("D:\\massRun\\");
                    var process = Process.Start("newtrunk.exe");
                    process.WaitForExit();

                    string[] lines = System.IO.File.ReadAllLines(dir[t] + "\\output\\c1\\INDICATX.xls");
                    double days = 0;
                    int periodLenght = 0;
                    for (int i = 0; i < lines.Count(); i++)
                    {
                        if (lines[i].Contains("39.22 Date of end of grain filling of main crop"))
                        {
                            string jday = lines[i].Split('\t')[2];
                            string[] day = jday.Split(' ');
                            jday = (day[day.Count() - 1]);

                            if (double.Parse(jday) != 0)
                            {
                                periodLenght++;
                                days += double.Parse(jday);
                            }


                        }
                    }
                    double harvestSim = (int)days / periodLenght;
                    if (harvestSim > 365)
                        harvestSim -= 365;

                    bias = harvestSim - harvestObs;

                    biasFile.WriteLine((guess1 + guess2) / 2);
                    biasFile.WriteLine(bias);

                    if (Math.Sign(bias) == Math.Sign(lowerboundery))
                    {
                        guess1 = newGuess;
                        lowerboundery = bias;
                    }
                    else
                    {
                        
                        guess2 = newGuess;
                        upperboundery = bias;
                    }

                    file.Close();
                   



                }
                biasFile.WriteLine();

            }
            biasFile.Close();


        }
    }
}
