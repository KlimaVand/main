using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for Dummy
    /// </summary>
    public class FieldPlanDelete : IHttpHandler
    {
        public void ProcessRequest(HttpContext context)
        {

        }
        public void ProcessRequest(HttpContext context, string aRotationID, string aFieldPlanID, string aTotalArea, string nFieldPlan)
        {

            int PARAMETER_FE_CERNAL = 1;   //Cernal yield in parameter table
            int PARAMETER_FE_STRAW = 2;   //Straw yield in parameter table   
            int PARAMETER_FE_SECONDCROP = 3;  //Second crop yield in parameter table 
            int PARAMETER_N_CERNAL = 4;   //Cernal nitrogen content in parameter table 
            int PARAMETER_N_STRAW = 5;   //Straw nitrogen content in parameter table 
            int PARAMETER_N_SECONDCROP = 6;   //SecondCrop nitrogen content in parameter table 
            int PARAMETER_P_CERNAL = 7;  //Cernal Phosphor content in parameter table 
            int PARAMETER_P_STRAW = 8;  //Straw Phosphor content in parameter table 
            int PARAMETER_P_SECONDCROP = 9;  //Straw Phosphor content in parameter table 

            string nCrop, NumberCrops, test;
            string CropArea, i;// array of crop areas NUMBERCROPS
            string arrCropAndArea;
            string aFieldPlanRotationID;
            string c, aFarmNumber, aScenarioID, arrFieldPlan, returnValue;
            string f, aArea, aSecondCrop;


            aFarmNumber = context.Session["farmNumber"].ToString();
            aScenarioID = context.Session["scenarioNumber"].ToString();
            /*
aRotationID  = context.Request.QueryString["RotationID"].ToString();
aFieldPlanID = context.Request.QueryString["FieldPlanID"].ToString();
aTotalArea   = context.Request.Form["inputArea_"+aRotationID+""].ToString();
nFieldPlan   = context.Request.Form["hiddenNFieldPlan_"+aRotationID+""].ToString();*/

            dbInterface database = new dbInterface();

            List<dbInterface.data> parameterList = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "aFarmNumber";
            data.value = aFarmNumber;
            parameterList.Add(data);
            data = new dbInterface.data();
            data.name = "aScenarioID";
            data.value = aScenarioID;
            parameterList.Add(data);
            data = new dbInterface.data();
            data.name = "aRotationID";
            data.value = aRotationID;
            parameterList.Add(data);
            data = new dbInterface.data();
            data.name = "@aFieldPlanID";
            data.value = aFieldPlanID;
            parameterList.Add(data);

            database.GetValuesFromDB("DeleteFieldPlan", parameterList);
            parameterList = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            parameterList.Add(data);
            data = new dbInterface.data();
            data.name = "aScenarioID";
            data.value = aScenarioID;
            parameterList.Add(data);
            data = new dbInterface.data();
            data.name = "aRotationID";
            data.value = aRotationID;
            parameterList.Add(data);
            data = new dbInterface.data();
            data.name = "@aFieldPlanRotationID";
            data.value = aFieldPlanID;
            parameterList.Add(data);


            database.GetValuesFromDB("DeleteFieldPlanRotation", parameterList);

            parameterList = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = context.Session["farmNumber"].ToString();
            parameterList.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = context.Session["scenarioNumber"].ToString();
            parameterList.Add(data);
            data = new dbInterface.data();
            data.name = "@aRotationID";
            data.value = aRotationID;
            parameterList.Add(data);
            //NumberCrops
            DataTable result = database.GetValuesFromDB("GetFieldPlanCropAndAreaList", parameterList);

            if (result.Rows[0][0] == "0")
            {
                parameterList = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "farmNumber";
                data.value = context.Session["farmNumber"].ToString();
                parameterList.Add(data);
                data = new dbInterface.data();
                data.name = "farmNumber";
                data.value = context.Session["scenarioNumber"].ToString();
                parameterList.Add(data);


                database.GetValuesFromDB("DeleteRotation", parameterList);
            }


            parameterList = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "aFarmNumber";
            data.value = context.Session["farmNumber"].ToString();
            parameterList.Add(data);

            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = context.Session["scenarioNumber"].ToString();
            parameterList.Add(data);
            database.GetValuesFromDB("CalcFarmTotal", parameterList);


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