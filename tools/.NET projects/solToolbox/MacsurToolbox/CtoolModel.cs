using System;
using System.Collections.Generic;

namespace AgroModelToolbox
{
    public class CtoolModel
    {
        double func(double time, double amount, double k, double tempCoefficient)
        {
            double value = amount * -k * tempCoefficient * time;
            return value;
        }
        double rk4decay(double t0, double u0, double dt, double k, double tempCoefficient)
        {
            double f1 = func(t0, u0, k, tempCoefficient);
            double f2 = func(t0 + dt / 2, u0 + dt * f1 / 2, k, tempCoefficient);
            double f3 = func(t0 + dt / 2, u0 + dt * f2 / 2, k, tempCoefficient);
            double f4 = func(t0 + dt, u0 + dt * f3, k, tempCoefficient);

            double u1 = u0 + dt * (f1 + 2.0 * f2 + 2.0 * f3 + f4) / 6.0;
            return u1;
        }


        double temperatureCoefficent(double temperature)
        {
            return 7.24 * Math.Exp(-3.432 + 0.168 * temperature * (1 - 0.5 * temperature / 36.9));

        }
        double CalcDampingDepth(double k, double rho)
        {
            return Math.Sqrt(2.0 * k / rho);
        }
        double Temperature(double avgTemperature, double day, double depth, double amplitude, double offset)
        {
            double rho = 3.1415926 * 2.0 / (365.0 * 24.0 * 3600.0);
            double Th_diff = 0.35E-6;
            double dampingDepth = CalcDampingDepth(Th_diff, rho);
            double retVal = avgTemperature + amplitude * Math.Exp(-depth / dampingDepth) * Math.Sin(rho * (day + offset) * 24.0 * 3600.0 - depth / dampingDepth);
            return retVal;

        }
        double[] decompositionHum(int numberOfLayers, double avgTemperature, double amplitude, double offset, double t, double i, double HUMdecompositionrate, ref double[] humc, double ROMfraction, ref double[] romc, bool WithTransport, bool C14, double DecayRate, System.IO.StreamWriter transportFile, System.IO.StreamWriter C02, int mode)
        {
            double[] humcLeft = new double[numberOfLayers];
            double[] CO2output = new double[numberOfLayers];
            for (int j = 0; j < (numberOfLayers); j++)
            {
                double depthInLayer;
                if (j == 0)
                    depthInLayer = 25.0 / 2.0;
                else
                    depthInLayer = 75.0 / 2.0 + 25.0 / 2.0;

                double tempCofficent = temperatureCoefficent(Temperature(avgTemperature, i, depthInLayer, amplitude, offset));

                double substract = 0;
                if (C14 == true)
                    substract = DecayRate * humc[j];
                double humAfterDecom = rk4decay(i * t, humc[j], t, HUMdecompositionrate, tempCofficent) - substract;

                humcLeft[j] = humcLeft[j] + humAfterDecom;
                humc[j] = humc[j] - humAfterDecom;



                double CO2 = humc[j] * 0.628;
                CO2output[j] = CO2;
                double transport;
                if (WithTransport == true)
                    transport = humc[j] * (1 - 0.628 - ROMfraction);
                else
                    transport = 0;


                double romification = humc[j] * ROMfraction;
                humc[j] = humc[j] - CO2 - transport - romification;
                if (j != (numberOfLayers - 1))
                    humcLeft[j + 1] = humcLeft[j + 1] + transport;
                else
                    humcLeft[j] = humcLeft[j] + transport;

                romc[j] = romc[j] + romification;
                if (mode == 0 || mode == 1)
                {
                    C02.Write(CO2.ToString() + '\t');
                    transportFile.Write(transport.ToString() + '\t');
                }


            }
            for (int j = 0; j < (numberOfLayers); j++)
                humc[j] = humcLeft[j];
            return CO2output;
        }
        void decompositionFom(int numberOfLayers, double avgTemperature, double amplitude, double offset, double t, double i, ref double[] fomc, double FOMdecompositionrate, double tF, double humification, ref double[] humc, bool WithTransport, bool C14, double DecayRate, System.IO.StreamWriter transportFile, System.IO.StreamWriter C02, int mode)
        {
            double[] fomcLeft = new double[numberOfLayers];
            double[] output = new double[4];
            for (int j = 0; j < numberOfLayers; j++)
            {
                double depthInLayer;
                if (j == 0)
                    depthInLayer = 25.0 / 2.0;
                else
                    depthInLayer = 75.0 / 2.0 + 25.0 / 2.0;
                double substract = 0;
                if (C14 == true)
                    substract = DecayRate * fomc[j];
                double tempCofficent = temperatureCoefficent(Temperature(avgTemperature, i, depthInLayer, amplitude, offset));

                double FomAfterDecom = rk4decay(i * t, fomc[j], t, FOMdecompositionrate, tempCofficent) - substract;

                fomcLeft[j] = fomcLeft[j] + FomAfterDecom;
                fomc[j] = fomc[j] - FomAfterDecom;
                double toLowerLayer;
                double inCorrentLayer;

                if (WithTransport == true)
                {
                    toLowerLayer = fomc[j] * tF;
                    inCorrentLayer = fomc[j] * (1 - tF);
                }
                else
                {
                    toLowerLayer = 0;
                    inCorrentLayer = 0;
                }


                if (j != (numberOfLayers - 1))
                    fomcLeft[j + 1] = fomcLeft[j + 1] + toLowerLayer;
                else
                    fomcLeft[j] = fomcLeft[j] + toLowerLayer;

                fomc[j] = inCorrentLayer;



                double CO2 = fomc[j] * (1 - humification);
                double humificationAmount = fomc[j] * humification;
                fomc[j] = fomc[j] - CO2 - humificationAmount;
                humc[j] = humc[j] + humificationAmount;
                if (mode == 2)
                {
                    C02.Write(CO2.ToString() + '\t');
                }
                if (mode == 0 || mode == 1)
                {
                    C02.Write(CO2.ToString() + '\t');
                    transportFile.Write(toLowerLayer.ToString() + '\t');
                }

            }
            for (int j = 0; j < numberOfLayers; j++)
                fomc[j] = fomcLeft[j];
        }
        double[] decompositionRom(int numberOfLayers, double avgTemperature, double amplitude, double offset, double t, double i, ref double[] romc, double ROMdecompositionrate, bool WithTransport, bool C14, double DecayRate, System.IO.StreamWriter transportFile, System.IO.StreamWriter C02, int mode)
        {
            double[] romcLeft = new double[numberOfLayers];
            double[] output = new double[4];
            double[] CO2output = new double[numberOfLayers];
            for (int j = 0; j < (numberOfLayers); j++)
            {
                double depthInLayer;
                if (j == 0)
                    depthInLayer = 25.0 / 2.0;
                else
                    depthInLayer = 75.0 / 2.0 + 25.0 / 2.0;

                double tempCofficent = temperatureCoefficent(Temperature(avgTemperature, i, depthInLayer, amplitude, offset));

                double substract = 0;
                if (C14 == true)
                    substract = DecayRate * romc[j];
                double romAfterDecom = rk4decay(i * t, romc[j], t, ROMdecompositionrate, tempCofficent) - substract;

                romcLeft[j] = romcLeft[j] + romAfterDecom;
                double romcLeftForInert = romc[j] - romAfterDecom;

                romc[j] = romc[j] - romAfterDecom;


                double CO2 = romc[j] * 0.628;
                CO2output[j] = CO2;
                double transport;
                if (WithTransport == true)
                    transport = romc[j] * (1 - 0.628);
                else
                    transport = 0;

                romc[j] = romc[j] - CO2 - transport;

                if (j != (numberOfLayers - 1))
                    romcLeft[j + 1] = romcLeft[j + 1] + transport;
                else
                    romcLeft[j] = romcLeft[j] + transport;
                if (mode == 0 || mode == 1)
                {
                    C02.Write(CO2.ToString() + '\t');
                    transportFile.Write(transport.ToString() + '\t');
                }

            }
            for (int j = 0; j < (numberOfLayers); j++)
                romc[j] = romcLeft[j];
            return CO2output;

        }
        public struct Data
        {
            public int year;
            public double UpperLayerC;
            public double LowerLayerC;
            public double ManureC;
            public double C14PlantProcent;
            public double C14Manure;
        }
        double R(double Clayfraction)
        {


            return 1.67 * (1.85 + 1.6 * Math.Exp(-7.86 * Clayfraction));
        }
        double CN(double cn)
        {

            return Math.Min(56.2 * Math.Pow(cn, -1.69), 1);
        }
        public void Main(List<double> TemperatureData, List<Data> allData, double DecayRateC14Manuer, double ROMdecompositionrateManureC14, double ROMfractionManureC14, double tFManureC14, double ClayfractionManureC14, double FOMdecompositionrateManureC14, double HUMfractionManureC14, double HUMdecompositionrateManureC14, double DecayRateC14Plant, double ROMdecompositionratePlantC14, double ROMfractionPlanC14t, double tFPlantC14, double ClayfractionPlantC14, double FOMdecompositionratePlantC14, double ROMdecompositionrateManure, double ROMfractionManure, double tFManure, double ClayfractionManure, double FOMdecompositionrateManure, double HUMfractionManure, double HUMdecompositionrateManure, double ROMfractionPlant, double tFPlant, double ClayfractionPlant, double FOMdecompositionratePlant, double PLoweLayer, double offset, double depth, double PupperLayer, double C14percent, double startAmountOfCarbon, double CNratio, double extraCarbon, double HUMdecompositionratePlant, double ROMdecompositionratePlant, double ROMfractionPlantC14, double HUMdecompositionratePlantC14, int mode)
        {
            bool WithTransport = true;
            extraCarbon = extraCarbon / 12;
            System.IO.StreamWriter transportFile = null;
            System.IO.StreamWriter C02 = null;
            System.IO.StreamWriter totalAmount = null;
            if (mode == 2)
            {
                totalAmount = new System.IO.StreamWriter("totalAmount.xls");

                totalAmount.Write("fomcPlant(1,1)" + '\t');
                totalAmount.Write("humcPlant(1,1) and romcPlant(1,1)" + '\t');
                totalAmount.Write("fomcManure(1,1)" + '\t');
                totalAmount.Write("humcManure(1,1) and romcManure(1,1)" + '\t');
                totalAmount.Write("fomcPlantC14(1,1)" + '\t');
                totalAmount.Write("humcPlantC14(1,1) and romcPlantC14(1,1)" + '\t');
                totalAmount.Write("fomcManureC14(1,1)C14" + '\t');
                totalAmount.Write("humcManureC14(1,1) and romcManureC14(1,1)" + '\t');
                totalAmount.Write("Total layer 1" + '\t');

                totalAmount.Write("fomcPlant(2,1)" + '\t');
                totalAmount.Write("humcPlant(2,1) and romcPlant(2,1)" + '\t');
                totalAmount.Write("fomcManure(2,1)" + '\t');
                totalAmount.Write("humcManure(2,1) and romcManure(2,1)" + '\t');
                totalAmount.Write("fomcPlantC14(2,1)" + '\t');
                totalAmount.Write("humcPlantC14(2,1) and romcPlantC14(2,1)" + '\t');
                totalAmount.Write("fomcManureC14(2,1)" + '\t');
                totalAmount.Write("humcManureC14(2,1) and romcManureC14(2,1)" + '\t');
                totalAmount.Write("Total layer 2" + '\t');
                totalAmount.WriteLine("Total" + '\t');
                C02 = new System.IO.StreamWriter("C02.xls");
                C02.Write("FomPl1" + '\t');
                C02.Write("FomPl2" + '\t');
                C02.Write("HumPl1 and RomPl1" + '\t');
                C02.Write("HumPl2 and RomPl2" + '\t');
                C02.Write("FomMl1" + '\t');
                C02.Write("FomMl2" + '\t');
                C02.Write("HumMl1 and RomMl1" + '\t');
                C02.Write("HumMl2 and RomMl2" + '\t');
                C02.Write("FomC14Pl1" + '\t');
                C02.Write("FomC14Pl2" + '\t');
                C02.Write("HumC14Pl1 and RomC14Pl1" + '\t');
                C02.Write("HumC14Pl2 and RomC14Pl2" + '\t');
                C02.Write("FomC14Ml1" + '\t');
                C02.Write("FomC14Ml2" + '\t');
                C02.Write("HumC14Ml1 and RomC14Ml1" + '\t');
                C02.WriteLine("HumC14Ml2 and RomC14Ml2" + '\t');

            }
            if (mode == 0 || mode == 1)
            {
                totalAmount = new System.IO.StreamWriter("totalAmount.xls");

                totalAmount.Write("fomcPlant(1,1)" + '\t');
                totalAmount.Write("humcPlant(1,1)" + '\t');
                totalAmount.Write("romcPlant(1,1)" + '\t');

                totalAmount.Write("fomcManure(1,1)" + '\t');
                totalAmount.Write("humcManure(1,1)" + '\t');
                totalAmount.Write("romcManure(1,1)" + '\t');

                if (mode == 0)
                {
                    totalAmount.Write("fomcPlantC14(1,1)" + '\t');
                    totalAmount.Write("humcPlantC14(1,1)" + '\t');
                    totalAmount.Write("romcPlantC14(1,1)" + '\t');

                    totalAmount.Write("fomcManureC14(1,1)" + '\t');
                    totalAmount.Write("humcManureC14(1,1)" + '\t');
                    totalAmount.Write("romcManureC14(1,1)" + '\t');
                }
                totalAmount.Write("total(1,1)" + '\t');

                totalAmount.Write("fomcPlant(2,1)" + '\t');
                totalAmount.Write("humcPlant(2,1)" + '\t');
                totalAmount.Write("romcPlant(2,1)" + '\t');

                totalAmount.Write("fomcManure(2,1)" + '\t');
                totalAmount.Write("humcManure(2,1)" + '\t');
                totalAmount.Write("romcManure(2,1)" + '\t');
                if (mode == 0)
                {
                    totalAmount.Write("fomcPlantC14(2,1)" + '\t');
                    totalAmount.Write("humcPlantC14(2,1)" + '\t');
                    totalAmount.Write("romcPlantC14(2,1)" + '\t');

                    totalAmount.Write("fomcManureC14(2,1)" + '\t');
                    totalAmount.Write("humcManureC14(2,1)" + '\t');
                    totalAmount.Write("romcManureC14(2,1)" + '\t');
                }

                totalAmount.WriteLine("total(2,1)");
                transportFile = new System.IO.StreamWriter("transport.xls");
                C02 = new System.IO.StreamWriter("C02.xls");
                C02.Write("FomPl1" + '\t');
                C02.Write("FomPl2" + '\t');
                C02.Write("HumPl1" + '\t');
                C02.Write("HumPl2" + '\t');
                C02.Write("RomPl1" + '\t');
                C02.Write("RomPl2" + '\t');
                C02.Write("FomMl1" + '\t');
                C02.Write("FomMl2" + '\t');
                C02.Write("HumMl1" + '\t');
                C02.Write("HumMl2" + '\t');
                C02.Write("RomMl1" + '\t');
                if (mode == 1)
                    C02.WriteLine("RomMl2" + '\t');
                if (mode == 0)
                {
                    C02.Write("RomMl2" + '\t');
                    C02.Write("FomC14Pl1" + '\t');
                    C02.Write("FomC14Pl2" + '\t');
                    C02.Write("HumC14Pl1" + '\t');
                    C02.Write("HumC14Pl2" + '\t');
                    C02.Write("RomC14Pl1" + '\t');
                    C02.Write("RomC14Pl2" + '\t');
                    C02.Write("FomC14Ml1" + '\t');
                    C02.Write("FomC14Ml2" + '\t');
                    C02.Write("HumC14Ml1" + '\t');
                    C02.Write("HumC14Ml2" + '\t');
                    C02.Write("RomC14Ml1" + '\t');
                    C02.WriteLine("RomC14Ml2");
                }

                transportFile.Write("FomPl1" + '\t');
                transportFile.Write("FomPl2" + '\t');
                transportFile.Write("HumPl1" + '\t');
                transportFile.Write("HumPl2" + '\t');
                transportFile.Write("RomPl1" + '\t');
                transportFile.Write("RomPl2" + '\t');
                transportFile.Write("FomMl1" + '\t');
                transportFile.Write("FomMl2" + '\t');
                transportFile.Write("HumMl1" + '\t');
                transportFile.Write("HumMl2" + '\t');
                transportFile.Write("RomMl1" + '\t');
                transportFile.Write("RomMl2");
                if (mode == 1)
                    transportFile.WriteLine("RomMl2" + '\t');
                if (mode == 0)
                {
                    transportFile.Write("FomC14Pl1" + '\t');
                    transportFile.Write("FomC14Pl2" + '\t');
                    transportFile.Write("HumC14Pl1" + '\t');
                    transportFile.Write("HumC14Pl2" + '\t');
                    transportFile.Write("RomC14Pl1" + '\t');
                    transportFile.Write("RomC14Pl2" + '\t');
                    transportFile.Write("FomC14Ml1" + '\t');
                    transportFile.Write("FomC14Ml2" + '\t');
                    transportFile.Write("HumC14Ml1" + '\t');
                    transportFile.Write("HumC14Ml2" + '\t');
                    transportFile.Write("RomC14Ml1" + '\t');
                    transportFile.WriteLine("RomC14Ml2");
                }
            }
            int i = 0;
            int numberOfLayers = 2;


            double startAmountLayer1 = startAmountOfCarbon * 0.47;
            double startAmountLayer2 = startAmountOfCarbon * 0.53;
            double[] fomcPlant = new double[numberOfLayers];
            double[] humcPlant = new double[numberOfLayers];
            double[] romcPlant = new double[numberOfLayers];

            double[] fomcManure = new double[numberOfLayers];
            double[] humcManure = new double[numberOfLayers];
            double[] romcManure = new double[numberOfLayers];

            double[] fomcPlantC14 = new double[numberOfLayers];
            double[] humcPlantC14 = new double[numberOfLayers];
            double[] romcPlantC14 = new double[numberOfLayers];

            double[] fomcManureC14 = new double[numberOfLayers];
            double[] humcManureC14 = new double[numberOfLayers];
            double[] romcManureC14 = new double[numberOfLayers];


            double min = 999999;
            double max = 0;
            for (int j = 0; j < TemperatureData.Count; j++)
            {
                if (TemperatureData[j] < min)
                    min = TemperatureData[j];

                if (TemperatureData[j] > max)
                    max = TemperatureData[j];
            }
            double amplitude = (max - min) / 2;

            int iterations = allData.Count * 12 - 1;
            double CNfraction = CN(CNratio);
            humcPlant[0] = startAmountLayer1 * PupperLayer * CNfraction;
            romcPlant[0] = startAmountLayer1 - humcPlant[0];
            humcPlant[1] = startAmountLayer2 * PLoweLayer * CNfraction;
            romcPlant[1] = startAmountLayer2 - humcPlant[1];


            humcPlantC14[0] = (startAmountLayer1) * PupperLayer * (C14percent / 100) * CNfraction;
            romcPlantC14[0] = (startAmountLayer1) * (C14percent / 100) - humcPlantC14[0];
            humcPlantC14[1] = (startAmountLayer2) * PLoweLayer * (C14percent / 100) * CNfraction;
            romcPlantC14[1] = (startAmountLayer2) * (C14percent / 100) - humcPlantC14[1];

            while (i <= iterations)
            {

                int month = i % 12 + 1;
                double tmp = (i) / 12;
                int year = (int)Math.Floor(tmp) + 1;
                double julianday = month * 30.4166;
                double t = 0.083;
                if ((month % 12) == 3)
                {
                    fomcManure[0] = fomcManure[0] + (allData[year - 1].ManureC * (1 - HUMfractionManure));
                    humcManure[0] = humcManure[0] + (allData[year - 1].ManureC * HUMfractionManure);

                    fomcManureC14[0] = fomcManureC14[0] + ((allData[year - 1].ManureC * (1 - HUMfractionManureC14))) * (allData[year - 1].C14Manure / 100);
                    humcManureC14[0] = humcManureC14[0] + ((allData[year - 1].ManureC * HUMfractionManureC14)) * (allData[year - 1].C14Manure / 100);

                }
                fomcPlant[0] = fomcPlant[0] + extraCarbon;
                if ((month % 12) == 4)
                {
                    fomcPlant[0] = fomcPlant[0] + (allData[year - 1].UpperLayerC) * 0.08;
                    fomcPlant[1] = fomcPlant[1] + (allData[year - 1].LowerLayerC) * 0.08;

                    fomcPlantC14[0] = fomcPlantC14[0] + (allData[year - 1].UpperLayerC * (allData[year - 1].C14PlantProcent / 100)) * 0.08;
                    fomcPlantC14[1] = fomcPlantC14[1] + (allData[year - 1].LowerLayerC * (allData[year - 1].C14PlantProcent / 100)) * 0.08;


                }
                if ((month % 12) == 5)
                {
                    fomcPlant[0] = fomcPlant[0] + (allData[year - 1].UpperLayerC) * 0.12;
                    fomcPlant[1] = fomcPlant[1] + (allData[year - 1].LowerLayerC) * 0.12;


                    fomcPlantC14[0] = fomcPlantC14[0] + (allData[year - 1].UpperLayerC * (allData[year - 1].C14PlantProcent / 100)) * 0.12;
                    fomcPlantC14[1] = fomcPlantC14[1] + (allData[year - 1].LowerLayerC * (allData[year - 1].C14PlantProcent / 100)) * 0.12;


                }
                if ((month % 12) == 6)
                {
                    fomcPlant[0] = fomcPlant[0] + (allData[year - 1].UpperLayerC) * 0.16;
                    fomcPlant[1] = fomcPlant[1] + (allData[year - 1].LowerLayerC) * 0.16;


                    fomcPlantC14[0] = fomcPlantC14[0] + (allData[year - 1].UpperLayerC * (allData[year - 1].C14PlantProcent / 100)) * 0.16;
                    fomcPlantC14[1] = fomcPlantC14[1] + (allData[year - 1].LowerLayerC * (allData[year - 1].C14PlantProcent / 100)) * 0.16;

                }
                if ((month % 12) == 7)
                {
                    fomcPlant[0] = fomcPlant[0] + (allData[year - 1].UpperLayerC) * 0.64;
                    fomcPlant[1] = fomcPlant[0] + (allData[year - 1].LowerLayerC) * 0.64;


                    fomcPlantC14[0] = fomcPlantC14[0] + (allData[year - 1].UpperLayerC * (allData[year - 1].C14PlantProcent / 100)) * 0.64;
                    fomcPlantC14[1] = fomcPlantC14[0] + (allData[year - 1].LowerLayerC * (allData[year - 1].C14PlantProcent / 100)) * 0.64;

                }
                double RfractionPlant = R(ClayfractionPlant);
                double humificationPlant = 1 / (RfractionPlant + 1);
                decompositionFom(numberOfLayers, TemperatureData[i], amplitude, offset, t, julianday, ref fomcPlant, FOMdecompositionratePlant, tFPlant, humificationPlant, ref humcPlant, WithTransport, false, 0, transportFile, C02, mode);
                double[] co2Hum = decompositionHum(numberOfLayers, TemperatureData[i], amplitude, offset, t, julianday, HUMdecompositionratePlant, ref humcPlant, ROMfractionPlant, ref romcPlant, WithTransport, false, 0, transportFile, C02, mode);
                double[] co2Rom = decompositionRom(numberOfLayers, TemperatureData[i], amplitude, offset, t, julianday, ref romcPlant, ROMdecompositionratePlant, WithTransport, false, 0, transportFile, C02, mode);
                if (mode == 2)
                {
                    C02.Write((co2Hum[0] + co2Rom[0]).ToString() + '\t');
                    C02.Write((co2Hum[1] + co2Rom[1]).ToString() + '\t');
                }
                double RfractionManure = R(ClayfractionManure);
                double humificationManure = 1 / (RfractionManure + 1);
                decompositionFom(numberOfLayers, TemperatureData[i], amplitude, offset, t, julianday, ref fomcManure, FOMdecompositionrateManure, tFManure, humificationManure, ref humcManure, WithTransport, false, 0, transportFile, C02, mode);
                co2Hum = decompositionHum(numberOfLayers, TemperatureData[i], amplitude, offset, t, julianday, HUMdecompositionrateManure, ref humcManure, ROMfractionManure, ref romcManure, WithTransport, false, 0, transportFile, C02, mode);
                co2Rom = decompositionRom(numberOfLayers, TemperatureData[i], amplitude, offset, t, julianday, ref romcManure, ROMdecompositionrateManure, WithTransport, false, 0, transportFile, C02, mode);
                if (mode == 2)
                {
                    C02.Write((co2Hum[0] + co2Rom[0]).ToString() + '\t');
                    C02.Write((co2Hum[1] + co2Rom[1]).ToString() + '\t');
                }
                if (mode != 1)
                {
                    double RfractionManureC14 = R(ClayfractionManureC14);
                    double humificationManureC14 = 1 / (RfractionManureC14 + 1);
                    double RfractionPlantC14 = R(ClayfractionPlantC14);
                    double humificationPlantC14 = 1 / (RfractionPlantC14 + 1);
                    decompositionFom(numberOfLayers, TemperatureData[i], amplitude, offset, t, julianday, ref fomcPlantC14, FOMdecompositionratePlantC14, tFPlantC14, humificationPlantC14, ref humcPlantC14, WithTransport, true, DecayRateC14Plant, transportFile, C02, mode);
                    co2Hum = decompositionHum(numberOfLayers, TemperatureData[i], amplitude, offset, t, julianday, HUMdecompositionratePlantC14, ref humcPlantC14, ROMfractionPlantC14, ref romcPlantC14, WithTransport, true, DecayRateC14Plant, transportFile, C02, mode);
                    co2Rom = decompositionRom(numberOfLayers, TemperatureData[i], amplitude, offset, t, julianday, ref romcPlantC14, ROMdecompositionratePlantC14, WithTransport, true, DecayRateC14Plant, transportFile, C02, mode);
                    if (mode == 2)
                    {
                        C02.Write((co2Hum[0] + co2Rom[0]).ToString() + '\t');
                        C02.Write((co2Hum[1] + co2Rom[1]).ToString() + '\t');
                    }
                    decompositionFom(numberOfLayers, TemperatureData[i], amplitude, offset, t, julianday, ref fomcManureC14, FOMdecompositionrateManureC14, tFManureC14, humificationManureC14, ref humcManureC14, WithTransport, true, DecayRateC14Manuer, transportFile, C02, mode);
                    co2Hum = decompositionHum(numberOfLayers, TemperatureData[i], amplitude, offset, t, julianday, HUMdecompositionrateManureC14, ref humcManureC14, ROMfractionManureC14, ref romcManureC14, WithTransport, true, DecayRateC14Manuer, transportFile, C02, mode);
                    co2Rom = decompositionRom(numberOfLayers, TemperatureData[i], amplitude, offset, t, julianday, ref romcManureC14, ROMdecompositionrateManureC14, WithTransport, true, DecayRateC14Manuer, transportFile, C02, mode);
                    if (mode == 2)
                    {
                        C02.Write((co2Hum[0] + co2Rom[0]).ToString() + '\t');
                        C02.WriteLine((co2Hum[1] + co2Rom[1]).ToString() + '\t');
                    }
                }
                i++;
                if (mode == 2)
                {
                    totalAmount.Write(fomcPlant[0].ToString() + '\t');
                    totalAmount.Write((humcPlant[0] + romcPlant[0]).ToString() + '\t');
                    totalAmount.Write(fomcManure[0].ToString() + '\t');
                    totalAmount.Write((humcManure[0] + romcManure[0]).ToString() + '\t');
                    totalAmount.Write(fomcPlantC14[0].ToString() + '\t');
                    totalAmount.Write((humcPlantC14[0] + romcPlantC14[0]).ToString() + '\t');
                    totalAmount.Write(fomcManureC14[0].ToString() + '\t');
                    totalAmount.Write((humcManureC14[0] + romcManureC14[0]).ToString() + '\t');
                    totalAmount.Write((fomcPlant[0] + humcPlant[0] + romcPlant[0] + fomcManure[0] + humcManure[0] + romcManure[0]).ToString() + '\t');

                    totalAmount.Write(fomcPlant[1].ToString() + '\t');
                    totalAmount.Write((humcPlant[1] + romcPlant[1]).ToString() + '\t');
                    totalAmount.Write(fomcManure[1].ToString() + '\t');
                    totalAmount.Write((humcManure[1] + romcManure[1]).ToString() + '\t');
                    totalAmount.Write(fomcPlantC14[1].ToString() + '\t');
                    totalAmount.Write((humcPlantC14[1] + romcPlantC14[1]).ToString() + '\t');
                    totalAmount.Write(fomcManureC14[1].ToString() + '\t');
                    totalAmount.Write((humcManureC14[1] + romcManureC14[1]).ToString() + '\t');
                    totalAmount.Write((fomcPlant[1] + humcPlant[1] + romcPlant[1] + fomcManure[1] + humcManure[1] + romcManure[1]).ToString() + '\t');
                    totalAmount.WriteLine((fomcPlant[0] + humcPlant[0] + romcPlant[0] + fomcManure[0] + humcManure[0] + romcManure[0] + fomcPlant[1] + humcPlant[1] + romcPlant[1] + fomcManure[1] + humcManure[1] + romcManure[1]).ToString() + '\t');
                }
                if (mode == 0 || mode == 1)
                {
                    C02.WriteLine();
                    transportFile.WriteLine();

                    totalAmount.Write(fomcPlant[0].ToString() + '\t');
                    totalAmount.Write(humcPlant[0].ToString() + '\t');
                    totalAmount.Write(romcPlant[0].ToString() + '\t');

                    totalAmount.Write(fomcManure[0].ToString() + '\t');
                    totalAmount.Write(humcManure[0].ToString() + '\t');
                    totalAmount.Write(romcManure[0].ToString() + '\t');
                    if (mode == 0)
                    {
                        totalAmount.Write(fomcPlantC14[0].ToString() + '\t');
                        totalAmount.Write(humcPlantC14[0].ToString() + '\t');
                        totalAmount.Write(romcPlantC14[0].ToString() + '\t');

                        totalAmount.Write(fomcManureC14[0].ToString() + '\t');
                        totalAmount.Write(humcManureC14[0].ToString() + '\t');
                        totalAmount.Write(romcManureC14[0].ToString() + '\t');
                        totalAmount.Write((fomcPlant[0] + humcPlant[0] + romcPlant[0] + fomcManure[0] + humcManure[0] + romcManure[0] + fomcPlantC14[0] + humcPlantC14[0] + romcPlantC14[0] + fomcManureC14[0] + humcManureC14[0] + romcManureC14[0]).ToString() + '\t');
                    }
                    if (mode == 1)
                        totalAmount.Write((fomcPlant[0] + humcPlant[0] + romcPlant[0] + fomcManure[0] + humcManure[0] + romcManure[0]).ToString() + '\t');


                    totalAmount.Write(fomcPlant[1].ToString() + '\t');
                    totalAmount.Write(humcPlant[1].ToString() + '\t');
                    totalAmount.Write(romcPlant[1].ToString() + '\t');

                    totalAmount.Write(fomcManure[1].ToString() + '\t');
                    totalAmount.Write(humcManure[1].ToString() + '\t');
                    totalAmount.Write(romcManure[1].ToString() + '\t');
                    if (mode == 0)
                    {
                        totalAmount.Write(fomcPlantC14[1].ToString() + '\t');
                        totalAmount.Write(humcPlantC14[1].ToString() + '\t');
                        totalAmount.Write(romcPlantC14[1].ToString() + '\t');

                        totalAmount.Write(fomcManureC14[1].ToString() + '\t');
                        totalAmount.Write(humcManureC14[1].ToString() + '\t');
                        totalAmount.Write(romcManureC14[1].ToString() + '\t');
                        totalAmount.WriteLine((fomcPlant[1] + humcPlant[1] + romcPlant[1] + fomcManure[1] + humcManure[1] + romcManure[1] + fomcPlantC14[1] + humcPlantC14[1] + romcPlantC14[1] + fomcManureC14[1] + humcManureC14[1] + romcManureC14[1]).ToString() + '\t');
                    }
                    if (mode == 1)
                        totalAmount.WriteLine((fomcPlant[1] + humcPlant[1] + romcPlant[1] + fomcManure[1] + humcManure[1] + romcManure[1]).ToString() + '\t');
                }

            }
            if (mode == 0 || mode == 1)
            {
                totalAmount.Close();
                C02.Close();
                transportFile.Close();
            }
            if (mode == 2)
            {
                totalAmount.Close();
            }
        }
    }
}