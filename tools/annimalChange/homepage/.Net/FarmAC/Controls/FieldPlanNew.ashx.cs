using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for FieldPlanNew
    /// </summary>
    public class FieldPlanNew : IHttpHandler
    {
        public void ProcessRequest(HttpContext context)
        {

        }
        public void ProcessRequest(string yieldLevel, string irrigation, string aScenarioID, string aYieldLevel, string aIrrigation, string aSoilTypeID, string soilTypeSession, string aFarmNumber, string changes, List<string> aFieldPlanIDList, List<string> aCropList, string totalArea, string rotationID, string SoilType, string Irrigation, string YieldLevel, string RotationName, int nFieldPlan, List<String> rStartDate, List<String> rEndDate, List<String> aFeedstuffList, List<String> aStrawFeedStuffList, Panel Panel1)
        {



            DataTable arrA;
            string nCrop, arrCrop, CropString, NumberCrops, test;
            string nYield, arrYield;
            string ObjectCoeff;
            string CropArea, i, j, k;//  ' array of crop areas NUMBERCROPS
            string arrCropAndArea, arrPreCrop;
            string arrRotationList, aFieldPlanRotationID;
            string c, aRotationID, aRotationName, aFieldPlanID, aCrop;
            double aArea;
            string nID, arrID;

            string nAreaSum, arrAreaSum;
            int aTotalArea;


            string aAfterCrop, aSecondCrop, aGrazingPart, aOrganicFertilizer;
            string aFeedStuff;
            string aStartDate, aEndDate;
            int rotationSpan;

            string currentLocale;

            //currentLocale=getLocale()
            //setLocale("da")




            aRotationID = rotationID;


            if (SoilType != soilTypeSession)
                aSoilTypeID = SoilType;

            if (Irrigation != irrigation)
                aIrrigation = Irrigation;

            if (YieldLevel != yieldLevel)
                aYieldLevel = YieldLevel;

            aRotationName = RotationName;





            aTotalArea = int.Parse(totalArea);



            dbInterface database = new dbInterface();
            database.InitFarmNDB();
            //update rotation record
            List<dbInterface.data> arrParameter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
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
            data.name = "@aRotationName";
            data.value = aRotationName;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aSoilType";
            data.value = aSoilTypeID;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aIrrigation";
            data.value = aIrrigation;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aYieldLevel";
            data.value = aYieldLevel;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aArea";
            data.value = aTotalArea.ToString();
            arrParameter.Add(data);
            database.GetValuesFromDB("InsertUpdateRotation", arrParameter);




            // update all field plan records



            if (rStartDate.ToString() == "")
                rotationSpan = 1;
            if (rEndDate.ToString() == "")
            {
                rotationSpan = 1;
            }
            else
            {

                int month = int.Parse(rStartDate[0].Split('/')[1]);
                int day = int.Parse(rStartDate[0].Split('/')[0]);
                int year = int.Parse(rStartDate[0].Split('/')[2]);
                DateTime start = new DateTime(year, month, day);
                DateTime rStartDateData = start;

                month = int.Parse(rEndDate[0].Split('/')[1]);
                day = int.Parse(rEndDate[0].Split('/')[0]);
                year = int.Parse(rEndDate[0].Split('/')[2]);
                DateTime end = new DateTime(year, month, day);
                DateTime rEndDateData = end;

                rotationSpan = (int)(rStartDateData - rEndDateData).Duration().TotalDays / 365;
            }
            if (rotationSpan <= 0)
                rotationSpan = 1;
            string aFeedstuff=null;
            string aStrawFeedStuff=null;
            if (changes != "")
            {
                for (int f = 0; f < aFieldPlanIDList.Count; f++)
                {
                    aFieldPlanID = aFieldPlanIDList[f];
                    aCrop = aCropList[f];
                    aArea = aTotalArea / rotationSpan;//Round(Cdbl(,5))'''Request.Form("inputArea_"+aRotationID+"_"+f+"")
                    aAfterCrop = "0";//Request.Form("selectAfterCrop_"+aRotationID+"_"+f+"")
                    aFeedstuff = aFeedstuffList[f];
                    aStrawFeedStuff = aStrawFeedStuffList[f];

                    aStartDate = rStartDate[f];
                    if (aStartDate != "")
                    {
                        aStartDate = (Mid(aStartDate, 7, 4) + "-" + Mid(aStartDate, 4, 2) + "-" + Mid(aStartDate, 1, 2));
                    }
                    else
                        aStartDate = null;

                 
                    aEndDate = rEndDate[f];
                    
                  //  file.WriteLine((Mid(aEndDate, 7, 4) + "-" + Mid(aEndDate, 4, 2) + "-" + Mid(aEndDate, 1, 2)));
                   
                    if (aEndDate != "")
                    {
                        aEndDate = (Mid(aEndDate, 7, 4) + "-" + Mid(aEndDate, 4, 2) + "-" + Mid(aEndDate, 1, 2));
                    }
                    else
                    {
                        aEndDate = null;
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
                    data.value = aRotationID;//;
                    arrParameter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aFieldPlanID";
                    data.value = aFieldPlanID;
                    arrParameter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aCrop";
                    data.value = aCrop;
                    arrParameter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aArea";
                    data.value = aArea.ToString();
                    arrParameter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aAfterCrop";
                    data.value = aAfterCrop;
                    arrParameter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aFeedStuffID";
                    data.value = aFeedstuff.ToString();
                    arrParameter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aStrawFeedStuffID";
                    data.value = aStrawFeedStuff;
                    arrParameter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aStartDate";
                    data.value = aStartDate;
                    arrParameter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aEndDate";
                    data.value = aEndDate;
                    arrParameter.Add(data);
      
                    database.GetValuesFromDB("InsertUpdateFieldPlan", arrParameter);

                }
            }
      
            // insert new field plan record

            string dictID, dictA;

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
            // nID 
            arrA = database.GetValuesFromDB("GetFirstUnusedCrop ", arrParameter);


            aFieldPlanID = (nFieldPlan + 1).ToString();//'arrID(0,dictID.item("MaxID"))+1
            aCrop = arrA.Rows[0][("CropID")].ToString();
            aArea = aTotalArea;

            aAfterCrop = "0";
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
            data.value = aFieldPlanID;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aCrop";
            data.value = aCrop;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aArea";
            data.value = aArea.ToString();
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

            aEndDate = rEndDate[rEndDate.Count - 1];
            int months = int.Parse(aEndDate.Split('/')[1]);
            int days = int.Parse(aEndDate.Split('/')[0]);
            int years = int.Parse(aEndDate.Split('/')[2]);
            DateTime endTime = new DateTime(years, months, days, 0, 0, 0);
            endTime = endTime.AddDays(1);

            //string aEndDate;

            aEndDate = endTime.Year.ToString();
            if (endTime.Month < 10)
                aEndDate += "-0" + endTime.Month.ToString();
            else
                aEndDate += "-" + endTime.Month.ToString();
            if (endTime.Day < 10)
                aEndDate += "-0" + endTime.Day.ToString();
            else
                aEndDate += "-" + endTime.Day.ToString();

       
            data = new dbInterface.data();
            data.name = "@aStartDate";
            data.value = aEndDate;
            arrParameter.Add(data);

            string startTime = rStartDate[0];
            months = int.Parse(startTime.Split('/')[1]);
            days = int.Parse(startTime.Split('/')[0]);
            years = int.Parse(startTime.Split('/')[2]);
            DateTime startT = new DateTime(years, months, days, 0, 0, 0);
            startT = startT.AddDays(-1);
            if (startT.Day == 29 && startT.Month == 2)
            {
                startT = startT.AddDays(-1);
            }
            while (startT < endTime)
            {
                startT = startT.AddYears(1);
            }


            string aStartTime ;
            
            aStartTime = startT.Year.ToString();
            if (startT.Month < 10)
                aStartTime += "-0" + startT.Month.ToString();
            else
                aStartTime += "-"+startT.Month.ToString();
            if (startT.Day < 10)
                aStartTime += "-0" + startT.Day.ToString();
            else
                aStartTime += "-"+startT.Day.ToString();
            data = new dbInterface.data();
            data.name = "@aEndDate";
            data.value = aStartTime;
            arrParameter.Add(data);

  
            database.GetValuesFromDB("InsertUpdateFieldPlan", arrParameter);



            aArea = double.Parse(totalArea) / (1 + aFieldPlanIDList.Count);
            for (int f = 0; f < aFieldPlanIDList.Count; f++)
            {
                aFieldPlanID = aFieldPlanIDList[f];
                aCrop = aCropList[f];
                
                aAfterCrop = "0";
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
                data.value = aRotationID;//;
                arrParameter.Add(data);
                data = new dbInterface.data();
                data.name = "@aFieldPlanRotationID";
                data.value = aFieldPlanID;
                arrParameter.Add(data);



                data = new dbInterface.data();
                data.name = "@aCrop";
                data.value = aCrop;
                arrParameter.Add(data);


                data = new dbInterface.data();
                data.name = "@aCrop_AfterCrop";
                data.value = aAfterCrop;
                arrParameter.Add(data);


                data = new dbInterface.data();
                data.name = "@aArea";
                data.value = aArea.ToString(); ;
                arrParameter.Add(data);




                aSecondCrop = aCropList[f];

                data = new dbInterface.data();
                data.name = "@aSecondCropID";
                data.value = aSecondCrop;
                arrParameter.Add(data);





                database.GetValuesFromDB("InsertUpdateFieldPlanRotationMinimum ", arrParameter);

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
            data.value = aRotationID.ToString();
            arrParameter.Add(data);

            aFieldPlanID = (nFieldPlan + 1).ToString();
            aCrop = arrA.Rows[0]["CropID"].ToString();

            aSecondCrop = aCropList[0];
            aAfterCrop = "0";
            data = new dbInterface.data();
            data.name = "@aFieldPlanRotationID";
            data.value = aFieldPlanID;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aPreviousCrop";
            data.value = "2";
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aPreCropOriginalID";
            data.value = "1";
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aPreCrop_AfterCrop";
            data.value = "1";
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aPreCropSecondCrop";
            data.value = "0";
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aCrop";
            data.value = aCrop;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aCrop_AfterCrop";
            data.value = aAfterCrop;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aArea";
            data.value = aArea.ToString();
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aOrganicFertilizer";
            data.value = "0";
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aGrazingPart";
            data.value = "0";
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aSalePart";
            data.value = "13";
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aStrawUseType";
            data.value = "3";
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aSecondCropID";
            data.value = "0";
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aPrePreCropID";
            data.value = "0";
            arrParameter.Add(data);
            DataTable returnValue = database.GetValuesFromDB("InsertUpdateFieldPlanRotation ", arrParameter);
            //context.Response.Write("returnValue "+returnValue);


            database.CloseFarmNDB();
            //context.Session["ShowResult"]=false;


        }

        public bool IsReusable
        {
            get
            {
                return false;
            }
        }
        public static string Mid(string s, int a, int b)
        {
            string temp = s.Substring(a - 1, b);
            return temp;
        }
    }
}