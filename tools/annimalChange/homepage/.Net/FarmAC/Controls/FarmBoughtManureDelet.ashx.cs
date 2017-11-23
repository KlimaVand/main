using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for CattleHerdDelete
    /// </summary>
    public class FarmBoughtManureDelet : IHttpHandler
    {

        public void ProcessRequest(HttpContext context)
        {
        }
        public void ProcessRequest(HttpContext context, string DeleteBoughtManureID)
        {

           string aFarmNumber =  context.Session["farmNumber"].ToString();
            string aScenarioID =  context.Session["scenarioNumber"].ToString();


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
            data.name = "@aBoughtManureID";
            data.value = DeleteBoughtManureID;
            paramter.Add(data);

             database.GetValuesFromDB ("DeleteBoughtManure",paramter);



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