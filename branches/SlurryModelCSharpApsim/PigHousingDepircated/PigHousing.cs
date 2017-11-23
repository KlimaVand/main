using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


public class PigHousing
{
    private List<animalSection> AnimalSections;
    private List<store> inhouseStores;

    [Event]
    public event DoubleDelegate NH3EmissionEvent;
    [Event]
    public event DoubleDelegate N2EmissionEvent;
    [Event]
    public event DoubleDelegate N2OEmissionEvent;
    [Event]
    public event DoubleDelegate CH4EmissionEvent;
    [Event]
    public event DoubleDelegate CO2EmissionEvent;
    [Param]
    public double SpiltDrinkingWater = 0;
    [Param]
    public int numberOfFloors = 0;
    [Param]
    public int numberOfAnimalSection = 0;
    [Param]
    public int numberOfInhouseStores = 0;
    [Param]
    public int VolatModel = 0;
    [Event]
    public event ManureDelegate AddSlurry;
    [EventHandler]
    public void OnInitialised()
    {
        AnimalSections = new List<animalSection>();
        inhouseStores = new List<store>();
        for (int i = 0; i < numberOfAnimalSection; i++)
        {
            animalSection section=new animalSection(numberOfFloors);
            AnimalSections.Add(section);
        }
        for (int i = 0; i < numberOfInhouseStores; i++)
        {
            store section = new store();
            inhouseStores.Add(section);
        }
    }
    [EventHandler]
    public void OnProcess()
    {
        Volatize();
        CleanManure();
    }
    private void CleanManure()
    {
        ManureType totalManure = new ManureType();
        for (int i = 0; i < AnimalSections.Count; i++)
        {
           totalManure.amount+= AnimalSections.ElementAt(i).CleanSection().amount;
        }
        for (int i = 0; i < inhouseStores.Count; i++)
        {
            if (inhouseStores.ElementAt(i).GetAmountInManureStore() < inhouseStores.ElementAt(i).GetCapacity())
            {
                inhouseStores.ElementAt(i).RcvManure(totalManure);
            }
        }
        totalManure.amount = 0;
        for (int i = 0; i < inhouseStores.Count; i++)
        {
            if (inhouseStores.ElementAt(i).GetAmountInManureStore() >= inhouseStores.ElementAt(i).GetCapacity())
            {
                totalManure.amount+= inhouseStores.ElementAt(i).CleanStore().amount;
            }
        }
        AddSlurry.Invoke(totalManure);


    }
    private void Volatize()
    {
        double NH3Emission = 0.0,  N2Emission = 0.0, N2OEmission = 0.0, CH4Emission = 0.0, CO2Emission = 0.0;
        for (int i = 0; i < AnimalSections.Count; i++)
        {
            double tempvolatAmmonia = 0.0, tempN2Emission = 0.0, tempN2OEmission = 0.0, tempCH4Emission = 0.0, tempCO2Emission = 0.0;
            AnimalSections.ElementAt(i).Volatize(ref tempvolatAmmonia, ref tempN2Emission, ref tempN2OEmission, ref tempCH4Emission, ref tempCO2Emission);
            NH3Emission += tempvolatAmmonia;
            N2Emission += tempN2Emission;
            N2OEmission += tempN2OEmission;
            CH4Emission += tempCH4Emission;
            CO2Emission += tempCO2Emission;
        }
        for (int i = 0; i < inhouseStores.Count; i++)
        {
            double tempvolatAmmonia = 0.0, tempN2Emission = 0.0, tempN2OEmission = 0.0, tempCH4Emission = 0.0, tempCO2Emission = 0.0;

            inhouseStores.ElementAt(i).Volatize(ref tempvolatAmmonia,ref tempN2Emission, ref tempN2OEmission, ref tempCH4Emission, ref tempCO2Emission);
            NH3Emission += tempvolatAmmonia;
            N2Emission += tempN2Emission;
            N2OEmission += tempN2OEmission;
            CH4Emission += tempCH4Emission;
            CO2Emission += tempCO2Emission;
        }
        DoubleType value = new DoubleType();
        value.Value = NH3Emission;
        NH3EmissionEvent.Invoke(value);
        value.Value = N2Emission;
        N2EmissionEvent.Invoke(value);
        value.Value = N2OEmission;
        N2OEmissionEvent.Invoke(value);
        value.Value = CH4Emission;
        CH4EmissionEvent.Invoke(value);
        value.Value = CO2Emission;
        CO2EmissionEvent.Invoke(value);
        
    }
    
}

