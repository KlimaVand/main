using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace opdateSoil
{
    class Program
    {
        static void Main(string[] args)
        {
            string[] lines = System.IO.File.ReadAllLines(@"JRC_soil_macsur_v2.csv");
            string[] fasset = System.IO.File.ReadAllLines(@"soil.dat");
            string[] env = System.IO.File.ReadAllLines(@"environ.dat");
            string[] field = System.IO.File.ReadAllLines(@"Fields.F01");
            string[] WW_pheno_v3 = System.IO.File.ReadAllLines(@"WW_pheno_v3.csv");
            System.IO.StreamWriter system = new System.IO.StreamWriter("system.dat");
            for(int i=1;i<lines.Count();i++)
            {
                string[] data = lines[i].Split(',');

                double math= double.Parse(data[8]);
                math = Math.Log10(math);
                string ConductivityFC = math.ToString();
                string CarbonPct = data[11];
                string DryBulkDensity = data[13];
                Console.WriteLine(data[8] + " " + data[11] + " " + data[13]);
                Directory.CreateDirectory(data[1] + "_" + data[0]);
                System.IO.StreamWriter file = new System.IO.StreamWriter(data[1] + "_" + data[0] + "/soil.dat");
                bool found = false;
                for(int j=0;j<fasset.Count();j++)
                {

                    if (fasset[j].Contains("ConductivityFC") && found==false)
                    {
                        file.WriteLine("ConductivityFC " + ConductivityFC);
                    }
                    else if (fasset[j].Contains("CarbonPct") && found == false)
                    {
                        file.WriteLine("CarbonPct " + CarbonPct);
                    }
                    else if (fasset[j].Contains("DryBulkDensity") && found == false)
                    {
                        file.WriteLine("DryBulkDensity " + DryBulkDensity);
                    }
                    else
                    {
                        file.WriteLine(fasset[j]);
                    }
                    if(fasset[j].Contains("[SoilLayer(3)]"))
                    {
                        found=true;
                    }
                }
                file.Close();

                file = new System.IO.StreamWriter(data[1] + "_" + data[0] + "/environ.dat");
                for (int j = 0; j < env.Count(); j++)
                {
                    if (env[j].Contains("lattitude"))
                    {
                        file.WriteLine("lattitude " + data[5]);
                    }
                    if (env[j].Contains("Z"))
                    {
                        file.WriteLine("Z " + data[4]);
                    }
                    else
                    {
                        file.WriteLine(env[j]);
                    }
                }
                file.Close();
                for (int k = 1; k <49; k++)
                {
                    system.WriteLine("[run(" +(( i-1)*48+k).ToString() + ")]");
                    system.WriteLine("StartYear                     1980");
                    system.WriteLine("StopYear                      1982");
                    system.WriteLine("EnvironmentalIndicator        1/8");
                    system.WriteLine("EconomicIndicatorDate	      1/8");
                    Directory.CreateDirectory(data[1] + "_" + data[0] + "/output/c" + k.ToString());
                    system.WriteLine("InputDirectory " + "C:\\massRun\\" + data[1] + "_" + data[0]);
                    system.WriteLine("OutputDirectory " + "C:\\massRun\\" + data[1] + "_" + data[0] + "/output/c" + k.ToString());
                    if(k!=25)
                        system.WriteLine("ClimateFileName " + "C:\\massRun\\climate\\" + data[1] + "_" + data[0] + "c"+k.ToString()+".clm");
                    else
                        system.WriteLine("ClimateFileName " + "C:\\massRun\\climate\\" + data[1] + "_" + data[0] + "c" + k.ToString() + "360.clm");
                    system.WriteLine("ForcedOperations        1");
                }
                DateTime sowingDay=new DateTime();
                DateTime harvestDay = new DateTime();
                for (int j = 1; j < WW_pheno_v3.Count(); j++)
                 {
                     string line = WW_pheno_v3[j];
                     string[] cell = line.Split(',');
                     string location = cell[3];

                     sowingDay = bacsDateConvert(cell[6]);
                     harvestDay = bacsDateConvert(cell[8]);
                 }
                for (int j = 1; j < 31; j++)
                {

                    System.IO.StreamWriter fieldOut;
                    if(j<10)
                        fieldOut= new System.IO.StreamWriter(data[1] + "_" + data[0] + "/fields.f0"+j.ToString());
                    else
                        fieldOut = new System.IO.StreamWriter(data[1] + "_" + data[0] + "/fields.f" + j.ToString());
                    for (int k = 0; k < field.Count();k++ )
                    {
                        
                        if (field[k].Contains("SowDate(0)"))
                        {
                            String outputString = sowingDay.Day + "/" + sowingDay.Month + "/" + sowingDay.Year;
                            fieldOut.WriteLine("SowDate(0) " + outputString);

                            sowingDay= sowingDay.AddYears(1);

                        }
                        else if (field[k].Contains("TillageDate(0)"))
                        {
                            String outputString = (sowingDay.Day-1) + "/" + sowingDay.Month + "/" + sowingDay.Year;
                            fieldOut.WriteLine("TillageDate(0) " + outputString);

                            sowingDay = sowingDay.AddYears(1);

                        }
                        else if (field[k].Contains("HarvestDate(0)"))
                        {
           
                            harvestDay = harvestDay.AddYears(1);
                            String outputString = harvestDay.Day + "/" + harvestDay.Month + "/" + harvestDay.Year;
                            fieldOut.WriteLine("HarvestDate(0) " + outputString);
                        }
                        else if (field[k].Contains("FertilizerDate(0)"))
                        {

                            String outputString = (harvestDay.Day + 1) + "/" + (harvestDay.Month - 3) + "/" + harvestDay.Year;
                            fieldOut.WriteLine("FertilizerDate(0) " + outputString);
                        }
                        else if (field[k].Contains("FertilizerDate(1)"))
                        {

                            String outputString = (harvestDay.Day + 1) + "/" +( harvestDay.Month-2) + "/" + harvestDay.Year;
                            fieldOut.WriteLine("FertilizerDate(1) " + outputString);
                        }
                        else
                            fieldOut.WriteLine(field[k]);
                    }

                    fieldOut.Close();
                }

            }
            system.WriteLine("[end]");
            system.Close();
        }
        static private DateTime bacsDateConvert(String bacsFormatDate)
        {
            int dateDays = Convert.ToInt16(bacsFormatDate.Substring(0, 3));

            DateTime outputDate = new DateTime();
            outputDate = Convert.ToDateTime("31-12-1980");

            outputDate = outputDate.AddDays(dateDays);

         


            return outputDate;
        }
    }
}
