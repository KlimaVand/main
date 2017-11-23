using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for data
    /// </summary>
    public class LandlessUpdate : IHttpHandler
    {
        public void ProcessRequest(HttpContext context)
        {
        }
        public void ProcessRequest(HttpContext context, string aLandless)
        {
            String aFarmNumber, aScenarioID;
            DataTable returnValue;
            aFarmNumber = HttpContext.Current.Session["farmNumber"].ToString();
            aScenarioID = HttpContext.Current.Session["scenarioNumber"].ToString();

            if (aLandless == "True")
                aLandless = "1";
            else
                aLandless = "0";

            dbInterface database = new dbInterface();
            database.InitFarmNDB();
            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            paramter.Add(data);


            data = new dbInterface.data();
            data.name = "@aScenario";
            data.value = aScenarioID;
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aLandless";
            data.value = aLandless;
            paramter.Add(data);

            returnValue = database.GetValuesFromDB("UpdateScenarioLandless", paramter);


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