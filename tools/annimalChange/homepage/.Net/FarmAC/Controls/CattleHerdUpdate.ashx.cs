using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Web;
using System.Web.UI.WebControls;
using System.Xml;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for CattleHerdUpdate
    /// </summary>
    public class CattleHerdUpdate : IHttpHandler
    {
        double feedEnergyAnimalYear;
        public void ProcessRequest(HttpContext context)
        {
        }
        public void ProcessRequest(HttpContext context, DataTable arrCattleHerd, List<DropDownList> allSelectCattleType, List<TextBox> AllInputAnimalYear, List<DropDownList> AllSelectStableType, List<string> strFeedplan, List<string> strAmount, List<List<DropDownList>> AllSelectManureStorageType)
        {
            string aFarmNumber, aScenarioID, i;
            string returnValue;
            string feedEnergyAnimalYear, feedEnergyTotal, feedEnergyTotalNorm, feedDryMatterAnimalYear, feedDryMatterTotal, feedNitrogenAnimalYear, feedNitrogenTotal, feedNitrogenAnimalYearNorm, feedNitrogenTotalNorm;
            string productionMilkNitrogenAnimalYear, productionMilkNitrogenTotal, productionGrowthNitrogenAnimalYear, productionGrowthNitrogenTotal;
            string manureNitrogenAnimalYear, manureNitrogenTotal, manureNitrogenOnField, manureNitrogenAnimalYearNorm, manureNitrogenTotalNorm;
            string manureNitrogenOnFieldAnimalYear, manureNitrogenOnFieldTotal;
            string manureNitrogenFaecesAnimalYear, manureNitrogenFaecesTotal;
            string manureNitrogenUrineAnimalYear, manureNitrogenUrineTotal;

            string aFeedstuffList, aAmountList;
            string aManureStorageString; 
            string nStorageType, aStorageTypeID, dictStorageType;
            string outputStr;

            DataTable arrStorageType;
           
            aFarmNumber = context.Session["farmNumber"].ToString();
            aScenarioID = context.Session["scenarioNumber"].ToString();


            string originalLocale, currentLocale;


            //originalLocale = GetLocale()        ' Get the original locale
            //currentLocale  = SetLocale("en-gb") ' Set the locale to UK
            context.Response.Write("<html><body>" + "\r");

            dbInterface database = new dbInterface();
            database.InitFarmNDB();

            for (int h = 0; h < arrCattleHerd.Rows.Count; h++)
            {
                string aCattleHerdID = arrCattleHerd.Rows[h]["CattleHerdID"].ToString();
                List<dbInterface.data> paramter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = aFarmNumber;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aScenarioID";
                data.value = aScenarioID;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aCattleHerdID";
                data.value = aCattleHerdID;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aCattleType";
                data.value = allSelectCattleType[h].SelectedItem.Attributes["dbValue"];
                paramter.Add(data);

                data.name = "@aAnimalYear";
                data.value = AllInputAnimalYear[h].Text;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aGrazing";
                data.value = arrCattleHerd.Rows[h]["Grazing"].ToString();
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aProductionLevel";
                data.value = arrCattleHerd.Rows[h]["ProductionLevel"].ToString();
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aProductionLevel2";
                data.value = arrCattleHerd.Rows[h]["ProductionLevel2"].ToString();

                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aEffectivity";
                data.value = "100";
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aStable";
                data.value = AllSelectStableType[h].SelectedItem.Attributes["dbValue"];
                paramter.Add(data);


                database.GetValuesFromDB("InsertUpdateCattleHerd", paramter);

                aFeedstuffList = strFeedplan[h];

                aAmountList = strAmount[h];
                aManureStorageString = "";
                for (int l = 0; l < AllSelectManureStorageType[h].Count; l++)
                {
                    aManureStorageString += AllSelectManureStorageType[h][l].SelectedItem.Attributes["dbValue"] + ",";
                }




                InsertUpdateFeedstuff(aFarmNumber, aScenarioID, aCattleHerdID, aFeedstuffList, aAmountList);
                InsertUpdateManureStorage(aFarmNumber, aScenarioID, aCattleHerdID, aManureStorageString);
            }

            CalcCattleHerdTotal();
            List<dbInterface.data> arr = new List<dbInterface.data>();
            List<dbInterface.data> arr1 = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;
            arr.Add(data1);
            arr1.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = aScenarioID;
            arr.Add(data1);
            arr1.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aCattleHerdID";
            data1.value = "-1";
            arr.Add(data1);

            database.GetValuesFromDB("DeleteCattleHerdCalculation", arr);//sletter alle gamle beregninger, alt hvor cattleherdID>-1
            database.GetValuesFromDB("DeleteGrazingBalance", arr1);//sletter alt i GrazingBalance-tabellen
            
        }
        void InsertUpdateFeedstuff(string aFarmNumber, string aScenarioID, string aCattleHerdID, string aFeedstuffList, string aAmountList)
        {
            dbInterface database = new dbInterface();
            string Feedstuff, arrParameter1, amount;
            string[] arrFeedstuff, arrAmount;
            List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();



            arrFeedstuff = aFeedstuffList.Split(',');
            arrAmount = aAmountList.Split(',');
            for (int f = 0; f < arrFeedstuff.Count(); f++)
            {
                arr1Parameter = new List<dbInterface.data>();
                dbInterface.data data1 = new dbInterface.data();
                data1.name = "@aFarmNumber";
                data1.value = aFarmNumber;
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aScenarioID";
                data1.value = aScenarioID;
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aCattleHerdID";
                data1.value = aCattleHerdID;
                arr1Parameter.Add(data1);
                if (arrFeedstuff[f] != "" && arrFeedstuff[f] != "-1" && arrFeedstuff[f] != null)
                {
                    if (arrAmount.Count() > f)
                        amount = (arrAmount[f]);
                    else
                        amount = "0";


                    data1 = new dbInterface.data();
                    data1.name = "@aFeedplanID";
                    data1.value = (f + 1).ToString();
                    arr1Parameter.Add(data1);
                    data1 = new dbInterface.data();
                    data1.name = "@aFeedstuffID";
                    data1.value = arrFeedstuff[f];
                    arr1Parameter.Add(data1);
                    data1 = new dbInterface.data();
                    data1.name = "@aAmount";
                    data1.value = amount;
                    arr1Parameter.Add(data1);
                    database.GetValuesFromDB("insertUpdateCattleHerdFeedstuff ", arr1Parameter);
                }
            }
            arr1Parameter = new List<dbInterface.data>();
            dbInterface.data data2 = new dbInterface.data();
            data2.name = "@aFarmNumber";
            data2.value = aFarmNumber;
            arr1Parameter.Add(data2);

            data2 = new dbInterface.data();
            data2.name = "@aScenarioID";
            data2.value = aScenarioID;
            arr1Parameter.Add(data2);


            data2 = new dbInterface.data();
            data2.name = "@aCattleHerdID";
            data2.value = aCattleHerdID;
            arr1Parameter.Add(data2);
            data2 = new dbInterface.data();
            data2.name = "@aFeedplanID";
            data2.value = (arrFeedstuff.Count() - 1).ToString();
            arr1Parameter.Add(data2);
            database.GetValuesFromDB("DeleteCattleHerdFeedstuff ", arr1Parameter);

        }

        void InsertUpdateManureStorage(string aFarmNumber, string aScenarioID, string aCattleHerdID, string aManureStorageString)
        {
            string[] arrManureStorage;
            string ManureStorage, arrParameter1;
            List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = aScenarioID;
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aCattleHerdID";
            data1.value = aCattleHerdID;
            arr1Parameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aManureStorageID";
            data1.value = "0";
            arr1Parameter.Add(data1);
            dbInterface database = new dbInterface();
            database.GetValuesFromDB("DeleteManureStorage ", arr1Parameter);


            arrManureStorage = aManureStorageString.Split(',');
            for (int s = 0; s < arrManureStorage.Count(); s++)
            {
                if (arrManureStorage[s] != "")
                {
                    arr1Parameter = new List<dbInterface.data>();
                    data1 = new dbInterface.data();
                    data1.name = "@aFarmNumber";
                    data1.value = aFarmNumber;
                    arr1Parameter.Add(data1);

                    data1 = new dbInterface.data();
                    data1.name = "@aScenarioID";
                    data1.value = aScenarioID;
                    arr1Parameter.Add(data1);
                    data1 = new dbInterface.data();
                    data1.name = "@aCattleHerdID";
                    data1.value = aCattleHerdID;
                    arr1Parameter.Add(data1);
                    data1 = new dbInterface.data();
                    data1.name = "@aManureStorageID";
                    data1.value = (s + 1).ToString();
                    arr1Parameter.Add(data1);
                    data1 = new dbInterface.data();
                    data1.name = "@aManureStorageTypeID";
                    data1.value = arrManureStorage[s];
                    arr1Parameter.Add(data1);

                    database.GetValuesFromDB("InsertUpdateManureStorage ", arr1Parameter);
                }

            }

        }
        public bool IsReusable
        {
            get
            {
                return false;
            }
        }
        void CalcCattleHerdTotal()
        {
            double currentEffectivity;
            string currentStable;
            string dictCattleHerd;
            int currentType;
            double currentProductionLevel, currentProductionLevel2;
            double currentAnimalYear, currentGrazing;
            int currentRace, currentAgeGroup;
            List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            arr1Parameter.Add(data1);
            dbInterface database = new dbInterface();
            // nCattleHerd;
            DataTable arrCattleHerd = database.GetValuesFromDB("GetCattleHerdList", arr1Parameter);

            arr1Parameter = new List<dbInterface.data>();
            data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "aScenarioID";
            data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            arr1Parameter.Add(data1);



            database.GetValuesFromDB("DeleteCattleHerdTotal", arr1Parameter);

            arr1Parameter = new List<dbInterface.data>();
            data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            arr1Parameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aProductionTypeID";
            data1.value = "3";
            arr1Parameter.Add(data1);

            database.GetValuesFromDB("DeleteStable", arr1Parameter);
            database.GetValuesFromDB("DeleteStorage", arr1Parameter);
            database.GetValuesFromDB("DeleteStorageNorm", arr1Parameter);


            for (int h = 0; h < arrCattleHerd.Rows.Count; h++)
            {
                currentRace = int.Parse(arrCattleHerd.Rows[h]["CattleRace"].ToString());
                currentAgeGroup = int.Parse(arrCattleHerd.Rows[h]["CattleAgeGroup"].ToString());
                currentType = int.Parse(arrCattleHerd.Rows[h]["CattleType"].ToString());
                currentAnimalYear = double.Parse(arrCattleHerd.Rows[h]["AnimalYear"].ToString());
                currentGrazing = double.Parse(arrCattleHerd.Rows[h]["Grazing"].ToString());
                currentProductionLevel = double.Parse(arrCattleHerd.Rows[h]["ProductionLevel"].ToString());
                currentProductionLevel2 = double.Parse(arrCattleHerd.Rows[h]["ProductionLevel2"].ToString());
                currentEffectivity = double.Parse(arrCattleHerd.Rows[h]["Effectivity"].ToString());
                currentStable = arrCattleHerd.Rows[h]["Stable"].ToString();

                CalcFeedEnergyGrazing(currentType, currentAnimalYear, currentGrazing);
                CalcFeedEnergy(currentRace, currentAgeGroup, currentType, currentAnimalYear, currentProductionLevel, currentProductionLevel2, currentEffectivity);
                CalcFeedDryMatter(currentRace, currentAgeGroup, currentType, currentAnimalYear, currentProductionLevel, currentProductionLevel2);
                CalcFeedNitrogen(currentRace, currentAgeGroup, currentType, currentAnimalYear, currentGrazing, currentEffectivity);
                CalcFeedPhosphorus(currentType);
                CalcProductionMilkNitrogen(currentRace, currentAgeGroup, currentType, currentAnimalYear, currentProductionLevel, currentProductionLevel2);
                CalcProductionGrowthNitrogen(currentRace, currentAgeGroup, currentType, currentAnimalYear, currentProductionLevel, currentProductionLevel2);
                CalcManureNitrogen(currentType, currentAnimalYear);
                CalcManureNitrogenOnField(currentType, currentAnimalYear, currentGrazing);
                CalcManureNitrogenFaecesUrine(currentType, currentAnimalYear, currentGrazing);

                CalcManureStableStorage(currentType, currentAnimalYear, currentStable, currentGrazing, currentRace, currentAgeGroup, currentEffectivity, currentGrazing, currentProductionLevel, currentProductionLevel2);
            }

        }
        void CalcFeedEnergyGrazing(int cattleType, double animalYear, double grazing)
        {


            double feedEnergyGrazing = animalYear * grazing;
            List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aCattleTypeID";
            data1.value = cattleType.ToString();
            arr1Parameter.Add(data1);



            data1 = new dbInterface.data();
            data1.name = "@aParameterID";
            data1.value = "301";
            arr1Parameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aParameterValue";
            data1.value = feedEnergyGrazing.ToString(); ;
            arr1Parameter.Add(data1);
            dbInterface data = new dbInterface();
            data.GetValuesFromDB("IncrementCattleHerdTotal", arr1Parameter);

        }
        void CalcFeedEnergy(int cattleRace, int cattleAgeGroup, int cattleType, double animalYear, double productionLevel, double productionLevel2, double effectivity)
        {
            double feedEnergyAnimalYear = CalcCattleFeedEnergy(cattleRace, cattleAgeGroup, productionLevel, productionLevel2);

            double feedEnergyTotal = animalYear * feedEnergyAnimalYear;

            List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aCattleTypeID";
            data1.value = cattleType.ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aParameterID";
            data1.value = "302";
            arr1Parameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aParameterValue";
            data1.value = feedEnergyTotal.ToString(); ;
            arr1Parameter.Add(data1);
            dbInterface data = new dbInterface();


            data.GetValuesFromDB("IncrementCattleHerdTotal", arr1Parameter);

        }

        void CalcFeedDryMatter(int cattleRace, int cattleAgeGroup, int cattleType, double animalYear, double productionLevel, double productionLevel2)
        {
            double feedDryMatterAnimalYear = CalcCattleFeedDryMatter(cattleRace, cattleAgeGroup, productionLevel, productionLevel2, feedEnergyAnimalYear);
            double feedDryMatterTotal = animalYear * feedDryMatterAnimalYear;

            List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aCattleTypeID";
            data1.value = cattleType.ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aParameterID";
            data1.value = "303";
            arr1Parameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aParameterValue";
            data1.value = feedDryMatterTotal.ToString(); ;
            arr1Parameter.Add(data1);
            dbInterface database = new dbInterface();


            database.GetValuesFromDB("IncrementCattleHerdTotal", arr1Parameter);
        }

        void CalcFeedNitrogen(int cattleRace, int cattleAgeGroup, int cattleType, double animalYear, double grazing, double effectivity)
        {
            double feedNitrogenAnimalYear = CalcCattleFeedNitrogen(cattleRace, cattleAgeGroup, feedEnergyAnimalYear, grazing) * 100 / effectivity;
            double feedNitrogenAnimalYearNorm = CalcCattleFeedNitrogen(cattleRace, cattleAgeGroup, feedEnergyAnimalYear, 0);
            double feedNitrogenTotal = animalYear * feedNitrogenAnimalYear;

            List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aCattleTypeID";
            data1.value = cattleType.ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aParameterID";
            data1.value = "304";
            arr1Parameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aParameterValue";
            data1.value = feedNitrogenTotal.ToString(); ;
            arr1Parameter.Add(data1);
            dbInterface database = new dbInterface();


            database.GetValuesFromDB("IncrementCattleHerdTotal", arr1Parameter);

        }

        void CalcFeedPhosphorus(int cattleType)
        {
            List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aCattleTypeID";
            data1.value = cattleType.ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aParameterID";
            data1.value = "305";
            arr1Parameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aParameterValue";
            data1.value = "0";
            arr1Parameter.Add(data1);
            dbInterface database = new dbInterface();

            database.GetValuesFromDB("InsertUpdateCattleHerdTotal", arr1Parameter);
        }
        void CalcProductionMilkNitrogen(int cattleRace, int cattleAgeGroup, int cattleType, double animalYear, double productionLevel, double productionLevel2)
        {
            double productionMilkNitrogenAnimalYear = CalcCattleProductionMilkNitrogen(cattleRace, cattleAgeGroup, productionLevel, productionLevel2);
            double productionMilkNitrogenTotal = animalYear * productionMilkNitrogenAnimalYear;
            List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aCattleTypeID";
            data1.value = cattleType.ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aParameterID";
            data1.value = "306";
            arr1Parameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aParameterValue";
            data1.value = productionMilkNitrogenTotal.ToString();
            arr1Parameter.Add(data1);
            dbInterface database = new dbInterface();

            database.GetValuesFromDB("IncrementCattleHerdTotal", arr1Parameter);
        }

        void CalcProductionGrowthNitrogen(int cattleRace, int cattleAgeGroup, int cattleType, double animalYear, double productionLevel, double productionLevel2)
        {
            double productionGrowthNitrogenAnimalYear = CalcCattleProductionGrowthNitrogen(cattleRace, cattleAgeGroup, productionLevel, productionLevel2);
            double productionGrowthNitrogenTotal = animalYear * productionGrowthNitrogenAnimalYear;


            List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aCattleTypeID";
            data1.value = cattleType.ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aParameterID";
            data1.value = "307";
            arr1Parameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aParameterValue";
            data1.value = productionGrowthNitrogenTotal.ToString();
            arr1Parameter.Add(data1);
            dbInterface database = new dbInterface();


            database.GetValuesFromDB("IncrementCattleHerdTotal", arr1Parameter);

        }

        void CalcManureNitrogen(int cattleType, double animalYear)
        {

            double feedNitrogenAnimalYear = 0;

            double productionMilkNitrogenAnimalYear = 0;

            double productionGrowthNitrogenAnimalYear = 0;

            double feedNitrogenAnimalYearNorm = 0;

            double manureNitrogenAnimalYear = CalcCattleManureNitrogen(feedNitrogenAnimalYear, productionMilkNitrogenAnimalYear, productionGrowthNitrogenAnimalYear);
            double manureNitrogenAnimalYearNorm = CalcCattleManureNitrogen(feedNitrogenAnimalYearNorm, productionMilkNitrogenAnimalYear, productionGrowthNitrogenAnimalYear);
            double manureNitrogenTotal = animalYear * manureNitrogenAnimalYear;
            double manureNitrogenTotalNorm = animalYear * manureNitrogenAnimalYearNorm;
            List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aCattleTypeID";
            data1.value = cattleType.ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aParameterID";
            data1.value = "308";
            arr1Parameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aParameterValue";
            data1.value = manureNitrogenTotal.ToString();
            arr1Parameter.Add(data1);
            dbInterface database = new dbInterface();


            database.GetValuesFromDB("IncrementCattleHerdTotal", arr1Parameter);

        }

        void CalcManureNitrogenOnField(int cattleType, double animalYear, double grazing)
        {

            double manureNitrogenAnimalYear = 0;
            double manureNitrogenOnFieldAnimalYear = CalcCattleManureNitrogenOnField(feedEnergyAnimalYear, grazing, manureNitrogenAnimalYear);
            double manureNitrogenOnFieldTotal = animalYear * manureNitrogenOnFieldAnimalYear;
            List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber ";
            data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aCattleTypeID";
            data1.value = cattleType.ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aParameterID";
            data1.value = "309";
            arr1Parameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aParameterValue";
            data1.value = manureNitrogenOnFieldTotal.ToString();
            arr1Parameter.Add(data1);
            dbInterface database = new dbInterface();

            database.GetValuesFromDB("IncrementCattleHerdTotal", arr1Parameter);
        }

        void CalcManureNitrogenFaecesUrine(int cattleType, double animalYear, double grazing)
        {

            double feedNitrogenAnimalYear = 0;

            double feedDryMatterAnimalYear = 0;

            double manureNitrogenAnimalYear = 0;

            double manureNitrogenFaecesAnimalYear = CalcCattleManureNitrogenInFaeces(cattleType, feedNitrogenAnimalYear, feedDryMatterAnimalYear, feedEnergyAnimalYear, grazing, manureNitrogenAnimalYear);

            double manureNitrogenFaecesTotal = animalYear * manureNitrogenFaecesAnimalYear;
            List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aCattleTypeID";
            data1.value = cattleType.ToString();
            arr1Parameter.Add(data1);



            data1 = new dbInterface.data();
            data1.name = "@aParameterID";
            data1.value = "310";
            arr1Parameter.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aParameterValue";
            data1.value = manureNitrogenFaecesTotal.ToString();
            arr1Parameter.Add(data1);
            dbInterface database = new dbInterface();

            database.GetValuesFromDB("IncrementCattleHerdTotal", arr1Parameter);


            double manureNitrogenUrineAnimalYear = manureNitrogenAnimalYear - manureNitrogenFaecesAnimalYear;
            double manureNitrogenUrineTotal = animalYear * manureNitrogenUrineAnimalYear;
            arr1Parameter = new List<dbInterface.data>();
            data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aScenarioID";
            data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aCattleTypeID";
            data1.value = cattleType.ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aParameterID";
            data1.value = "311";
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aParameterValue";
            data1.value = manureNitrogenUrineTotal.ToString();
            arr1Parameter.Add(data1);

            database.GetValuesFromDB("IncrementCattleHerdTotal", arr1Parameter);

        }

        void CalcManureStableStorage(int aCattleTypeID, double animalYear, string aStableID, double aGrazing, int cattleRace, int cattleAgeGroup, double effectivity, double grazing, double productionLevel, double productionLevel2)
        {
            string nStable, dictStable;
            DataTable arrStable;
            double stableNitrogenLoss, stableBeddingNitrogen, stableBeddingNitrogenNorm;

            double storageNitrogenAfterLoss, storageNitrogenLossDenitrification, stableNitrogenNorm;
            double GrazingPart;
            double storageNitrogen, stableNitrogenAfterLoss, stableNitrogenNormAfterLoss, storageNitrogenNormAfterLoss, storageNitrogenLossAmmonia, stableNitrogen;
            List<dbInterface.data> arr1Parameter = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aCattleTypeID";
            data1.value = aCattleTypeID.ToString();
            arr1Parameter.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aStableID";
            data1.value = aStableID;
            arr1Parameter.Add(data1);
            dbInterface database = new dbInterface();
            // nStable 
            arrStable = database.GetValuesFromDB("GetCattleStableList ", arr1Parameter);

            for (int s = 0; s < arrStable.Rows.Count; s++)
            {
                if (feedEnergyAnimalYear != 0)
                {
                    GrazingPart = aGrazing / feedEnergyAnimalYear;
                }
                else
                {
                    GrazingPart = 0;
                }
                string dataFromDB = arrStable.Rows[s][4].ToString();
                if (dataFromDB.CompareTo("") == 0)
                    dataFromDB = "0";
                stableBeddingNitrogen = double.Parse(dataFromDB) * animalYear * 0.85 * 0.005 * (1 - GrazingPart);
                string st=arrStable.Rows[s][4].ToString();
                if (st == "")
                    st = "0";
                stableBeddingNitrogenNorm = double.Parse(st) * animalYear * 0.85 * 0.005;
                if (stableBeddingNitrogen == double.NaN)
                    stableBeddingNitrogen = 0;

                if (stableBeddingNitrogenNorm == double.NaN)
                    stableBeddingNitrogenNorm = 0;

                arr1Parameter = new List<dbInterface.data>();
                data1 = new dbInterface.data();
                data1.name = "@aFarmNumber ";
                data1.value = HttpContext.Current.Session["FarmNumber"].ToString();
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aScenarioID ";
                data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                arr1Parameter.Add(data1);



                data1 = new dbInterface.data();
                data1.name = "@aCattleTypeID";
                data1.value = aCattleTypeID.ToString(); ;
                arr1Parameter.Add(data1);

                string PARAMETER_BEDDING_N = "312";
                data1 = new dbInterface.data();
                data1.name = "@aParameterID";
                data1.value = PARAMETER_BEDDING_N;
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aParameterValue";
                data1.value = stableBeddingNitrogen.ToString();
                arr1Parameter.Add(data1);

                database.GetValuesFromDB("IncrementCattleHerdTotal", arr1Parameter);
                double feedNitrogenAnimalYear = CalcCattleFeedNitrogen(cattleRace, cattleAgeGroup, feedEnergyAnimalYear, grazing) * 100 / effectivity;
                double feedNitrogenAnimalYearNorm = CalcCattleFeedNitrogen(cattleRace, cattleAgeGroup, feedEnergyAnimalYear, 0);
                double productionMilkNitrogenAnimalYear = CalcCattleProductionMilkNitrogen(cattleRace, cattleAgeGroup, productionLevel, productionLevel2);
                double productionGrowthNitrogenAnimalYear = CalcCattleProductionGrowthNitrogen(cattleRace, cattleAgeGroup, productionLevel, productionLevel2);
                double manureNitrogenAnimalYear = CalcCattleManureNitrogen(feedNitrogenAnimalYear, productionMilkNitrogenAnimalYear, productionGrowthNitrogenAnimalYear);
                double manureNitrogenAnimalYearNorm = CalcCattleManureNitrogen(feedNitrogenAnimalYearNorm, productionMilkNitrogenAnimalYear, productionGrowthNitrogenAnimalYear);
                double manureNitrogenTotal = animalYear * manureNitrogenAnimalYear;
                double manureNitrogenTotalNorm = animalYear * manureNitrogenAnimalYearNorm;
                double manureNitrogenOnFieldAnimalYear = CalcCattleManureNitrogenOnField(feedEnergyAnimalYear, grazing, manureNitrogenAnimalYear);
                double manureNitrogenOnFieldTotal = animalYear * manureNitrogenOnFieldAnimalYear;
                //manureNitrogenTotal = 0, manureNitrogenOnFieldTotal = 0, manureNitrogenTotalNorm = 0;
                st = arrStable.Rows[s][3].ToString();
                if (st == "")
                    st = "0";
                stableNitrogen = (manureNitrogenTotal - manureNitrogenOnFieldTotal) * double.Parse(st) / 100 + stableBeddingNitrogen;
                stableNitrogenNorm = (manureNitrogenTotalNorm) * double.Parse(st) / 100 + stableBeddingNitrogenNorm;

                st = arrStable.Rows[s][7].ToString();
                if (st == "")
                    st = "0";
                stableNitrogenLoss = stableNitrogen * double.Parse(st) / 100;

                stableNitrogenAfterLoss = stableNitrogen - stableNitrogenLoss;
                stableNitrogenNormAfterLoss = stableNitrogenNorm * (100 - double.Parse(st)) / 100;

                arr1Parameter = new List<dbInterface.data>();
                data1 = new dbInterface.data();
                data1.name = "@aFarmNumber";
                data1.value = HttpContext.Current.Session["FarmNumber"].ToString(); ;
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aScenarioID";
                data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                arr1Parameter.Add(data1);
                data1 = new dbInterface.data();
                data1.name = "@aProductionTypeID";
                data1.value = "3";
                arr1Parameter.Add(data1);
                data1 = new dbInterface.data();
                data1.name = "@aStableID";
                data1.value = "3";
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aDryMatterLoss";
                data1.value = "0";
                arr1Parameter.Add(data1);

                string PARAM0ETER_BEDDING_N = "312";
                data1 = new dbInterface.data();
                data1.name = "@aDryMatterAfterLoss";
                data1.value = PARAM0ETER_BEDDING_N;
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aNitrogenLoss";
                data1.value = "0";
                arr1Parameter.Add(data1);
                data1 = new dbInterface.data();
                data1.name = "@aNitrogenAfterLoss";
                data1.value = "0";
                arr1Parameter.Add(data1);

                database.GetValuesFromDB("IncrementStable", arr1Parameter);


                storageNitrogen = stableNitrogenAfterLoss;
                st = arrStable.Rows[s][9].ToString();
                if (st == "")
                    st = "0";
                storageNitrogenLossAmmonia = storageNitrogen * double.Parse(st) / 100;
                st = arrStable.Rows[s][10].ToString();
                if (st == "")
                    st = "0";
                storageNitrogenLossDenitrification = storageNitrogen * double.Parse(st) / 100;

                storageNitrogenAfterLoss = storageNitrogen - storageNitrogenLossAmmonia - storageNitrogenLossDenitrification;

                st = arrStable.Rows[s][9].ToString();
                if (st == "")
                    st = "0";
                string st2 = arrStable.Rows[s][10].ToString();
                if (st2 == "")
                    st2 = "0";
                storageNitrogenNormAfterLoss = stableNitrogenNormAfterLoss * (100 - double.Parse(st) - double.Parse(st2) / 100);
                arr1Parameter = new List<dbInterface.data>();
                data1 = new dbInterface.data();
                data1.name = "@aFarmNumber";
                data1.value = HttpContext.Current.Session["FarmNumber"].ToString(); ;
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aScenarioID";
                data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                arr1Parameter.Add(data1);
                data1 = new dbInterface.data();
                data1.name = "@aProductionTypeID";
                data1.value = "3";
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aStorageID";
                data1.value = arrStable.Rows[s][1].ToString();
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aNitrogenStableBeforeLoss";
                data1.value = stableNitrogen.ToString();
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aNitrogenStableLoss";
                data1.value = stableNitrogenLoss.ToString(); ;
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aNitrogenStableAfterLoss";
                data1.value = stableNitrogenAfterLoss.ToString();
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aNitrogenStorageLossAmmonia";
                data1.value = storageNitrogenLossAmmonia.ToString();
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aNitrogenStorageLossDenitrification";
                data1.value = storageNitrogenLossDenitrification.ToString();
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aNitrogenStorageAfterLoss";
                data1.value = storageNitrogenAfterLoss.ToString();
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aDryMatterStableBeforeLoss";
                data1.value = "0";
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aDryMatterStableLoss";
                data1.value = "0";
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aDryMatterStableAfterLoss";
                data1.value = "0";
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aDryMatterStorageLoss";
                data1.value = "0";
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aDryMatterStorageAfterLoss";
                data1.value = "0";
                arr1Parameter.Add(data1);



                database.GetValuesFromDB("IncrementStorage", arr1Parameter);

                arr1Parameter = new List<dbInterface.data>();
                data1 = new dbInterface.data();
                data1.name = "@aFarmNumber";
                data1.value = HttpContext.Current.Session["FarmNumber"].ToString(); ;
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aScenarioID";
                data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aProductionTypeID";
                data1.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aStorageID";
                data1.value = arrStable.Rows[s][1].ToString();
                arr1Parameter.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aNitrogenNormAfterLoss";
                data1.value = storageNitrogenNormAfterLoss.ToString();
                arr1Parameter.Add(data1);

                database.GetValuesFromDB("IncrementStorageNorm", arr1Parameter);


            }
        }
        double CalcCattleManureNitrogen(double fltFeedNitrogen, double fltMilkNitrogen, double fltGrowthNitrogen)
        {
            double returnValue;
            returnValue = fltFeedNitrogen - fltMilkNitrogen - fltGrowthNitrogen;
            return returnValue;

        }
        double CalcCattleManureNitrogenInFaeces(int intCattleType, double fltFeedNitrogen, double fltFeedDryMatter, double fltFeedEnergy, double fltFeedEnergyGrazing, double fltManureNitrogen)
        {
            double returnValue;
            switch (intCattleType)
            {
                case 7:// 'Ammekøer
                    double tmp = fltFeedEnergy * (0.36 * fltFeedEnergy - 0.12 * fltFeedEnergyGrazing);
                    if (tmp != 0)
                        returnValue = fltManureNitrogen / tmp;
                    else
                        returnValue = 0;
                    break;
                default:
                    returnValue = ((0.04 * fltFeedNitrogen * 1000 / 365) + ((Math.Pow((fltFeedDryMatter / 365), 2) * 1.8) / 6.25) + (((fltFeedDryMatter / 365) * 20) / 6.25)) * 365 / 1000;
                    break;
            }
            return returnValue;
        }
        double CalcCattleFeedDryMatter(int intCattleRace, int intCattleAgeGroup, double fltProductionLevel, double fltProductionLevel2, double fltEnergy)
        {
            double returnValue;

            switch (intCattleRace)
            {
                case 1: //'Tunge racer
                    switch (intCattleAgeGroup)
                    {
                        case 1: // 'Malkekøer
                            returnValue = fltEnergy * (1.10 + (8500 - fltProductionLevel) * 0.00002);
                            break;
                        case 2: //'Opdræt
                            returnValue = fltEnergy * 1.35 + (fltProductionLevel - 700) * 0.00025;
                            break;
                        case 3: //'Ungtyre
                            returnValue = fltEnergy * 1.05;
                            break;
                        default:
                            returnValue = 0;
                            break;
                    }
                    break;
                case 2: //'Jersey
                    switch (intCattleAgeGroup)
                    {
                        case 1: // 'Malkekøer
                            returnValue = fltEnergy * (1.10 + (7500 - fltProductionLevel) * 0.00002);
                            break;
                        case 2: // 'Opdræt
                            returnValue = fltEnergy * 1.35 + (fltProductionLevel - 500) * 0.00025;
                            break;
                        case 3: // 'Ungtyre
                            returnValue = fltEnergy * 1.05;
                            break;
                        default:
                            returnValue = 0;
                            break;
                    }
                    break;
                case 3: //'Ammekøer
                    returnValue = fltEnergy * 1.5;
                    break;
                default:
                    returnValue = 0;
                    break;
            }
            return returnValue;
        }
        double CalcCattleFeedNitrogen(int intCattleRace, int intCattleAgeGroup, double fltEnergy, double fltGrazing)
        {
            double returnValue;
            switch (intCattleRace)
            {
                case 1:// 'Tunge racer
                    switch (intCattleAgeGroup)
                    {
                        case 1:// 'Malkekøer
                            returnValue = fltEnergy * 0.028;
                            break;
                        case 2:// 'Opdræt
                            if (fltEnergy == 0)
                                returnValue = 0;
                            else
                                returnValue = fltEnergy * 0.025 + 10 * (fltGrazing / fltEnergy);
                            break;
                        case 3:// 'Ungtyre
                            returnValue = fltEnergy * 0.022;
                            break;
                        default:
                            returnValue = 0;
                            break;
                    } break;
                case 2:// 'Jersey
                    switch (intCattleAgeGroup)
                    {
                        case 1:// 'Malkekøer
                            returnValue = fltEnergy * 0.028;
                            break;
                        case 2:// 'Opdræt
                            returnValue = fltEnergy * 0.025 + 10 * (fltGrazing / fltEnergy);
                            break;
                        case 3:// 'Ungtyre
                            returnValue = fltEnergy * 0.022;
                            break;
                        default:
                            returnValue = 0;
                            break;
                    }
                    break;
                case 3:// 'Ammekøer
                    returnValue = fltEnergy * 0.031;
                    break;
                default:
                    returnValue = 0;
                    break;
            }
            return returnValue;
        }

        double CalcCattleProductionGrowthNitrogen(int intCattleRace, int intCattleAgeGroup, double fltProductionLevel, double fltProductionLevel2)
        {
            double returnValue;

            switch (intCattleRace)
            {
                case 1://'Tunge racer
                    switch (intCattleAgeGroup)
                    {
                        case 1://'Malkekøer
                            returnValue = 2;
                            break;
                        case 2:// 'Opdræt
                            returnValue = fltProductionLevel * 365 * 0.000025;
                            break;
                        case 3:// 'Ungtyre
                            returnValue = fltProductionLevel * 365 * 0.000025;
                            break;
                        default:
                            returnValue = 0;
                            break;
                    }
                    break;
                case 2:// 'Jersey
                    switch (intCattleAgeGroup)
                    {
                        case 1:// 'Malkekøer
                            returnValue = 1;
                            break;
                        case 2:// 'Opdræt
                            returnValue = fltProductionLevel * 365 * 0.000025;
                            break;
                        case 3://'Ungtyre
                            returnValue = fltProductionLevel * 365 * 0.000025;
                            break;
                        default:
                            returnValue = 0;
                            break;
                    }
                    break;
                case 3://'Ammekøer
                    returnValue = 2;
                    break;
                default:
                    returnValue = 0;
                    break;
            }
            return returnValue;
        }
        double CalcCattleProductionMilkNitrogen(int intCattleRace, int intCattleAgeGroup, double fltProductionLevel, double fltProductionLevel2)
        {
            double returnValue;

            switch (intCattleRace)
            {
                case 1:// 'Tunge racer
                    switch (intCattleAgeGroup)
                    {
                        case 1:// 'Malkekøer
                            returnValue = fltProductionLevel * 0.00518;
                            break;
                        default:
                            returnValue = 0;
                            break;
                    }
                    break;
                case 2:// 'Jersey
                    switch (intCattleAgeGroup)
                    {
                        case 1:// 'Malkekøer
                            returnValue = fltProductionLevel * 0.00488;
                            break;
                        default:
                            returnValue = 0;
                            break;
                    }
                    break;
                case 3://'Ammekøer
                    returnValue = ((fltProductionLevel2 - 279) * 9 + 1243) * 0.00518;
                    break;
                default:
                    returnValue = 0;
                    break;
            }
            return returnValue;
        }
        double CalcCattleManureNitrogenOnField(double fltFeedEnergy, double fltFeedEnergyGrazing, double fltManureNitrogen)
        {
            double returnValue;
            returnValue = fltFeedEnergyGrazing / fltFeedEnergy * fltManureNitrogen;
            if (fltFeedEnergy * fltManureNitrogen == 0)
                returnValue = 0;
            return returnValue;
        }

        double CalcCattleFeedEnergy(int intCattleRace, int intCattleAgeGroup, double fltProductionLevel, double fltProductionLevel2)
        {
            double returnValue;

            switch (intCattleRace)
            {
                case 1:// 'Tunge racer

                    switch (intCattleAgeGroup)
                    {
                        case 1: //'Malkekøer
                            returnValue = 1635 + 0.4 * fltProductionLevel + 0.0000167 * Math.Pow(fltProductionLevel, 2);
                            break;
                        case 2:// 'Opdræt
                            returnValue = (1683 + 2.19 * (fltProductionLevel - 700)) / 0.8905;
                            break;
                        case 3://'Ungtyre
                            returnValue = 1971 + (1100 - fltProductionLevel) * 1.09;
                            break;
                        default:
                            returnValue = 0;
                            break;

                    }
                    break;
                case 2:// 'Jersey
                    switch (intCattleAgeGroup)
                    {
                        case 1:// 'Malkekøer
                            returnValue = 1192 + 0.4 * fltProductionLevel + 0.0000167 * Math.Pow(fltProductionLevel, 2);
                            break;
                        case 2:// 'Opdræt
                            returnValue = (1236 + 1.83 * (fltProductionLevel - 500)) / 0.9155;
                            break;
                        case 3:// 'Ungtyre
                            returnValue = 1266 + (900 - fltProductionLevel) * 0.55;
                            break;
                        default:
                            returnValue = 0;
                            break;
                    }
                    break;
                case 3:// 'Ammekøer
                    returnValue = ((fltProductionLevel / 200 + 1.5) * 1.1) * 365 + 0.4 * ((fltProductionLevel2 - 279) * 9 + 1243) + 101 + 88;
                    break;
                default:
                    returnValue = 0;
                    break;
            }

            return returnValue;
        }
    }




}