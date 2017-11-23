using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for CattleHerdNew
    /// </summary>
    public class CattleHerdNew : IHttpHandler
    {

        public void ProcessRequest(HttpContext context)
        {

            String h, aFarmNumber, aScenarioID;
            string returnValue;
            string feedEnergyAnimalYear, feedEnergyTotal, feedEnergyTotalNorm, feedDryMatterAnimalYear, feedDryMatterTotal, feedNitrogenAnimalYear, feedNitrogenTotal, feedNitrogenAnimalYearNorm, feedNitrogenTotalNorm;
            string productionMilkNitrogenAnimalYear, productionMilkNitrogenTotal, productionGrowthNitrogenAnimalYear, productionGrowthNitrogenTotal;
            string manureNitrogenAnimalYear, manureNitrogenTotal, manureNitrogenOnField, manureNitrogenAnimalYearNorm, manureNitrogenTotalNorm;
            string manureNitrogenOnFieldAnimalYear, manureNitrogenOnFieldTotal;
            string manureNitrogenFaecesAnimalYear, manureNitrogenFaecesTotal;
            string manureNitrogenUrineAnimalYear, manureNitrogenUrineTotal;
            string nCattleHerd, arrCattleHerd;
            int aCattleHerdID;
            string nID, dictID;
            DataTable arrID, arrCattleType;
            String nCattleType, dictCattleType;

            aFarmNumber = context.Session["farmNumber"].ToString();
            aScenarioID = context.Session["scenarioNumber"].ToString();
            dbInterface database = new dbInterface();
            database.InitFarmNDB();


            List<dbInterface.data> arrParameter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            arrParameter.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = aScenarioID;
            arrParameter.Add(data);
            arrID = database.GetValuesFromDB("GetCattleHerdMaxID", arrParameter);
            arrParameter = new List<dbInterface.data>();
            arrCattleType = database.GetValuesFromDB("GetCattleTypeList", arrParameter);
            object stuff = arrID.Rows[0]["MaxID"];
            string stuff2 = stuff.ToString();
            if (arrID.Rows[0]["MaxID"].ToString() != "")
                aCattleHerdID = int.Parse(arrID.Rows[0]["MaxID"].ToString()) + 1;
            else
                aCattleHerdID = 1;


            List<dbInterface.data> paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            paramter.Add(data);


            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = aScenarioID;
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aCattleHerdID";
            data.value = aCattleHerdID.ToString();
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aCattleType";
            data.value = "1";
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aAnimalYear";
            data.value = "1";
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aGrazing";
            data.value = arrCattleType.Rows[0]["GrazingDefault"].ToString();
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aProductionLevel";
            data.value = arrCattleType.Rows[0]["ProductionLevelDefault"].ToString();
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aProductionLevel2";
            data.value = arrCattleType.Rows[0]["ProductionLevel2Default"].ToString();
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@aEffectivity";
            data.value = "100";
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aStable";
            data.value = "6";
            paramter.Add(data);
            database.GetValuesFromDB("InsertUpdateCattleHerd", paramter);

            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            paramter.Add(data);


            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = aScenarioID;
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aCattleHerdID";
            data.value = aCattleHerdID.ToString();
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aManureStorageID";
            data.value = "1";
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aManureStorageTypeID";
            data.value = "3";
            paramter.Add(data);

            database.GetValuesFromDB("InsertUpdateManureStorage ", paramter);




            //redim preserve arrParameter(1)
            context.Session["ShowResult"] = "False";


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