using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.UI.WebControls;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for FarmUpdate
    /// </summary>
    public class FarmUpdate : IHttpHandler
    {
        public void ProcessRequest(HttpContext context)
        {
        }
        public void ProcessRequest(HttpContext context, List<DropDownList> boughtManure, string aFarmOwnerName, string aFarmOwnerAddress, string aSoilType, string aFarmType, string aSelectedScenario, string aYieldLevel, string agroZone, string farmDescription, string atmosphericDeposition)
        {

            String aFarmNumber, aFarmOwnerZipCode, aFarmOwnerTown, nFarm;


            string d;
            string aSoldManureID, aSoldManureAmount, aSoldManureType, aBoughtManureID, aBoughtManureAmount, aBoughtManureType, aBoughtManureUtilization, aBoughtManureConversion;

            string dictFarm, arr;



            aFarmNumber = context.Session["farmNumber"].ToString();

            aFarmOwnerZipCode = "";//'Request.Form("inputZip")
            aFarmOwnerTown = "";//'Request.Form("inputTown")

            //farmDescription   = context.Request.Form["inputDescription"];


            context.Response.Write("farmDescription : " + farmDescription);

            context.Session["farmType"] = aFarmType;
            context.Session["scenarioNumber"] = aSelectedScenario;
            context.Session["soilType"] = aSoilType;
            context.Session["irrigation"] = "0";
            context.Session["yieldLevel"] = aYieldLevel;

            dbInterface database = new dbInterface();
            database.InitFarmNDB();

            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data dataa = new dbInterface.data();
            dataa.name = "@FarmNumber";
            dataa.value = context.Session["farmNumber"].ToString();
            paramter.Add(dataa);
            dataa.name = "@ScenarioID";
            dataa.value = aSelectedScenario;
            paramter.Add(dataa);
            DataTable arrBoughtManure = database.GetValuesFromDB("GetBoughtManureList ", paramter);
            int antal= arrBoughtManure.Rows.Count;
            for (int bm = 0; bm < boughtManure.Count; bm++)
            {

                aBoughtManureID =( bm).ToString(); ;
                aBoughtManureAmount ="0";
                aBoughtManureType = boughtManure[bm].SelectedItem.Attributes["dbValue"];

                aBoughtManureUtilization = context.Request.Form["BoughtUtilization_" + bm + ""];
                aBoughtManureConversion = context.Request.Form["BoughtConversion_" + bm + ""];
                List<dbInterface.data> arrParameter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = aFarmNumber;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aScenarioID";
                data.value = aSelectedScenario;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aBoughtManureID";
                data.value = (int.Parse(aBoughtManureID)+1).ToString();
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aBoughtManureAmount";
                data.value = aBoughtManureAmount;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aBoughtManureType";
                data.value = aBoughtManureType;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aBoughtManureUtilization";
                data.value = aBoughtManureUtilization;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aBoughtManureConversion";
                data.value = aBoughtManureConversion;
                arrParameter.Add(data);

                context.Response.Write(aBoughtManureID);
                context.Response.Write(aBoughtManureType);
                arrBoughtManure = database.GetValuesFromDB("GetBoughtManureList ", paramter);
                antal = arrBoughtManure.Rows.Count;
                database.GetValuesFromDB("InsertUpdateBoughtManure", arrParameter);
                arrBoughtManure = database.GetValuesFromDB("GetBoughtManureList ", paramter);
                antal = arrBoughtManure.Rows.Count;

            }
            arrBoughtManure = database.GetValuesFromDB("GetBoughtManureList ", paramter);
            antal = arrBoughtManure.Rows.Count;
            List<dbInterface.data> arrParameter1 = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;
            arrParameter1.Add(data1);

            database.GetValuesFromDB("DeleteFarmDeliveryAll", arrParameter1);




            arrParameter1 = new List<dbInterface.data>();
            data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;
            arrParameter1.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aDelivery";
            data1.value = "1";
            arrParameter1.Add(data1);

            database.GetValuesFromDB("InsertFarmDelivery", arrParameter1);

            arrParameter1 = new List<dbInterface.data>();
            data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;
            arrParameter1.Add(data1);


            data1 = new dbInterface.data();
            data1.name = "@aDelivery";
            data1.value = "4";
            arrParameter1.Add(data1);

            database.GetValuesFromDB("InsertFarmDelivery", arrParameter1);

            arrParameter1 = new List<dbInterface.data>();
            data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;
            arrParameter1.Add(data1);


            data1 = new dbInterface.data();
            data1.name = "@aDelivery";
            data1.value = "5";
            arrParameter1.Add(data1);
            database.GetValuesFromDB("InsertFarmDelivery", arrParameter1);
            arrParameter1 = new List<dbInterface.data>();
            data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;
            arrParameter1.Add(data1);


            data1 = new dbInterface.data();
            data1.name = "@aDelivery";
            data1.value = "6";
            arrParameter1.Add(data1);
            database.GetValuesFromDB("InsertFarmDelivery", arrParameter1);




            arrParameter1 = new List<dbInterface.data>();
            data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;
            arrParameter1.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aFarmOwnerName";
            data1.value = aFarmOwnerName;
            arrParameter1.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aFarmOwnerAddress";
            data1.value = aFarmOwnerAddress;
            arrParameter1.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aFarmOwnerZipCode";
            data1.value = aFarmOwnerZipCode;
            arrParameter1.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aFarmOwnerTown";
            data1.value = aFarmOwnerTown;
            arrParameter1.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aSoilType";
            data1.value = aSoilType;
            arrParameter1.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aIrrigationType";
            data1.value = "0";
            arrParameter1.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aFarmType";
            data1.value = aFarmType;
            arrParameter1.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aSelectedScenario";
            data1.value = aSelectedScenario;
            arrParameter1.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aYieldLevel";
            data1.value = aYieldLevel;
            arrParameter1.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aAgroZone";
            data1.value = agroZone;
            arrParameter1.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aFarmDescription";
            data1.value = farmDescription;
            arrParameter1.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "@aAtmosphericDeposition";
            data1.value = atmosphericDeposition;
            arrParameter1.Add(data1);


            database.GetValuesFromDB("InsertUpdate_Farm", arrParameter1);
            arrParameter1 = new List<dbInterface.data>();
            data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;
            arrParameter1.Add(data1);
            DataTable arrFarm = database.GetValuesFromDB("GetFarmData", arrParameter1);


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