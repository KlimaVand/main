using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
 

using System.ComponentModel;
using System.Drawing;

using System.Text;

using System.Data.Sql;
using System.Threading;
using System.Globalization;
using Resources;
using System.Net;
using System.Diagnostics;

namespace FarmAC.Controls
{
    public partial class ctrlFarm : System.Web.UI.UserControl
    {
       
        DataTable arrBoughtManure;
        DataTable arrFarm;
        DataTable arrIrrigationType;
        List<DropDownList> allmanure = new List<DropDownList>();
        string farmNumber ;
        string scenarioNumber;
        protected void Page_Load(object sender, EventArgs e)
        {

            object st = HttpContext.Current.Session["UserInit"];
            if (HttpContext.Current.Session["UserInit"] != null)
                if (HttpContext.Current.Session["UserInit"].ToString() == "2")
            {
                try
                {
                    farmNumber = HttpContext.Current.Session["farmNumber"].ToString();
                    scenarioNumber = HttpContext.Current.Session["scenarioNumber"].ToString();
                }
                 catch
                {
                    farmNumber = "-1";
                    scenarioNumber = "-1";
                }
                if (!Page.IsPostBack)
                {

                }
                else
                {

                }
                if (farmNumber!="-1")
                CreateTabFarm();
            }
        }

        protected void RadGridFarm_DataBound(object sender, EventArgs e)
        {
            //  if(RadGridFarm.MasterTableView.Items.Count>0)
            //   RadGridFarm.MasterTableView.Items[0].Selected=true;
        }
        string language;


        void CreateTabFarm()
        {
            HttpContext.Current.Session["FileName"] = null;
            
            HttpContext.Current.Session["AllNodes"] = null;


            inputAddress.AutoPostBack = true;
            inputDescription.AutoPostBack = true;
            UploadFile.Click += new EventHandler(UploadFileEvent);
            Button_CreateXML.Click += new EventHandler(generateXML);
            dbInterface db = new dbInterface(); ;
            string path = db.GetConnectionString("ModelOutput");
            if (File.Exists(path + "error_" + farmNumber + "_" + scenarioNumber + ".txt"))
            {
                System.IO.StreamReader file = new System.IO.StreamReader(path + "error_" + farmNumber + "_" + scenarioNumber + ".txt");
                TextArea1.Text = "There is a problem with running the model: ";
                while (!file.EndOfStream)
                    TextArea1.Text += file.ReadLine() + "\n";
                file.Close();
            }
            else
            {
                TextArea1.Text = "no problem with running model";
             
            }
            string stuff = TextArea1.Text;

            Thread.CurrentThread.CurrentUICulture = CultureInfo.GetCultureInfo("da-DK");
            string tmp=strings.sprog1;
            Thread.CurrentThread.CurrentUICulture = CultureInfo.GetCultureInfo("en-GB");
            tmp = strings.sprog1;
            atmosphericDeposition.AutoPostBack = true;
            if (HttpContext.Current.Session["languageSelected"] != null)
                language = HttpContext.Current.Session["languageSelected"].ToString();
            else
                language = "uk";

            string[] listOfFiles;
            try
            {
                string pathAlt = db.GetConnectionString("Alternative");
                listOfFiles = Directory.GetFiles(pathAlt + "\\" + farmNumber);
            }
            catch
            {
                listOfFiles = new string[0];
            }

            if (listOfFiles.Count() == 0)
                this.RemoveOldFile.Visible = false;
            else
            {
                this.RemoveOldFile.Visible = true;
               
            }
            string browser = HttpContext.Current.Request.UserAgent;
            string start;
            if (browser.Contains("Mozilla") && !browser.Contains("Chrome"))
                start = "<br/><a href=/";
            else
                start = "<br/><a href=";
            if (listOfFiles.Count() == 1)
                filePresent.Text = "File present:<br/> ";
            if (listOfFiles.Count() > 1)
                filePresent.Text = "Files present:<br/> ";
            for (int i = 0; i < listOfFiles.Count();i++ )
            {
                string[] file = listOfFiles[i].Split('\\');
                filePresent.Text+=start;
                filePresent.Text += (((listOfFiles[i].Replace("C:", "") + " target=\"new\">" + file[file.Count() - 1] + "</a>")));
             //   filePresent.Text += file[file.Count() - 1] + "<br/>";

            }
                OK.Visible = false;
            NewName.Visible = false;
            double nScenario;

            DataTable arrScenario;
            double nSoilType, aSoilType;
            DataTable arrSoilType;
            int aIrrigationType;

            double aDeliveryType;


            DataTable arrFarmDelivery;
            double nFarmType;
            DataTable arrFarmType;

            double nStorageType;
            DataTable arrStorageType;



            DataTable arrSoldManure;
            double nCropYear, aCropYear;
            DataTable arrCropYear;
            double nAgroEcologicalZone;
            int aAgroEcologicalZone;
            DataTable arrAgroEcologicalZone;
            double manureVersion = 0; ;
            dbInterface database = new dbInterface();
            database.InitFarmNDB();
            //CreateLanguageDictionary

            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = farmNumber;
            paramter.Add(data);
            arrFarm = database.GetValuesFromDB("GetFarmData", paramter);

            arrScenario = database.GetValuesFromDB("GetSelectListScenario ", paramter);
            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@FarmNumber";
            data.value = farmNumber;
            paramter.Add(data);
            data.name = "@ScenarioID";
            data.value = scenarioNumber;
            paramter.Add(data);
            arrBoughtManure=database.GetValuesFromDB ("GetBoughtManureList ",paramter);
           
                paramter = new List<dbInterface.data>();
            arrSoilType = database.GetValuesFromDB("GetSelectListSoilType", paramter);


            arrFarmType = database.GetValuesFromDB("GetSelectListFarmType", paramter);


            arrStorageType = database.GetValuesFromDB("GetOrganicStorageTypesList", paramter);
            Panel2.Height =0;
             for (int i = 0; i<arrBoughtManure.Rows.Count;i++)
            {
                DropDownList manure = new DropDownList();
                manure.AutoPostBack = true;
                manure.ID = "manure" + i.ToString();
                for (int j = 0; j < arrStorageType.Rows.Count; j++)
                {

                    manure.Items.Add(arrStorageType.Rows[j][0] + "" + db.GetLanguageString("str"+arrStorageType.Rows[j][1],language));
                    manure.Items[manure.Items.Count - 1].Attributes.Add("dbValue", arrStorageType.Rows[j][0].ToString());
                    string test = arrBoughtManure.Rows[i]["BoughtManureType"].ToString();
                    string test2 = arrStorageType.Rows[j][0].ToString();
                    if (arrBoughtManure.Rows[i]["BoughtManureType"].ToString() == arrStorageType.Rows[j][0].ToString())
                        manure.SelectedIndex = j;
                }
                Panel2.Controls.Add(manure);
                allmanure.Add(manure);
                Button strDelete = new Button();
                strDelete.Text = "delete";
                strDelete.ID = "delete" + i.ToString();
                strDelete.Attributes.Add("dbValue",arrBoughtManure.Rows[i]["BoughtManureID"].ToString());
                strDelete.Click += new EventHandler(DeleteManure);
                Panel2.Controls.Add(strDelete);
                 string stff=Panel2.Height.ToString();
                 stff = stff.Replace("px","");
                 Panel2.Height = 20 + int.Parse(stff);
               
            }
            for(int i=0;i<allmanure.Count;i++)
            {
                allmanure[i].SelectedIndexChanged += new EventHandler(selectAgroZone_SelectedIndexChanged);
            }

             Panel2.Enabled=true;
             Panel2.Width = 600;
            arrCropYear = database.GetValuesFromDB("GetSelectListCropYear", paramter);

            paramter = new List<dbInterface.data>();
            arrAgroEcologicalZone = database.GetValuesFromDB("GetAgroEcologicalZoneList", paramter);

            string agroNr = arrFarm.Rows[0]["AgroEcologicalZone"].ToString();
            aAgroEcologicalZone = int.Parse(agroNr);
            HttpContext.Current.Session[("AgroEcologicalZone")] = aAgroEcologicalZone;


            HttpContext.Current.Session[("version")] = manureVersion;
            if (!Page.IsPostBack)
            {
                atmosphericDeposition.Text = arrFarm.Rows[0]["AtmosphericNDeposition"].ToString();
            }
            string aScenario = (arrFarm.Rows[0]["SelectedScenario"].ToString());


            //aCropYear =Convert.ToDouble(arrFarm.Rows[0]["CropYear"].ToString());

            aSoilType = (int)arrFarm.Rows[0]["SoilType"];

            //       aIrrigationType =(int) arrFarm.Rows[0]["Irrigation"];
            if (arrFarm.Rows[0]["Irrigation"] != null)
                aIrrigationType = 1;
            else
                aIrrigationType = 0;



            arrIrrigationType = database.GetValuesFromDB("GetSelectListIrrigation", paramter);
            string[][] arrDeliveryType = new string[2][];
            for (int i = 0; i < 2; i++)
                arrDeliveryType[i] = new string[3];

            aDeliveryType = (int)arrFarm.Rows[0]["Delivery"];




            arrDeliveryType[1][0] = database.GetLanguageString("strDELIVERYINCORP", language);
            arrDeliveryType[0][1] = "2";

            arrDeliveryType[1][1] = database.GetLanguageString("strDELIVERYTUBE", language);
            arrDeliveryType[0][2] = "3";

            arrDeliveryType[1][2] = database.GetLanguageString("strBredspredning af fast husdyrgødning", language);


            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@FarmNumber";
            data.value = farmNumber;
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@ScenarioID";
            data.value = aScenario.ToString();
            paramter.Add(data);


            arrBoughtManure = database.GetValuesFromDB("GetBoughtManureList", paramter);

            arrSoldManure = database.GetValuesFromDB("GetSoldManureList", paramter);
            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = farmNumber;
            paramter.Add(data);

            arrFarmDelivery = database.GetValuesFromDB("GetFarmDeliveryData ", paramter);

            database.CloseFarmNDB();
            if (!Page.IsPostBack)
            {
                inputName.Text = arrFarm.Rows[0]["FarmOwnerName"].ToString();
                inputAddress.Text = arrFarm.Rows[0]["FarmOwnerAddress"].ToString();
                inputDescription.Text = arrFarm.Rows[0]["FarmDescription"].ToString();
            }

            int index = -1;

            for (int i = 0; i < arrAgroEcologicalZone.Rows.Count; i++)
            {
                string Name = arrAgroEcologicalZone.Rows[i]["ZoneName"].ToString();
                string ID = arrAgroEcologicalZone.Rows[i]["AgroEcologicalZoneID"].ToString();

                if (selectAgroZone.Items.Count <= i)
                {
                    selectAgroZone.Items.Add(ID + " " + Name);
                }

                selectAgroZone.Items[i].Attributes.Add("dbValue", ID);
                if (ID == aAgroEcologicalZone.ToString())
                    index = i;
            }

            if (!Page.IsPostBack)
                selectAgroZone.SelectedIndex = (index);
            selectAgroZone.SelectedIndexChanged += new EventHandler(selectAgroZone_SelectedIndexChanged);
            selectAgroZone.AutoPostBack = true;
            /* int dfsgdg = selectAgroZone.Items.Count;
             int dfsgdsdg = arrFarmType.Rows.Count;
             while (selectAgroZone.Items.Count > arrFarmType.Rows.Count)
             {
                 selectAgroZone.Items.RemoveAt(selectScenario.Items.Count - 1);
             }*/
            string zone = selectAgroZone.SelectedItem.Attributes["dbValue"];
            int SelectedIndex = -1;

            for (int i = 0; i < arrFarmType.Rows.Count; i++)
            {
                if (selectFarmType.Items.Count <= i)
                {

                    selectFarmType.Items.Add(arrFarmType.Rows[i][0] + " " + db.GetLanguageString("str"+arrFarmType.Rows[i][1],language));
                }

                selectFarmType.Items[i].Attributes.Add("dbValue", arrFarmType.Rows[i][0].ToString());
                if (arrFarmType.Rows[i][0].ToString() == HttpContext.Current.Session["farmType"].ToString())
                {
                    SelectedIndex = i;
                }
            }
            while (selectFarmType.Items.Count > arrFarmType.Rows.Count)
            {
                selectFarmType.Items.RemoveAt(selectScenario.Items.Count - 1);
            }
            int t = selectFarmType.SelectedIndex;
            if (!Page.IsPostBack)
                selectFarmType.SelectedIndex = SelectedIndex;

            selectFarmType.SelectedIndexChanged += new EventHandler(selectAgroZone_SelectedIndexChanged);
            selectFarmType.AutoPostBack = true;

            for (int i = 0; i < arrScenario.Rows.Count; i++)
            {
                if (selectScenario.Items.Count <= i)
                {
                    selectScenario.Items.Add(arrScenario.Rows[i][0] + " " + arrScenario.Rows[i][1]);

                }
                string stf = arrScenario.Rows[i][0].ToString();
                selectScenario.Items[i].Attributes.Add("dbValue", arrScenario.Rows[i][0].ToString());
                if (!Page.IsPostBack)
                    if (aScenario == arrScenario.Rows[i][0].ToString())
                        selectScenario.SelectedIndex = i;

            }
            while (selectScenario.Items.Count > arrScenario.Rows.Count)
            {
                selectScenario.Items.RemoveAt(selectScenario.Items.Count - 1);
            }


            selectScenario.AutoPostBack = true;
            selectScenario.SelectedIndexChanged += new EventHandler(updateSelectScenario);

         

        }
        void CreateUpdateFormFarm()
        {
            Response.Write("<form name=" + "update" + " action=" + "javascript:document.forms.formFarm.submit();" + " method=" + "post" + ">" + "\r");
            Response.Write(" <input type=" + "hidden" + " name=" + "saveUpdates" + " id=" + "saveUpdates" + " value=" + "false" + ">" + "\r");

            Response.Write("</form>" + "\r");
        }

        protected void adaptation_Click(object sender, EventArgs e)
        {
            Panel1.Controls.Add(new LiteralControl("<script>function myFunction() {    var myWindow = window.open(\"\", \"_self\");    myWindow.document.write(\"<p>I replaced the current window.</p>\");}</script>"));
            string[] input = new string[5];
            input[0] = farmNumber;
            input[1] = selectScenario.SelectedItem.Attributes["dbValue"];
            input[2] = "1";
            input[3] = "1";
            input[4] = "-1";
            //   input[1] = selected
            string t = Directory.GetCurrentDirectory();

            dbInterface db = new dbInterface();
            string generatedXML=db.GenerateInputXML(int.Parse(input[0]), int.Parse(input[1]));
            if (generatedXML == "")
            {
                string path = db.GetConnectionString("ModelOutput");
                utils util = new utils();
               util.deleteFils();
             /*   model model = new model();
                Thread.CurrentThread.CurrentCulture = CultureInfo.CreateSpecificCulture("en-GB");
                model.setup(input);
                HttpContext ctx = HttpContext.Current;
                Thread oThread = new Thread(new ThreadStart(() =>
                {
                    HttpContext.Current = ctx;
                    model.run();
                }));
                oThread.Start();
                while (!oThread.IsAlive) ;

                oThread.Join();*/

               ProcessStartInfo psi = new ProcessStartInfo();
               psi.FileName = db.GetConnectionString("exefile"); 
               psi.Arguments = input[0] + " " + input[1] + " " + input[2] + " " + input[3] + " " + input[4];
               psi.WorkingDirectory = System.IO.Path.GetDirectoryName(db.GetConnectionString("xmlFile"));
               System.Diagnostics.Process p = System.Diagnostics.Process.Start(psi);
               p.WaitForExit();
                TextArea1.Visible = true;
                if (File.Exists(path + "error_" + farmNumber + "_" + scenarioNumber + ".txt"))
                {
                    System.IO.StreamReader file = new System.IO.StreamReader(path + "error_" + farmNumber + "_" + scenarioNumber + ".txt");
                    TextArea1.Text = "There is a problem with running the model: ";
                    while (!file.EndOfStream)
                        TextArea1.Text += file.ReadLine() + "\n";
                    file.Close();
                }
                else
                {
                    System.IO.StreamWriter file = new System.IO.StreamWriter(path+"test.txt");
                    file.WriteLine(path + "error_" + farmNumber + "_" + scenarioNumber + ""+db.GetConnectionString("xmlFile")+" .txt");
                    file.Close();
                    TextArea1.Text = "no problem with running model";
                    db.saveResults();
                    HttpContext.Current.Session["ShowResult"] = true;
                }
            }
            else
            {
                string path = db.GetConnectionString("ModelOutput");
                System.IO.StreamWriter file = new System.IO.StreamWriter(path + "error_" + farmNumber + "_" + scenarioNumber + ".txt");
                file.WriteLine("The file has not been generated: " + generatedXML);
                file.Close();
                TextArea1.Text = "The file has not been generated";
            }
            ViewState["UserInit"] = "2";
            HttpContext.Current.Session["UserInit"] = "2";
            
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);

        }
        protected void NewManure(object sender, EventArgs e)
        {
            FarmBoughtManureNew manure = new FarmBoughtManureNew();
            manure.ProcessRequest(HttpContext.Current);
            ViewState["UserInit"] = "2";
            HttpContext.Current.Session["UserInit"] = "2";
            HttpContext.Current.Session["ShowResult"] = false;
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }
        protected void Projection_Click(object sender, EventArgs e)
        {

          

            utils util = new utils();
            util.deleteFils();

            string[] input = new string[5];
            input[0] = farmNumber;
            input[1] = selectScenario.SelectedItem.Attributes["dbValue"];
            input[2] = "1";
            input[3] = "1";
            input[4] = "-1";
            bool BaselineFound = false;
            for (int i = 0; i < selectScenario.Items.Count; i++)
            {
                if (selectScenario.Items[i].Text.Contains("Baseline") || selectScenario.Items[i].Text.Contains("baseline"))
                {
                    input[4] = selectScenario.Items[i].Attributes["dbValue"];
                    BaselineFound = true;
                }
            }
            dbInterface db = new dbInterface();
            string generatedXML=db.GenerateInputXML(int.Parse(input[0]), int.Parse(input[1]));
            if (generatedXML == "")
            {

                Thread.CurrentThread.CurrentCulture = CultureInfo.CreateSpecificCulture("en-GB");
                
                ProcessStartInfo psi = new ProcessStartInfo();
                psi.FileName = db.GetConnectionString("exefile") ;
                psi.Arguments = input[0] + " " + input[1] + " " + input[2] + " " + input[3] + " " + input[4];
                psi.WorkingDirectory = System.IO.Path.GetDirectoryName(db.GetConnectionString("xmlFile"));
                System.Diagnostics.Process p = System.Diagnostics.Process.Start(psi);
                p.WaitForExit();
            TextArea1.Visible = true;
            string path = db.GetConnectionString("ModelOutput");

            if (File.Exists(path + "error_" + farmNumber + "_" + scenarioNumber + ".txt"))
            {
                System.IO.StreamReader file = new System.IO.StreamReader((path + "error_" + farmNumber + "_" + scenarioNumber + ".txt"));
                TextArea1.Text = file.ReadLine();
                file.Close();
            }
            else
            {


                TextArea1.Text="no problem with running model. ";

                if (BaselineFound == false)
                {
                    System.IO.StreamWriter file = new System.IO.StreamWriter(path + "error_" + farmNumber + "_" + scenarioNumber + ".txt");
                    file.WriteLine(" Did run as Adaoptation since there was no baseline");
                    file.Close();
                }
                db.saveResults();
                HttpContext.Current.Session["ShowResult"] = true;
            }
            }
            else
            {
                string path = db.GetConnectionString("ModelOutput");
                System.IO.StreamWriter file = new System.IO.StreamWriter(path + "error_" + farmNumber + "_" + scenarioNumber + ".txt");
                file.WriteLine("The file has not been generated: " + generatedXML);
                file.Close();
                TextArea1.Text = "The file has not been generated";
            }
            ViewState["UserInit"] = "2";
            HttpContext.Current.Session["UserInit"] = "2";

            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);

        }

        protected void selectAgroZone_SelectedIndexChanged(object sender, EventArgs e)
        {
            FarmUpdate test = new FarmUpdate();

            string farmType = selectFarmType.SelectedItem.Attributes["dbValue"];
            string tmp = inputName.Text;
            string tmp2 = inputAddress.Text;
            test.ProcessRequest(HttpContext.Current, allmanure, inputName.Text, inputAddress.Text, HttpContext.Current.Session["scenarioNumber"].ToString(), selectFarmType.SelectedItem.Attributes["dbValue"], selectScenario.SelectedItem.Attributes["dbValue"], arrFarm.Rows[0]["YieldLevel"].ToString(), selectAgroZone.SelectedItem.Attributes["dbValue"], inputDescription.Text, atmosphericDeposition.Text);
            HttpContext.Current.Session["ShowResult"] = false;
            Panel1.Controls.Clear();
            Response.Redirect(Request.RawUrl);
        }

        protected void inputAddress_TextChanged(object sender, EventArgs e)
        {
            selectAgroZone_SelectedIndexChanged(sender, e);
        }

        protected void Button_DeleteScenario_Click(object sender, EventArgs e)
        {
            FarmScenarioDelete tmp = new FarmScenarioDelete();

            tmp.ProcessRequest(HttpContext.Current, selectScenario.SelectedItem.Attributes["dbValue"]);

            HttpContext.Current.Session["UserInit"] = "2";
            HttpContext.Current.Session["ShowResult"] = false;
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }

        protected void inputName_TextChanged(object sender, EventArgs e)
        {
            selectAgroZone_SelectedIndexChanged(sender, e);
        }

        protected void Button_NewScenario_Click(object sender, EventArgs e)
        {
            FarmScenarioNew1 tmp = new FarmScenarioNew1();
            // inputZip.Text, inputTown.Text
            tmp.ProcessRequest(HttpContext.Current, inputName.Text, inputAddress.Text, "", "", arrFarm.Rows[0]["SoilType"].ToString(), arrIrrigationType.Rows[0][0].ToString(), selectFarmType.SelectedItem.Attributes["dbValue"].ToString(), arrFarm.Rows[0]["YieldLevel"].ToString(), inputDescription.Text, atmosphericDeposition.Text, selectScenario.SelectedItem.Attributes["dbValue"], selectAgroZone.SelectedItem.Attributes["dbValue"], false);
            HttpContext.Current.Session["ShowResult"] = false;
            ViewState["UserInit"] = "2";
            HttpContext.Current.Session["UserInit"] = "2";

            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }
        protected void atmosphericDeposition_TextChanged(object sender, EventArgs e)
        {
            selectAgroZone_SelectedIndexChanged(sender, e);
        }

        protected void Button_CopyScenario_Click(object sender, EventArgs e)
        {
            FarmScenarioNew1 tmp = new FarmScenarioNew1();
            // inputZip.Text, inputTown.Text
            tmp.ProcessRequest(HttpContext.Current, inputName.Text, inputAddress.Text, "", "", arrFarm.Rows[0]["SoilType"].ToString(), arrIrrigationType.Rows[0][0].ToString(), selectFarmType.SelectedItem.Attributes["dbValue"].ToString(), arrFarm.Rows[0]["YieldLevel"].ToString(), inputDescription.Text, atmosphericDeposition.Text, selectScenario.SelectedItem.Attributes["dbValue"], selectAgroZone.SelectedItem.Attributes["dbValue"], true);
            HttpContext.Current.Session["ShowResult"] = false;
            CreateTabFarm();
        }
        protected void OKEvent(object sender, EventArgs e)
        {

            OK.Visible = false;
            NewName.Visible = false;
            Button_Rename.Visible = true;
            string name = NewName.Text;
            selectScenario.SelectedItem.Text = NewName.Text;
            dbInterface db=new dbInterface();
            List<FarmAC.Controls.dbInterface.data> parameterList =new List<FarmAC.Controls.dbInterface.data>();
            FarmAC.Controls.dbInterface.data data=new FarmAC.Controls.dbInterface.data();
            data.value=farmNumber;
            data.name="@aFarmNumber";
            parameterList.Add(data);

            data=new FarmAC.Controls.dbInterface.data();
            data.value=scenarioNumber;
            data.name="@aSelectedScenario";
            parameterList.Add(data);

            data.value = NewName.Text;
            data.name="@aScenarioName";
            parameterList.Add(data);
            db.GetValuesFromDB("InsertUpdateScenario", parameterList);


        }
        protected void RenameEvent(object sender, EventArgs e)
        {

            OK.Visible = true;
            NewName.Visible = true;
            Button_Rename.Visible = false;
            NewName.Text = selectScenario.SelectedItem.Text;
        }
        protected void UploadFileEvent(object sender, EventArgs e)
        {
            dbInterface db = new dbInterface();
            string path = db.GetConnectionString("Alternative");
            if (!Directory.Exists(path + "\\" + farmNumber))
                Directory.CreateDirectory(path + "\\" + farmNumber);
            try
            {
                FileUpload1.SaveAs(path + "\\" + farmNumber + "\\" + FileUpload1.FileName);
            }
            catch
            {

            }
            ViewState["UserInit"] = "2";
            HttpContext.Current.Session["UserInit"] = "2";
            HttpContext.Current.Session["ShowResult"] = false;
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);

        }
        protected void RemoveOldFileEvent(object sender, EventArgs e)
        {
            dbInterface db = new dbInterface();
            string path = db.GetConnectionString("Alternative");
            Directory.Delete(path + farmNumber, true);


            RemoveOldFile.Visible = false;
            ViewState["UserInit"] = "2";
            HttpContext.Current.Session["ShowResult"] = false;
            HttpContext.Current.Session["UserInit"] = "2";

            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }
        protected void updateSelectScenario(object sender, EventArgs e)
        {
            HttpContext.Current.Session["scenarioNumber"] = selectScenario.SelectedItem.Attributes["dbValue"].ToString();

            HttpContext.Current.Session["UserInit"] = "2";
            selectAgroZone_SelectedIndexChanged(sender, e);
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }
         protected void DeleteManure(object sender, EventArgs e)
        {
            FarmBoughtManureDelet tmp = new FarmBoughtManureDelet();
            Button knap = (Button)sender;
            string value = knap.Attributes["dbValue"];
            tmp.ProcessRequest(HttpContext.Current, value);
            HttpContext.Current.Session["UserInit"] = "2";
            HttpContext.Current.Session["ShowResult"] = false;
            Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
        }
         protected void generateXML(object sender, EventArgs e)
         {
             if (HttpContext.Current.Request["selectFarm"] != null)
             {
                
                 HttpContext.Current.Session["logout"] = true;
             }
             dbInterface db = new dbInterface();
            string genFile= db.GenerateInputXML(long.Parse(farmNumber), int.Parse(scenarioNumber));
             if(genFile=="")
             {
             string path = db.GetConnectionString("FarmFiles");
            path= path.Replace("C:", "");
             string filename= "farm_" + farmNumber + "_" + scenarioNumber + ".xml";
             path += filename;
             Response.AppendHeader("Content-Disposition", "attachment; filename="+filename);
             Response.TransmitFile(Server.MapPath(path));
             Response.End();
             }
            else
            {
                string path = db.GetConnectionString("ModelOutput");
                System.IO.StreamWriter file = new System.IO.StreamWriter(path + "error_" + farmNumber + "_" + scenarioNumber + ".txt");
                file.WriteLine("The file has not been generated: "+genFile);
                TextArea1.Text = "The file has not been generated";
                file.Close();
            }
         }

        

    }
}