using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.Services.Description;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;

using System.ComponentModel;
using System.Drawing;

using System.Web.UI.Design;
using System.ComponentModel.Design;
using System.Reflection;
using System.IO;
using System.Xml;

using System.Configuration;

using System.Data.SqlClient;
using System.Text.RegularExpressions;

namespace FarmAC.Controls
{
    
   public partial class resultGenralt : System.Web.UI.UserControl
   {

       Label Label1;

       static dbInterface db = new dbInterface();
       static string path = db.GetConnectionString("ModelOutput");
       protected void Page_Load(object sender, EventArgs e)
       {
       }
       string balance="C";
     
       public void commonArea(string type, Panel Panel1)
       {
           balance = type;
           Label1 = new Label();
           GridView1 = new GridView();

           GridView2 = new GridView();
           GridView3 = new GridView();
           GridView1.BackColor = Color.FromArgb(255, 255, 255);
           GridView2.BackColor = Color.FromArgb(255, 255, 255);
           GridView3.BackColor = Color.FromArgb(255, 255, 255);
           string stf=(path + "error_" + HttpContext.Current.Session["farmNumber"].ToString() + "_" + HttpContext.Current.Session["scenarioNumber"].ToString() + ".txt");
           if (!File.Exists(path + "error_" + HttpContext.Current.Session["farmNumber"].ToString() + "_" + HttpContext.Current.Session["scenarioNumber"].ToString() + ".txt"))
           {
               dbInterface db=new dbInterface();
               
               
               string tmp = path.Replace("C:", "");
               string browser=HttpContext.Current.Request.UserAgent;
               string start;
               if (browser.Contains("Mozilla") && !browser.Contains("Chrome"))
                     start="<br/><a href=/";
               else
                   start="<br/><a href=";
               Panel1.Controls.Add(new LiteralControl(start));
               if (File.Exists(path + "outputFarm" + HttpContext.Current.Session["farmNumber"].ToString() + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"].ToString() + ".xml"))
                   Panel1.Controls.Add(new LiteralControl((  path.Replace("C:", "") + "outputFarm" + HttpContext.Current.Session["farmNumber"].ToString() + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"].ToString() + ".xml target=\"new\">" + "strViewOutputXml" + "</a>")));
               Panel1.Controls.Add(new LiteralControl(start));
               if (File.Exists(path + "outputFarm" + HttpContext.Current.Session["farmNumber"].ToString() + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"].ToString() + ".xls"))
                   Panel1.Controls.Add(new LiteralControl(( path.Replace("C:", "") + "outputFarm" + HttpContext.Current.Session["farmNumber"].ToString() + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"].ToString() + ".xls target=\"new\">" + "strViewOutputXls" + "</a>")));
               Panel1.Controls.Add(new LiteralControl(start));
               if (File.Exists(path + "outputFarm" + HttpContext.Current.Session["farmNumber"].ToString() + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"].ToString() + "livetockfile.xls"))
                   Panel1.Controls.Add(new LiteralControl(( path.Replace("C:", "") + "outputFarm" + HttpContext.Current.Session["farmNumber"].ToString() + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"].ToString() + "livetockfile.xls target=\"new\">Livestock result as xls</a>")));
               Panel1.Controls.Add(new LiteralControl(start));
               if (File.Exists(path + "outputFarm" + HttpContext.Current.Session["farmNumber"].ToString() + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"].ToString() + "plantfile.xls"))
                   Panel1.Controls.Add(new LiteralControl(( path.Replace("C:", "") + "outputFarm" + HttpContext.Current.Session["farmNumber"].ToString() + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"].ToString() + "plantfile.xls target=\"new\">" + "strViewOutputPlantXls" + "</a>")));
               Panel1.Controls.Add(new LiteralControl(start));
               if (File.Exists(path + "dataCtool_" + HttpContext.Current.Session["farmNumber"].ToString() + "_" + HttpContext.Current.Session["scenarioNumber"].ToString() + ".txt"))
                   Panel1.Controls.Add(new LiteralControl((path.Replace("C:", "") + "dataCtool_" + HttpContext.Current.Session["farmNumber"].ToString() + "_" + HttpContext.Current.Session["scenarioNumber"].ToString() + ".txt target=\"new\">dataCtool-txt-file</a>")));
               Panel1.Controls.Add(new LiteralControl(start));
               if (File.Exists(path + "outputFarm" + HttpContext.Current.Session["farmNumber"].ToString() + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"].ToString() + "CtoolFile.xls"))
                   Panel1.Controls.Add(new LiteralControl((path.Replace("C:", "") + "outputFarm" + HttpContext.Current.Session["farmNumber"].ToString() + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"].ToString() + "CtoolFile.xls target=\"new\">" + "strViewOutputCtoolXls" + "</a>")));
               Panel1.Controls.Add(new LiteralControl(start));
               if (File.Exists(path + "outputFarm" + HttpContext.Current.Session["farmNumber"].ToString() + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"].ToString() + "Debug.xls"))
                   Panel1.Controls.Add(new LiteralControl((path.Replace("C:", "") + "outputFarm" + HttpContext.Current.Session["farmNumber"].ToString() + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"].ToString() + "Debug.xls target=\"new\">" + "waterDebug" + "</a>")));

               start = "<br/><a href=\"";
               Panel1.Controls.Add(new LiteralControl(start));
               string graphServer2 = db.GetConnectionString("graphServer2");

               Panel1.Controls.Add(new LiteralControl((graphServer2 + "/app/?file=" + "outputFarm" + HttpContext.Current.Session["farmNumber"].ToString() + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"].ToString() + "\" target=\"new\">" + "graphs" + "</a>")));
               Panel1.Controls.Add(new LiteralControl("<br/> or </a>"));
               start = "<br/>(<a href=\"";
               Panel1.Controls.Add(new LiteralControl(start));
               string graphServer = db.GetConnectionString("graphServer");
             
               Panel1.Controls.Add(new LiteralControl((graphServer+"/app/?file=" + "outputFarm" + HttpContext.Current.Session["farmNumber"].ToString() + "ScenarioNr" + HttpContext.Current.Session["scenarioNumber"].ToString() + "\" target=\"new\">" + "graphs" + "</a>)")));
               
               
               commonSetting();

           }
           else
           {
               System.IO.StreamReader file = new System.IO.StreamReader(path + "error_" + HttpContext.Current.Session["farmNumber"].ToString() + "_" + HttpContext.Current.Session["scenarioNumber"].ToString() + ".txt");
               Label1.Text = "There is a problem with running the model: ";
               while (!file.EndOfStream)
                   Label1.Text += file.ReadLine() + "\n";
           }
           Panel1.Controls.Add(Label1);



            align(GridView1);
           align(GridView2);
             align(GridView3);
           Panel1.Controls.Add(GridView1);
           Panel1.Controls.Add(GridView2);
           Panel1.Controls.Add(GridView3);
           
       }
       public void align(GridView view)
       {
           for (int i = 0; i < view.Rows.Count; i++)
               view.Rows[i].Cells[1].HorizontalAlign = HorizontalAlign.Right;
       }
      public void commonSetting()
      {
          string fileName = "outputFarm23456ScenarioNr2";
          fileName = "outputFarm"+HttpContext.Current.Session["farmNumber"].ToString()+"ScenarioNr"+ HttpContext.Current.Session["scenarioNumber"].ToString()+".xml" ;

          
          
      
        
          try
          {
              //Label1.Text = "Name of xml-file: "+fileName ;
              ShowResultXml(path + fileName, balance);

          }
          catch
          {
              try
              {
                  //Label1.Text = "Name of xml-file: "+fileName ;
                  ShowResultXml(path+  fileName, balance);

              }
              catch (IOException ioe)
              {
                  Label1.Text = "Something went wrong" + ioe.Message;
              }
          }
      }
      private void ShowResultXml(string xmlPath, string balance)
      {
          object obj = new object();
          System.Data.DataTable ds = new System.Data.DataTable();
          System.Data.DataTable ds1 = new System.Data.DataTable();
          System.Data.DataTable ds2 = new System.Data.DataTable();
          System.Data.DataTable ds3 = new System.Data.DataTable();

          System.Data.DataTable tempds = new System.Data.DataTable();
          System.Data.DataTable empty = new System.Data.DataTable();

          XmlReaderSettings setting = new XmlReaderSettings();

          setting.IgnoreWhitespace = true;
          setting.CloseInput = true;
          //setting.CloseInput
          XmlReader reader2 = XmlReader.Create(xmlPath, setting);
          ds.Columns.Add("Description");
          ds.Columns.Add("Value");
          ds.Columns.Add("Unit");
          ds1.Columns.Add("Description");
          ds1.Columns.Add("Value");
          ds1.Columns.Add("Unit");
          ds2.Columns.Add("Description");
          ds2.Columns.Add("Value");
          ds2.Columns.Add("Unit");
          ds3.Columns.Add("Description");
          ds3.Columns.Add("Value");
          ds3.Columns.Add("Unit");
          empty.Columns.Add("Description");
          empty.Columns.Add("Value");
          empty.Columns.Add("Unit");
          tempds = empty;
          int valueAdded = 0;
          System.Data.DataRow dr = tempds.NewRow();
          while (reader2.Read())
          {
              //while (reader2.NodeType == XmlNodeType.Element || reader2.NodeType == XmlNodeType.Text && (reader2.Name != "Farm" || reader2.Name != "FarmBalance" || reader2.Name != "FarmCBalance"))
              while (reader2.NodeType == XmlNodeType.Element || reader2.NodeType == XmlNodeType.Text && (reader2.Name != "Farm" || reader2.Name != "FarmBalance"))
              {
                  if (reader2.NodeType == XmlNodeType.Element && reader2.Name == "FarmCBalance") { tempds = ds; }
                  if (reader2.NodeType == XmlNodeType.Element && reader2.Name == "FarmNBalance") { tempds = ds1; }
                  if (reader2.NodeType == XmlNodeType.Element && reader2.Name == "FarmDirectGHG") { tempds = ds2; }
                  if (reader2.NodeType == XmlNodeType.Element && reader2.Name == "FarmIndirectGHG") { tempds = ds3; }
                  if ((reader2.NodeType == XmlNodeType.Element && reader2.Name == "LiveStock") || (reader2.NodeType == XmlNodeType.Element && reader2.Name == "CropSequenceClass") || (reader2.NodeType == XmlNodeType.Element && reader2.Name == "Housing") || (reader2.NodeType == XmlNodeType.Element && reader2.Name == "FeedItem")) { tempds = empty; }
                  if (valueAdded == 0) { dr = tempds.NewRow(); }
                  if (reader2.NodeType == XmlNodeType.Element && reader2.Name == "Value" || reader2.Name == "Description" || reader2.Name == "Units")
                  {
                      //if (reader2.Name == "Value") { reader2.Read(); dr[1] = string.Format("{0:0.0}", reader2.Value.ToString()); valueAdded++; }//lader ikke til at virke
                      if (reader2.Name == "Value") { 
                          reader2.Read(); 
                          dr[1] = (reader2.Value.ToString());
                          int errorCounter = Regex.Matches(reader2.Value.ToString(), @"[a-zA-Z]").Count;
                          if (errorCounter==0)
                          dr[1] = Math.Round(double.Parse(dr[1].ToString()),3);
                          
                          valueAdded++; 
                      }
                      if (reader2.Name == "Description") { reader2.Read(); dr[0] = (reader2.Value.ToString()); valueAdded++; }
                      if (reader2.Name == "Units") { reader2.Read(); dr[2] = (reader2.Value.ToString()); valueAdded++; }
                  }
                  else { reader2.Read(); }
                  if (valueAdded == 3)
                  { 
                      tempds.Rows.Add(dr);
                      valueAdded = 0;
                  }
              }

          }
          reader2.Close();

          if (balance == "N")
          {
              GridView1.DataSource = ds1;
              //string.Format("{0:0.0}", Convert.ToDouble(ds1.Columns[1].Table));
              GridView1.DataBind();
              //string.Format("{0:0.0}",Convert.ToDouble(GridView1.DataKeys[1].Value));
          }
          if (balance == "C")
          {
              GridView1.DataSource = ds;
              GridView1.Caption = "FarmCBalance";
              GridView1.DataBind();
          }
          if (balance == "GHG")
          {
              GridView2.DataSource = ds2;
              GridView2.DataBind();
              GridView3.DataSource = ds3;
              GridView3.DataBind();
          }
         
      }
  
    
   }
}            