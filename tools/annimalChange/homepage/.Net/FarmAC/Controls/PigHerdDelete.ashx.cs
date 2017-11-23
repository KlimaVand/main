using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for PigHerdDelete
    /// </summary>
    public class PigHerdDelete : IHttpHandler
    {
        public void ProcessRequest(HttpContext context)
        {
        }
        public void ProcessRequest(HttpContext context, string PigHerdID, string farmNumber, string scenarioNumber)
        {
            string h, nPigHerd, aFarmNumber, aScenarioID, aPigHerdID, aPigType, aAnimalYear, aProductionVar1, aProductionVar2, aProductionVar3, aFeedLevel, aProteinLevel, aStableType1, aStableType2, returnValue, feedNitrogenAnimalYearNormM;
            string arrPigHerd, feedEnergyTotal, feedNitrogenAnimalYear, feedNitrogenTotal, GrowthNitrogenType, GrowthNitrogenTypeNorm, GrowthNitrogenTotal, SowGrowthNitrogen, SowGrowthNitrogenTotal, GrowthNitrogenIn, GrowthNitrogenOut, GrowthNitrogenInTotal, GrowthNitrogenOutTotal, manureNitrogenAnimalYear, manureNitrogenTotal, manureNitrogenFaecesAnimalYear, manureNitrogenFaecesTotal, manureNitrogenUrineAnimalYear, manureNitrogenUrineTotal, manureNitrogenAnimalYearNorm, manureNitrogenTotalNorm;

            aFarmNumber = context.Session["farmNumber"].ToString();
            aScenarioID = context.Session["scenarioNumber"].ToString();
            aPigHerdID = PigHerdID;
            dbInterface database = new dbInterface();
            database.InitFarmNDB();

            List<dbInterface.data> arrParameter = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;
            arrParameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = aScenarioID;
            arrParameter.Add(data1);


            data1 = new dbInterface.data();
            data1.name = "@aPigHerdID";
            data1.value = aPigHerdID;
            arrParameter.Add(data1);


            database.GetValuesFromDB("DeletePigHerd", arrParameter);
            FunctionPig tmp = new FunctionPig();

            tmp.CalcPigHerdTotal(farmNumber, scenarioNumber);

            arrParameter = new List<dbInterface.data>();
            data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;


            database.GetValuesFromDB("CalcFarmTotal", arrParameter);


            database.CloseFarmNDB();

        }

        public bool IsReusable
        {
            get
            {
                return false;
            }
        }
    }
}