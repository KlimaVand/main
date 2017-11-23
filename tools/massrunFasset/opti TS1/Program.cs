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
            double floweringObs = -1; //climate file
            string[] clima = System.IO.File.ReadAllLines("WW_pheno_v3.csv");
            double harvestObs = int.Parse(clima[35].Split(',')[8]); //grit number 35_126
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
                        floweringObs = double.Parse(clima[climaLine].Split(',')[7]);
                        harvestObs = double.Parse(clima[climaLine].Split(',')[8]);
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
                    System.IO.StreamWriter file = new System.IO.StreamWriter(dir[t] + "\\RunParameters.dat");
                    file.WriteLine("[WinterWheat]");
                    file.WriteLine("TS1 " + newGuess.ToString());
                    file.WriteLine("TS2 520");
                    file.WriteLine("TS3 250");
                    file.WriteLine("[end]");

                    file.Close();
                    Directory.SetCurrentDirectory("D:\\massRun\\");
                    var process = Process.Start("newtrunk.exe");
                    process.WaitForExit();
                    double floweringSim;//get 39.21 Date of flowering of main crop, Avg of 30 years of sims
                    string[] lines = System.IO.File.ReadAllLines(dir[t] + "\\output\\c1\\INDICATX.xls");
                    double days = 0;
                    int periodLenght = 0;
                    for (int i = 0; i < lines.Count(); i++)
                    {
                        if (lines[i].Contains("39.21 Date of flowering of main crop"))
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
                    floweringSim = (int)days / periodLenght;
                    if (floweringSim > 365)
                        floweringSim -= 365;
                    if (floweringSim > harvestObs)
                    {
                        bias = 999;
                    }
                    else
                        bias = floweringSim - floweringObs;

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
