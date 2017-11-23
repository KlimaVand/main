using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Linq;

namespace GrassClover
{
    class Program
    {
        static void Main(string[] args)
        {
             FileInformation settings = new FileInformation("system.xml");
            
            int minSetting = 99, maxSetting = 0;
            settings.setPath("settings");
            settings.getSectionNumber(ref minSetting, ref maxSetting);
            for (int settingsID = minSetting; settingsID <= maxSetting; settingsID++)
            {
                settings.setPath("settings");
                if (settings.doesIDExist(settingsID))
                {
                    settings.setPath("settings(" + settingsID+")");
                    string input = settings.getItemString("inputData");
                    string outputDir = settings.getItemString("outputDir");
                    FileInformation data = new FileInformation(input);
                    data.setPath("input(-1).DMgrassMax(-1)");
                    double DMgrassMax = data.getItemDouble("Value");//
                    data.setPath("input(-1).DMcloverMax(-1)");
                    double DMcloverMax = data.getItemDouble("Value");// 8000;
                    data.setPath("input(-1).GrassNconcMin(-1)");
                    double GrassNconcMin = data.getItemDouble("Value");// 1.0;
                    data.setPath("input(-1).GrassNconcMax(-1)");
                    double GrassNconcMax = data.getItemDouble("Value");//3.5;
                    data.setPath("input(-1).CloverNconcMax(-1)");
                    double CloverNconcMax = data.getItemDouble("Value");// 3.5;
                    data.setPath("input(-1).fixationEfficiency(-1)");
                    double fixationEfficiency = data.getItemDouble("Value");// 3.5;
                    data.setPath("input(-1).CloverDMdeclineFactor(-1)");
                    double CloverDMdeclineFactor = data.getItemDouble("Value");//0.4;
                    data.setPath("input(-1).nonHarvRatioGrass(-1)");
                    double nonHarvRatioGrass = data.getItemDouble("Value");// 1.0;
                    data.setPath("input(-1).nonHarvRatioClover(-1)");
                    double nonHarvRatioClover = data.getItemDouble("Value");// 1.0;
                    data.setPath("input(-1).GrassNonharvestableNconc(-1)");
                    double GrassNonharvestableNconc = data.getItemDouble("Value");// 1.5;
                    data.setPath("input(-1).CloverNonharvestableNconc(-1)");
                    double CloverNonharvestableNconc = data.getItemDouble("Value");//3.5;
                    data.setPath("input(-1).uptakeWeightingFactor(-1)");
                    double uptakeWeightingFactor = data.getItemDouble("Value");//k = 1;
                    data.setPath("input(-1).lossFrac(-1)");
                    double lossFrac = data.getItemDouble("Value");//0.2;
                    data.setPath("input(-1).maxFert(-1)");
                    double maxFert = data.getItemDouble("Value");//0.2;
                    data.setPath("input(-1).steps(-1)");
                    int steps = data.getItemInt("Value");//0.2;
                    string outfileName;
                    if (maxSetting > 1)
                        outfileName = outputDir + "Results" + settingsID + ".txt";
                    else
                        outfileName = outputDir + "Results.txt";
                    System.IO.StreamWriter file = new System.IO.StreamWriter(outfileName);
                    file.WriteLine("fertN_kg" + "\t" + "soilMinN_kg" + "\t" + "%Ngrass" + "\t" + "DMgrass_kg" + "\t" + "DMclover_kg" + "\t" + "DMtot_kg" +
                        "\t" + "Nupgrass_kg" + "\t" + "Nupclover_kg" + "\t" + "Nfix_kg" + "\t" +
                        "Nyield_kg" + "\t" + "residN_kg" + "\t" + "propClov" + "\t" + "fix rate_kg/Mg" +
                        "\t" + "grassNyield_kg" + "\t" + "cloverNyield_kg" + "\t" + "NconcTot%" + "\t" + "Nloss_kg");

                    double GrassTopMaxN = DMgrassMax * GrassNconcMax / 100;
                    double GrassUptakeMax = DMgrassMax * ((GrassNconcMax / 100) + nonHarvRatioGrass * (GrassNonharvestableNconc / 100));
                    double grassNconcSlope = 0;
                    double CloverDMdecline = 0;
                    if (DMgrassMax > 0)
                    {
                        grassNconcSlope = (GrassNconcMax - GrassNconcMin) / DMgrassMax;
                        CloverDMdecline=CloverDMdeclineFactor * DMcloverMax / DMgrassMax;
                    }
                    double fertIncrement = maxFert / steps;
                    double availN = 0;
                    double fertN = 0;
                    double grassNconc = 0;
                    double grassNuptake = 0;
                    double grassDMtop = 0;
                    double grassResidN = 0;
                    double cloverResidN = 0;
                    double cloverDMtop = 0;
                    double cloverNuptake = 0;
                    double fixedN = 0;
                    double grassNyield = 0;
                    double cloverNyield = 0;
                    double totResidN = 0;
                    double soilMineralN = 0;
                    double totalNloss = 0;
                    double Ninput = 0;
                    double Noutput = 0;
                    double k = uptakeWeightingFactor * DMgrassMax / (DMgrassMax + DMcloverMax);
                    if (k > 1.0)
                        k = 1.0;
                    for (int j = 0; j < steps + 1; j++)
                    {
                        fertN = fertIncrement * j;
                        availN = 0;
                        if (fertN == 300)
                            Console.Write("");
                        for (int i = 0; i < 100; i++)
                        {
                            if (i == 99)
                                Console.Write("");
                            availN = fertN + soilMineralN;
                            Ninput = availN;
                            grassNuptake = k * availN;
                            if (grassNuptake > availN)
                                grassNuptake = availN;
                            if (grassNuptake > GrassUptakeMax)
                                grassNuptake = GrassUptakeMax;
                            else
                                grassNuptake = availN * k;
                            if (grassNuptake < GrassUptakeMax)
                            {
                                if (grassNuptake == 0)
                                    grassDMtop = 0;
                                else
                                {
                                    double constant = -grassNuptake;
                                    double lin = (GrassNconcMin / 100) + (GrassNonharvestableNconc / 100) / nonHarvRatioGrass;
                                    double quad = ((GrassNconcMax - GrassNconcMin) / 100) / DMgrassMax;
                                    grassDMtop = (-lin + Math.Sqrt((lin * lin - 4 * quad * constant))) / (2 * quad);
                                }
                            }
                            else
                            {
                                grassDMtop = DMgrassMax;
                                grassNuptake = GrassUptakeMax;
                            }
                            if (grassDMtop == DMgrassMax)
                                grassNconc = GrassNconcMax;
                            else
                                grassNconc = GrassNconcMin + grassNconcSlope * grassDMtop;
                            grassResidN = nonHarvRatioGrass * grassDMtop * GrassNonharvestableNconc / 100;
                            grassNyield = grassDMtop * grassNconc / 100;
                            cloverDMtop = DMcloverMax - CloverDMdecline * grassDMtop;
                            if (cloverDMtop < 0)
                                cloverDMtop = 0;
                            cloverResidN = cloverDMtop * nonHarvRatioClover * CloverNonharvestableNconc / 100;
                            double maxCloverNuptake = cloverResidN + cloverDMtop * CloverNconcMax / 100;
                            if (availN > grassNuptake)
                                cloverNuptake = availN - grassNuptake;
                            if (cloverNuptake > maxCloverNuptake)
                                cloverNuptake = maxCloverNuptake;
                            fixedN = cloverResidN + fixationEfficiency * (cloverDMtop * (CloverNconcMax / 100) - cloverNuptake);
                            Ninput += fixedN;
                            cloverNyield = cloverDMtop * CloverNconcMax / 100;
                            totResidN = cloverResidN + grassResidN;
                            soilMineralN = (1 - lossFrac) * totResidN;
                            totalNloss = lossFrac * totResidN + availN-(cloverNuptake+grassNuptake);
                            Noutput = grassNyield+cloverNyield+ soilMineralN+totalNloss;
                        }
                        if (Math.Abs(Ninput-Noutput)>0.001)
                            Console.WriteLine("N budget error");
                        double Nyield = grassNyield + cloverNyield;
                        double totDMyield = cloverDMtop + grassDMtop;
                        double propClover = 0;
                        if (totDMyield>0)
                            propClover=cloverDMtop / totDMyield;
                        double propFixed = fixedN / (grassNuptake + cloverNuptake + fixedN);
                        double fixRate = 0;
                        double HarvNconc = 0;
                        if (totDMyield>0)
                            HarvNconc=100* Nyield / totDMyield;
                        if (cloverDMtop > 0)
                            fixRate = 1000 * fixedN / cloverDMtop;
                        Console.WriteLine("fertN " + fertN.ToString("F2") + " soilMinN " + availN.ToString("F2") + " %Ng " + grassNconc.ToString("F2") + " DMg " + grassDMtop.ToString("F2") + " DMc " +
                            cloverDMtop.ToString("F2") + " Nupg " + grassNuptake.ToString("F2") + " Nupc " + cloverNuptake.ToString("F2") + " Nfix " +
                            fixedN.ToString("F2") + " Nyield " + Nyield.ToString("F2") + " propClov " + propClover.ToString("F2") + " fixRate " +
                            fixRate.ToString("F2"));

                        file.WriteLine(fertN.ToString("F2") + "\t" + soilMineralN.ToString("F2") + "\t" + grassNconc.ToString("F2") + "\t" +
                            grassDMtop.ToString("F2") + "\t" + cloverDMtop.ToString("F2") + "\t" + totDMyield.ToString("f2") + "\t" +
                            grassNuptake.ToString("F2") + "\t" + cloverNuptake.ToString("F2") + "\t" + fixedN.ToString("F2") + "\t" +
                            Nyield.ToString("F2") + "\t" + totResidN.ToString("F2") + "\t" + propClover.ToString("F2") + "\t" + fixRate.ToString("F2") +
                            "\t" + grassNyield.ToString("F2") + "\t" + cloverNyield.ToString("F2") + "\t" + HarvNconc.ToString("F2") + "\t" + totalNloss.ToString("F2"));

                    }
                    file.Close();
                }
            }
        }
    }
}
