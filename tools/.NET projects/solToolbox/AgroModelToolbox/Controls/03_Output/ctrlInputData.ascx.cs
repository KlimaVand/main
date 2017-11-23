using System;
using ChartDirector;
using simpleModel;
using CToolModel;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;
using System.Data;
using System.Data.SqlClient;
using AgroModelToolbox.Class;


namespace AgroModelToolbox.Controls._03_Output
{
    public partial class ctrlInputData : System.Web.UI.UserControl
    {
        private string con;
        private string strSelectCommand;
        private static double[] dataX = new double[23];
        private double[] dataY, dataY2;
        private simpleModel.simpleModel simpleModel = new simpleModel.simpleModel();
        private double N_org, N_fert, CN3060, a1, a2, a3, C_30, CN30, C3060, Nr, Nc, C_10, Yp, Clay_10, NRE, NRR, NPCT, EP_season, P_season;
        private double CN3060_2, C_30_2, CN30_2, C3060_2, Nr_2, Nc_2, Yp_2, NRE_2, NRR_2, NPCT_2, EP_season_2, P_season_2;
        private double N_total, Norg_share, CN2550, C25, CN25, C2550, Clay_5, Temp, C_resid, D, fx, RTlogic, ypl;//additional and renamed parameters 19/1/15
        private double Norg_share_2, CN2550_2, C25_2, CN25_2, C2550_2, Clay_5_2, Temp_2, C_resid_2, D_2, fx_2, RTlogic_2, ypl_2;//additional and renamed parameters 19/1/15

        private double[] dataX_CT;
        private double[] dataY_CT;
        private double[] dataY1_CT;
        private double[] dataY_CT_alt;
        private double[] dataY1_CT_alt;
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
        string color = "red";
        ScatterLayer aScatterLayer;
        ScatterLayer aScatterLayer1;
        string scenario = null;
        string scenarioAlternative = null;
        string scenarioInput = null;
        string scenarioAlternativeInput = null;
        string text = null;
        string textAlternative = null;
        int Grid_Code;
        double BULKDA, BULKDB, SOC_percent, SOC, SOC_sub_percent, SOC_sub, SOC1_percent, SOC1, SOC1_sub_percent, SOC1_sub;

        protected void Page_Load(object sender, EventArgs e)
        {


            a1 = 0.0049;
            a2 = 0.12;
            a3 = 0.3;
            Clay_5 = 21;
            Clay_5_2 = 21;
            Temp = 9; //Fra lookup_Climate
            Temp_2 = 9; //Fra lookup_Climate
            C_resid = 4; //Kommer som delprodukt af de allometriske beregninger til C-Tool 
            C_resid_2 = 4; //Kommer som delprodukt af de allometriske beregninger til C-Tool 
            D = 5;// Convert.ToDouble(TextBox_D.Text.ToString()); //User-defined. Hvis reduceret jordbearbejdning: Jordbearbejdningsdybde i cm.
            D_2 = 5;// Convert.ToDouble(TextBox_D_2.Text.ToString()); //User-defined. Hvis reduceret jordbearbejdning: Jordbearbejdningsdybde i cm.
            fx = 2;
            fx_2 = 2;
            RTlogic = Convert.ToDouble(DropDown_RTlogic.SelectedValue.ToString()); //User-defined: Alm. jordbearbejdning = 0, Reduceret jordbearbejdning = 1, No-tillage = 2;
            RTlogic_2 = Convert.ToDouble(DropDown_RTlogic_2.SelectedValue.ToString()); //User-defined: Alm. jordbearbejdning = 0, Reduceret jordbearbejdning = 1, No-tillage = 2;
            ypl = 20;// Convert.ToDouble(DropDown_Yp.SelectedValue.ToString()); ;
            ypl_2 = 20;// Convert.ToDouble(DropDown_Yp_2.SelectedValue.ToString());

            C25 = 3;//Convert.ToDouble(Dropdown_C30.SelectedValue.ToString());
            C25_2 = 3;//Convert.ToDouble(Dropdown_C30_2.SelectedValue.ToString());
            CN25 = 12; //Convert.ToDouble(DropDown_CN30.SelectedValue.ToString());
            CN25_2 = 12;// Convert.ToDouble(DropDown_CN30_2.SelectedValue.ToString());
            C2550 = 1;// Convert.ToDouble(Dropdown_C3060.SelectedValue.ToString());
            C2550_2 = 1;// Convert.ToDouble(Dropdown_C3060_2.SelectedValue.ToString());
            CN2550 = 12;// Convert.ToDouble(DropDown_CN3060.SelectedValue.ToString());
            CN2550_2 = 12;// Convert.ToDouble(DropDown_CN3060_2.SelectedValue.ToString());
            Norg_share = 0.5;// Convert.ToDouble(DropDown_Norg_share.SelectedValue.ToString()); ;
            Norg_share_2 = 0.5;// Convert.ToDouble(DropDown_Norg_share_2.SelectedValue.ToString()); ;

            Nr = 100;// Convert.ToDouble(DropDown_Nr.SelectedValue.ToString());
            Nr_2 = 100;// Convert.ToDouble(DropDown_Nr_2.SelectedValue.ToString());
            Nc = 50;// Convert.ToDouble(DropDown_Nc.SelectedValue.ToString());
            Nc_2 = 50;// Convert.ToDouble(DropDown_Nc_2.SelectedValue.ToString());
            Yp = 9;// Convert.ToDouble(DropDown_Yp.SelectedValue.ToString()); ;
            Yp_2 = 9;// Convert.ToDouble(DropDown_Yp_2.SelectedValue.ToString());
            NRE = 0.7;// Convert.ToDouble(DropDown_NRE.SelectedValue.ToString());
            NRE_2 = 0.7;// Convert.ToDouble(DropDown_NRE_2.SelectedValue.ToString());
            NRR = 0.8;// Convert.ToDouble(DropDown_NRR.SelectedValue.ToString());
            NRR_2 = 0.8;// Convert.ToDouble(DropDown_NRR_2.SelectedValue.ToString());
            NPCT = 1.5;// Convert.ToDouble(DropDown_NPCT.SelectedValue.ToString());
            NPCT_2 = 1.5;// Convert.ToDouble(DropDown_NPCT_2.SelectedValue.ToString());
            EP_season = 300;// Convert.ToDouble(DropDown_EP_season.SelectedValue.ToString());
            EP_season_2 = 300;// Convert.ToDouble(DropDown_EP_season_2.SelectedValue.ToString());
            P_season = 200;// Convert.ToDouble(DropDown_P_season.SelectedValue.ToString());
            P_season_2 = 200;// Convert.ToDouble(DropDown_P_season_2.SelectedValue.ToString());



            LabelMessage.Text = "";

           // CreateChart();
            scenario = "1";// DropDownList1.SelectedValue;
            scenarioAlternative = "5";// DropDownList2.SelectedValue;
            BULKDA = 1.5;
            BULKDB = 1.7;

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
            startAmountOfCarbon = Convert.ToDouble(TextBox_InitC.Text.ToString());
            resultSet = test.Main(TemperatureData, allData, DecayRateC14Manuer, ROMdecompositionrateManureC14, ROMfractionManureC14, tFManureC14, ClayfractionManureC14, FOMdecompositionrateManureC14, HUMfractionManureC14, HUMdecompositionrateManureC14, DecayRateC14Plant, ROMdecompositionratePlantC14, ROMfractionPlantC14, tFPlantC14, ClayfractionPlantC14, FOMdecompositionratePlantC14, ROMdecompositionrateManure, ROMfractionManure, tFManure, ClayfractionManure, FOMdecompositionrateManure, HUMfractionManure, HUMdecompositionrateManure, ROMfractionPlant, tFPlant, ClayfractionPlant, FOMdecompositionratePlant, PLoweLayer, offset, depth, PupperLayer, C14percent, startAmountOfCarbon, CNratio, extraCarbon, HUMdecompositionratePlant, ROMdecompositionratePlant, ROMfractionPlantC14, HUMdecompositionratePlantC14, Convert.ToInt32(mode[0]));
            resultSet.DataSetName = "Test 1";// DropDownList1.SelectedItem.Text;
            FillInput(dataAlternative, linesAlternative, TemperatureLinesAlternative);
            startAmountOfCarbon = Convert.ToDouble(TextBox_InitC_2.Text.ToString());
            resultSetAlternative = test.Main(TemperatureData, allData, DecayRateC14Manuer, ROMdecompositionrateManureC14, ROMfractionManureC14, tFManureC14, ClayfractionManureC14, FOMdecompositionrateManureC14, HUMfractionManureC14, HUMdecompositionrateManureC14, DecayRateC14Plant, ROMdecompositionratePlantC14, ROMfractionPlantC14, tFPlantC14, ClayfractionPlantC14, FOMdecompositionratePlantC14, ROMdecompositionrateManure, ROMfractionManure, tFManure, ClayfractionManure, FOMdecompositionrateManure, HUMfractionManure, HUMdecompositionrateManure, ROMfractionPlant, tFPlant, ClayfractionPlant, FOMdecompositionratePlant, PLoweLayer, offset, depth, PupperLayer, C14percent, startAmountOfCarbon, CNratio, extraCarbon, HUMdecompositionratePlant, ROMdecompositionratePlant, ROMfractionPlantC14, HUMdecompositionratePlantC14, Convert.ToInt32(mode[0]));
//            resultSetAlternative.DataSetName = DropDownList2.SelectedItem.Text;
            CreateChartCtool(resultSet,resultSetAlternative);
            if (color.Equals("blue"))
                color = "red";
            else
                color = "blue";
//            C25 = dataY1_CT[15]/;
//            C2550 = 
//            CreateChart();
        }


        private void CreateChartCtool(System.Data.DataSet set, System.Data.DataSet set2)
        {
            ScatterLayer aScatterLayer;
            ScatterLayer aScatterLayer1;
            ScatterLayer aScatterLayer2;
            ScatterLayer aScatterLayer3;
            Chart.setLicenseCode("DEVP-2A2N-7A2A-ZQFB-77D9-D646");
            string chartName = set.DataSetName;
            dataX_CT = new double[16];
            dataX_CT[0] = 0;
            dataX_CT[1] = 2;
            dataX_CT[2] = 4;
            dataX_CT[3] = 6;
            dataX_CT[4] = 8;
            dataX_CT[5] = 10;
            dataX_CT[6] = 12;
            dataX_CT[7] = 14;
            dataX_CT[8] = 16;
            dataX_CT[9] = 18;
            dataX_CT[10] = 20;
            dataX_CT[11] = 22;
            dataX_CT[12] = 24;
            dataX_CT[13] = 26;
            dataX_CT[14] = 28;
            dataX_CT[15] = 30;
            dataY_CT = new double[dataX_CT.Length];
            dataY1_CT = new double[dataX_CT.Length];
            dataY_CT_alt = new double[dataX_CT.Length];
            dataY1_CT_alt = new double[dataX_CT.Length];
            dataY_CT[0] = Convert.ToDouble(set.Tables["totalAmount"].Rows[368].ItemArray[13]);
            dataY_CT[1] = Convert.ToDouble(set.Tables["totalAmount"].Rows[380].ItemArray[13]);
            dataY_CT[2] = Convert.ToDouble(set.Tables["totalAmount"].Rows[392].ItemArray[13]);
            dataY_CT[3] = Convert.ToDouble(set.Tables["totalAmount"].Rows[404].ItemArray[13]);
            dataY_CT[4] = Convert.ToDouble(set.Tables["totalAmount"].Rows[416].ItemArray[13]);
            dataY_CT[5] = Convert.ToDouble(set.Tables["totalAmount"].Rows[428].ItemArray[13]);
            dataY_CT[6] = Convert.ToDouble(set.Tables["totalAmount"].Rows[440].ItemArray[13]);
            dataY_CT[7] = Convert.ToDouble(set.Tables["totalAmount"].Rows[452].ItemArray[13]);
            dataY_CT[8] = Convert.ToDouble(set.Tables["totalAmount"].Rows[464].ItemArray[13]);
            dataY_CT[9] = Convert.ToDouble(set.Tables["totalAmount"].Rows[476].ItemArray[13]);
            dataY_CT[10] = Convert.ToDouble(set.Tables["totalAmount"].Rows[488].ItemArray[13]);
            dataY_CT[11] = Convert.ToDouble(set.Tables["totalAmount"].Rows[500].ItemArray[13]);
            dataY_CT[12] = Convert.ToDouble(set.Tables["totalAmount"].Rows[512].ItemArray[13]);
            dataY_CT[13] = Convert.ToDouble(set.Tables["totalAmount"].Rows[524].ItemArray[13]);
            dataY_CT[14] = Convert.ToDouble(set.Tables["totalAmount"].Rows[536].ItemArray[13]);
            dataY_CT[15] = Convert.ToDouble(set.Tables["totalAmount"].Rows[548].ItemArray[13]);
            SOC = dataY_CT[15];
//fill array with "blue" data topsoil
            dataY_CT_alt[0] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[368].ItemArray[12]);
            dataY_CT_alt[1] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[380].ItemArray[12]);
            dataY_CT_alt[2] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[392].ItemArray[12]);
            dataY_CT_alt[3] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[404].ItemArray[12]);
            dataY_CT_alt[4] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[416].ItemArray[12]);
            dataY_CT_alt[5] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[428].ItemArray[12]);
            dataY_CT_alt[6] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[440].ItemArray[12]);
            dataY_CT_alt[7] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[452].ItemArray[12]);
            dataY_CT_alt[8] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[464].ItemArray[12]);
            dataY_CT_alt[9] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[476].ItemArray[12]);
            dataY_CT_alt[10] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[488].ItemArray[12]);
            dataY_CT_alt[11] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[500].ItemArray[12]);
            dataY_CT_alt[12] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[512].ItemArray[12]);
            dataY_CT_alt[13] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[524].ItemArray[12]);
            dataY_CT_alt[14] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[536].ItemArray[12]);
            dataY_CT_alt[15] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[548].ItemArray[12]);
            SOC1 = dataY_CT_alt[15];
            dataY1_CT[0] = Convert.ToDouble(set.Tables["totalAmount"].Rows[368].ItemArray[27]);
            dataY1_CT[1] = Convert.ToDouble(set.Tables["totalAmount"].Rows[380].ItemArray[27]);
            dataY1_CT[2] = Convert.ToDouble(set.Tables["totalAmount"].Rows[392].ItemArray[27]);
            dataY1_CT[3] = Convert.ToDouble(set.Tables["totalAmount"].Rows[404].ItemArray[27]);
            dataY1_CT[4] = Convert.ToDouble(set.Tables["totalAmount"].Rows[416].ItemArray[27]);
            dataY1_CT[5] = Convert.ToDouble(set.Tables["totalAmount"].Rows[428].ItemArray[27]);
            dataY1_CT[6] = Convert.ToDouble(set.Tables["totalAmount"].Rows[440].ItemArray[27]);
            dataY1_CT[7] = Convert.ToDouble(set.Tables["totalAmount"].Rows[452].ItemArray[27]);
            dataY1_CT[8] = Convert.ToDouble(set.Tables["totalAmount"].Rows[464].ItemArray[27]);
            dataY1_CT[9] = Convert.ToDouble(set.Tables["totalAmount"].Rows[476].ItemArray[27]);
            dataY1_CT[10] = Convert.ToDouble(set.Tables["totalAmount"].Rows[488].ItemArray[27]);
            dataY1_CT[11] = Convert.ToDouble(set.Tables["totalAmount"].Rows[500].ItemArray[27]);
            dataY1_CT[12] = Convert.ToDouble(set.Tables["totalAmount"].Rows[512].ItemArray[27]);
            dataY1_CT[13] = Convert.ToDouble(set.Tables["totalAmount"].Rows[524].ItemArray[27]);
            dataY1_CT[14] = Convert.ToDouble(set.Tables["totalAmount"].Rows[536].ItemArray[27]);
            dataY1_CT[15] = Convert.ToDouble(set.Tables["totalAmount"].Rows[548].ItemArray[27]);
            SOC_sub = dataY1_CT[15];
//fill array with "blue" data subsoil
            dataY1_CT_alt[0] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[368].ItemArray[27]);
            dataY1_CT_alt[1] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[380].ItemArray[27]);
            dataY1_CT_alt[2] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[392].ItemArray[27]);
            dataY1_CT_alt[3] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[404].ItemArray[27]);
            dataY1_CT_alt[4] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[416].ItemArray[27]);
            dataY1_CT_alt[5] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[428].ItemArray[27]);
            dataY1_CT_alt[6] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[440].ItemArray[27]);
            dataY1_CT_alt[7] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[452].ItemArray[27]);
            dataY1_CT_alt[8] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[464].ItemArray[27]);
            dataY1_CT_alt[9] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[476].ItemArray[27]);
            dataY1_CT_alt[10] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[488].ItemArray[27]);
            dataY1_CT_alt[11] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[500].ItemArray[27]);
            dataY1_CT_alt[12] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[512].ItemArray[27]);
            dataY1_CT_alt[13] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[524].ItemArray[27]);
            dataY1_CT_alt[14] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[536].ItemArray[27]);
            dataY1_CT_alt[15] = Convert.ToDouble(set2.Tables["totalAmount"].Rows[548].ItemArray[27]);
            SOC1_sub = dataY1_CT_alt[15];
            SOC_percent = SOC / BULKDA / 10000 / 0.25 * 100;
            SOC1_percent = SOC1 / BULKDA / 10000 / 0.25 * 100;
            SOC_sub_percent = SOC_sub / BULKDB / 10000 / 0.25 * 100;
            SOC1_sub_percent = SOC1_sub / BULKDB / 10000 / 0.25 * 100;
            XYChart c = new XYChart(500, 390, 0xFFFFFF, 0xFFFFFF);
            //c.setRoundedFrame();
            c.addLegend(50, 5);
//            c.addTitle("<*br*>" + chartName, "arialbd.ttf", 12);
            c.setPlotArea(50, 100, 400, 230, 0xFFFFFF);

            c.xAxis().setTitle("Years");
            c.xAxis().setLinearScale(0, 30, 5);

            c.yAxis().setTitle("Soil organic carbon [t/ha]");
            c.yAxis().setLinearScale(0, 400, 50);

            //if (color == "blue")
            //{ aScatterLayer = c.addScatterLayer(dataX_CT, dataY_CT, "Organic C in topsoil", Chart.CircleSymbol, 1, 0x0000ff, Chart.Transparent); }
            //else
            aScatterLayer = c.addScatterLayer(dataX_CT, dataY_CT, "Organic C in topsoil", Chart.CircleSymbol, 1, 0xff0000, Chart.Transparent);
//            color = "blue";
            aScatterLayer2 = c.addScatterLayer(dataX_CT, dataY_CT_alt, "Organic C in topsoil", Chart.CircleSymbol, 1, 0x0000ff, Chart.Transparent);
            aScatterLayer1 = c.addScatterLayer(dataX_CT, dataY1_CT, "Organic C in subsoil (red)", Chart.CircleSymbol, 1, 0x00ff00, Chart.Transparent);
            aScatterLayer3 = c.addScatterLayer(dataX_CT, dataY1_CT_alt, "Organic C in subsoil (blue)", Chart.CircleSymbol, 1, 0xffff00, Chart.Transparent);
            aScatterLayer.setLineWidth(2);
            aScatterLayer1.setLineWidth(2);
            aScatterLayer2.setLineWidth(2);
            aScatterLayer3.setLineWidth(2);
            //switch (number)
            //{
            //    case 1:
            WebChartViewerCToolModelChart_1.Image = c.makeWebImage(Chart.PNG);
            //break;
            //case 2:
            //WebChartViewerCToolModelChart_2.Image = c.makeWebImage(Chart.PNG);
            //break;
            dataX[0] = 0;
            dataX[1] = 10;
            dataX[2] = 20;
            dataX[3] = 30;
            dataX[4] = 40;
            dataX[5] = 45;
            dataX[6] = 50;
            dataX[7] = 60;
            dataX[8] = 75;
            dataX[9] = 80;
            dataX[10] = 90;
            dataX[11] = 105;
            dataX[12] = 120;
            dataX[13] = 130;
            dataX[14] = 140;
            dataX[15] = 150;
            dataX[16] = 160;
            dataX[17] = 170;
            dataX[18] = 180;
            dataX[19] = 190;
            dataX[20] = 200;
            dataX[21] = 210;
            dataX[22] = 220;

            dataY = new double[dataX.Length];
            dataY2 = new double[dataX.Length];
            C25 = SOC_percent;
            C2550 = SOC_sub_percent;
            C25_2 = SOC1_percent;
            C2550_2 = SOC1_sub_percent;
            for (int y = 0; y < dataX.Length; y++)
            {
                N_total = dataX[y];
                dataY[y] = simpleModel.calculate(N_total, Norg_share, CN2550, a1, a2, a3, C25, CN25, C2550, Nr, Nc, Yp, Clay_5, NRE, NRR, NPCT, EP_season, P_season, Temp, C_resid, ypl, D, fx, RTlogic);
                dataY2[y] = simpleModel.calculate(N_total, Norg_share_2, CN2550_2, a1, a2, a3, C25_2, CN25_2, C2550_2, Nr_2, Nc_2, Yp_2, Clay_5_2, NRE_2, NRR_2, NPCT_2, EP_season_2, P_season_2, Temp_2, C_resid_2, ypl_2, D_2, fx_2, RTlogic_2);
            }



            XYChart c1 = new XYChart(500, 260, 0xFFFFFF, 0xFFFFFF);
            c1.setPlotArea(50, 10, 400, 200, 0xFFFFFF);

            c1.xAxis().setTitle("N input [kg N/ha]");
            c1.xAxis().setLinearScale(0, 250, 50);

            c1.yAxis().setTitle("Crop yield [t/ha]");
            c1.yAxis().setLinearScale(0, 14, 2);

            aScatterLayer2 = c1.addScatterLayer(dataX, dataY, "", Chart.CircleSymbol, 1, 0xff0000, Chart.Transparent);
            aScatterLayer2.setLineWidth(2);
            aScatterLayer1 = c1.addScatterLayer(dataX, dataY2, "", Chart.CircleSymbol, 1, 0x0000ff, Chart.Transparent);
            aScatterLayer1.setLineWidth(2);

            WebChartViewerSimpleModelChart.Image = c1.makeWebImage(Chart.PNG);

            //}
        }

        protected void ImagebuttonMap_Click(object sender, System.Web.UI.ImageClickEventArgs e)
        {
            //con = Utility.GetConnectionString("conSmartsoil");

            //Grid_Code = Convert.ToInt32(HiddenField_Grid_Code.Value.ToString());
            //strSelectCommand = "SELECT * FROM dbo.Grid WHERE Grid_Code = " + Grid_Code + "";
            ImagebuttonMap.Visible = false;
            ChartHolder.Visible = true;
            PlaceHolderInput.Visible = true;
        }

        void FillInput(string[] data, string[] lines, string[] TemperatureLines)
        {
            string type = "";
            for (int i = 0; i < data.Length; i++)
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
                    type = "Crop";
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
    }
}