using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for PigHerdUpdate
    /// </summary>
    public class PigHerdUpdate : IHttpHandler
    {
        public void ProcessRequest(HttpContext context)
        {

        }


        public void ProcessRequest(HttpContext context, string farmNumber, string scenarioNumber)
        {

            string aFarmNumber, aScenarioID, aPigHerdID, aPigType, aAnimalYear, aProductionVar1, aProductionVar2, aProductionVar3, aFeedLevel, aProteinLevel, aStableType1, aStableType2, returnValue, feedNitrogenAnimalYearNorm;
            string arrPigHerd, feedEnergyTotal, feedNitrogenAnimalYear, feedNitrogenTotal, GrowthNitrogenType, GrowthNitrogenTypeNorm, GrowthNitrogenTotal, SowGrowthNitrogen, SowGrowthNitrogenTotal, GrowthNitrogenIn, GrowthNitrogenOut, GrowthNitrogenInTotal, GrowthNitrogenOutTotal, manureNitrogenAnimalYear, manureNitrogenTotal, manureNitrogenFaecesAnimalYear, manureNitrogenFaecesTotal, manureNitrogenUrineAnimalYear, manureNitrogenUrineTotal, manureNitrogenAnimalYearNorm, manureNitrogenTotalNorm;

            string aFeedstuffList, aAmountList;
            string aManureStorageString1, aManureStorageString2;
            string originalLocale, currentLocale;
            string nStorageType, arrStorageType, aStorageTypeID, dictStorageType, s;
            int nPigHerd;


            context.Response.Write(context.Request.Form);


            aFarmNumber = context.Session["farmNumber"].ToString();
            aScenarioID = context.Session["scenarioNumber"].ToString();
            nPigHerd = int.Parse(context.Request.Form["hiddenNPigHerd"].ToString());
            dbInterface database = new dbInterface();
            for (int h = 0; h < nPigHerd; h++)
            {
                aPigHerdID = context.Request.Form["hiddenPigHerdID_" + h + ""];
                aPigType = context.Request.Form["selectPigType_" + h + ""];
                aAnimalYear = context.Request.Form["inputAnimalYear_" + h + ""];
                aProductionVar1 = context.Request.Form["inputProductionVar1_" + h + ""];
                aProductionVar2 = context.Request.Form["inputProductionVar2_" + h + ""];
                aProductionVar3 = context.Request.Form["inputProductionVar3_" + h + ""];
                aFeedLevel = context.Request.Form["inputFeedLevel_" + h + ""];
                aProteinLevel = context.Request.Form["inputProteinLevel_" + h + ""];
                aStableType1 = context.Request.Form["selectStableType1_" + h + ""];
                aStableType2 = context.Request.Form["selectStableType2_" + h + ""];
                if (aStableType2 == "")
                {
                    aStableType2 = "0";
                }

                List<dbInterface.data> arrParameter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = aFarmNumber;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aScenarioID";
                data.value = aScenarioID;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aPigHerdID";
                data.value = aPigHerdID;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aCattleType";
                data.value = aPigType;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aAnimalYear";
                data.value = aAnimalYear;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aProteinLevel";
                data.value = aProteinLevel;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aFeedLevel";
                data.value = aFeedLevel;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aProductionVar1";
                data.value = aProductionVar1;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aProductionVar2";
                data.value = aProductionVar2;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aProductionVar3";
                data.value = aProductionVar3;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aStableType1";
                data.value = aStableType1;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aStableType2";
                data.value = aStableType2;
                arrParameter.Add(data);


                database.GetValuesFromDB("InsertUpdatePigHerd", arrParameter);
                aFeedstuffList = context.Request.Form["hiddenFeedstuffList_" + aPigHerdID + ""].ToString();
                aAmountList = context.Request.Form["hiddenAmountList_" + aPigHerdID + ""];
                aManureStorageString1 = context.Request.Form["selectManureStorageType1_" + h + ""];
                aManureStorageString2 = context.Request.Form["selectManureStorageType2_" + h + ""];
                if (aManureStorageString1 == "")
                    aManureStorageString1 = "0";
                if (aManureStorageString2 == "")
                    aManureStorageString2 = "0";
                if (aStableType1 == "0")
                    aManureStorageString1 = "0";
                if (aStableType2 == "0")
                    aManureStorageString2 = "0";
                InsertUpdateFeedstuff(aFarmNumber, aScenarioID, aPigHerdID, aFeedstuffList, aAmountList);

                InsertUpdateManureStorage(aFarmNumber, aScenarioID, aPigHerdID, "1", aManureStorageString1);
                InsertUpdateManureStorage(aFarmNumber, aScenarioID, aPigHerdID, "2", aManureStorageString2);

            }
            FunctionPig tmp = new FunctionPig();
            tmp.CalcPigHerdTotal(farmNumber, scenarioNumber);



            database.CloseFarmNDB();
            context.Response.Redirect("data.asp?TB=" + context.Session["tabNumber"] + "");
            context.Response.End();
        }
        void InsertUpdateFeedstuff(string aFarmNumber, string aScenarioID, string aPigHerdID, string aFeedstuffList, string aAmountList)
        {
            string Feedstuff, arrParameter1, amount;
            string[] arrFeedstuff, arrAmount;
            List<dbInterface.data> arrParameter = new List<dbInterface.data>();
            dbInterface database = new dbInterface();

            arrFeedstuff = aFeedstuffList.Split(',');
            arrAmount = aAmountList.Split(',');


            for (int f = 0; f < arrFeedstuff.Count(); f++)
            {

                if (arrFeedstuff[f] != "" && arrFeedstuff[f] == "-1" != (arrFeedstuff[f] != null))
                {
                    if (arrAmount.Count() >= f)
                        amount = arrAmount[f];
                    else
                        amount = "0";
                    dbInterface.data data = new dbInterface.data();
                    data.name = "@aFarmNumber";
                    data.value = aFarmNumber;
                    arrParameter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aScenarioID";
                    data.value = aScenarioID;
                    arrParameter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aPigHerdID";
                    data.value = aPigHerdID;
                    arrParameter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@FeedstuffNr";
                    data.value = (f + 1).ToString();
                    arrParameter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@arrFeedstuff";
                    data.value = arrFeedstuff[f];
                    arrParameter.Add(data);


                    data = new dbInterface.data();
                    data.name = "@amount";
                    data.value = amount.ToString();
                    arrParameter.Add(data);

                    database.GetValuesFromDB("insertUpdatePigHerdFeedstuff ", arrParameter);
                    HttpContext.Current.Response.Write("<br/><br/>amount" + amount);
                }
            }
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
            data1 = new dbInterface.data();
            data1.name = "@arrFeedstuff";
            data1.value = arrFeedstuff.Count().ToString();
            arrParameter.Add(data1);

            database.GetValuesFromDB("DeletePigHerdFeedstuff ", arrParameter);

        }

        void InsertUpdateManureStorage(string aFarmNumber, string aScenarioID, string aPigHerdID, string aStableSubTypeID, string aManureStorageString)
        {
            List<dbInterface.data> arrParameter = new List<dbInterface.data>();
            dbInterface database = new dbInterface();
            string[] arrManureStorage;
            string ManureStorage;


            arrManureStorage = aManureStorageString.Split(',');
            for (int s = 0; s < arrManureStorage.Count(); s++)
            {
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

                data1 = new dbInterface.data();
                data1.name = "@aStableSubTypeID";
                data1.value = aStableSubTypeID;
                arrParameter.Add(data1);
                data1 = new dbInterface.data();
                data1.name = "@arrManureStorage";
                data1.value = (s + 1).ToString();
                arrParameter.Add(data1);


                data1 = new dbInterface.data();
                data1.name = "@arrManureStorage";
                data1.value = arrManureStorage[s];
                arrParameter.Add(data1);

                database.GetValuesFromDB("InsertUpdatePigManureStorage ", arrParameter);
            }
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            arrParameter.Add(data);


            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = aScenarioID;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aPigHerdID";
            data.value = aPigHerdID;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aStableSubTypeID";
            data.value = aStableSubTypeID;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@arrManureStorage";
            data.value = (arrManureStorage.Count() + 1).ToString();
            arrParameter.Add(data);

            database.GetValuesFromDB("DeletePigManureStorage ", arrParameter);

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