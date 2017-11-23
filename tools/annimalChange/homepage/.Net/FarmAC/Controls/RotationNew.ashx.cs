using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for RotationNew
    /// </summary>
    public class RotationNew : IHttpHandler
    {

        public void ProcessRequest(HttpContext context)
        {

            string aFarmNumber, aScenarioID, aSoilTypeID, aIrrigationType, aFieldPlanID, aCrop, aArea;
            string nID, dictID;
            int aRotationID;
            DataTable arrID, returnValue;
            string nFarm, dictFarm;
            DataTable arrFarm;
            string aYieldLevel;
            string aAfterCrop;

            aFarmNumber = context.Session["farmNumber"].ToString();
            aScenarioID = context.Session["scenarioNumber"].ToString(); ;
            dbInterface database = new dbInterface();

            database.InitFarmNDB();
            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            paramter.Add(data);
            //nFarm 
            arrFarm = database.GetValuesFromDB("GetFarmData ", paramter);


            aSoilTypeID = arrFarm.Rows[0]["SoilType"].ToString();
            aIrrigationType = arrFarm.Rows[0]["Irrigation"].ToString();
            if (aIrrigationType == "False")
                aIrrigationType = "0";
            else
                aIrrigationType = "1";
            aYieldLevel = arrFarm.Rows[0]["YieldLevel"].ToString();


            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = aScenarioID;
            paramter.Add(data);
            //nID;
            arrID = database.GetValuesFromDB("GetRotationMaxID ", paramter);

            if (arrID.Rows[0]["MaxID"].ToString().CompareTo("") == 0)
                aRotationID = 1;
            else
                aRotationID = int.Parse(arrID.Rows[0]["MaxID"].ToString()) + 1;


            List<dbInterface.data> arr = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = aScenarioID;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aRotationID";
            data.value = aRotationID.ToString();
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aRotationName";
            data.value = aRotationID.ToString();
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aSoilType";
            data.value = aSoilTypeID.ToString();
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aIrrigation";
            data.value = aIrrigationType;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aYieldLevel";
            data.value = aYieldLevel;
            arr.Add(data);


            returnValue = database.GetValuesFromDB("InsertUpdateRotation", arr);


            aFieldPlanID = "1";
            aCrop = "1";
            aArea = "0";
            aAfterCrop = "0";


            arr = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            arr.Add(data);


            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = aScenarioID;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aRotationID";
            data.value = aRotationID.ToString();
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aFieldPlanID";
            data.value = aFieldPlanID;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aCrop";
            data.value = aCrop;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aArea";
            data.value = aArea;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aAfterCrop";
            data.value = aAfterCrop;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aFeedStuffID";
            data.value = "0";
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aStrawFeedStuffID";
            data.value = "0";
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aStartDate";
            data.value = null;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aEndDate";
            data.value = null;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aIrrigation";
            data.value = "0";
            arr.Add(data);



            returnValue = database.GetValuesFromDB("InsertUpdateFieldPlan", arr);

            aAfterCrop = "0";
            arr = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            arr.Add(data);


            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = aScenarioID;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aRotationID";
            data.value = aRotationID.ToString();
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aFieldPlanRotationID";
            data.value = aFieldPlanID;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aPreviousCrop";
            data.value = "2";
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aPreCropOriginalID";
            data.value = "1";
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aPreCrop_AfterCrop";
            data.value = "1";
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aPreCropSecondCrop";
            data.value = "0";
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aCrop";
            data.value = aCrop;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aCrop_AfterCrop";
            data.value = aAfterCrop;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aArea";
            data.value = aArea;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aOrganicFertilizer";
            data.value = "0";
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aGrazingPart";
            data.value = "0";
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aSalePart";
            data.value = "0";
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aStrawUseType";
            data.value = "3";
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aSecondCropID";
            data.value = aCrop;
            arr.Add(data);

            data = new dbInterface.data();
            data.name = "@aPrepreCropID";
            data.value = "0";
            arr.Add(data);

            returnValue = database.GetValuesFromDB("InsertUpdateFieldPlanRotation ", arr);



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