using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for FarmBoughtManureNew
    /// </summary>
    public class FarmBoughtManureNew : IHttpHandler
    {

        public void ProcessRequest(HttpContext context)
        {
            String aFarmNumber, aScenarioID, aBoughtManureID, aBoughtManureType, aBoughtManureAmount, returnValue, nID;
            String dictID, arr;
            DataTable arrID;
            aFarmNumber = context.Session["farmNumber"].ToString();
            aScenarioID = context.Session["scenarioNumber"].ToString();
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

            arrID = database.GetValuesFromDB("GetBoughtManureMaxID", paramter);
            context.Response.Write("<br> GetBoughtManureMaxID " + arrID.Rows.Count + " " + arrID.Rows[0]["MaxID"]);

            aBoughtManureID = (int.Parse(arrID.Rows[0]["MaxID"].ToString()) + 1).ToString();
            aBoughtManureType = "1";
            aBoughtManureAmount = "0";

            //response.write "InsertUpdateBoughtManure "&aFarmNumber&", "&aScenarioID&", "&aBoughtManureID&", "&aBoughtManureAmount&", "&aBoughtManureType

            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = aScenarioID;
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@aBoughtManureID";
            data.value = aBoughtManureID;
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@aBoughtManureAmount";
            data.value = aBoughtManureAmount;
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aBoughtManureType";
            data.value = aBoughtManureType;
            paramter.Add(data);

            database.GetValuesFromDB("InsertUpdateBoughtManure", paramter);
            //returnValue  = InsertUpdateBoughtManure(aFarmNumber, aScenarioID, aBoughtManureID, aBoughtManureAmount, aBoughtManureType)

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