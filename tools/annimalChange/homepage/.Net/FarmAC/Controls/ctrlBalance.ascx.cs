using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace FarmAC.Controls
{

    public partial class ctrlBalance : System.Web.UI.UserControl
    {

        double NUMBER_MANURE_COL = 9;
        double LOSS_MANURE_N_FIELD = 0.07;
        string[][] arrManure;
        int[][] arrFeedBalance;
        DataTable arrFeedBalance1;

        double[] dictFeedBalance;
        int nStorageType;
        DataTable arrStorageType;
        Scripting.Dictionary dictStorageType;
        double[] dictRotation;
        double[] arrParameter;
        double[] returnValue;
        DataTable arrManureNContent;
        // string version = (string) HttpContext.Current.Session["version"];


        int NUMBER_FEED_ROW = 0;
        int NUMBER_FEED_COL = 0;
        double PARAMETER_GROWTH_N;
        double PARAMETER_MANURE_N_FIELD;
        int nStorageTyper;
        double[] CDbl;
        double PARAMETER_N;
        string farmNumber;
        string scenarioNumber ;
        protected void Page_Load(object sender, EventArgs e)
        {
            if (HttpContext.Current.Session["UserInit"] != null)
                if (HttpContext.Current.Session["UserInit"].ToString() == "8")
            {
                CreateTabBalance();
            }
        }

        private void CreateTabBalance()
        {
            farmNumber = HttpContext.Current.Session["farmNumber"].ToString();
            scenarioNumber = HttpContext.Current.Session["scenarioNumber"].ToString();
            double HighEnergyConc = 0;

            string language;
            if (HttpContext.Current.Session["languageSelected"] != null)
                language = HttpContext.Current.Session["languageSelected"].ToString();
            else
                language = "uk";
            DataTable arrManure;
            //time16=now;
            int[] arrParameter = new int[2];
            string farNumber = Session["farmNumber"].ToString();
            try
            {
                arrParameter[0] = int.Parse(farNumber);
            }
            catch
            {
                arrParameter[0] = 0;
            }
            try
            {
                arrParameter[1] = int.Parse(Session["scenarioNumber"].ToString());
            }
            catch
            {
                arrParameter[1] = 0;
            }

            dbInterface database = new dbInterface();
            database.InitFarmNDB();

            //CreateLanguageDictionary


            List<dbInterface.data> paramter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@FarmNumber";
            data.value = Session["farmNumber"].ToString();
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@ScenarioID";
            data.value = Session["scenarioNumber"].ToString();
            paramter.Add(data);
            arrFeedBalance1 = database.GetValuesFromDB("getAllFeedplansModelled ", paramter);
            paramter = new List<dbInterface.data>();
            arrStorageType = database.GetValuesFromDB("getAllStorageTypesList ", paramter);
            nStorageType = arrStorageType.Rows.Count;


            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@FarmNumber";
            data.value = Session["farmNumber"].ToString(); ;
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@ScenarioID";
            data.value = Session["scenarioNumber"].ToString();
            paramter.Add(data);

            arrManureNContent = database.GetValuesFromDB("GetModelledManure", paramter);

            try
            {
                paramter = new List<dbInterface.data>();
                data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = Session["farmNumber"].ToString();
                paramter.Add(data);
                data = new dbInterface.data();
                data.name = "@aScenarioID";
                data.value = Session["scenarioNumber"].ToString();
                paramter.Add(data);
                arrManure = database.GetValuesFromDB("GetManure_N_Content", paramter);
            }
            catch
            {

            }



            arrFeedBalance = new int[NUMBER_FEED_ROW][];


            Panel1.Controls.Add(new LiteralControl("<h3>" + database.GetLanguageString("strMANURE", language) + "– kg N</h3>" + "\r"));
            CreateTableManure();


            Panel1.Controls.Add(new LiteralControl("<h3>" + database.GetLanguageString("strFEED", language) + "</h3>" + "\r"));
            CreateTableFeed(HighEnergyConc);
            database.CloseFarmNDB();
            CreateUpdateFormBalance();

        }

        void CreateTableManure()
        {

            string language;
            if (HttpContext.Current.Session["languageSelected"] != null)
                language = HttpContext.Current.Session["languageSelected"].ToString();
            else
                language = "uk";
            dbInterface database = new dbInterface();
            Panel1.Controls.Add(new LiteralControl("  <table class=" + "data" + " border=" + "0" + ">" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    <tr>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">&nbsp;</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " colspan=" + "2" + ">&nbsp;&nbsp;" + database.GetLanguageString("strPRODUCTION", language) + "</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " colspan=" + "2" + ">&nbsp;&nbsp;" + database.GetLanguageString("strBOUGHT", language) + "</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " colspan=" + "2" + ">&nbsp;&nbsp;" + database.GetLanguageString("strSOLD", language) + "</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " colspan=" + "2" + ">&nbsp;&nbsp;" + database.GetLanguageString("strHOUSINGUSE", language) + "</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strMANURETYPE", language) + "</th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));


            for (int d = 0; d < arrManureNContent.Rows.Count; d++)
            {
                Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("      <td align=" + "left" + ">&nbsp;&nbsp;" + arrManureNContent.Rows[d]["StorageName"] + "</td>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("      <td></td><td align=" + "right" + ">" + Math.Round(double.Parse(arrManureNContent.Rows[d]["AmountProduced"].ToString()), 2) + "</td>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("      <td></td><td align=" + "right" + ">" + Math.Round(double.Parse(arrManureNContent.Rows[d]["Bought"].ToString()), 2) + "</td>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("      <td></td><td align=" + "right" + ">" + Math.Round(double.Parse(arrManureNContent.Rows[d]["Sold"].ToString()), 2) + "</td>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("      <td></td><td align=" + "right" + ">" + Math.Round(double.Parse(arrManureNContent.Rows[d]["Amount"].ToString()), 2) + "</td>" + "\r"));
                Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
            }

            Panel1.Controls.Add(new LiteralControl("    <tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " colspan=" + "8" + "><hr/></th>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + "\r"));
            Panel1.Controls.Add(new LiteralControl("  </table>" + "\r"));

        }

        void CreateTableFeed1(double HighEnergyConc, dbInterface database)
        {
            string language;
            if (HttpContext.Current.Session["languageSelected"] != null)
                language = HttpContext.Current.Session["languageSelected"].ToString();
            else
                language = "uk";
            Panel1.Controls.Add(new LiteralControl("  <table class=" + "data" + " border=" + "0" + ">" + '\r'));

            Panel1.Controls.Add(new LiteralControl("    <tr>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">&nbsp;</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " colspan=" + "2" + ">" + database.GetLanguageString("strFIELDPROD", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " colspan=" + "2" + ">" + database.GetLanguageString("strBOUGHT", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " colspan=" + "2" + ">" + database.GetLanguageString("strSOLD", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + " colspan=" + "2" + ">" + database.GetLanguageString("strHOUSINGUSE", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strInternalExchge", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("    <tr>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strFeedtype", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strFE", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strKGN", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strFE", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strKGN", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strFE", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strKGN", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strFE", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strKGN", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strFE", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + '\r'));

            for (int r = 0; r < NUMBER_FEED_ROW; r++)
            {
                Panel1.Controls.Add(new LiteralControl("    <tr>" + '\r'));
                Panel1.Controls.Add(new LiteralControl("      <td align=" + "left" + ">" + r + ")" + "</td>" + '\r'));
                for (int c = 1; c < NUMBER_FEED_COL - 1; c++)
                {
                    Panel1.Controls.Add(new LiteralControl("      <td align=" + "right" + ">&nbsp;&nbsp;" + arrFeedBalance[r][c]));
                    if ((r == 0 || r == 1) && c == 9 && arrFeedBalance[r][9] != 0)
                    {
                        Panel1.Controls.Add(new LiteralControl("*"));
                    }
                    Panel1.Controls.Add(new LiteralControl("</td>" + '\r'));
                }
                Panel1.Controls.Add(new LiteralControl("    </tr>" + '\r'));
            }

            Panel1.Controls.Add(new LiteralControl("    <tr>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + "strIaltTOTAL" + "</th>" + '\r'));
            for (int c = 1; c < NUMBER_FEED_COL - 2; c++)
                Panel1.Controls.Add(new LiteralControl("      <th align=" + "right" + ">&nbsp;&nbsp;" + arrFeedBalance[NUMBER_FEED_ROW][c] + "</th>" + '\r'));

            Panel1.Controls.Add(new LiteralControl("    </tr>" + '\r'));


            Panel1.Controls.Add(new LiteralControl("    <tr>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <td align=" + "left" + " colspan=" + "10" + "><hr/></td>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("    </tr>" + '\r'));

            Panel1.Controls.Add(new LiteralControl("  </table>" + '\r'));


            double percentFE, percentFEBought, percentN, percentNBought, milkPrFE, effStable, nMilk;

            double TotalCattleFE, TotalCattleN, NeffSupplement, NeffPig, AddOn, Nneed;
            DataTable b;

            DataTable a;

            DataTable arrMilk;
            TotalCattleFE = arrFeedBalance[0][7] + arrFeedBalance[1][7] + arrFeedBalance[2][7] + arrFeedBalance[3][7] + arrFeedBalance[4][7];
            TotalCattleN = arrFeedBalance[0][8] + arrFeedBalance[1][8] + arrFeedBalance[2][8] + arrFeedBalance[3][8] + arrFeedBalance[4][8];
            List<dbInterface.data> paramter = new List<dbInterface.data>();

            dbInterface.data data = new dbInterface.data();
            data.name = " @aFarmNumber";
            data.value = (string)farmNumber;
            paramter.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = (string)scenarioNumber;
            paramter.Add(data);
            arrMilk = database.GetValuesFromDB("CalcTotalMilkkg", paramter);

            nMilk = (int)arrMilk.Rows[0]["SumMilkkg"];




            if (TotalCattleFE > 0)
            {
                percentFE = (arrFeedBalance[0][7] + arrFeedBalance[1][7] + arrFeedBalance[2][7]) / TotalCattleFE * 100;
                percentFEBought = arrFeedBalance[4][7] / TotalCattleFE * 100;
                milkPrFE = (int)arrMilk.Rows[0][0] / TotalCattleFE * 100;//   ' total kg mælk 
            }
            else
            {
                percentFE = 0;
                percentFEBought = 0;
                milkPrFE = 0;
            }

            if (arrMilk.Rows[0][0] == null)
                arrMilk.Rows[0][0] = 0;


            if (TotalCattleN > 0)
            {
                percentN = (arrFeedBalance[0][8] + arrFeedBalance[1][8] + arrFeedBalance[2][8]) / TotalCattleN * 100;
                percentNBought = arrFeedBalance[4][8] / TotalCattleN * 100;
                data = new dbInterface.data();
                data.name = "PARAMETER_GROWTH_N";
                data.value = PARAMETER_GROWTH_N.ToString();
                paramter.Add(data);

                a = database.GetValuesFromDB("GetFarmTotal", paramter);
                effStable = ((double)a.Rows[0][0] + (double)arrMilk.Rows[0][0] * 0.00518) / TotalCattleN * 100;
            }
            else
            {
                percentN = 0;
                percentNBought = 0;
                effStable = 0;
            }

            if (arrFeedBalance[3][7] + arrFeedBalance[4][7] > 0)
                NeffSupplement = (arrFeedBalance[3][8] + arrFeedBalance[4][8]) / (arrFeedBalance[3][7] + arrFeedBalance[4][7]);
            else
                NeffSupplement = 0;


            Panel1.Controls.Add(new LiteralControl("  <table class=" + "data" + " border=" + "0" + ">" + '\r'));

            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "407";
            data.value = "407";
            paramter.Add(data);
            DataTable tmp = (database.GetValuesFromDB("GetFarmTotal", paramter));
            AddOn = (double)tmp.Rows[0][0];

            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "404";
            data.value = "404";
            paramter.Add(data);
            tmp = (database.GetValuesFromDB("GetFarmTotal", paramter));
            Nneed = (double)tmp.Rows[0][0]; ;


            if (Nneed > 0)
                NeffPig = AddOn / Nneed * 100;
            else
                NeffPig = 0;





            Panel1.Controls.Add(new LiteralControl("  </table>" + '\r'));

            if (arrFeedBalance[0][9] == 0)
                Panel1.Controls.Add(new LiteralControl("<b>&nbsp;&nbsp;*&nbsp; " + ("strWarningTurnover") + "</b><br>"));

            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "PARAMETER_N";
            data.value = "PARAMETER_N";
            paramter.Add(data);
            a = (database.GetValuesFromDB("GetFarmTotal", paramter));


            paramter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "404";
            data.value = "404";
            paramter.Add(data);
            b = (database.GetValuesFromDB("GetFarmTotal", paramter));

            if (arrFeedBalance[NUMBER_FEED_ROW][8] != ((double)a.Rows[0][0] + (double)b.Rows[0][0]))
                Panel1.Controls.Add(new LiteralControl("&nbsp;&nbsp; " + ("strKvælstofbehov") + ((double)a.Rows[0][0] + (double)b.Rows[0][0]) + " " + ("strKGNBARE") + "<br>"));

            //Advarsel hvis concentrationen FE/DM er højere i marken end behovet er i kvægbesætningen.    
            if (HighEnergyConc > 0)
                Panel1.Controls.Add(new LiteralControl("&nbsp;&nbsp; " + ("strHighEnergyConc") + "<br>"));


        }

        void InitTableManure(dbInterface database)
        {


            DataTable arrDummy;




            for (int r = 0; r < nStorageType; r++)
            {

                int found = 0;
                foreach (KeyValuePair<string, int> pair in dictStorageType)
                {
                    if (pair.Key == "StorageName")
                        found = pair.Value;
                }
                arrManure[r][0] = arrStorageType.Rows[r][found].ToString();
            }

            arrManure[nStorageType][0] = "strIaltTOTAL";

            double[] preserve = new double[(int)arrParameter[2]];
            for (int r = 0; r < nStorageTyper; r++)
            {

                int found = 0;
                foreach (KeyValuePair<string, int> pair in dictStorageType)
                {
                    if (pair.Key == "StorageID")
                        found = pair.Value;
                }

                List<dbInterface.data> paramter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.name = "arrStorageType.Rows[r][StorageID]";
                data.value = arrStorageType.Rows[r]["StorageID"].ToString();
                paramter.Add(data);

                arrManure[r][1] = database.GetValuesFromDB("GetNitrogenStableBeforeLoss", paramter).Rows[0][0].ToString(); ;
            }

            arrParameter[2] = PARAMETER_MANURE_N_FIELD;

            for (int r = 0; r < nStorageTyper; r++)
            {


                List<dbInterface.data> paramter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.name = "arrStorageType.Rows[r][StorageID]";
                data.value = arrStorageType.Rows[r]["StorageID"].ToString();
                paramter.Add(data);

                arrManure[r][2] = database.GetValuesFromDB("GetNitrogenStableLoss", paramter).Rows[0][0].ToString();
            }


            for (int r = 0; r < nStorageTyper; r++)
            {
                List<dbInterface.data> paramter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.name = "arrStorageType.Rows[r][StorageID]";
                data.value = arrStorageType.Rows[r]["StorageID"].ToString();
                paramter.Add(data);


                arrManure[r][3] = database.GetValuesFromDB("GetNitrogenStorageLoss", paramter).Rows[0][0].ToString();
            }

            for (int r = 0; r < nStorageTyper; r++)
            {
                List<dbInterface.data> paramter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.name = "arrStorageType.Rows[r][StorageID]";
                data.value = arrStorageType.Rows[r]["StorageID"].ToString();
                paramter.Add(data);
                arrManure[r][4] = database.GetValuesFromDB("GetNitrogenSpreadLoss", paramter).Rows[0][0].ToString();
            }
            int foundValue = 0;
            foreach (KeyValuePair<string, int> pair in dictStorageType)
            {
                if (pair.Key == "StorageID")
                    foundValue = pair.Value;
            }

            for (int r = 0; r < nStorageTyper; r++)
            {
                List<dbInterface.data> paramter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.name = "@aFarmNumber";
                data.value = farmNumber.ToString();
                paramter.Add(data);
                data = new dbInterface.data();
                data.name = "@aScenarioID";
                data.value = scenarioNumber.ToString();
                paramter.Add(data);
                data = new dbInterface.data();
                data.name = "@aColumnName";
                data.value = "BoughtManure";
                paramter.Add(data);

                arrDummy = database.GetValuesFromDB("GetManure", paramter);
                if (arrDummy.Rows.Count != 0)
                {
                    int found = 0;
                    foreach (KeyValuePair<string, int> pair in dictStorageType)
                    {
                        if (pair.Key == "BoughtManure")
                            found = pair.Value;
                    }
                    arrManure[r][5] = arrDummy.Rows[0]["BoughtManure"].ToString();
                }
                else
                    arrManure[r][5] = "0";


            }

            List<dbInterface.data> paramter1 = new List<dbInterface.data>();
            dbInterface.data data1 = new dbInterface.data();
            data1.name = "farmNumber";
            data1.value = farmNumber.ToString();
            paramter1.Add(data1);
            data1 = new dbInterface.data();
            data1.name = "scenarioNumber";
            data1.value = scenarioNumber.ToString();
            paramter1.Add(data1);
            arrManure[2][5] = database.GetValuesFromDB("fnGetBoughtGrazingN ", paramter1).Rows[0][0].ToString();
            arrManure[2][4] = ((double.Parse(arrManure[2][5]) + double.Parse(arrManure[2][1])) * LOSS_MANURE_N_FIELD).ToString(); // Special case for grazing
            // Nitrogen need on field
            for (int r = 0; r < nStorageType; r++)
            {
                paramter1 = new List<dbInterface.data>();
                data1 = new dbInterface.data();
                data1.name = "arrStorageType";
                data1.value = arrStorageType.Rows[r]["StorageID"].ToString();
                paramter1.Add(data1);

                arrManure[r][7] = database.GetValuesFromDB("GetNitrogenFieldUse", paramter1).Rows[0][0].ToString();
            }

            for (int r = 0; r < nStorageType; r++)
            {


                paramter1 = new List<dbInterface.data>();
                data1 = new dbInterface.data();
                data1.name = "@aFarmNumber";
                data1.value = farmNumber;
                paramter1.Add(data1);
                data1 = new dbInterface.data();
                data1.name = "@aScenarioID";
                data1.value = scenarioNumber;
                paramter1.Add(data1);

                data1 = new dbInterface.data();
                data1.name = "@aColumnName";
                data1.value = "SoldManure";
                paramter1.Add(data1);
                arrDummy = database.GetValuesFromDB("GetManure", paramter1);
                if (arrDummy.Rows.Count != 0)
                {

                    arrManure[r][6] = arrDummy.Rows[0]["SoldManure"].ToString();
                }
                else
                    arrManure[r][6] = "0";

            }




            for (int r = 0; r < nStorageType; r++)
                arrManure[r][8] = "0";



            for (int r = 0; r < nStorageType; r++)
                for (int c = 1; c < NUMBER_MANURE_COL; c++)
                    arrManure[nStorageType][c] = (CDbl[int.Parse(arrManure[nStorageType][c])] + CDbl[int.Parse(arrManure[r][c])]).ToString();

        }

        void CreateTableFeed(double HighEnergyConc)
        {
            string language;
            if (HttpContext.Current.Session["languageSelected"] != null)
                language = HttpContext.Current.Session["languageSelected"].ToString();
            else
                language = "uk";
            dbInterface database = new dbInterface();
            double bought, sold;

            Panel1.Controls.Add(new LiteralControl("  <table class=" + "data" + " border=" + "0" + ">" + '\r'));

            Panel1.Controls.Add(new LiteralControl("    <tr>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "right" + ">&nbsp;</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strFIELDPROD", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "right" + ">" + database.GetLanguageString("strBOUGHT", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "right" + ">" + database.GetLanguageString("strSOLD", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "right" + ">" + database.GetLanguageString("strHOUSINGUSE", language) + "</th>" + '\r'));

            Panel1.Controls.Add(new LiteralControl("    </tr>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("    <tr>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "left" + ">" + database.GetLanguageString("strFeedtype", language) + "</th>" + '\r'));
            Panel1.Controls.Add(new LiteralControl("      <th align=" + "right" + ">&nbsp;&nbsp;" + database.GetLanguageString("strKg_toerstof", language) + "</th>" + '\r'));

            Panel1.Controls.Add(new LiteralControl("      <th align=" + "right" + ">&nbsp;&nbsp;" + database.GetLanguageString("strKg_toerstof", language) + "</th>" + '\r'));

            Panel1.Controls.Add(new LiteralControl("      <th align=" + "right" + ">&nbsp;&nbsp;" + database.GetLanguageString("strKg_toerstof", language) + "</th>" + '\r'));

            Panel1.Controls.Add(new LiteralControl("      <th align=" + "right" + ">&nbsp;&nbsp;" + database.GetLanguageString("strKg_toerstof", language) + "</th>" + '\r'));


            Panel1.Controls.Add(new LiteralControl("    </tr>" + '\r'));

            for (int r = 0; r < arrFeedBalance1.Rows.Count; r++)
            {
                bought = (double)arrFeedBalance1.Rows[r][3];
                sold = (double)arrFeedBalance1.Rows[r][4];

                if (bought < 0)
                {
                    sold = sold + bought;
                    bought = 0;
                }
                
                Panel1.Controls.Add(new LiteralControl("    <tr>" + '\r'));
                Panel1.Controls.Add(new LiteralControl("      <td align=" + "left" + ">&nbsp;&nbsp;" + arrFeedBalance1.Rows[r][0] + "</td>" + '\r'));
                Panel1.Controls.Add(new LiteralControl("      <td align=" + "right" + ">" + Math.Round(double.Parse(arrFeedBalance1.Rows[r][1].ToString()), 2) + "</td>" + '\r'));

                Panel1.Controls.Add(new LiteralControl("      <td align=" + "right" + ">" + Math.Round(double.Parse(arrFeedBalance1.Rows[r][2].ToString()), 2) + "</td>" + '\r'));

                Panel1.Controls.Add(new LiteralControl("      <td align=" + "right" + ">" + Math.Round(double.Parse(arrFeedBalance1.Rows[r][3].ToString()), 2) + "</td>" + '\r'));

                Panel1.Controls.Add(new LiteralControl("      <td align=" + "right" + ">" + Math.Round(double.Parse(arrFeedBalance1.Rows[r][4].ToString()), 2) + "</td>" + '\r'));

                Panel1.Controls.Add(new LiteralControl("    </tr>" + '\r'));
            }
            Panel1.Controls.Add(new LiteralControl("  </table>" + '\r'));
        }

        void CreateUpdateFormBalance()
        {
            Panel1.Controls.Add(new LiteralControl("<form name=" + "update" + " action=" + "Dummy.asp" + " method=" + "post" + ">" + '\r'));
            Panel1.Controls.Add(new LiteralControl("<input type=" + "hidden" + " name=" + "saveUpdates" + " value=" + "false" + ">" + '\r'));
            Panel1.Controls.Add(new LiteralControl("</form>" + '\r'));

        }
    }
}