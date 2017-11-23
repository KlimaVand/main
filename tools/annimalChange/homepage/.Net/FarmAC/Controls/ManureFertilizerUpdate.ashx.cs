using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.UI.WebControls;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for ManureFertilizerUpdate
    /// </summary>
    public class ManureFertilizerUpdate : IHttpHandler
    {

        public void ProcessRequest(HttpContext context)
        {
        }
        public void ProcessRequest(HttpContext context, List<List<DropDownList>> AllMonth, List<List<DropDownList>> AllSelManure_, List<List<DropDownList>> AllSelDelivery_, List<List<TextBox>> AllDeliveryAmount)
        {
            string aFarmNumber, aScenario, aNPercent, X;
            int d = 0;
            string aLoss, aRotationID, dictFieldPlanRotation;
            string aUtilDegree;
            double amount;

            string monthvalue, aArea;
            DataTable arrFieldPlanRotation;
            aFarmNumber = context.Session["farmNumber"].ToString();
            aScenario = context.Session["scenarioNumber"].ToString();
            bool del = false;


            context.Response.Write(context.Request.Form);
            context.Response.Write("<br/><br/>");
            dbInterface database = new dbInterface();
            database.InitFarmNDB();
            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = context.Session["farmNumber"].ToString(); ;
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenario";
            data.value = context.Session["scenarioNumber"].ToString();
            paramter.Add(data);
            //nFieldPlanRotation = 
            arrFieldPlanRotation = database.GetValuesFromDB("GetFieldplanRotationsForManure", paramter);

            List<dbInterface.data> arrParameter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = aFarmNumber;
            arrParameter.Add(data);


            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = aScenario;
            arrParameter.Add(data);


            database.GetValuesFromDB("DeleteManureFertilizerDelivery", arrParameter);

            for (d = 0; d < AllMonth.Count(); d++)
            {

                for (int m = 0; m < AllMonth[d].Count(); m++)
                {

                    amount = double.Parse(AllDeliveryAmount[d][m].Text.ToString());


                    monthvalue = AllMonth[d][m].SelectedItem.Attributes["dbValue"];


                    arrParameter = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.name = "@arrManureList";

                    data.value = AllSelManure_[d][m].SelectedItem.Attributes["dbValue"];
                    arrParameter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@arrDeliveryList";
                    data.value = AllSelDelivery_[d][m].SelectedItem.Attributes["dbValue"];
                    arrParameter.Add(data);

                    aLoss = "0";//'CallSPReturn("GetLoss", arrParameter)
                    //	arrParameterUtil[0] = arrManureList[m];
                    aUtilDegree = "0";//'CallSPReturn("GetManureUtilizationDegree", arrParameterUtil)

                    List<dbInterface.data> arrParameterInsert = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.name = "@aFarmNumber";
                    data.value = aFarmNumber;
                    arrParameterInsert.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aScenarioID";
                    data.value = aScenario;
                    arrParameterInsert.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aRotationID";
                    data.value = arrFieldPlanRotation.Rows[d]["RotationID"].ToString();
                    arrParameterInsert.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aFieldPlanRotationID";
                    data.value = arrFieldPlanRotation.Rows[d]["FieldPlanRotationID"].ToString();
                    arrParameterInsert.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aStorageID";
                    data.value = AllSelManure_[d][m].SelectedItem.Attributes["dbValue"];
                    arrParameterInsert.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aDeliveryID";
                    data.value = AllSelDelivery_[d][m].SelectedItem.Attributes["dbValue"];
                    arrParameterInsert.Add(data);

                    if (amount >= 0)
                    {
                        data = new dbInterface.data();
                        data.name = "@aKg_N_Delivered";
                        data.value = amount.ToString(); ;
                        arrParameterInsert.Add(data);

                        data = new dbInterface.data();
                        data.name = "@aKg_N_Loss";
                        data.value = "0";
                        arrParameterInsert.Add(data);

                        data = new dbInterface.data();
                        data.name = "@aKg_N_Utilized";
                        data.value = "0";
                        arrParameterInsert.Add(data);//'cstr(Cdbl(arrAmountList(m)*aUtilDegree/100))'Cdbl(arrHGdistribute(a,dictSalesOrderList("N_UtilizationDegree"))*arrResult(a,b))/100''Decide how this is going to be calculated
                        data = new dbInterface.data();
                        data.name = "@aMonth";
                        data.value = monthvalue;
                        arrParameterInsert.Add(data);//'Cdbl(arrHGdistribute(a,dictSalesOrderList("N_UtilizationDegree"))*arrResult(a,b))/100''Decide how this is going to be calculated

                       data = new dbInterface.data();
                       data.name = "@aMFID";
                        data.value = (m).ToString();
                        arrParameterInsert.Add(data);

                        database.GetValuesFromDB("InsertUpdateManureFertilizerDelivery", arrParameterInsert);
                        context.Response.Write("<br/>m: " + m);
                    }


                }

            }
            database.CloseFarmNDB();

            //context.Response.Redirect("data.asp?TB="+context.Session["tabNumber"]+"");
            //context.Response.End();

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