using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    public class utils : System.Web.UI.UserControl
    {

        public utils()
        {
        }
        public void deleteFils()
        {
            HttpContext.Current.Session["FileName"] =null;
            HttpContext.Current.Session["AllNodes"] =null;
            dbInterface db = new dbInterface();
            string path = db.GetConnectionString("ModelOutput");
            string[] input = new string[2];
            input[0] = HttpContext.Current.Session["farmNumber"].ToString();
            input[1] = HttpContext.Current.Session["scenarioNumber"].ToString();
            if (File.Exists(path + "dataCtool_" + input[0] + "ScenarioNr" + input[1] + ".txt"))
                File.Delete(path + "dataCtool_" + input[0] + "_" + input[1] + ".txt");
            if (File.Exists(path + "done" + input[0] + "_" + input[1] + ".txt"))
                File.Delete(path + "done" + input[0] + "_" + input[1] + ".txt");
            if (File.Exists(path + "log" + input[0] + "_" + input[1] + ".txt"))
                File.Delete(path + "log" + input[0] + "_" + input[1] + ".txt");
            if (File.Exists(path + "error_" + input[0] + "_" + input[1] + ".txt"))
                File.Delete(path + "error_" + input[0] + "_" + input[1] + ".txt");
            if (File.Exists(path + "outputFarm" + input[0] + "ScenarioNr" + input[1] + ".xml"))
                File.Delete(path + "outputFarm" + input[0] + "ScenarioNr" + input[1] + ".xml");
            if (File.Exists(path + "outputFarm" + input[0] + "ScenarioNr" + input[1] + ".xls"))
                File.Delete(path + "outputFarm" + input[0] + "ScenarioNr" + input[1] + ".xls");

            if (File.Exists(path + "outputFarm" + input[0] + "ScenarioNr" + input[1] + "CtoolFile.xml"))
                File.Delete(path + "outputFarm" + input[0] + "ScenarioNr" + input[1] + "CtoolFile.xml");
            if (File.Exists(path + "outputFarm" + input[0] + "ScenarioNr" + input[1] + "CtoolFile.xls"))
                File.Delete(path + "outputFarm" + input[0] + "ScenarioNr" + input[1] + "CtoolFile.xls");
            if (File.Exists(path + "outputFarm" + input[0] + "ScenarioNr" + input[1] + "livetockfile.xls"))
                File.Delete(path + "outputFarm" + input[0] + "ScenarioNr" + input[1] + "livetockfile.xls");
            if (File.Exists(path + "outputFarm" + input[0] + "ScenarioNr" + input[1] + "plantfile.xls"))
                File.Delete(path + "outputFarm" + input[0] + "ScenarioNr" + input[1] + "plantfile.xls");
        }
    }
}