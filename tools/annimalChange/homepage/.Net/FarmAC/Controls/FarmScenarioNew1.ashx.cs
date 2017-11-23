using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for FarmScenarioNew1
    /// </summary>
    public class FarmScenarioNew1 : IHttpHandler
    {
        public void ProcessRequest(HttpContext context)
        {

        }

        public void ProcessRequest(HttpContext context, string inputName, string inputAddress, string inputZip, string inputTown, string selectSoilType, string selectIrrigation, string selectFarmType, string selectYieldLevel, string inputDescription, string atmosphericDeposition, string selectScenario, string selectAgroZone, bool copy)
        {

            string aFarmNumber, aScenarioID, aScenarioName, nID, aFarmOwnerName, aFarmOwnerAddress, aFarmOwnerZipCode, aFarmOwnerTown;
            DataTable arrID, dictID, returnValue, arrFarm;
            string aSoilType, aIrrigationType, aDelivery, aFarmType, aSelectedScenario;
            string aYieldLevel, nFarm;
            string MaxScenarioID, sc, dictMaxID;
            string arr, dictFarm, farmDescription;
            int NewScenarioID;

            if (selectAgroZone == "")
                selectAgroZone = "1";
            aFarmNumber = context.Session["farmNumber"].ToString();

            dbInterface database = new dbInterface();
            database.InitFarmNDB();
            List<dbInterface.data> parameterList = new List<dbInterface.data>();
            dbInterface.data datas = new dbInterface.data();
            datas.name = "@FarmNumber";
            datas.value = aFarmNumber;
            parameterList.Add(datas);
            arrID = database.GetValuesFromDB("GetMaxScenarioIDPrFarm", parameterList);
            if (copy)
            {

                NewScenarioID = int.Parse(arrID.Rows[0][0].ToString()) + 1;
                parameterList = new List<dbInterface.data>();
                datas = new dbInterface.data();
                datas.name = "@FarmNumber";
                datas.value = aFarmNumber;
                parameterList.Add(datas);

                datas = new dbInterface.data();
                datas.name = "@FromScenarioID";
                datas.value = selectScenario;
                parameterList.Add(datas);


                datas = new dbInterface.data();
                datas.name = "ToScenarioID";
                datas.value = NewScenarioID.ToString();
                parameterList.Add(datas);

                database.GetValuesFromDB("copy_Scenario", parameterList);
            }
            else
            {
                parameterList = new List<dbInterface.data>();
                datas = new dbInterface.data();
                datas.name = "aFarmNumber";
                datas.value = aFarmNumber;
                parameterList.Add(datas);


                dictID = database.GetValuesFromDB("GetScenarioMaxID", parameterList);

                context.Response.Write("<br> nID " + dictID.Rows.Count);
                NewScenarioID = int.Parse(dictID.Rows[0]["MaxID"].ToString()) + 1;
                context.Response.Write("<br> aScenarioID " + NewScenarioID);
                aScenarioName = "Scenario " + NewScenarioID.ToString();


                parameterList = new List<dbInterface.data>();
                datas = new dbInterface.data();
                datas.name = "@aFarmNumber";
                datas.value = aFarmNumber;
                parameterList.Add(datas);
                datas = new dbInterface.data();
                datas.name = "@aSelectedScenario";
                datas.value = NewScenarioID.ToString();
                parameterList.Add(datas);

                datas = new dbInterface.data();
                datas.name = "@aScenarioName";
                datas.value = aScenarioName;
                parameterList.Add(datas);


                returnValue = database.GetValuesFromDB("InsertUpdateScenario", parameterList);

                //	context.Response.Write( "<br> InsertUpdateScenario " + returnValue.Rows[0][0] );
            }


            parameterList = new List<dbInterface.data>();
            datas = new dbInterface.data();
            datas.name = "@aFarmNumber";
            datas.value = aFarmNumber;
            parameterList.Add(datas);
            datas = new dbInterface.data();
            datas.name = "@aSelectedScenario";
            datas.value = NewScenarioID.ToString();
            parameterList.Add(datas);


            returnValue = database.GetValuesFromDB("UpdateSelectedScenario", parameterList);
            //context.Response.Write( "<br> InsertUpdateScenario " + returnValue.Rows[0][0] );


            //set dictFarm = CreateObject("Scripting.Dictionary")
            parameterList = new List<dbInterface.data>();
            datas = new dbInterface.data();
            datas.name = "@aFarmNumber";
            datas.value = aFarmNumber;
            parameterList.Add(datas);
            arrFarm = database.GetValuesFromDB("GetFarmData", parameterList);


            context.Session["scenarioName"] = arrFarm.Rows[0]["ScenarioName"];

            context.Response.Write("<br> scenarioName " + context.Session["scenarioName"]);


            aFarmNumber = context.Session["farmNumber"].ToString();
            aFarmOwnerName = inputName;
            aFarmOwnerAddress = inputAddress;
            aFarmOwnerZipCode = inputZip;
            aFarmOwnerTown = inputTown;
            aSoilType = selectSoilType;
            aIrrigationType = selectIrrigation;
            if (aIrrigationType == "")
                aIrrigationType = "0";
            aDelivery = "1"; //'Request.Form("")
            aFarmType = selectFarmType;
            aSelectedScenario = NewScenarioID.ToString();
            aYieldLevel = selectYieldLevel;
            farmDescription = inputDescription;


            context.Session["farmType"] = aFarmType;
            context.Session["scenarioNumber"] = aSelectedScenario;

            context.Session["soilType"] = aSoilType;
            context.Session["irrigation"] = aIrrigationType;
            context.Session["yieldLevel"] = aYieldLevel;

            parameterList = new List<dbInterface.data>();
            datas = new dbInterface.data();
            datas.name = "@aFarmNumber";
            datas.value = aFarmNumber;
            parameterList.Add(datas);


            datas = new dbInterface.data();
            datas.name = "@aFarmOwnerName";
            datas.value = aFarmOwnerName;
            parameterList.Add(datas);

            datas = new dbInterface.data();
            datas.name = "@aFarmOwnerAddress";
            datas.value = aFarmOwnerAddress;
            parameterList.Add(datas);

            datas = new dbInterface.data();
            datas.name = "@aFarmOwnerZipCode";
            datas.value = aFarmOwnerZipCode;
            parameterList.Add(datas);

            datas = new dbInterface.data();
            datas.name = "@aFarmOwnerTown";
            datas.value = aFarmOwnerTown;
            parameterList.Add(datas);
            datas = new dbInterface.data();
            datas.name = "@aSoilType";
            datas.value = aSoilType;
            parameterList.Add(datas);
            datas = new dbInterface.data();
            datas.name = "@aIrrigationType";
            datas.value = aIrrigationType;
            parameterList.Add(datas);
            datas = new dbInterface.data();
            datas.name = "@aFarmType";
            datas.value = aFarmType;
            parameterList.Add(datas);
            datas = new dbInterface.data();
            datas.name = "@aSelectedScenario";
            datas.value = aSelectedScenario;
            parameterList.Add(datas);
            datas = new dbInterface.data();
            datas.name = "@aYieldLevel";
            datas.value = aYieldLevel;
            parameterList.Add(datas);
            datas = new dbInterface.data();
            datas.name = "@aAgroZone";
            datas.value = selectAgroZone;
            parameterList.Add(datas);
            datas = new dbInterface.data();
            datas.name = "@aFarmDescription";
            datas.value = farmDescription;
            parameterList.Add(datas);



            returnValue = database.GetValuesFromDB("InsertUpdate_Farm", parameterList);
            //	context.Response.Write("<br> InsertUpdateFarm " + returnValue.Rows[0][0]);

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