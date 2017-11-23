using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


class animalSection
{
    private List<floor> FloorsInSections;
    [Param]
    public int NrOfAnimals = 0;
    [Param]
    public int SpiltDrinkingWater = 0;
    [Param]
    public double StrawAdded =0;
    private animalSection()
    {
    }
    public animalSection(int numberOfFloors)
    {
        FloorsInSections = new List<floor>();
        for (int i = 0; i < numberOfFloors; i++)
        {
            floor section=new floor();
            FloorsInSections.Add(section);
        }
    }
    public void Volatize(ref double volatAmmonia,ref double N2Emission, ref double N2OEmission, ref double CH4Emission, ref double CO2Emission)
    {
        double tempvolatAmmonia = 0.0, tempN2Emission = 0.0, tempN2OEmission = 0.0, tempCH4Emission = 0.0, tempCO2Emission = 0.0;
        for (int i = 0; i < FloorsInSections.Count; i++)
        {

            FloorsInSections.ElementAt(i).Volatize(ref tempvolatAmmonia, ref tempN2Emission, ref tempN2OEmission, ref tempCH4Emission, ref tempCO2Emission);
            volatAmmonia += tempvolatAmmonia;
            N2Emission += tempN2Emission;
            N2OEmission += tempN2OEmission;
            CH4Emission += tempCH4Emission;
            CO2Emission += tempCO2Emission;
        }

    }
    [EventHandler]
    public void OnProcess()
    {
        ManureType WASHWATER = new ManureType();
        WASHWATER.amount = NrOfAnimals * SpiltDrinkingWater / 1000.0;
        RcvManure(WASHWATER);
        ManureType WHEATSTRAW = new ManureType();
        for (int i = 0; i < FloorsInSections.Count(); i++)
        {
            WHEATSTRAW.amount = NrOfAnimals * StrawAdded * (1/FloorsInSections.Count)/ 1000.0; //add bedding material
            FloorsInSections.ElementAt(i).RcvManure(WHEATSTRAW);
        }
    }
    public void RcvManure(ManureType aUrine)
    {
        ManureType someManure = new ManureType();

        for (int i = 0; i < FloorsInSections.Count(); i++)
        {
            someManure = aUrine;

            someManure.amount = aUrine.amount * (1 / FloorsInSections.Count);



            FloorsInSections.ElementAt(i).RcvManure(someManure);
        }

    }
    public ManureType CleanSection()
    {
        ManureType manure = new ManureType();
 
        for (int i = 0; i < FloorsInSections.Count; i++)
        {
            manure.amount += FloorsInSections.ElementAt(i).CleanFloor(NrOfAnimals).amount;
        }
        return manure;

    }
}
