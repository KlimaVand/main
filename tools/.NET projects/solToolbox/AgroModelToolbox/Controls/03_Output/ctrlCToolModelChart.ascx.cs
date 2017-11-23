using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;
using CToolModel;
using System.Data;
using ChartDirector;




namespace AgroModelToolbox.Controls._03_Output
{
    public partial class ctrlCtoolModelChart : System.Web.UI.UserControl
    {
        private double[] dataX;
        private double[] dataY;
        private double[] dataY1;
        CToolModel.CTool test = new CToolModel.CTool();
        System.Data.DataSet resultSet = new System.Data.DataSet();
        System.Data.DataSet resultSetAlternative = new System.Data.DataSet();
        double PLoweLayer, offset, depth;
        double PupperLayer, C14percent, startAmountOfCarbon;

        double CNratio, extraCarbon, HUMdecompositionratePlant;
        double FOMdecompositionratePlant, ClayfractionPlant, tFPlant;

        double ROMfractionPlant, ROMdecompositionratePlant, HUMdecompositionrateManure;
        double HUMfractionManure, FOMdecompositionrateManure, ClayfractionManure;
        double tFManure, ROMfractionManure, ROMdecompositionrateManure;




        double HUMdecompositionratePlantC14, FOMdecompositionratePlantC14, ClayfractionPlantC14;
        double tFPlantC14, ROMfractionPlantC14, ROMdecompositionratePlantC14;
        double DecayRateC14Plant, HUMdecompositionrateManureC14, HUMfractionManureC14;
        double FOMdecompositionrateManureC14, ClayfractionManureC14, tFManureC14;

        double ROMfractionManureC14, ROMdecompositionrateManureC14, DecayRateC14Manuer;
        List<CToolModel.CTool.Data> allData = new List<CToolModel.CTool.Data>();
        List<double> TemperatureData = new List<double>();
        string color="red";
        ScatterLayer aScatterLayer;
        ScatterLayer aScatterLayer1;
        string scenario = null;
        string scenarioAlternative = null;
        string scenarioInput = null;
        string scenarioAlternativeInput = null;
        string text = null;
        string textAlternative = null;
//        char [] delimiterArr;

        protected void Page_Load(object sender, EventArgs e)
        {
            LabelMessage.Text = "";
            scenario = DropDownList1.SelectedValue;
            scenarioAlternative = DropDownList2.SelectedValue;
            try
            {
                text = System.IO.File.ReadAllText(@"C:\UploadNET\CtoolInput\input_" + scenario + ".txt");
                textAlternative = System.IO.File.ReadAllText(@"C:\UploadNET\CtoolInput\input_" + scenarioAlternative + ".txt");
            }
            catch
            {
                LabelMessage.Text = "cannot read the input.txt files";
                Environment.Exit(0);
            }
            char[] delimiterArr = { '\r' };
            string[] data = text.Split(delimiterArr);
            string[] dataAlternative = textAlternative.Split('\r');

            string[] lines = null;
            string[] linesAlternative = null;
            try
            {
                lines = System.IO.File.ReadAllLines(@"C:\UploadNET\CtoolInput\data_" + scenario + ".txt");
                linesAlternative = System.IO.File.ReadAllLines(@"C:\UploadNET\CtoolInput\data_" + scenarioAlternative + ".txt");
               // lines = System.IO.File.ReadAllLines(@"C:\UploadNET\CtoolInput\data_4.txt");
               // linesAlternative = System.IO.File.ReadAllLines(@"C:\UploadNET\CtoolInput\data_4.txt");
            }
            catch
            {
                LabelMessage.Text = "cannot read the data.txt file";
                Environment.Exit(0);
            }
            string[] TemperatureLines = null;
            string[] TemperatureLinesAlternative = null;
            try
            {
                TemperatureLines = System.IO.File.ReadAllLines(@"C:\UploadNET\CtoolInput\temperature_" + scenario + ".txt");
                TemperatureLinesAlternative = System.IO.File.ReadAllLines(@"C:\UploadNET\CtoolInput\temperature_" + scenarioAlternative + ".txt");
            }
            catch
            {
                LabelMessage.Text = "cannot read the temperature.txt file";
                Environment.Exit(0);
            }


            string[] mode = null;
            try
            {
                mode = System.IO.File.ReadAllLines(@"C:\UploadNET\CtoolInput\mode.txt");
            }
            catch
            {
                LabelMessage.Text = "cannot read the mode.txt file";
                Environment.Exit(0);
            }
            FillInput(data, lines, TemperatureLines);
            resultSet = test.Main(TemperatureData, allData, DecayRateC14Manuer, ROMdecompositionrateManureC14, ROMfractionManureC14, tFManureC14, ClayfractionManureC14, FOMdecompositionrateManureC14, HUMfractionManureC14, HUMdecompositionrateManureC14, DecayRateC14Plant, ROMdecompositionratePlantC14, ROMfractionPlantC14, tFPlantC14, ClayfractionPlantC14, FOMdecompositionratePlantC14, ROMdecompositionrateManure, ROMfractionManure, tFManure, ClayfractionManure, FOMdecompositionrateManure, HUMfractionManure, HUMdecompositionrateManure, ROMfractionPlant, tFPlant, ClayfractionPlant, FOMdecompositionratePlant, PLoweLayer, offset, depth, PupperLayer, C14percent, startAmountOfCarbon, CNratio, extraCarbon, HUMdecompositionratePlant, ROMdecompositionratePlant, ROMfractionPlantC14, HUMdecompositionratePlantC14, Convert.ToInt32(mode[0]));
            resultSet.DataSetName = DropDownList1.SelectedItem.Text;
            FillInput(dataAlternative, linesAlternative, TemperatureLinesAlternative);
            resultSetAlternative = test.Main(TemperatureData, allData, DecayRateC14Manuer, ROMdecompositionrateManureC14, ROMfractionManureC14, tFManureC14, ClayfractionManureC14, FOMdecompositionrateManureC14, HUMfractionManureC14, HUMdecompositionrateManureC14, DecayRateC14Plant, ROMdecompositionratePlantC14, ROMfractionPlantC14, tFPlantC14, ClayfractionPlantC14, FOMdecompositionratePlantC14, ROMdecompositionrateManure, ROMfractionManure, tFManure, ClayfractionManure, FOMdecompositionrateManure, HUMfractionManure, HUMdecompositionrateManure, ROMfractionPlant, tFPlant, ClayfractionPlant, FOMdecompositionratePlant, PLoweLayer, offset, depth, PupperLayer, C14percent, startAmountOfCarbon, CNratio, extraCarbon, HUMdecompositionratePlant, ROMdecompositionratePlant, ROMfractionPlantC14, HUMdecompositionratePlantC14, Convert.ToInt32(mode[0]));
            resultSetAlternative.DataSetName = DropDownList2.SelectedItem.Text;
//            LabelMessage.Text = "Model run is OK";
            CreateChart(1, resultSet);
            if (color.Equals("blue"))
                color = "red";
            else
                color = "blue";
            CreateChart(2, resultSetAlternative);
            RadGrid1.DataSource = resultSet.Tables[2];
            RadGrid1.DataBind();
            RadGrid2.DataSource = resultSetAlternative.Tables[2];
            RadGrid2.DataBind();
        }

        protected void OK_Click(object sender, EventArgs e)
        {
            //LabelMessage.Text = "";
            //scenario = DropDownList1.SelectedValue;
            //scenarioAlternative = DropDownList2.SelectedValue;
            //text = null;
            //textAlternative = null;
            //try
            //{
            //    text = System.IO.File.ReadAllText(@"C:\UploadNET\CtoolInput\input_" + scenario + ".txt");
            //    textAlternative = System.IO.File.ReadAllText(@"C:\UploadNET\CtoolInput\input_" + scenarioAlternative + ".txt");
            //}
            //catch
            //{
            //    LabelMessage.Text = "cannot read the input.txt files";
            //    Environment.Exit(0);
            //}
            //char [] delimiterArr = {'\r','\t'};

            //string[] data = text.Split(delimiterArr);
            //string[] dataAlternative = textAlternative.Split('\r');

            //string[] lines = null;
            //string[] linesAlternative = null;
            //try
            //{
            //    lines = System.IO.File.ReadAllLines(@"C:\UploadNET\CtoolInput\data_" + scenario + ".txt");
            //    linesAlternative = System.IO.File.ReadAllLines(@"C:\UploadNET\CtoolInput\data_" + scenarioAlternative + ".txt");
            //}
            //catch
            //{
            //    LabelMessage.Text = "cannot read the data.txt file";
            //    Environment.Exit(0);
            //}
            //string[] TemperatureLines = null;
            //string[] TemperatureLinesAlternative = null;
            //try
            //{
            //    TemperatureLines = System.IO.File.ReadAllLines(@"C:\UploadNET\CtoolInput\temperature_" + scenario + ".txt");
            //    TemperatureLinesAlternative = System.IO.File.ReadAllLines(@"C:\UploadNET\CtoolInput\temperature_" + scenarioAlternative + ".txt");
            //}
            //catch
            //{
            //    LabelMessage.Text = "cannot read the temperature.txt file";
            //    Environment.Exit(0);
            //}


            //string[] mode = null;
            //try
            //{
            //    mode = System.IO.File.ReadAllLines(@"C:\UploadNET\CtoolInput\mode.txt");
            //}
            //catch
            //{
            //    LabelMessage.Text = "cannot read the mode.txt file";
            //    Environment.Exit(0);
            //}
            //FillInput(data,lines,TemperatureLines);
            //resultSet = test.Main(TemperatureData, allData, DecayRateC14Manuer, ROMdecompositionrateManureC14, ROMfractionManureC14, tFManureC14, ClayfractionManureC14, FOMdecompositionrateManureC14, HUMfractionManureC14, HUMdecompositionrateManureC14, DecayRateC14Plant, ROMdecompositionratePlantC14, ROMfractionPlantC14, tFPlantC14, ClayfractionPlantC14, FOMdecompositionratePlantC14, ROMdecompositionrateManure, ROMfractionManure, tFManure, ClayfractionManure, FOMdecompositionrateManure, HUMfractionManure, HUMdecompositionrateManure, ROMfractionPlant, tFPlant, ClayfractionPlant, FOMdecompositionratePlant, PLoweLayer, offset, depth, PupperLayer, C14percent, startAmountOfCarbon, CNratio, extraCarbon, HUMdecompositionratePlant, ROMdecompositionratePlant, ROMfractionPlantC14, HUMdecompositionratePlantC14, Convert.ToInt32(mode[0]));
            //resultSet.DataSetName = DropDownList1.SelectedItem.Text;
            //FillInput(dataAlternative, linesAlternative, TemperatureLinesAlternative);
            //resultSetAlternative = test.Main(TemperatureData, allData, DecayRateC14Manuer, ROMdecompositionrateManureC14, ROMfractionManureC14, tFManureC14, ClayfractionManureC14, FOMdecompositionrateManureC14, HUMfractionManureC14, HUMdecompositionrateManureC14, DecayRateC14Plant, ROMdecompositionratePlantC14, ROMfractionPlantC14, tFPlantC14, ClayfractionPlantC14, FOMdecompositionratePlantC14, ROMdecompositionrateManure, ROMfractionManure, tFManure, ClayfractionManure, FOMdecompositionrateManure, HUMfractionManure, HUMdecompositionrateManure, ROMfractionPlant, tFPlant, ClayfractionPlant, FOMdecompositionratePlant, PLoweLayer, offset, depth, PupperLayer, C14percent, startAmountOfCarbon, CNratio, extraCarbon, HUMdecompositionratePlant, ROMdecompositionratePlant, ROMfractionPlantC14, HUMdecompositionratePlantC14, Convert.ToInt32(mode[0]));
            //resultSetAlternative.DataSetName = DropDownList2.SelectedItem.Text;
            //LabelMessage.Text = "Model run is OK";
            //CreateChart(1,resultSet);
            //if (color.Equals("blue"))
            //    color = "red";
            //else
            //    color = "blue"; 
            //CreateChart(2, resultSetAlternative);
            //RadGrid1.DataSource = resultSet.Tables[2];
            //RadGrid1.DataBind();
            //RadGrid2.DataSource = resultSetAlternative.Tables[2];
            //RadGrid2.DataBind();
            Page_Load(sender, e);

        }
        void FillInput(string[] data, string[] lines, string[] TemperatureLines)
        {
            string type = "";
            for (int i = 0; i < data.Length ; i++)
            {
                if (data[i].Contains("PLoweLayer"))
                    PLoweLayer = Convert.ToDouble(data[i].Split('\t')[1]);
                if (data[i].Contains("offset"))
                    offset = Convert.ToDouble(data[i].Split('\t')[1]);
                if (data[i].Contains("depth"))
                    depth = Convert.ToDouble(data[i].Split('\t')[1]);
                if (data[i].Contains("PupperLayer"))
                    PupperLayer = Convert.ToDouble(data[i].Split('\t')[1]);
                if (data[i].Contains("Initial PMC(%)"))
                    C14percent = Convert.ToDouble(data[i].Split('\t')[1]);
                if (data[i].Contains("Initial C(t/ha)"))
                    startAmountOfCarbon = Convert.ToDouble(data[i].Split('\t')[1]);
                if (data[i].Contains("C/N"))
                    CNratio = Convert.ToDouble(data[i].Split('\t')[1]);
                if (data[i].Contains("Amended C"))
                    extraCarbon = Convert.ToDouble(data[i].Split('\t')[1]);
                if (data[i].Equals("\nCrop\t"))
                    type="Crop";
                if (data[i].Equals("\nManure\t"))
                    type = "Manure";
                if (data[i].Equals("\nCropC14\t"))
                    type = "CropC14";
                if (data[i].Equals("\nManureC14\t"))
                    type = "ManureC14";
                if ((data[i].Contains("HUMdecompositionrate") && (type.Equals("Crop"))))
                    HUMdecompositionratePlant = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("FOMdecompositionrate") && (type.Equals("Crop"))))
                    FOMdecompositionratePlant = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("clayfraction") && (type.Equals("Crop"))))
                    ClayfractionPlant = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("tF") && (type.Equals("Crop"))))
                    tFPlant = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("ROMfraction") && (type.Equals("Crop"))))
                    ROMfractionPlant = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("ROMdecompositionrate") && (type.Equals("Crop"))))
                    ROMdecompositionratePlant = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("HUMdecompositionrate") && (type.Equals("Manure"))))
                    HUMdecompositionrateManure = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("HUMfraction") && (type.Equals("Manure"))))
                    HUMfractionManure = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("FOMdecompositionrate") && (type.Equals("Manure"))))
                    FOMdecompositionrateManure = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("clayfraction") && (type.Equals("Manure"))))
                    ClayfractionManure = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("tF") && (type.Equals("Manure"))))
                    tFManure = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("ROMfraction") && (type.Equals("Manure"))))
                    ROMfractionManure = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("ROMdecompositionrate") && (type.Equals("Manure"))))
                    ROMdecompositionrateManure = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("HUMdecompositionrate") && (type.Equals("CropC14"))))
                    HUMdecompositionratePlantC14 = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("FOMdecompositionrate") && (type.Equals("CropC14"))))
                    FOMdecompositionratePlantC14 = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("clayfraction") && (type.Equals("CropC14"))))
                    ClayfractionPlantC14 = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("tF") && (type.Equals("CropC14"))))
                    tFPlantC14 = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("ROMfraction") && (type.Equals("CropC14"))))
                    ROMfractionPlantC14 = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("ROMdecompositionrate") && (type.Equals("CropC14"))))
                    ROMdecompositionratePlantC14 = Convert.ToDouble(data[39].Split('\t')[1]);
                if ((data[i].Contains("decay rate") && (type.Equals("CropC14"))))
                    DecayRateC14Plant = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("HUMdecompositionrate") && (type.Equals("ManureC14"))))
                    HUMdecompositionrateManureC14 = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("HUMfraction") && (type.Equals("ManureC14"))))
                    HUMfractionManureC14 = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("FOMdecompositionrate") && (type.Equals("ManureC14"))))
                    FOMdecompositionrateManureC14 = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("clayfraction") && (type.Equals("ManureC14"))))
                    ClayfractionManureC14 = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("tF") && (type.Equals("ManureC14"))))
                    tFManureC14 = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("ROMfraction") && (type.Equals("ManureC14"))))
                    ROMfractionManureC14 = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("ROMdecompositionrate") && (type.Equals("ManureC14"))))
                    ROMdecompositionrateManureC14 = Convert.ToDouble(data[i].Split('\t')[1]);
                if ((data[i].Contains("decay rate") && (type.Equals("ManureC14"))))
                    DecayRateC14Manuer = Convert.ToDouble(data[i].Split('\t')[1]);


            }


            //PLoweLayer = Convert.ToDouble(data[1].Split('\t')[1]);
            //offset = Convert.ToDouble(data[2].Split('\t')[1]);
            //depth = Convert.ToDouble(data[3].Split('\t')[1]);
            //PupperLayer = Convert.ToDouble(data[4].Split('\t')[1]);
            //C14percent = Convert.ToDouble(data[5].Split('\t')[1]);

            //startAmountOfCarbon = Convert.ToDouble(data[6].Split('\t')[1]);

            //CNratio = Convert.ToDouble(data[7].Split('\t')[1]);

            //extraCarbon = Convert.ToDouble(data[8].Split('\t')[1]);




            //HUMdecompositionratePlant = Convert.ToDouble(data[11].Split('\t')[1]);
            //FOMdecompositionratePlant = Convert.ToDouble(data[13].Split('\t')[1]);
            //ClayfractionPlant = Convert.ToDouble(data[14].Split('\t')[1]);
            //tFPlant = Convert.ToDouble(data[15].Split('\t')[1]);

            //ROMfractionPlant = Convert.ToDouble(data[17].Split('\t')[1]);
            //ROMdecompositionratePlant = Convert.ToDouble(data[18].Split('\t')[1]);


            //HUMdecompositionrateManure = Convert.ToDouble(data[21].Split('\t')[1]);
            //HUMfractionManure = Convert.ToDouble(data[22].Split('\t')[1]);

            //FOMdecompositionrateManure = Convert.ToDouble(data[24].Split('\t')[1]);
            //ClayfractionManure = Convert.ToDouble(data[25].Split('\t')[1]);
            //tFManure = Convert.ToDouble(data[26].Split('\t')[1]);

            //ROMfractionManure = Convert.ToDouble(data[28].Split('\t')[1]);
            //ROMdecompositionrateManure = Convert.ToDouble(data[29].Split('\t')[1]);




            //HUMdecompositionratePlantC14 = Convert.ToDouble(data[11].Split('\t')[1]);
            //FOMdecompositionratePlantC14 = Convert.ToDouble(data[34].Split('\t')[1]);
            //ClayfractionPlantC14 = Convert.ToDouble(data[35].Split('\t')[1]);
            //tFPlantC14 = Convert.ToDouble(data[36].Split('\t')[1]);

            //ROMfractionPlantC14 = Convert.ToDouble(data[38].Split('\t')[1]);
            //ROMdecompositionratePlantC14 = Convert.ToDouble(data[39].Split('\t')[1]);
            //DecayRateC14Plant = Convert.ToDouble(data[40].Split('\t')[1]);

            //HUMdecompositionrateManureC14 = Convert.ToDouble(data[43].Split('\t')[1]);
            //HUMfractionManureC14 = Convert.ToDouble(data[44].Split('\t')[1]);
            //FOMdecompositionrateManureC14 = Convert.ToDouble(data[46].Split('\t')[1]);
            //ClayfractionManureC14 = Convert.ToDouble(data[47].Split('\t')[1]);
            //tFManureC14 = Convert.ToDouble(data[48].Split('\t')[1]);

            //ROMfractionManureC14 = Convert.ToDouble(data[50].Split('\t')[1]);
            //ROMdecompositionrateManureC14 = Convert.ToDouble(data[51].Split('\t')[1]);
            //DecayRateC14Manuer = Convert.ToDouble(data[52].Split('\t')[1]);
            for (int j = 1; j < lines.Count(); j++)
            {
                string[] items = lines[j].Split('\t');
                CToolModel.CTool.Data item = new CToolModel.CTool.Data();
                string tmp = items[0];
                if (tmp.CompareTo("") != 0)
                {
                    item.year = Convert.ToInt32(items[0]);
                    item.UpperLayerC = Convert.ToDouble(items[1]);
                    item.LowerLayerC = Convert.ToDouble(items[2]);
                    item.ManureC = Convert.ToDouble(items[3]);
                    item.C14PlantProcent = Convert.ToDouble(items[4]);
                    item.C14Manure = Convert.ToDouble(items[5]);
                    allData.Add(item);
                }
            }
            for (int j = 0; j < TemperatureLines.Count(); j++)
            {
                TemperatureData.Add(Convert.ToDouble(TemperatureLines[j]));
            }
        }
        private void CreateChart(int number, System.Data.DataSet set)
        {

            Chart.setLicenseCode("DEVP-2A2N-7A2A-ZQFB-77D9-D646");
            string chartName = set.DataSetName;
            dataX = new double[26];
            dataX[0] = 0;
            dataX[1] = 2;
            dataX[2] = 4;
            dataX[3] = 6;
            dataX[4] = 8;
            dataX[5] = 10;
            dataX[6] = 12;
            dataX[7] = 14;
            dataX[8] = 16;
            dataX[9] = 18;
            dataX[10] = 20;
            dataX[11] = 22;
            dataX[12] = 24;
            dataX[13] = 26;
            dataX[14] = 28;
            dataX[15] = 30;
            dataX[16] = 32;
            dataX[17] = 34;
            dataX[18] = 36;
            dataX[19] = 38;
            dataX[20] = 40;
            dataX[21] = 42;
            dataX[22] = 44;
            dataX[23] = 46;
            dataX[24] = 48;
            dataX[25] = 50;
            dataY = new double[dataX.Length];
            dataY1 = new double[dataX.Length];
            dataY[0] = Convert.ToDouble(set.Tables["totalAmount"].Rows[368].ItemArray[12]);
            dataY[1] = Convert.ToDouble(set.Tables["totalAmount"].Rows[380].ItemArray[12]);
            dataY[2] = Convert.ToDouble(set.Tables["totalAmount"].Rows[392].ItemArray[12]);
            dataY[3] = Convert.ToDouble(set.Tables["totalAmount"].Rows[404].ItemArray[12]);
            dataY[4] = Convert.ToDouble(set.Tables["totalAmount"].Rows[416].ItemArray[12]);
            dataY[5] = Convert.ToDouble(set.Tables["totalAmount"].Rows[428].ItemArray[12]);
            dataY[6] = Convert.ToDouble(set.Tables["totalAmount"].Rows[440].ItemArray[12]);
            dataY[7] = Convert.ToDouble(set.Tables["totalAmount"].Rows[452].ItemArray[12]);
            dataY[8] = Convert.ToDouble(set.Tables["totalAmount"].Rows[464].ItemArray[12]);
            dataY[9] = Convert.ToDouble(set.Tables["totalAmount"].Rows[476].ItemArray[12]);
            dataY[10] = Convert.ToDouble(set.Tables["totalAmount"].Rows[488].ItemArray[12]);
            dataY[11] = Convert.ToDouble(set.Tables["totalAmount"].Rows[500].ItemArray[12]);
            dataY[12] = Convert.ToDouble(set.Tables["totalAmount"].Rows[512].ItemArray[12]);
            dataY[13] = Convert.ToDouble(set.Tables["totalAmount"].Rows[524].ItemArray[12]);
            dataY[14] = Convert.ToDouble(set.Tables["totalAmount"].Rows[536].ItemArray[12]);
            dataY[15] = Convert.ToDouble(set.Tables["totalAmount"].Rows[548].ItemArray[12]);
            dataY[16] = Convert.ToDouble(set.Tables["totalAmount"].Rows[560].ItemArray[12]);
            dataY[17] = Convert.ToDouble(set.Tables["totalAmount"].Rows[572].ItemArray[12]);
            dataY[18] = Convert.ToDouble(set.Tables["totalAmount"].Rows[584].ItemArray[12]);
            dataY[19] = Convert.ToDouble(set.Tables["totalAmount"].Rows[596].ItemArray[12]);
            dataY[20] = Convert.ToDouble(set.Tables["totalAmount"].Rows[608].ItemArray[12]);
            dataY[21] = Convert.ToDouble(set.Tables["totalAmount"].Rows[620].ItemArray[12]);
            dataY[22] = Convert.ToDouble(set.Tables["totalAmount"].Rows[632].ItemArray[12]);
            dataY[23] = Convert.ToDouble(set.Tables["totalAmount"].Rows[644].ItemArray[12]);
            dataY[24] = Convert.ToDouble(set.Tables["totalAmount"].Rows[656].ItemArray[12]);
            dataY[25] = Convert.ToDouble(set.Tables["totalAmount"].Rows[668].ItemArray[12]);

            dataY1[0] = Convert.ToDouble(set.Tables["totalAmount"].Rows[368].ItemArray[27]);
            dataY1[1] = Convert.ToDouble(set.Tables["totalAmount"].Rows[380].ItemArray[27]);
            dataY1[2] = Convert.ToDouble(set.Tables["totalAmount"].Rows[392].ItemArray[27]);
            dataY1[3] = Convert.ToDouble(set.Tables["totalAmount"].Rows[404].ItemArray[27]);
            dataY1[4] = Convert.ToDouble(set.Tables["totalAmount"].Rows[416].ItemArray[27]);
            dataY1[5] = Convert.ToDouble(set.Tables["totalAmount"].Rows[428].ItemArray[27]);
            dataY1[6] = Convert.ToDouble(set.Tables["totalAmount"].Rows[440].ItemArray[27]);
            dataY1[7] = Convert.ToDouble(set.Tables["totalAmount"].Rows[452].ItemArray[27]);
            dataY1[8] = Convert.ToDouble(set.Tables["totalAmount"].Rows[464].ItemArray[27]);
            dataY1[9] = Convert.ToDouble(set.Tables["totalAmount"].Rows[476].ItemArray[27]);
            dataY1[10] = Convert.ToDouble(set.Tables["totalAmount"].Rows[488].ItemArray[27]);
            dataY1[11] = Convert.ToDouble(set.Tables["totalAmount"].Rows[500].ItemArray[27]);
            dataY1[12] = Convert.ToDouble(set.Tables["totalAmount"].Rows[512].ItemArray[27]);
            dataY1[13] = Convert.ToDouble(set.Tables["totalAmount"].Rows[524].ItemArray[27]);
            dataY1[14] = Convert.ToDouble(set.Tables["totalAmount"].Rows[536].ItemArray[27]);
            dataY1[15] = Convert.ToDouble(set.Tables["totalAmount"].Rows[548].ItemArray[27]);
            dataY1[16] = Convert.ToDouble(set.Tables["totalAmount"].Rows[560].ItemArray[27]);
            dataY1[17] = Convert.ToDouble(set.Tables["totalAmount"].Rows[572].ItemArray[27]);
            dataY1[18] = Convert.ToDouble(set.Tables["totalAmount"].Rows[584].ItemArray[27]);
            dataY1[19] = Convert.ToDouble(set.Tables["totalAmount"].Rows[596].ItemArray[27]);
            dataY1[20] = Convert.ToDouble(set.Tables["totalAmount"].Rows[608].ItemArray[27]);
            dataY1[21] = Convert.ToDouble(set.Tables["totalAmount"].Rows[620].ItemArray[27]);
            dataY1[22] = Convert.ToDouble(set.Tables["totalAmount"].Rows[632].ItemArray[27]);
            dataY1[23] = Convert.ToDouble(set.Tables["totalAmount"].Rows[644].ItemArray[27]);
            dataY1[24] = Convert.ToDouble(set.Tables["totalAmount"].Rows[656].ItemArray[27]);
            dataY1[25] = Convert.ToDouble(set.Tables["totalAmount"].Rows[668].ItemArray[27]);


            XYChart c = new XYChart(460, 460, 0xFFFFCC, 0x000000);
            c.setRoundedFrame();
            c.addLegend(5,5);
            c.addTitle("<*br*>" + chartName, "arialbd.ttf", 12);
            c.setPlotArea(80, 80, 300, 300, 0xFFFFFF);

            c.xAxis().setTitle("Years");
            c.xAxis().setLinearScale(0, 50, 5);

            c.yAxis().setTitle("Soil organic carbon [t/ha]");
            c.yAxis().setLinearScale(0, 400, 50);

            if (color=="blue")
            {aScatterLayer = c.addScatterLayer(dataX, dataY, "Organic C in topsoil", Chart.CircleSymbol, 1, 0x0000ff, Chart.Transparent);}
            else
            { aScatterLayer = c.addScatterLayer(dataX, dataY, "Organic C in topsoil", Chart.CircleSymbol, 1, 0xff0000, Chart.Transparent); }

            aScatterLayer1 = c.addScatterLayer(dataX, dataY1, "Organic C in subsoil", Chart.CircleSymbol, 1, 0x00ff00, Chart.Transparent);
            aScatterLayer.setLineWidth(2);
            aScatterLayer1.setLineWidth(2);
            switch (number)
            {
                case 1:
                    WebChartViewerCToolModelChart_1.Image = c.makeWebImage(Chart.PNG);
                    break;
                case 2:
                    WebChartViewerCToolModelChart_2.Image = c.makeWebImage(Chart.PNG);
                    break;

            }
        }
    }
}