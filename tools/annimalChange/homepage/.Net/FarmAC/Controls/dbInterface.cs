using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Data.SqlClient;
using System.IO;
using System.Linq;
using System.Text;
using System.Web;
using System.Xml;
using System.Xml.Linq;


namespace FarmAC.Controls
{
    public class dbInterface
    {


        public void InitFarmNDB()
        {


        }

        public void CloseFarmNDB()
        {

        }
        public string GetConnectionString(string name)
        {
            // Assume the name won't be found.
            string returnValue = null;

            ConnectionStringSettings settings = ConfigurationManager.ConnectionStrings[name];
            if (settings != null)
            {
                returnValue = settings.ConnectionString;
            }
            return returnValue;
        }
        public struct data
        {
            public void SetValue(string aValue)
            {
                value = aValue;
            }
            public string name;
            public string value;
        }
        public string GenerateInputXML(long FarmNumber, int ScenarioID)
        {
            string con = GetConnectionString("FarmAC");
            SqlConnection sqlconn = null;
            try
            {
                sqlconn = new SqlConnection(con);
                sqlconn.Open();

                SqlCommand cmd = new SqlCommand("GetFarmData_model", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;


                cmd.Parameters.Add("@aFarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                SqlDataAdapter adapter = new SqlDataAdapter(cmd);
                DataSet farmData = new DataSet();
                adapter.Fill(farmData, "Farm");


                cmd = new SqlCommand("GetScenarioData_model", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@FarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@ScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter1 = new SqlDataAdapter(cmd);
                adapter1.Fill(farmData, "SelectedScenario");
                farmData.Relations.Add(farmData.Tables["Farm"].Columns["FarmNumber"], farmData.Tables["SelectedScenario"].Columns["FarmNumber"]);
                farmData.Relations[0].Nested = true;

                cmd = new SqlCommand("GetInventorySystem_model", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@FarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@ScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter1_1 = new SqlDataAdapter(cmd);
                adapter1_1.Fill(farmData, "InventorySystem");
                farmData.Relations.Add(farmData.Tables["SelectedScenario"].Columns["IS-ID"], farmData.Tables["InventorySystem"].Columns["Value"]);
                farmData.Relations[1].Nested = true;


                cmd = new SqlCommand("GetRotationList_model", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@aFarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@aScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter2 = new SqlDataAdapter(cmd);
                adapter2.Fill(farmData, "Rotation");
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["SelectedScenario"].Columns["FarmNumber"] }, new DataColumn[] { farmData.Tables["Rotation"].Columns["FarmNumber"] });
                farmData.Relations[2].Nested = true;

                cmd = new SqlCommand("GetCropYieldList_model", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@aFarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@aScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter3 = new SqlDataAdapter(cmd);
                adapter3.Fill(farmData, "Crop");
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["Rotation"].Columns["Identity"] }, new DataColumn[] { farmData.Tables["Crop"].Columns["RotationID"] });
                farmData.Relations[3].Nested = true;

                cmd = new SqlCommand("GetManureApplied", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@aFarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@aScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter4 = new SqlDataAdapter(cmd);
                adapter4.Fill(farmData, "Manure_applied");
                //farmData.Tables["manure_applied"].Columns.Add("Identity", new int(), farmData.Tables["manure_applied"].Rows.GetEnumerator);
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["Crop"].Columns["RotationID"], farmData.Tables["Crop"].Columns["Identity"] }, new DataColumn[] { farmData.Tables["manure_applied"].Columns["RotationID"], farmData.Tables["manure_applied"].Columns["FieldPlanRotationID"] });
                farmData.Relations[4].Nested = true;

                cmd = new SqlCommand("GetFertilizerApplied", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@aFarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@aScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter5 = new SqlDataAdapter(cmd);
                adapter5.Fill(farmData, "Fertilizer_applied");
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["Crop"].Columns["RotationID"], farmData.Tables["Crop"].Columns["Identity"] }, new DataColumn[] { farmData.Tables["fertilizer_applied"].Columns["RotationID"], farmData.Tables["fertilizer_applied"].Columns["FieldPlanRotationID"] });
                farmData.Relations[5].Nested = true;

                System.Data.DataTable dataTable = farmData.Tables["Manure_applied"];
                var dataRows = dataTable.AsEnumerable();
                foreach (var row in dataRows)
                {
                    var cellList = row.ItemArray.ToList();
                    row.ItemArray = cellList.Select(x => x.ToString().Trim()).ToArray();
                }

                dataTable = farmData.Tables["Fertilizer_applied"];
                dataRows = dataTable.AsEnumerable();
                foreach (var row in dataRows)
                {
                    var cellList = row.ItemArray.ToList();
                    row.ItemArray = cellList.Select(x => x.ToString().Trim()).ToArray();
                }

                cmd = new SqlCommand("GetLivestockRuminants", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@FarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@ScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter6 = new SqlDataAdapter(cmd);
                adapter6.Fill(farmData, "Livestock");
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["SelectedScenario"].Columns["Identity"] }, new DataColumn[] { farmData.Tables["livestock"].Columns["ScenarioID"] });
                farmData.Relations[6].Nested = true;

                cmd = new SqlCommand("GetLivestockNonRuminants", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@FarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@ScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter7 = new SqlDataAdapter(cmd);
                adapter7.Fill(farmData, "Livestock");

                cmd = new SqlCommand("GetRuminantFeedplanList", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@FarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@ScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter8 = new SqlDataAdapter(cmd);
                adapter8.Fill(farmData, "itemFed");
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["livestock"].Columns["HerdID"], farmData.Tables["livestock"].Columns["LivestockType"], farmData.Tables["livestock"].Columns["Species_group"] }, new DataColumn[] { farmData.Tables["itemFed"].Columns["HerdID"], farmData.Tables["itemFed"].Columns["LivestockType"], farmData.Tables["itemFed"].Columns["Species_group"] });
                farmData.Relations[7].Nested = true;

                cmd = new SqlCommand("GetNonRuminantFeedplanList", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@FarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@ScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter9 = new SqlDataAdapter(cmd);
                adapter9.Fill(farmData, "itemFed");

                dataTable = farmData.Tables["itemFed"];
                dataRows = dataTable.AsEnumerable();
                foreach (var row in dataRows)
                {
                    var cellList = row.ItemArray.ToList();
                    row.ItemArray = cellList.Select(x => x.ToString().Trim()).ToArray();
                }

                cmd = new SqlCommand("GetHousingList", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@FarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@ScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter10 = new SqlDataAdapter(cmd);
                adapter10.Fill(farmData, "Housing");
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["livestock"].Columns["HerdID"], farmData.Tables["livestock"].Columns["LivestockType"], farmData.Tables["livestock"].Columns["Species_group"] }, new DataColumn[] { farmData.Tables["Housing"].Columns["HerdID"], farmData.Tables["Housing"].Columns["LivestockType"], farmData.Tables["Housing"].Columns["Species_group"] });
                farmData.Relations[8].Nested = true;

                dataTable = farmData.Tables["Housing"];
                dataRows = dataTable.AsEnumerable();
                foreach (var row in dataRows)
                {
                    var cellList = row.ItemArray.ToList();
                    row.ItemArray = cellList.Select(x => x.ToString().Trim()).ToArray();
                }

                cmd = new SqlCommand("GetHarvestMethod", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@aFarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@aScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter11 = new SqlDataAdapter(cmd);
                adapter11.Fill(farmData, "HarvestMethod");
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["crop"].Columns["Identity"], farmData.Tables["crop"].Columns["RotationID"] }, new DataColumn[] { farmData.Tables["HarvestMethod"].Columns["Identity"], farmData.Tables["HarvestMethod"].Columns["RotationID"] });
                farmData.Relations[9].Nested = true;

                //cmd = new SqlCommand("GetProportionGrazed", sqlconn);
                //cmd.CommandType = CommandType.StoredProcedure;

                //cmd.Parameters.Add("@aFarmNumber", SqlDbType.BigInt);
                //cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                //cmd.Parameters.Add("@aScenarioID", SqlDbType.BigInt);
                //cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                //SqlDataAdapter adapter12 = new SqlDataAdapter(cmd);
                //adapter12.Fill(farmData, "Proportion_grazed");
                //farmData.Relations.Add(new DataColumn[] { farmData.Tables["crop"].Columns["Identity"], farmData.Tables["crop"].Columns["RotationID"] }, new DataColumn[] { farmData.Tables["Proportion_grazed"].Columns["FieldPlanRotationID"], farmData.Tables["Proportion_grazed"].Columns["RotationID"] });
                //farmData.Relations[10].Nested = true;

                cmd = new SqlCommand("GetApplicationTechniqueM", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@FarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@ScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter13 = new SqlDataAdapter(cmd);
                adapter13.Fill(farmData, "Applic_technique_Manure");
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["Manure_applied"].Columns["DeliveryID"], farmData.Tables["Manure_applied"].Columns["RotationID"], farmData.Tables["Manure_applied"].Columns["FieldPlanRotationID"], farmData.Tables["Manure_applied"].Columns["StorageType"], farmData.Tables["Manure_applied"].Columns["MFId"] }, new DataColumn[] { farmData.Tables["Applic_technique_Manure"].Columns["NameI"], farmData.Tables["Applic_technique_Manure"].Columns["RotationID"], farmData.Tables["Applic_technique_Manure"].Columns["FieldPlanRotationID"], farmData.Tables["Applic_technique_Manure"].Columns["StorageID"], farmData.Tables["Applic_technique_Manure"].Columns["MFId"] });
                farmData.Relations[10].Nested = true;

                cmd = new SqlCommand("GetApplicationTechniqueF", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@FarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@ScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter14 = new SqlDataAdapter(cmd);
                adapter14.Fill(farmData, "Applic_technique_Fertilizer");
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["Fertilizer_applied"].Columns["DeliveryID"], farmData.Tables["Fertilizer_applied"].Columns["RotationID"], farmData.Tables["Fertilizer_applied"].Columns["FieldPlanRotationID"], farmData.Tables["Fertilizer_applied"].Columns["StorageID"], farmData.Tables["Fertilizer_applied"].Columns["MFId"] }, new DataColumn[] { farmData.Tables["Applic_technique_Fertilizer"].Columns["NameI"], farmData.Tables["Applic_technique_Fertilizer"].Columns["RotationID"], farmData.Tables["Applic_technique_Fertilizer"].Columns["FieldPlanRotationID"], farmData.Tables["Applic_technique_Fertilizer"].Columns["StorageID"], farmData.Tables["Applic_technique_Fertilizer"].Columns["MFId"] });
                farmData.Relations[11].Nested = true;

                cmd = new SqlCommand("GetProductList", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@aFarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@aScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter15 = new SqlDataAdapter(cmd);
                adapter15.Fill(farmData, "Product");
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["Crop"].Columns["Identity"], farmData.Tables["Crop"].Columns["RotationID"] }, new DataColumn[] { farmData.Tables["Product"].Columns["FieldPlanRotationID"], farmData.Tables["Product"].Columns["RotationID"] });
                farmData.Relations[12].Nested = true;

                dataTable = farmData.Tables["Product"];
                dataRows = dataTable.AsEnumerable();
                foreach (var row in dataRows)
                {
                    var cellList = row.ItemArray.ToList();
                    row.ItemArray = cellList.Select(x => x.ToString().Trim()).ToArray();
                }

                cmd = new SqlCommand("GetExpectedYield", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@aFarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@aScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter16 = new SqlDataAdapter(cmd);
                adapter16.Fill(farmData, "Expected_yield");
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["Product"].Columns["FieldPlanRotationID"], farmData.Tables["Product"].Columns["RotationID"], farmData.Tables["Product"].Columns["FeedCode"] }, new DataColumn[] { farmData.Tables["Expected_yield"].Columns["FieldPlanRotationID"], farmData.Tables["Expected_yield"].Columns["RotationID"], farmData.Tables["Expected_yield"].Columns["FeedstuffID"] });
                farmData.Relations[13].Nested = true;

                cmd = new SqlCommand("GetManureRecipient", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@FarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@ScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter17 = new SqlDataAdapter(cmd);
                adapter17.Fill(farmData, "ManureRecipient");
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["Housing"].Columns["HousingType"], farmData.Tables["Housing"].Columns["HerdID"], farmData.Tables["Housing"].Columns["Species_group"] }, new DataColumn[] { farmData.Tables["ManureRecipient"].Columns["Stable"], farmData.Tables["ManureRecipient"].Columns["HerdID"], farmData.Tables["ManureRecipient"].Columns["Species_group"] });
                farmData.Relations[14].Nested = true;

                dataTable = farmData.Tables["ManureRecipient"];
                dataRows = dataTable.AsEnumerable();
                foreach (var row in dataRows)
                {
                    var cellList = row.ItemArray.ToList();
                    row.ItemArray = cellList.Select(x => x.ToString().Trim()).ToArray();
                }


                cmd = new SqlCommand("GetEnergySystem", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;


                SqlDataAdapter adapter18 = new SqlDataAdapter(cmd);
                adapter18.Fill(farmData, "EnergySystem");
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["SelectedScenario"].Columns["ES-ID"] }, new DataColumn[] { farmData.Tables["EnergySystem"].Columns["Value"] });
                farmData.Relations[15].Nested = true;

                cmd = new SqlCommand("GetDeposition_model", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@aFarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;

                SqlDataAdapter adapter19 = new SqlDataAdapter(cmd);
                adapter19.Fill(farmData, "NDepositionRate");

                farmData.Relations.Add(new DataColumn[] { farmData.Tables["Farm"].Columns["Identity"] }, new DataColumn[] { farmData.Tables["NdepositionRate"].Columns["FarmNumber"] });
                farmData.Relations[16].Nested = true;

                cmd = new SqlCommand("GetPotentialYield", sqlconn);
                cmd.CommandType = CommandType.StoredProcedure;

                cmd.Parameters.Add("@aFarmNumber", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = FarmNumber;
                cmd.Parameters.Add("@aScenarioID", SqlDbType.BigInt);
                cmd.Parameters[cmd.Parameters.Count - 1].Value = ScenarioID;

                SqlDataAdapter adapter20 = new SqlDataAdapter(cmd);
                adapter20.Fill(farmData, "Potential_yield");
                farmData.Relations.Add(new DataColumn[] { farmData.Tables["Product"].Columns["FieldPlanRotationID"], farmData.Tables["Product"].Columns["RotationID"], farmData.Tables["Product"].Columns["FeedCode"] }, new DataColumn[] { farmData.Tables["Potential_yield"].Columns["FieldPlanRotationID"], farmData.Tables["Potential_yield"].Columns["RotationID"], farmData.Tables["Potential_yield"].Columns["FeedstuffID"] });
                farmData.Relations[17].Nested = true;
                //printobj(farmData, "c:\\upload\\AnimalChange\\farm_" + FarmNumber + "_" + ScenarioID + ".xml");
                //print(farmData, "c:\\upload\\AnimalChange\\farm_" + FarmNumber + "_" + ScenarioID + ".xml");
                string path = GetConnectionString("FarmFiles");
                path += "farm_" + FarmNumber + "_" + ScenarioID + ".xml";
                farmData.WriteXml(path);

            }
                catch(Exception e)
            {
                string tmp = e.Message;
                return tmp;
            }
            finally
            {
                if (sqlconn != null)
                { sqlconn.Close(); }
            }
            return "";

        }
        public string GetLanguageString(string str, string language)
        {
            dbInterface database = new dbInterface();
            List<dbInterface.data> languageSettings = new List<dbInterface.data>();
            dbInterface.data item = new dbInterface.data();
            item.name = "@language";
            item.value = language;// "uk";
            languageSettings.Add(item);
            item = new dbInterface.data();
            item.name = "@keyword";
            item.value = str;
            languageSettings.Add(item);
            return database.GetValuesFromDB("GetLanguageString", languageSettings).Rows[0][0].ToString();

        }
        public DataTable GetValuesFromDB(string command, List<data> parameterList)
        {
            string con = GetConnectionString("FarmAC");
            DataTable tbl = new DataTable();
            SqlConnection conSQL = null;
            try
            {
                using (conSQL = new SqlConnection(con))
                {
                    conSQL.Open();
                    SqlCommand cmdGet = null;



                    using (cmdGet = new SqlCommand(command, conSQL))
                    {
                        foreach (data tmp in parameterList)
                        {
                            cmdGet.CommandType = CommandType.StoredProcedure;
                            cmdGet.Parameters.AddWithValue(tmp.name, tmp.value);

                        }
                        using (SqlDataAdapter da = new SqlDataAdapter(cmdGet))
                        {
                            da.Fill(tbl);

                        }
                        conSQL.Close();
                    }
                }
            }
            catch (Exception ex)
            {
                dbInterface datbase = new dbInterface();
                string dil = datbase.GetConnectionString("FarmFiles");
                System.IO.StreamWriter file = new System.IO.StreamWriter(dil + "errorDB"+HttpContext.Current.Session["farmNumber"].ToString()+"_"+HttpContext.Current.Session["scenarioNumber"].ToString()+".txt");
                file.WriteLine(ex.Message);
                file.WriteLine(command);
                foreach (data tmp in parameterList)
                {
                    file.WriteLine(tmp.name +" "+ tmp.value);
                }

                file.Close();
            }

            return tbl;
        }
        public void saveResults()
        {
            DataTable arrFieldPlanRotation;
string  nFieldPlanRotation, dictFieldPlanRotation;
string CropSequence, yieldcoll,  elem, mainyield, secondyield, count, returnValue;
string FedstuffProductItemU,FedstuffProductItemP,FedstuffProductItemB, outputStrf, itmf, itm,it;
            List<data> parameter=new List<data>();
data datainput =new data();
datainput.name = "aFarmNumber";
            datainput.value=HttpContext.Current.Session["farmNumber"].ToString();
            parameter.Add(datainput);
            datainput =new data();
            datainput.name = "aScenarioID";
            datainput.value=HttpContext.Current.Session["scenarioNumber"].ToString();
            parameter.Add(datainput);

//nFieldPlanRotation = CallSP("GetFieldPlanRotationsForExpectedYield "&Session("FarmNumber")&","&Session("scenarioNumber"),arrFieldPlanRotation,dictFieldPlanRotation)
           arrFieldPlanRotation= GetValuesFromDB("GetFieldPlanRotationsForExpectedYield",parameter);

     GetValuesFromDB("DeleteManureTotal",parameter);
     GetValuesFromDB("DeleteFeedstuffProductBalance",parameter);//'sletter alt i FeedstuffProductBalance-tabellen

     string path = GetConnectionString("ModelOutput") + "outputFarm" + HttpContext.Current.Session["FarmNumber"] + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"] + ".xml";




  /*Set producedManure = objXMLDOM.getElementsByTagName("producedManure")
  Set exportedManure = objXMLDOM.getElementsByTagName("exportedManure")
  Set importedManure = objXMLDOM.getElementsByTagName("importedManure")
  Set ExpectedYield = objXMLDOM.getElementsByTagName("ExpectedYield")
  Set FedstuffProductItemU = objXMLDOM.getElementsByTagName("FeedAndProductsUsed")
  Set FedstuffProductItemP = objXMLDOM.getElementsByTagName("FeedAndProductsProduced")
  Set FedstuffProductItemB = objXMLDOM.getElementsByTagName("FeedAndProductTradeBalance")*/

XmlDocument doc = new XmlDocument();


doc.Load(path);
XmlNodeList producedManure = doc.GetElementsByTagName("producedManure");


            foreach (XmlNode man in producedManure)
            {
                if (man.ChildNodes.Count != 0)
                {
                    parameter = new List<data>();
                    datainput = new data();
                    datainput.name = "@aFarmNumber";
                    datainput.value = HttpContext.Current.Session["farmNumber"].ToString();
                    parameter.Add(datainput);
                    datainput = new data();
                    datainput.name = "@aScenarioID";
                    datainput.value = HttpContext.Current.Session["scenarioNumber"].ToString();
                    parameter.Add(datainput);


                    datainput = new data();
                    datainput.name = "@aStorageTypeID";
                    datainput.value = man.ChildNodes[0].ChildNodes[2].ChildNodes[3].InnerText;
                    parameter.Add(datainput);


                    datainput = new data();
                    datainput.name = "@aParameterID";
                    datainput.value = "1100";
                    parameter.Add(datainput);


                    double N = double.Parse(man.ChildNodes[0].ChildNodes[9].ChildNodes[3].InnerText);

                    N += double.Parse(man.ChildNodes[0].ChildNodes[10].ChildNodes[3].InnerText);
                    datainput = new data();
                    datainput.name = "@aParameterValue";
                    datainput.value = N.ToString();
                    parameter.Add(datainput);


                    datainput = new data();
                    datainput.name = "@aStorageName";
                    datainput.value = man.ChildNodes[0].ChildNodes[0].ChildNodes[3].InnerText;
                    parameter.Add(datainput);
                    datainput = new dbInterface.data();
                    datainput.name = "@aSpeciesGroup";
                    datainput.value = man.ChildNodes[0].ChildNodes[1].ChildNodes[3].InnerText;
                    parameter.Add(datainput);
                    GetValuesFromDB("InsertUpdateManureTotal ", parameter);
                }
        
}

XmlNodeList exportedManure = doc.GetElementsByTagName("exportedManure");


foreach (XmlNode man in exportedManure)
{
    if (man.InnerText != "")
    {
        parameter = new List<data>();
        datainput = new data();
        datainput.name = "@aFarmNumber";
        datainput.value = HttpContext.Current.Session["farmNumber"].ToString();
        parameter.Add(datainput);
        datainput = new data();
        datainput.name = "@aScenarioID";
        datainput.value = HttpContext.Current.Session["scenarioNumber"].ToString();
        parameter.Add(datainput);


        datainput = new data();
        datainput.name = "@aStorageTypeID";
        datainput.value = exportedManure.Item(0).ChildNodes[0].ChildNodes[1].ChildNodes[3].InnerText;
        parameter.Add(datainput);


        datainput = new data();
        datainput.name = "@aParameterID";
        datainput.value = "1101";
        parameter.Add(datainput);


        double N = double.Parse(man.ChildNodes[0].ChildNodes[9].ChildNodes[3].InnerText);
        N += double.Parse(man.ChildNodes[0].ChildNodes[10].ChildNodes[3].InnerText);

        datainput = new data();
        datainput.name = "@aParameterValue";
        datainput.value = N.ToString();
        parameter.Add(datainput);


        datainput = new data();
        datainput.name = "@aStorageName";
        datainput.value = man.ChildNodes[0].ChildNodes[0].ChildNodes[3].InnerText;
        parameter.Add(datainput);

        datainput = new dbInterface.data();
        datainput.name = "@aSpeciesGroup";
        datainput.value = man.ChildNodes[0].ChildNodes[1].ChildNodes[3].InnerText;
        parameter.Add(datainput);
        GetValuesFromDB("InsertUpdateManureTotal ", parameter);
    }
}
XmlNodeList importedManure = doc.GetElementsByTagName("importedManure");

foreach (XmlNode man in importedManure)
{
    if (man.ChildNodes.Count != 0)
    {
        parameter = new List<data>();
        datainput = new data();
        datainput.name = "@aFarmNumber";
        datainput.value = HttpContext.Current.Session["farmNumber"].ToString();
        parameter.Add(datainput);
        datainput = new data();
        datainput.name = "@aScenarioID";
        datainput.value = HttpContext.Current.Session["scenarioNumber"].ToString();
        parameter.Add(datainput);


        datainput = new data();
        datainput.name = "@aStorageTypeID";
        datainput.value = man.ChildNodes[0].ChildNodes[1].ChildNodes[3].InnerText;
        parameter.Add(datainput);


        datainput = new data();
        datainput.name = "@aParameterID";
        datainput.value = "1102";
        parameter.Add(datainput);


        double N = double.Parse(man.ChildNodes[0].ChildNodes[9].ChildNodes[3].InnerText);

        N += double.Parse(man.ChildNodes[0].ChildNodes[10].ChildNodes[3].InnerText);
        datainput = new data();
        datainput.name = "@aParameterValue";
        datainput.value = N.ToString();
        parameter.Add(datainput);


        datainput = new data();
        datainput.name = "@aStorageName";
        datainput.value = man.ChildNodes[0].ChildNodes[0].ChildNodes[3].InnerText;
        parameter.Add(datainput);

        datainput = new dbInterface.data();
        datainput.name = "@aSpeciesGroup";
        datainput.value = man.ChildNodes[0].ChildNodes[1].ChildNodes[3].InnerText;
        parameter.Add(datainput);
        GetValuesFromDB("InsertUpdateManureTotal ", parameter);
    }
        
}
XmlNodeList ExpectedYield = doc.GetElementsByTagName("importedManure");
int i = 0;
if (importedManure.Item(0).ChildNodes.Count > 0)
{
    XmlNodeList elems = ExpectedYield.Item(0).ChildNodes[0].ChildNodes[0].ChildNodes;
foreach(XmlNode product in elems)
{

   string  stff=product.ChildNodes[ product.ChildNodes.Count-1].Name;
   if (product.ChildNodes[product.ChildNodes.Count - 1].Name == "product")
   {
       if (product.ChildNodes[product.ChildNodes.Count - 1].PreviousSibling.Name == "product")
       {
           mainyield = product.ChildNodes[product.ChildNodes.Count - 1].PreviousSibling.Value;
           secondyield = product.ChildNodes[product.ChildNodes.Count - 1].Value;
           if (i > 0)
               i = i + 1;
       }
       else
       {
           mainyield = product.ChildNodes[product.ChildNodes.Count - 1].Value;
           secondyield = "0";
           if (i > 0)
               i = i + 1;

       }
   }
   else
   {
       mainyield = "0";
       secondyield = "0";
       if (i > 0)
           i = i + 1;

   }
          parameter=new List<data>();
      datainput =new data();
      datainput.name = "@aFarmNumber";
            datainput.value=HttpContext.Current.Session["farmNumber"].ToString();
            parameter.Add(datainput);
                datainput =new data();
                datainput.name = "@aScenarioID";
            datainput.value=HttpContext.Current.Session["scenarioNumber"].ToString();
            parameter.Add(datainput);

                datainput =new data();
                datainput.name = "@aRotationID";
            datainput.value=arrFieldPlanRotation.Rows[i]["RotationID"].ToString();
            parameter.Add(datainput);
                    datainput =new data();
                    datainput.name = "@aFieldPlanRotationID";
            datainput.value=arrFieldPlanRotation.Rows[i]["FieldPlanRotationID"].ToString();
            parameter.Add(datainput);

                        datainput =new data();
                        datainput.name = "@aModelledMainYield";
            datainput.value=mainyield;
            parameter.Add(datainput);

                            datainput =new data();
                            datainput.name = "@aModelledSecondYield";
            datainput.value=secondyield;
            parameter.Add(datainput);

  GetValuesFromDB("UpdateFieldPlanRotationModelledYield ", parameter);
}
        }

XmlNodeList FeedAndProductsUsed = doc.GetElementsByTagName("FeedAndProductsUsed");


foreach (XmlNode feeditem in FeedAndProductsUsed)
{
    if (feeditem.InnerText != "")
    {
        for (i = 0; i < feeditem.ChildNodes.Count; i++)
        {
            parameter = new List<data>();
            datainput = new data();
            datainput.name = "@aFarmNumber";
            datainput.value = HttpContext.Current.Session["farmNumber"].ToString();
            parameter.Add(datainput);
            datainput = new data();
            datainput.name = "@aScenarioID";
            datainput.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            parameter.Add(datainput);



            datainput = new data();
            datainput.name = "@aFeedstuffName";
            datainput.value = feeditem.ChildNodes[i].ChildNodes[0].ChildNodes[3].InnerText;
            parameter.Add(datainput);


            datainput = new data();
            datainput.name = "@aSumFeeding";
            datainput.value = feeditem.ChildNodes[i].ChildNodes[1].ChildNodes[3].InnerText;
            parameter.Add(datainput);

            GetValuesFromDB("InsertUpdateFeedstuffProductBalanceF", parameter);
        }
    }
}
XmlNodeList FeedAndProductsProduced = doc.GetElementsByTagName("FeedAndProductsProduced");


foreach (XmlNode feeditem in FeedAndProductsProduced)
{
    if (feeditem.ChildNodes.Count != 0)
    {
        for (i = 0; i < feeditem.ChildNodes.Count; i++)
        {
            parameter = new List<data>();
            datainput = new data();
            datainput.name = "@aFarmNumber";
            datainput.value = HttpContext.Current.Session["farmNumber"].ToString();
            parameter.Add(datainput);
            datainput = new data();
            datainput.name = "@aScenarioID";
            datainput.value = HttpContext.Current.Session["scenarioNumber"].ToString();
            parameter.Add(datainput);



            datainput = new data();
            datainput.name = "@aFeedstuffName";
            datainput.value = feeditem.ChildNodes[i].ChildNodes[0].ChildNodes[3].InnerText;
            parameter.Add(datainput);

            datainput = new data();
            datainput.name = "@aSumProduction";
            datainput.value = feeditem.ChildNodes[i].ChildNodes[1].ChildNodes[3].InnerText;
            parameter.Add(datainput);

            GetValuesFromDB("InsertUpdateFeedstuffProductBalanceP", parameter);
        }
    }
}
}

    }
}