using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web;

namespace FarmAC.Controls
{
    class LoginVerify
    {

        public void main()
        {
            DataTable arrFarm = null;

            string fLoginName = (string)HttpContext.Current.Request["inputLoginName"];
            string fPassword = (string)HttpContext.Current.Request["inputPassword"];
            dbInterface database = new dbInterface();
            database.InitFarmNDB();
            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.value = fLoginName;
            data.name = "@LoginName";
            paramter.Add(data);
            data.value = fPassword;
            data.name = "@Password";
            paramter.Add(data);
            DataTable arrUser = database.GetValuesFromDB("GetUserByLogin ", paramter);

            if (arrUser != null)
            {
                string aUserID = (string)arrUser.Rows[0]["UserID"];
                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.value = aUserID;
                data.name = "@aUserID";
                paramter.Add(data);
                arrFarm = database.GetValuesFromDB("GetFarmList", paramter);

            }
            database.CloseFarmNDB();

            if (arrUser != null)
            {
                HttpContext.Current.Session["hasLogin"] = "yes";

                HttpContext.Current.Session["userID"] = arrUser.Rows[0]["UserID"];


                HttpContext.Current.Session["user"] = arrUser.Rows[0]["UserName"];
                HttpContext.Current.Session["userType"] = arrUser.Rows[0]["UserType"];
                HttpContext.Current.Session["farmNumber"] = arrFarm.Rows[0]["FarmNumber"];
                HttpContext.Current.Session["farmOwner"] = arrFarm.Rows[0]["FarmOwner"];
                HttpContext.Current.Session["scenarioNumber"] = arrFarm.Rows[0]["SelectedScenario"];
                HttpContext.Current.Session["scenarioName"] = arrFarm.Rows[0]["ScenarioName"];
                HttpContext.Current.Session["soilType"] = arrFarm.Rows[0]["SoilType"];
                HttpContext.Current.Session["irrigation"] = arrFarm.Rows[0]["Irrigation"];
                if (HttpContext.Current.Session["irrigation"] == "true")
                    HttpContext.Current.Session["irrigation"] = "1";
                else
                    HttpContext.Current.Session["irrigation"] = "0";

                HttpContext.Current.Session["farmType"] = arrFarm.Rows[0]["FarmType"];
                HttpContext.Current.Session["yieldLevel"] = arrFarm.Rows[0]["YieldLevel"];
                HttpContext.Current.Session["tabNumber"] = "1";
            }
            else
            {
                HttpContext.Current.Session["hasLogin"] = "no";
            }
            string returnURL;
            if ((String)HttpContext.Current.Session["hasLogin"] == "yes")
            {
                HttpContext.Current.Session["loginTry"] = "yes";

                if (int.Parse((string)HttpContext.Current.Session["userType"]) >= 100)
                {
                    HttpContext.Current.Session.Timeout = 40;
                    returnURL = "SelectFarm.asp";
                }
                else
                {
                    returnURL = "start.asp";
                }
            }
            else
            {
                HttpContext.Current.Session["loginTry"] = "no";
                returnURL = "default.asp";
            }

            HttpContext.Current.Response.Redirect(returnURL);
            HttpContext.Current.Response.End();
        }

    }

}
