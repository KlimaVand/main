using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for RotationUpdate
    /// </summary>
    public class RotationUpdate : IHttpHandler
    {
        public void ProcessRequest(HttpContext context)
        {
        }
        public void ProcessRequest(HttpContext context, int aRotationID,List<TextBox> straw, List<TextBox> inputGrazingPart_, List<TextBox> mainYield, List<TextBox> OrganicFertilizer, List<DropDownList> aStrawUseType)
        {



            String c, aFarmNumber, aScenarioID, aSoilTypeID, aFieldPlanRotationID, aCropID, aArea, aOrganicFertilizer, aGrazingPart, aSalePart;
            DataTable returnValue;
            string aPreviousCropID, aPreCropOriginalID, aPreCropSecondCrop, aPreCrop_AfterCropID, aPrePreCropID;

            String nAreaSum, arrAreaSum, aSecondCrop;
            String aCrop_AfterCrop;
            String aMainYield, aCatchYield;


            aFarmNumber = HttpContext.Current.Session["farmNumber"].ToString();
            aScenarioID = HttpContext.Current.Session["scenarioNumber"].ToString();


            dbInterface database = new dbInterface();
            database.InitFarmNDB();
            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = (string)HttpContext.Current.Session["farmNumber"];
            paramter.Add(data);
            data.name = "@aScenarioID";
            data.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            paramter.Add(data);
            data.name = "@aRotationID";
            data.value = aRotationID.ToString();
            paramter.Add(data);
            DataTable arrFieldPlanRotation = database.GetValuesFromDB("GetFieldPlanRotationList", paramter);
            for (int f = 0; f < arrFieldPlanRotation.Rows.Count; f++)
            {
                aFieldPlanRotationID = arrFieldPlanRotation.Rows[f]["FieldPlanRotationID"].ToString();
                aPreviousCropID = "2";//'Request.Form("hiddenPreviousCropID_"+aRotationID+"_"+f+"")
                aPreCropOriginalID = "1";//'Request.Form("hiddenPreCropOriginalID_"+aRotationID+"_"+f+"")
                aPreCropSecondCrop = "0";//'Request.Form("PreCropSecondCrop_"+aRotationID+"_"+f+"")
                aPreCrop_AfterCropID = "1";//'Request.Form("hiddenPreCrop_AfterCropID_"+aRotationID+"_"+f+"")
                aPrePreCropID = "0";//'Request.Form("hiddenPrePreCropID_"+aRotationID+"_"+f+"")

                aCropID = arrFieldPlanRotation.Rows[f]["Crop"].ToString();
                aCrop_AfterCrop = arrFieldPlanRotation.Rows[f]["Crop_AfterCrop"].ToString();
                aArea = arrFieldPlanRotation.Rows[f]["Area"].ToString();
                aOrganicFertilizer = OrganicFertilizer[f].Text;
                if (aOrganicFertilizer == "")
                    aOrganicFertilizer = "0";

                aGrazingPart = inputGrazingPart_[f].Text;
                aSalePart = "0";

                aSecondCrop = arrFieldPlanRotation.Rows[f]["SecondCropID"].ToString();
                aMainYield = mainYield[f].Text;
                aCatchYield = null;

                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = HttpContext.Current.Session["farmNumber"].ToString();
                paramter.Add(data);
                data = new dbInterface.data();
                data.name = "@aScenarioID";
                data.value = aScenarioID;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aRotationID";
                data.value = aRotationID.ToString();
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aFieldPlanRotationID";
                data.value = aFieldPlanRotationID;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aPreviousCrop";
                data.value = aPreviousCropID;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aPreCropOriginalID";
                data.value = aPreCropOriginalID;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aPreCrop_AfterCrop";
                data.value = aPreCrop_AfterCropID;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aPreCropSecondCrop";
                data.value = aPreCropSecondCrop;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aCrop";
                data.value = aCropID;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aCrop_AfterCrop";
                data.value = aCrop_AfterCrop;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aArea";
                data.value = aArea;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aOrganicFertilizer";
                data.value = aOrganicFertilizer;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aGrazingPart";
                data.value = aGrazingPart;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aSalePart";
                data.value = aSalePart;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aStrawUseType";
                if(aStrawUseType[f].SelectedIndex!=-1)
                    data.value = aStrawUseType[f].SelectedItem.Attributes["dbValue"].ToString();
                else
                    data.value = "0";
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aSecondCropID";
                data.value = aSecondCrop;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aPrePreCropID";
                data.value = aPrePreCropID;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aMainYield";
                data.value = aMainYield;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aCatchYield";
                data.value = aCatchYield;
                paramter.Add(data);

                data = new dbInterface.data();
                data.name = "@aStrawYield";
                data.value = straw[f].Text;
                paramter.Add(data);

                returnValue = database.GetValuesFromDB("InsertUpdateFieldPlanRotation ", paramter);



            }



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