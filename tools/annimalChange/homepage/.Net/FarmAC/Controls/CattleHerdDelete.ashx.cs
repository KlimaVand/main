using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for CattleHerdDelete
    /// </summary>
    public class CattleHerdDelete : IHttpHandler
    {

        public void ProcessRequest(HttpContext context)
        {
        }
        public void ProcessRequest(HttpContext context, string CattleHerdID)
        {

            dbInterface database = new dbInterface();
            database.InitFarmNDB();

            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = context.Session["farmNumber"].ToString();
            paramter.Add(data);




            data = new dbInterface.data();
            data.name = "@aScenarioID ";
            data.value = context.Session["scenarioNumber"].ToString(); ;
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aCattleHerdID";
            data.value = CattleHerdID;
            paramter.Add(data);

            database.GetValuesFromDB("DeleteCattleHerd", paramter);




            database.CloseFarmNDB();
            context.Session["ShowResult"] = false;
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