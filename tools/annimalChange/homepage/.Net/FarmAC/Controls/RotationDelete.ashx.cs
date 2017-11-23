using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for RotationDelete
    /// </summary>
    public class RotationDelete : IHttpHandler
    {
        public void ProcessRequest(HttpContext context)
        {

        }
        public void ProcessRequest(HttpContext context, string rotationID)
        {

            string aFarmNumber, aScenarioID, aRotationID;
            string nAreaSum, arrAreaSum, arrParameter;
            DataTable returnValue;

            aFarmNumber = context.Session["farmNumber"].ToString();
            aScenarioID = context.Session["scenarioNumber"].ToString();
            aRotationID = rotationID;//context.Request.QueryString["RotationID"];
            dbInterface database = new dbInterface();
            database.InitFarmNDB();


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
            data.name = "@aRotationID";
            data.value = aRotationID;
            paramter.Add(data);


            database.GetValuesFromDB("DeleteRotation", paramter);

            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = aScenarioID;
            paramter.Add(data);
            returnValue = database.GetValuesFromDB("CalcFarmTotal", paramter);


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