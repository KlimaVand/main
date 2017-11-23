using System;

public class DummyAnimal
{
    double heatProduction;
    double bodyWeight;
    double dailyMilkYield;
    double dayOfPregnancy;
    int animalType;
    int number;
    double maximumTemperature; //should be made dependent on humidity

//!

    public DummyAnimal(int AnanimalType, int Anumber, double AbodyWeight, double AdailyMilkYield, double AdayOfPregnancy, double AmaximumTemperature)
	{
        animalType = AnanimalType;
        bodyWeight = AbodyWeight;
        dailyMilkYield = AdailyMilkYield;
        dayOfPregnancy = AdayOfPregnancy;
        number = Anumber;
        maximumTemperature = AmaximumTemperature;
    }
    //From 4th Report of Working Group on Climatization of Animal Houses Heat and moisture production at animal and house levels Editors: Pedersen, S. & Sällvik, K. 2002
    //equation 4
    //returns heat production in W
    double HeatProduction()
    {
        double ret_val = 0 ;
        switch (animalType)
        {
            case 1: ret_val = 5.6 * Math.Pow(bodyWeight, 0.75) + 22.0 * dailyMilkYield + 1.6E-5 * Math.Pow(dayOfPregnancy, 3);
        break;
        }
        return ret_val;
    }

    public double GetHeatProduction()
    {
        double ret_val = 0;
        ret_val = number * HeatProduction();
        return ret_val;
    }

    public double GetmaximumTemperature()
    {
        return maximumTemperature;
    }
}
