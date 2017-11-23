using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for PigHerdNew
    /// </summary>
    public class PigHerdNew : IHttpHandler
    {

        public void ProcessRequest(HttpContext context)
        {

        }

        public void ProcessRequest(HttpContext context, string farmNumber, string scenarioNumber)
        {
            string h, nPigHerd, aFarmNumber, aScenarioID, aPigType, aAnimalYear = "", aProductionVar1 = "", aProductionVar2 = "", aProductionVar3 = "", aFeedLevel = "", aProteinLevel = "", aStableType1 = "", aStableType2 = "", returnValue, feedNitrogenAnimalYearNorm;
            string arrPigHerd, feedEnergyTotal, feedNitrogenAnimalYear, feedNitrogenTotal, GrowthNitrogenType, GrowthNitrogenTypeNorm, GrowthNitrogenTotal, SowGrowthNitrogen, SowGrowthNitrogenTotal, GrowthNitrogenIn, GrowthNitrogenOut, GrowthNitrogenInTotal, GrowthNitrogenOutTotal, manureNitrogenAnimalYear, manureNitrogenTotal, manureNitrogenFaecesAnimalYear, manureNitrogenFaecesTotal, manureNitrogenUrineAnimalYear, manureNitrogenUrineTotal, manureNitrogenAnimalYearNorm, manureNitrogenTotalNorm;
            string nID, nPigType, dictID, dictPigType;
            DataTable arrID, arrPigType;
            int aPigHerdID;
            aFarmNumber = context.Session["farmNumber"].ToString();
            aScenarioID = context.Session["scenarioNumber"].ToString();
            aPigType = "2";

            context.Response.Write(" <br> aPigType " + aPigType);
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
            //nID
            arrID = database.GetValuesFromDB("GetPigHerdMaxID", arrParameter);
            arrParameter = new List<dbInterface.data>();

            //nPigType
            arrPigType = database.GetValuesFromDB("GetPigTypeList", arrParameter);


            string stff = arrID.Rows[0][0].ToString();
            if (arrID.Rows[0][0].ToString().CompareTo("") != 0)
                aPigHerdID = int.Parse(arrID.Rows[0][0].ToString()) + 1;
            else
                aPigHerdID = 1;


            for (int i = 0; i < arrPigType.Rows.Count; i++)
            {
                if (aPigType == arrPigType.Rows[i][0])
                {

                    aAnimalYear = "1";
                    aProductionVar1 = arrPigType.Rows[i][13].ToString();
                    aProductionVar2 = arrPigType.Rows[i][11].ToString();
                    aProductionVar3 = arrPigType.Rows[i][9].ToString();
                    aFeedLevel = arrPigType.Rows[i][3].ToString();
                    aProteinLevel = arrPigType.Rows[i][5].ToString();
                    aStableType1 = "0";
                    aStableType2 = "0";

                }

            }


            arrParameter = new List<dbInterface.data>();
            data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;
            arrParameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = aScenarioID;
            arrParameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aPigHerdID";
            data1.value = aPigHerdID.ToString();
            arrParameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aCattleType";
            data1.value = aPigType;
            arrParameter.Add(data1);


            data1 = new dbInterface.data();
            data1.name = "@aAnimalYear";
            data1.value = aAnimalYear.ToString();
            arrParameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aProteinLevel";
            data1.value = aProteinLevel;
            arrParameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aFeedLevel";
            data1.value = aFeedLevel;
            arrParameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aProductionVar1";
            data1.value = aProductionVar1;
            arrParameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aProductionVar2";
            data1.value = aProductionVar2;
            arrParameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aProductionVar3";
            data1.value = aProductionVar3;
            arrParameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aStableType1";
            data1.value = aStableType1;
            arrParameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aStableType2";
            data1.value = aStableType2;
            arrParameter.Add(data1);


            database.GetValuesFromDB("InsertUpdatePigHerd", arrParameter);

            FunctionPig tmp = new FunctionPig();
            tmp.CalcPigHerdTotal(farmNumber, scenarioNumber);
            arrParameter = new List<dbInterface.data>();
            data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;
            arrParameter.Add(data1);

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