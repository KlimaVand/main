using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Xml;
using System.IO;
using System.Xml.Serialization;
using System.Globalization;

namespace AnimalChange_ModelInterface
{
    public partial class ShowResult : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            string fileName = "outputFarm23456ScenarioNr2";
            fileName = Request.QueryString["filename"];
            string balance = Request.QueryString["balance"];
            if (balance == null) { balance = "N"; }
            if (fileName == null) { fileName = " outputFarm23456ScenarioNr1.xml"; }
            try
            {
                //Label1.Text = "Name of xml-file: "+fileName ;
                ShowResultXml("C://upload/AnimalChange/ModelOutput/" + fileName, balance );

            }
            catch
            {
                try
                {
                    //Label1.Text = "Name of xml-file: "+fileName ;
                    ShowResultXml("C://upload/AnimalChange/ModelOutput/ " + fileName, balance);

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
            XmlReader reader2 = XmlReader.Create(xmlPath,setting);
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
                      while (reader2.NodeType == XmlNodeType.Element || reader2.NodeType == XmlNodeType.Text && (reader2.Name != "Farm" || reader2.Name != "FarmBalance" ))
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
                                if (reader2.Name == "Value") { reader2.Read(); dr[1] = (reader2.Value.ToString()); valueAdded++; }
                                if (reader2.Name == "Description") { reader2.Read(); dr[0] = (reader2.Value.ToString()); valueAdded++; }
                                if (reader2.Name == "Units") { reader2.Read(); dr[2] = (reader2.Value.ToString()); valueAdded++; }
                                }
                             else{reader2.Read();}
                            if (valueAdded==3)
                            { tempds.Rows.Add(dr); valueAdded = 0; }
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
            //GridView1.RowHeaderColumn.;
            //GridView1.Columns[0].ItemStyle.HorizontalAlign = System.Web.UI.WebControls.HorizontalAlign.Left;
            //reader2.Close();
            ////XmlReaderSettings setting = new XmlReaderSettings();
            //XmlDocument NBalance = new XmlDocument();
            //XmlNode elem1 = NBalance.CreateNode(XmlNodeType.Element, "FarmCBalance", null);
            //XmlNode elem2 = NBalance.CreateNode(XmlNodeType.Element, "FarmNBalance", null);
            //XmlNode elem3 = NBalance.CreateNode(XmlNodeType.Element, "FarmDirectGHG", null);
            //XmlNode elem4 = NBalance.CreateNode(XmlNodeType.Element, "FarmIndirectGHG", null);
            //elem1.InnerXml = "<FarmNBalance></FarmNBalance>";
            ////string text = "<xml><error></error>";
            //setting.IgnoreWhitespace = true;
            //XmlReader reader = XmlReader.Create(xmlPath, setting);
            //while (reader.Read())
            //{
            //    if (reader.NodeType == XmlNodeType.Element && (reader.Name == "FarmCBalance"))
            //    { elem1.InnerXml = reader.ReadInnerXml(); }
            //    if (reader.NodeType == XmlNodeType.Element && (reader.Name == "FarmNBalance"))
            //    { elem2.InnerXml = reader.ReadInnerXml(); }
            //    if (reader.NodeType == XmlNodeType.Element && (reader.Name == "FarmDirectGHG"))
            //    { elem3.InnerXml = reader.ReadInnerXml(); }
            //    if (reader.NodeType == XmlNodeType.Element && (reader.Name == "FarmIndirectGHG"))
            //    { elem4.InnerXml = reader.ReadInnerXml(); }
            //}
            //reader.Close();
            //if (balance == "N")
            //{
            //    GridView1.DataSource = ds1;// elem2;
            //    GridView1.DataBind();
            //}
            //if (balance == "C")
            //{
            //    GridView1.DataSource = elem1;
            //    GridView1.Caption = "FarmCBalance";
            //    GridView1.DataBind();
            //}
            //if (balance == "GHG")
            //{
            //    GridView2.DataSource = elem3;
            //    GridView2.DataBind();
            //    GridView3.DataSource = elem4;
            //    GridView3.DataBind();
            //}
        }
    }
}
