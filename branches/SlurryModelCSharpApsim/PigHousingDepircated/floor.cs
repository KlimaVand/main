using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

class floor
{
    store liquid;


    [Param]
    public double WashingWater = 0;
    public floor()
    {
  
        liquid = new store();

    }
    public void RcvManure(ManureType aUrine)
    {
        liquid.RcvManure(aUrine);



    }
    public ManureType CleanFloor()
    {
        ManureType returnValue = new ManureType();
        returnValue.amount = liquid.CleanStore().amount;

        return returnValue;
    }

    public void Volatize(ref double volatAmmonia, ref  double N2Emission, ref double N2OEmission, ref  double CH4Emission, ref  double CO2Emission)
    {
        liquid.Volatize(ref volatAmmonia, ref N2Emission, ref N2OEmission, ref  CH4Emission, ref  CO2Emission);

    }
    public ManureType CleanFloor(int NrOfAnimals)
    {
        ManureType manure = new ManureType();
        manure.amount = WashingWater * NrOfAnimals / 1000.0;
        return manure;
    }

}
