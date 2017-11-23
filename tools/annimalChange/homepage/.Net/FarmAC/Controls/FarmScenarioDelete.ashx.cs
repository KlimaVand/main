using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for FarmScenarioDelete
    /// </summary>
    public class FarmScenarioDelete : IHttpHandler
    {
        public void ProcessRequest(HttpContext context)
        {
        }
        public void ProcessRequest(HttpContext context, string aScenarioID)
        {
            String aFarmNumber, returnValue, aSelectedScenario, nScenario;
            String dictScenario, arr;
            DataTable arrScenario;

            aFarmNumber = context.Session["farmNumber"].ToString();

            dbInterface database = new dbInterface();
            database.InitFarmNDB();

            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            paramter.Add(data);
            arrScenario = database.GetValuesFromDB("GetSelectListScenario", paramter);



            if (aScenarioID != arrScenario.Rows[0][0])
            {
                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = aFarmNumber;
                paramter.Add(data);
                data = new dbInterface.data();
                data.name = "@aScenarioID";
                data.value = aScenarioID;
                paramter.Add(data);


                database.GetValuesFromDB("DeleteScenario", paramter);

            }

            context.Session["scenarioNumber"] = arrScenario.Rows[0][0];
            context.Session["scenarioName"] = arrScenario.Rows[0][1].ToString();
            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@aSelectedScenario";
            data.value = "1";
            paramter.Add(data);
            database.GetValuesFromDB("UpdateSelectedScenario", paramter);


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