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
    public class FieldPlanUpdate : IHttpHandler
    {

        public void ProcessRequest(HttpContext context)
        {

        }
        public void ProcessRequest(HttpContext context, string aRotationID, string area, string rotationName, string soiltype, List<string> aStartDate, List<string> aEndDate, List<string> aCrop, List<string> aFeedstuffList, List<string> aStrawFeedStuffList, List<string> irregation)
        {


            String NumberCrops;
            String aFieldPlanRotationID;
            String c, aFarmNumber, aScenarioID, aSoilTypeID, aRotationIrrigation, aArea, aSecondCrop;
            DataTable returnValue;
            double aTotalArea;
            int nFieldPlan;

            String nFieldPlanRotation, arrFieldPlanRotation;
            String aYieldLevel;
            List<dbInterface.data> arrParameter;
            String aAfterCrop;
            String aFeedStuff, aGrazingPart;
            String rStartDate, rEndDate;
            int rotationSpan;
            String changes;





            aFarmNumber = HttpContext.Current.Session["farmNumber"].ToString();
            aScenarioID = HttpContext.Current.Session["scenarioNumber"].ToString();

            aSoilTypeID = HttpContext.Current.Session["soilType"].ToString();
            aYieldLevel = HttpContext.Current.Session["yieldLevel"].ToString();

            if (aRotationID == "")
                aRotationID = "1";

            changes = HttpContext.Current.Request["changes_" + aRotationID + ""];
            aRotationIrrigation = HttpContext.Current.Request.Form["Irrigation_" + aRotationID + ""];
            if (aRotationIrrigation == "")
                aRotationIrrigation = "0";
            if (soiltype != HttpContext.Current.Session["SoilType"])
            {
                aSoilTypeID = soiltype;
            }
            if (HttpContext.Current.Request.Form["YieldLevel_" + aRotationID + ""] != HttpContext.Current.Session["yieldLevel"])
            {
                aYieldLevel = HttpContext.Current.Request.Form["YieldLevel_" + aRotationID + ""];
            }





            aTotalArea = double.Parse(area);

            dbInterface database = new dbInterface();
            database.InitFarmNDB();
            List<dbInterface.data> paramter222 = new List<dbInterface.data>();
            dbInterface.data data111 = new dbInterface.data();
            data111.name = "@aFarmNumber";
            data111.value = HttpContext.Current.Session["farmNumber"].ToString();
            paramter222.Add(data111);
            data111.name = "@aScenarioID";
            data111.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            paramter222.Add(data111);
            data111 = new dbInterface.data();
            data111.name = "@aRotationID";
            data111.value = aRotationID;
            paramter222.Add(data111);
            int number = database.GetValuesFromDB("GetFieldPlanList", paramter222).Rows.Count;
            //update rotation record
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

            data = new dbInterface.data();
            data.name = "@aRotationName";
            data.value = rotationName;
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aSoilType";
            data.value = aSoilTypeID;
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aIrrigation";
            data.value = "0";
            paramter.Add(data);

            //this part should be deleted when DB is updated
            data = new dbInterface.data();
            data.name = "@aYieldLevel";
            data.value = "100";
            paramter.Add(data);

            data = new dbInterface.data();
            data.name = "@aArea";
            data.value = aTotalArea.ToString();
            paramter.Add(data);

            number = database.GetValuesFromDB("GetFieldPlanList", paramter222).Rows.Count;
            returnValue = database.GetValuesFromDB("InsertUpdateRotation", paramter);
            number = database.GetValuesFromDB("GetFieldPlanList", paramter222).Rows.Count;
            arrParameter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = aScenarioID;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aRotationID";
            data.value = aRotationID;
            arrParameter.Add(data);
            List<dbInterface.data> arrParameterFP = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            arrParameterFP.Add(data);

            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = aScenarioID;
            arrParameterFP.Add(data);

            data = new dbInterface.data();
            data.name = "@aRotationID";
            data.value = aRotationID;
            arrParameterFP.Add(data);



            int month = int.Parse(aStartDate[0].Split('/')[1]);
            int day = int.Parse(aStartDate[0].Split('/')[0]);
            int year = int.Parse(aStartDate[0].Split('/')[2]);

            DateTime start = new DateTime(year, month, day);
            month = int.Parse(aEndDate[aEndDate.Count - 1].Split('/')[1]);
            day = int.Parse(aEndDate[aEndDate.Count - 1].Split('/')[0]);
            year = int.Parse(aEndDate[aEndDate.Count - 1].Split('/')[2]);
            DateTime stop = new DateTime(year, month, day);
            rotationSpan = (start - stop).Duration().Days;

            if (rotationSpan <= 0)
                rotationSpan = 1;
            for (int f = 0; f < aStartDate.Count; f++)
            {
                string aFeedstuff=aFeedstuffList[f];
                string aStrawFeedStuff= aStrawFeedStuffList[f];


                aArea = (aTotalArea / rotationSpan).ToString();
                aAfterCrop = "0";





                if (aStartDate[f].Contains("/") || aStartDate[f].Contains("\\"))
                {
                    aStartDate[f] = (aStartDate[f].Split('/')[2] + "-" + aStartDate[f].Split('/')[1] + "-" + aStartDate[f].Split('/')[0]);
                    aEndDate[f] = (aEndDate[f].Split('/')[2] + "-" + aEndDate[f].Split('/')[1] + "-" + aEndDate[f].Split('/')[0]);
                }
                arrParameter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = aFarmNumber;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aScenarioID";
                data.value = aScenarioID;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aRotationID";
                data.value = aRotationID;
                arrParameter.Add(data);

                data = new dbInterface.data();
                data.name = "@aFieldPlanID";
                data.value = (f + 1).ToString(); ;
                arrParameter.Add(data);
                data = new dbInterface.data();
                data.name = "@aCrop";
                data.value = aCrop[f];
                arrParameter.Add(data);
                data = new dbInterface.data();
                data.name = "@aArea";
                data.value = aArea;
                arrParameter.Add(data);
                data = new dbInterface.data();
                data.name = "@aAfterCrop";
                data.value = aAfterCrop;
                arrParameter.Add(data);
                data = new dbInterface.data();
                data.name = "@aFeedStuffID";
                data.value = aFeedstuff;
                arrParameter.Add(data);
                data = new dbInterface.data();
                data.name = "@aStrawFeedStuffID";
                data.value = aStrawFeedStuff;
                arrParameter.Add(data);
                data = new dbInterface.data();
                data.name = "@aStartDate";
                data.value = aStartDate[f];
                arrParameter.Add(data);
                data = new dbInterface.data();
                data.name = "@aEndDate";
                data.value = aEndDate[f];
                arrParameter.Add(data);
                data = new dbInterface.data();
                data.name = "@aIrrigation";
                data.value = irregation[f];
                arrParameter.Add(data);
                arrParameterFP = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = aFarmNumber;
                arrParameterFP.Add(data);

                data = new dbInterface.data();
                data.name = "@aScenarioID";
                data.value = aScenarioID;
                arrParameterFP.Add(data);

                data = new dbInterface.data();
                data.name = "@aRotationID";
                data.value = aRotationID;
                arrParameterFP.Add(data);
                data = new dbInterface.data();
                data.name = "@aFieldPlanRotationID";
                data.value = (f+1).ToString();
                arrParameterFP.Add(data);
                data = new dbInterface.data();
                data.name = "@aCrop";
                data.value = aCrop[f]; ;
                arrParameterFP.Add(data);
                data = new dbInterface.data();
                data.name = "@aCrop_AfterCrop";
                data.value = aAfterCrop;
                arrParameterFP.Add(data);
                data = new dbInterface.data();
                data.name = "@aArea";
                data.value = aArea;
                arrParameterFP.Add(data);



                aSecondCrop = aCrop[f]; ;

                data = new dbInterface.data();
                data.name = "@aSecondCropID";
                data.value = aSecondCrop;
                arrParameterFP.Add(data);

                data = new dbInterface.data();
                data.name = "@aOrganicFertilizer";
                data.value = null;
                arrParameterFP.Add(data);



                number = database.GetValuesFromDB("GetFieldPlanList", paramter222).Rows.Count;
                returnValue = database.GetValuesFromDB("InsertUpdateFieldPlan", arrParameter);
                number = database.GetValuesFromDB("GetFieldPlanList", paramter222).Rows.Count;
                returnValue = database.GetValuesFromDB("InsertUpdateFieldPlanRotationMinimum ", arrParameterFP);
                number = database.GetValuesFromDB("GetFieldPlanList", paramter222).Rows.Count;

            }

            arrParameter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = aScenarioID;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aRotationID";
            data.value = aRotationID;
            arrParameter.Add(data);
            data = new dbInterface.data();
            data.name = "@aFieldPlanRotationID";
            data.value = aStartDate.Count().ToString();
            arrParameter.Add(data);
            number = database.GetValuesFromDB("GetFieldPlanList", paramter222).Rows.Count;
            returnValue = database.GetValuesFromDB("DeleteFieldPlanRotationSurplus ", arrParameter);
            number = database.GetValuesFromDB("GetFieldPlanList", paramter222).Rows.Count;


            database.CloseFarmNDB();
            HttpContext.Current.Session["ShowResult"] = false;


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