using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for FarmScenarioNameUpdate
    /// </summary>
    public class FarmScenarioNameUpdate : IHttpHandler
    {

        public void ProcessRequest(HttpContext context)
        {
            string aFarmNumber, aScenarioID, aScenarioName, arrID, nID, aFarmOwnerName, aFarmOwnerAddress, aFarmOwnerZipCode, aFarmOwnerTown;
            string aSoilType, aIrrigationType, aDelivery, aFarmType, aSelectedScenario, returnValue;
            string aYieldLevel;
            string MaxScenarioID, NewScenarioID, sc, dictMaxID, arr;
            dbInterface database = new dbInterface();
            database.InitFarmNDB();
            //CreateLanguageDictionary
            if (context.Request["newScenarioName"] != "+")
            {

                List<dbInterface.data> paramter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.name = "@farmNumber";
                data.value = (string)context.Session["farmNumber"];
                paramter.Add(data);
                data = new dbInterface.data();
                data.name = "@scenarioNumber";
                data.value = context.Session["scenarioNumber"].ToString();
                paramter.Add(data);
                data = new dbInterface.data();
                data.name = "@newScenarioName";
                data.value = context.Request["newScenarioName"].ToString();
                paramter.Add(data);


                database.GetValuesFromDB("InsertUpdateScenario", paramter);

                aFarmNumber = context.Session["farmNumber"].ToString();
                aSelectedScenario = context.Session["scenarioNumber"].ToString();

                context.Session["scenarioName"] = context.Request["newScenarioName"];
            }
            database.CloseFarmNDB();
            //BeginHeader str.Item("strRENAME"),"+","+"
            //CreateStyleLink "styles/stylesheet.css"
            //EndHeader
            //BeginBodyData


            context.Response.Write("<form action=" + "FarmScenarioNameUpdate.asp" + " method = " + "post" + " target=" + "FarmNparent" + " onSubmit=" + "window.close();" + ">");
            context.Response.Write("<input type = " + "text" + " size=" + "30" + " name=" + "newScenarioName" + " value=" + "" + context.Session["scenarioName"] + "" + "  />");
            context.Response.Write("<input type = " + "submit" + " value=" + "OK" + "  />");
            context.Response.Write("</form>");
            context.Response.Write("<form name=" + "update" + ">");
            context.Response.Write("<input type = " + "hidden" + " name=" + "saveUpdates" + " value=" + "true" + "  />");
            context.Response.Write("</form>");
            //EndBody

            if (context.Request["newScenarioName"] != "")
            {
                context.Response.Redirect("start.asp");
                context.Response.End();
            }
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