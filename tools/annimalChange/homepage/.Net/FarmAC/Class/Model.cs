using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Data.OleDb;
using System.Data.SqlClient;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Web;
using System.Xml;

namespace FarmAC.Class
{
    public class Model
    {
        private AppSettingsReader cfg = null;

        private string sqlconnstr = "";

        public Model()
        {

            cfg = new AppSettingsReader();
            sqlconnstr = cfg.GetValue("sqlConnectionStringPublic", typeof(string)).ToString();
            if (HttpContext.Current.Server.MachineName == "JPM-LP-MAJWIN7" || HttpContext.Current.Server.MachineName == "GEFION-TEST-WEB")
            {

                if (HttpContext.Current.Server.MachineName == "GEFION-TEST-WEB")
                {
                    sqlconnstr = cfg.GetValue("sqlConnectionStringTest", typeof(string)).ToString(); //Denne når jeg kører på testserveren
                }
                if (HttpContext.Current.Server.MachineName == "JPM-LP-MAJWIN7")
                {
                    sqlconnstr = cfg.GetValue("sqlConnectionStringTest_new", typeof(string)).ToString(); //Denne når jeg kører lokalt
                    //sqlconnstr = cfg.GetValue("sqlConnectionStringTest", typeof(string)).ToString(); //Denne når jeg kører lokalt
                }
            }
        }

        //[WebMethod]
        public void RunModelFarmN_DB(long FarmNumber, int ScenarioID)
        {
            GenerateInputXML(FarmNumber, ScenarioID);
            string path = Directory.GetCurrentDirectory().ToString();
            Directory.SetCurrentDirectory(@"C:\Executables\AnimalChange\inputFiles");
            //RunAnimalChangeModel(@"C:\Executables\AnimalChange\AnimalChange.exe", "" + FarmNumber.ToString() + " " + ScenarioID.ToString() + "");
            RunAnimalChangeModel(@"C:\Executables\AnimalChange\FarmAC.exe", "" + FarmNumber.ToString() + " " + ScenarioID.ToString() + "");
            //saveOutputToDB("outputFarm23456ScenarioNr2.xml");
        }

        //[WebMethod]
        public void RunModelFarmN_DB_1(long FarmNumber, int ScenarioID, int partParameter1, int partParameter2)
        {
            GenerateInputXML(FarmNumber, ScenarioID);
            string path = Directory.GetCurrentDirectory().ToString();
            Directory.SetCurrentDirectory(@"C:\Executables\AnimalChange\inputFiles");
            //RunAnimalChangeModel(@"C:\Executables\AnimalChange\AnimalChange.exe", "" + FarmNumber.ToString() + " " + ScenarioID.ToString() + "");
            RunAnimalChangeModel(@"C:\Executables\AnimalChange\FarmAC.exe", "" + FarmNumber.ToString() + " " + ScenarioID.ToString() + " " + partParameter1.ToString() + " " + partParameter2.ToString() + "");
            //saveOutputToDB("outputFarm23456ScenarioNr2.xml");
        }

        //[WebMethod]
        public void RunModelFarmN_DB_2(long FarmNumber, int ScenarioID, int partParameter1, int partParameter2, int partParameter3)
        {
            GenerateInputXML(FarmNumber, ScenarioID);
            string path = Directory.GetCurrentDirectory().ToString();
            Directory.SetCurrentDirectory(@"C:\Executables\AnimalChange\inputFiles");
            //RunAnimalChangeModel(@"C:\Executables\AnimalChange\AnimalChange.exe", "" + FarmNumber.ToString() + " " + ScenarioID.ToString() + "");
            RunAnimalChangeModel(@"C:\Executables\AnimalChange\FarmAC.exe", "" + FarmNumber.ToString() + " " + ScenarioID.ToString() + " " + partParameter1.ToString() + " " + partParameter2.ToString() + " " + partParameter3.ToString() + "");
            //saveOutputToDB("outputFarm23456ScenarioNr2.xml");
        }

        //[WebMethod]
        public void RunPartModelFarmN_DB(long FarmNumber, int ScenarioID, int partParameter)
        {
            GenerateInputXML(FarmNumber, ScenarioID);
            string path = Directory.GetCurrentDirectory().ToString();
            Directory.SetCurrentDirectory(@"C:\Executables\AnimalChange\inputFiles");
            //RunAnimalChangeModel(@"C:\Executables\AnimalChange\AnimalChange.exe", "" + FarmNumber.ToString() + " " + ScenarioID.ToString() + " " + partParameter.ToString() + "");
            RunAnimalChangeModel(@"C:\Executables\AnimalChange\FarmAC.exe", "" + FarmNumber.ToString() + " " + ScenarioID.ToString() + " " + partParameter.ToString() + "");
        }


        //[WebMethod]
        public void GenerateInputXML(long FarmNumber, int ScenarioID)
        {
            SqlConnection sqlconn = null;
            try
            {
                sqlconn = new SqlConnection(sqlconnstr);
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
                print(farmData, "c:\\upload\\AnimalChange\\farm_" + FarmNumber + "_" + ScenarioID + ".xml");
            }
            finally
            {
                if (sqlconn != null)
                { sqlconn.Close(); }
            }

        }

        //[WebMethod]
        public void saveOutputToDB(string filename)
        {
            //filename = "outputFarm.xml";
            string xmlPath = "C://upload/AnimalChange/ModelOutput/ " + filename;
            XmlReaderSettings setting = new XmlReaderSettings();
            XmlDocument NBalance = new XmlDocument();
            XmlNode elem1 = NBalance.CreateNode(XmlNodeType.Element, "FarmCBalance", null);
            XmlNode elem2 = NBalance.CreateNode(XmlNodeType.Element, "FarmNBalance", null);
            XmlNode elem3 = NBalance.CreateNode(XmlNodeType.Element, "FarmDirectGHG", null);
            XmlNode elem4 = NBalance.CreateNode(XmlNodeType.Element, "FarmIndirectGHG", null);
            elem1.InnerXml = "<FarmNBalance></FarmNBalance>";
            //string text = "<xml><error></error>";
            setting.IgnoreWhitespace = true;
            XmlReader reader = XmlReader.Create(xmlPath, setting);
            while (reader.Read())
            {
                if (reader.NodeType == XmlNodeType.Element && (reader.Name == "FarmCBalance"))
                { elem1.InnerXml = reader.ReadInnerXml(); }
                if (reader.NodeType == XmlNodeType.Element && (reader.Name == "FarmNBalance"))
                { elem2.InnerXml = reader.ReadInnerXml(); }
                if (reader.NodeType == XmlNodeType.Element && (reader.Name == "FarmDirectGHG"))
                { elem3.InnerXml = reader.ReadInnerXml(); }
                if (reader.NodeType == XmlNodeType.Element && (reader.Name == "FarmIndirectGHG"))
                { elem4.InnerXml = reader.ReadInnerXml(); }
            }
            reader.Close();
            printobj(elem1, "c:\\Cbalance.txt");
            printobj(elem2, "c:\\Nbalance.txt");
            printobj(elem3, "c:\\DirectGHG.txt");
            printobj(elem4, "c:\\IndirectGHG.txt");
        }

        public void print(System.Data.DataSet set, string path)
        {
            //set.RemotingFormat = SerializationFormat.Xml;
            set.WriteXml(path);
        }
        public void printobj(object obj, string path)
        {
            Stream fs = new FileStream(path, FileMode.Create);
            XmlWriterSettings settings = new XmlWriterSettings();
            settings.CheckCharacters = true;
            settings.Encoding = Encoding.UTF8;
            //XmlWriter writer = new XmlTextWriter(fs, Encoding.Unicode);
            XmlWriter writer = XmlTextWriter.Create(fs, settings);
            System.Xml.Serialization.XmlSerializer w = new System.Xml.Serialization.XmlSerializer(obj.GetType());
            w.Serialize(fs, obj);
        }
        private void ReadExcel(System.Data.DataSet set, string filePath, string sheet, string tableName)
        {
            string selectCommand = "SELECT * FROM [Sheet" + sheet + "$]";
            string strConn = string.Format("Provider=Microsoft.ACE.OLEDB.12.0;Data Source={0};Extended Properties=\"Excel 12.0 Xml;HDR=YES;IMEX=1;TypeGuessRows=0;ImportMixedTypes=Text\"", filePath);
            using (OleDbConnection dbConnection = new OleDbConnection(strConn))
            {
                using (OleDbDataAdapter dbAdapter = new OleDbDataAdapter(selectCommand, dbConnection)) //rename sheet if required!
                    dbAdapter.Fill(set, tableName);
                //int rows = table.Rows.Count;
            }
        }
        private void RunAnimalChangeModel(string aModelPath, string argumentString)
        {
            Thread.CurrentThread.CurrentCulture = new CultureInfo("en-GB");
            Thread.CurrentThread.CurrentUICulture = new CultureInfo("en-GB");
            Process ACmodel = new Process();
            ACmodel.StartInfo.FileName = aModelPath;
            ACmodel.StartInfo.Arguments = argumentString;
            ACmodel.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;

            ACmodel.Start();
        }
    }
}