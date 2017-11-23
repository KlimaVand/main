
using FarmAC.Controls;
using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;

namespace FarmAC
{
    /// <summary>
    /// Summary description for FarmSelectedScenarioUpdate
    /// </summary>
    public class FarmSelectedScenarioUpdate : IHttpHandler
    {

        public void ProcessRequest(HttpContext context)
        {

            string aFarmNumber, aSelectedScenario, returnValue, nFarm, dictFarm, arr;

            aFarmNumber = context.Session["farmNumber"].ToString();
            dbInterface database = new dbInterface();

            database.InitFarmNDB();


            aSelectedScenario = context.Request.Form["selectScenario"];

            if (context.Session["scenarioNumber"] != aSelectedScenario)
            {
                context.Response.Write(context.Session["ShowResult"]);
                context.Session["ShowResult"] = false;
                context.Response.Write(context.Session["ShowResult"]);
            }

            List<dbInterface.data> arrParameter1 = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;
            arrParameter1.Add(data1);

            data1 = new dbInterface.data();
            data1.name = "@aSelectedScenario";
            data1.value = aSelectedScenario;
            arrParameter1.Add(data1);

            database.GetValuesFromDB("UpdateSelectedScenario", arrParameter1);
            arrParameter1 = new List<dbInterface.data>();
            data1 = new dbInterface.data();
            data1.name = "@aFarmNumber";
            data1.value = aFarmNumber;
            arrParameter1.Add(data1);
            DataTable arrFarm = database.GetValuesFromDB("GetFarmData", arrParameter1);

            context.Session["scenarioName"] = arrFarm.Rows[0]["ScenarioName"];
            context.Session["scenarioNumber"] = aSelectedScenario;


            context.Response.Redirect("start.asp");
            context.Response.End();
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