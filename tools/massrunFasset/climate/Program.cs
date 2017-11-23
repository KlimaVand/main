using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace climate
{
    class Program
    {
        static void Main(string[] args)
        {

           string[] tmp= Directory.GetFiles("climate/");
            for(int i=0;i<tmp.Count();i++)
            {
                string[] lines = System.IO.File.ReadAllLines(tmp[i]);
                string point = tmp[i].Substring(8,7);
                string[] points = point.Split('_');
                Directory.CreateDirectory("output");
                System.IO.StreamWriter c1 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c1.clm");
                System.IO.StreamWriter c2 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c2.clm");
                System.IO.StreamWriter c3 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c3.clm");
                System.IO.StreamWriter c4 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c4.clm");
                System.IO.StreamWriter c5 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c5.clm");
                System.IO.StreamWriter c6 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c6.clm");
                System.IO.StreamWriter c7 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c7.clm");
                System.IO.StreamWriter c8 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c8.clm");
                System.IO.StreamWriter c9 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c9.clm");
                System.IO.StreamWriter c10 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c10.clm");
                System.IO.StreamWriter c11 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c11.clm");
                System.IO.StreamWriter c12 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c12.clm");
                System.IO.StreamWriter c13 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c13.clm");
                System.IO.StreamWriter c14 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c14.clm");
                System.IO.StreamWriter c15 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c15.clm");
                System.IO.StreamWriter c16 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c16.clm");
                System.IO.StreamWriter c17 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c17.clm");
                System.IO.StreamWriter c18 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c18.clm");
                System.IO.StreamWriter c19 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c19.clm");

                System.IO.StreamWriter c20 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c20.clm");
                System.IO.StreamWriter c21 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c21.clm");
                System.IO.StreamWriter c22 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c22.clm");
                System.IO.StreamWriter c23 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c23.clm");
                System.IO.StreamWriter c24 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c24.clm");
                System.IO.StreamWriter c25360 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c25360.clm");
                System.IO.StreamWriter c25499 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c25499.clm");
                System.IO.StreamWriter c26 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c26.clm");
                System.IO.StreamWriter c27 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c27.clm");
                System.IO.StreamWriter c28 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c28.clm");
                System.IO.StreamWriter c29 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c29.clm");
                System.IO.StreamWriter c30 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c30.clm");
                System.IO.StreamWriter c31 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c31.clm");
                System.IO.StreamWriter c32 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c32.clm");
                System.IO.StreamWriter c33 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c33.clm");
                System.IO.StreamWriter c34 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c34.clm");
                System.IO.StreamWriter c35 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c35.clm");
                System.IO.StreamWriter c36 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c36.clm");
                System.IO.StreamWriter c37 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c37.clm");
                System.IO.StreamWriter c38 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c38.clm");
                System.IO.StreamWriter c39 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c39.clm");
                System.IO.StreamWriter c40 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c40.clm");
                System.IO.StreamWriter c41 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c41.clm");
                System.IO.StreamWriter c42 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c42.clm");
                System.IO.StreamWriter c43 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c43.clm");
                System.IO.StreamWriter c44 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c44.clm");
                System.IO.StreamWriter c45 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c45.clm");
                System.IO.StreamWriter c46 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c46.clm");
                System.IO.StreamWriter c47 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c47.clm");
                System.IO.StreamWriter c48 = new System.IO.StreamWriter("output/" + points[0] + "_" + points[1] + "c48.clm");
      
                for (int j = 1; j < lines.Count(); j++)
                {
                    
                    string[] cells = lines[j].Split(',');
                    
                    string time = cells[2];
                    string year = time.Substring(0, 4);
                    string month = time.Substring(4,2);
                    string day = time.Substring(6, 2);
                    string windspeed=(((double.Parse(cells[6]) * 1000) / 24) / 3600).ToString();
                    if (cells[0] == "0" && cells[1] == "0_0")
                        c1.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t'+"360");
                    else if (cells[0] == "2" && cells[1] == "GFDL-CM3_45")
                        c2.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "3" && cells[1] == "GFDL-CM3_45")
                        c3.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "2" && cells[1] == "GFDL-CM3_85")
                        c4.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "3" && cells[1] == "GFDL-CM3_85")
                        c5.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "2" && cells[1] == "GISS-E2-R_45")
                        c6.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "3" && cells[1] == "GISS-E2-R_45")
                        c7.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "2" && cells[1] == "GISS-E2-R_85")
                        c8.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "3" && cells[1] == "GISS-E2-R_85")
                        c9.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "2" && cells[1] == "HadGEM2-ES_26")
                        c10.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "3" && cells[1] == "HadGEM2-ES_26")
                        c11.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "2" && cells[1] == "HadGEM2-ES_45")
                        c12.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "3" && cells[1] == "HadGEM2-ES_45")
                        c13.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "2" && cells[1] == "HadGEM2-ES_85")
                        c14.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "3" && cells[1] == "HadGEM2-ES_85")
                        c15.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "2" && cells[1] == "MIROC5_45")
                        c16.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "3" && cells[1] == "MIROC5_45")
                        c17.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "2" && cells[1] == "MIROC5_85")
                        c18.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "3" && cells[1] == "MIROC5_85")
                        c19.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "2" && cells[1] == "MPI-ESM-MR_26")
                        c20.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "3" && cells[1] == "MPI-ESM-MR_26")
                        c21.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "2" && cells[1] == "MPI-ESM-MR_45")
                        c22.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "3" && cells[1] == "MPI-ESM-MR_45")
                        c23.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "2" && cells[1] == "MPI-ESM-MR_85")
                        c24.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    else if (cells[0] == "3" && cells[1] == "MPI-ESM-MR_85")
                        c25360.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "360");
                    if (cells[0] == "2" && cells[1] == "GFDL-CM3_45")
                        c25499.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "499");
                    else if (cells[0] == "3" && cells[1] == "GFDL-CM3_45")
                        c26.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "532");
                    else if (cells[0] == "2" && cells[1] == "GFDL-CM3_85")
                        c27.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "571");
                    else if (cells[0] == "3" && cells[1] == "GFDL-CM3_85")
                        c28.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "801");
                    else if (cells[0] == "2" && cells[1] == "GISS-E2-R_45")
                        c29.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "499");
                    else if (cells[0] == "3" && cells[1] == "GISS-E2-R_45")
                        c30.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "532");
                    else if (cells[0] == "2" && cells[1] == "GISS-E2-R_85")
                        c31.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "571");
                    else if (cells[0] == "3" && cells[1] == "GISS-E2-R_85")
                        c32.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "801");
                    else if (cells[0] == "2" && cells[1] == "HadGEM2-ES_26")
                        c33.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "442");
                    else if (cells[0] == "3" && cells[1] == "HadGEM2-ES_26")
                        c34.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "429");
                    else if (cells[0] == "2" && cells[1] == "HadGEM2-ES_45")
                        c35.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "499");
                    else if (cells[0] == "3" && cells[1] == "HadGEM2-ES_45")
                        c36.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "532");
                    else if (cells[0] == "2" && cells[1] == "HadGEM2-ES_85")
                        c37.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "571");
                    else if (cells[0] == "3" && cells[1] == "HadGEM2-ES_85")
                        c38.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "801");
                    else if (cells[0] == "2" && cells[1] == "MIROC5_45")
                        c39.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "499");
                    else if (cells[0] == "3" && cells[1] == "MIROC5_45")
                        c40.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "532");
                    else if (cells[0] == "2" && cells[1] == "MIROC5_85")
                        c41.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "571");
                    else if (cells[0] == "3" && cells[1] == "MIROC5_85")
                        c42.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "801");
                    else if (cells[0] == "2" && cells[1] == "MPI-ESM-MR_26")
                        c43.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "442");
                    else if (cells[0] == "3" && cells[1] == "MPI-ESM-MR_26")
                        c44.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "429");
                    else if (cells[0] == "2" && cells[1] == "MPI-ESM-MR_45")
                        c45.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "499");
                    else if (cells[0] == "3" && cells[1] == "MPI-ESM-MR_45")
                        c46.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "532");
                    else if (cells[0] == "2" && cells[1] == "MPI-ESM-MR_85")
                        c47.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "571");
                    else if (cells[0] == "3" && cells[1] == "MPI-ESM-MR_85")
                        c48.WriteLine(year + '\t' + month + '\t' + day + '\t' + cells[4] + '\t' + cells[3] + '\t' + cells[8] + '\t' + cells[7] + '\t' + "-99" + '\t' + cells[9] + '\t' + windspeed + '\t' + cells[11] + '\t' + "801");
                    

                }
                c1.Close();
                c2.Close();
                c3.Close();
                c4.Close();
                c5.Close();
                c6.Close();
                c7.Close();
                c8.Close();
                c9.Close();
                c10.Close();
                c11.Close();
                c12.Close();
                c13.Close();
                c14.Close();
                c15.Close();
                c16.Close();
                c17.Close();
                c18.Close();
                c19.Close();
                c20.Close();
                c21.Close();
                c22.Close();
                c23.Close();
                c24.Close();
                c25360.Close();
                c25499.Close();
                c26.Close();
                c27.Close();
                c28.Close();
                c29.Close();
                c30.Close();
                c31.Close();
                c32.Close();
                c33.Close();
                c34.Close();
                c35.Close();
                c36.Close();
                c37.Close();
                c38.Close();
                c39.Close();
                c40.Close();
                c41.Close();
                c42.Close();
                c43.Close();
                c44.Close();
                c45.Close();
                c46.Close();
                c47.Close();
                c48.Close();

            }
        }
    }
}
