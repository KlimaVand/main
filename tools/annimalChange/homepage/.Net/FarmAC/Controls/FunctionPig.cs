using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Web;

namespace FarmAC.Controls
{
    class FunctionPig
    {
        public void CalcPigHerdTotal(string farmNumber, string scenarioNumber)
        {
            double currentAnimalYear, currentFeedLevel, currentProteinLevel;
            string currentType, currentProductionVar1, currentProductionVar2, currentProductionVar3;
            string currentNitrogenContentLiveWeight, currentNitrogenContentInput;
            string currentStable1, currentStable2, currentNitrogenDigestability, dictPigHerd;
            dbInterface database = new dbInterface();
            List<dbInterface.data> arrParameter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = farmNumber;
            arrParameter.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = scenarioNumber;
            arrParameter.Add(data);
            //nPigHerd 
            DataTable arrPigHerd = database.GetValuesFromDB("GetPigHerdList", arrParameter);




            arrParameter = new List<dbInterface.data>();
            data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = farmNumber;
            arrParameter.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = scenarioNumber;
            DataTable returnValue = database.GetValuesFromDB("DeletePigHerdTotal", arrParameter);
            //  Response.Write( "<br> returnValue " + returnValue.Rows[0][0]);

            data = new dbInterface.data();
            data.name = "@4";
            data.value = "4";
            returnValue = database.GetValuesFromDB("DeleteStable", arrParameter);
            returnValue = database.GetValuesFromDB("DeleteStorage", arrParameter);
            returnValue = database.GetValuesFromDB("DeleteStorageNorm", arrParameter);

            for (int h = 0; h < arrPigHerd.Rows.Count; h++)
            {
                currentType = arrPigHerd.Rows[h][6].ToString();
                currentAnimalYear = double.Parse(arrPigHerd.Rows[h][3].ToString());
                currentProductionVar1 = arrPigHerd.Rows[h][12].ToString();
                currentProductionVar2 = arrPigHerd.Rows[h][13].ToString();
                currentProductionVar3 = arrPigHerd.Rows[h][14].ToString();
                currentProteinLevel = double.Parse(arrPigHerd.Rows[h][4].ToString());
                currentFeedLevel = double.Parse(arrPigHerd.Rows[h][5].ToString());
                currentNitrogenContentLiveWeight = arrPigHerd.Rows[h][10].ToString();
                currentNitrogenContentInput = arrPigHerd.Rows[h][11].ToString();
                currentNitrogenDigestability = arrPigHerd.Rows[h][23].ToString();
                currentStable1 = arrPigHerd.Rows[h][15].ToString();
                currentStable2 = arrPigHerd.Rows[h][16].ToString();

                CalcFeedEnergyP(currentType, currentAnimalYear, currentFeedLevel, farmNumber, scenarioNumber);
                CalcFeedNitrogenP(currentType, currentAnimalYear, currentFeedLevel, currentProteinLevel, farmNumber, scenarioNumber);
                CalcGrowthNitrogenP(currentType, currentAnimalYear, currentProductionVar1, currentProductionVar2, currentProductionVar3, currentNitrogenContentLiveWeight, currentNitrogenContentInput);

                CalcManureNitrogenP(currentType, currentAnimalYear);

                CalcManureNitrogenFaecesUrineP(currentType, currentAnimalYear, currentNitrogenDigestability);

                CalcManureStableStorageP(currentType, currentAnimalYear, currentStable1, currentStable2);

            }
        }
        void CalcFeedEnergyP(string aType, double aAnimalYear, double aFeedLevel, string farmNumber, string scenarioNumber)
        {
            double feedEnergyTotal = aAnimalYear * aFeedLevel;

            dbInterface database = new dbInterface();
            List<dbInterface.data> arrParameter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = farmNumber;
            arrParameter.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = scenarioNumber;
            arrParameter.Add(data);
            data = new dbInterface.data();
            data.name = "@aType";
            data.value = aType;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = "402";
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@feedEnergyTotal";
            data.value = feedEnergyTotal.ToString();
            arrParameter.Add(data);

            database.GetValuesFromDB("IncrementPigHerdTotal", arrParameter);
        }

        void CalcFeedNitrogenP(string aType, double aAnimalYear, double aFeedLevel, double aProteinLevel, string farmNumber, string scenarioNumber)
        {
            double feedNitrogenAnimalYear = aFeedLevel * aProteinLevel / 6250;

            //  feedNitrogenAnimalYearNorm = arrPigHerd(h,8)*arrPigHerd(h,9)/6250

            double feedNitrogenTotal = aAnimalYear * feedNitrogenAnimalYear;

            dbInterface database = new dbInterface();
            List<dbInterface.data> arrParameter = new List<dbInterface.data>();
            dbInterface.data data = new dbInterface.data();
            data.name = "@aFarmNumber";
            data.value = farmNumber;
            arrParameter.Add(data);
            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = scenarioNumber;
            arrParameter.Add(data);
            data = new dbInterface.data();
            data.name = "@aType";
            data.value = aType;
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@aScenarioID";
            data.value = "404";
            arrParameter.Add(data);

            data = new dbInterface.data();
            data.name = "@feedNitrogenTotal";
            data.value = feedNitrogenTotal.ToString();
            arrParameter.Add(data);

            database.GetValuesFromDB("IncrementPigHerdTotal", arrParameter);
        }
        void CalcGrowthNitrogenP(string aType, double aAnimalYear, string aProductionVar1, string aProductionVar2, string aProductionVar3, string aNitrogenContentLiveWeight, string aNitrogenContentInput)
        {
            throw new System.ArgumentException("CalcGrowthNitrogenP is not translated since feedNitrogenAnimalYearNorm is missing");
            /*
           GrowthNitrogenType = CalcPigGrowthNitrogen(aType, aProductionVar1, aProductionVar2, aProductionVar3, aNitrogenContentLiveWeight, aNitrogenContentInput)
           GrowthNitrogenTypeNorm = CalcPigGrowthNitrogen(aType, arrPigHerd(h,30), arrPigHerd(h,31), arrPigHerd(h,32), aNitrogenContentLiveWeight, aNitrogenContentInput)
           SowGrowthNitrogen = CalcSowGrowthNitrogen(aType, aProductionVar3, aNitrogenContentLiveWeight)
           GrowthNitrogenIn = CalcPigGrowthNitrogenIn(aType, aProductionVar1, aProductionVar2, aProductionVar3, aNitrogenContentLiveWeight, aNitrogenContentInput)
           GrowthNitrogenOut = CalcPigGrowthNitrogenOut(aType, aProductionVar1, aProductionVar2, aProductionVar3, aNitrogenContentLiveWeight, aNitrogenContentInput)
           feedNitrogenAnimalYearNorm = feedNitrogenAnimalYearNorm * GrowthNitrogenType / GrowthNitrogenTypeNorm
        '   Response.write "<br> feedNitrogenAnimalYearNorm " & feedNitrogenAnimalYearNorm


           GrowthNitrogenTotal = aAnimalYear*GrowthNitrogenType
           SowGrowthNitrogenTotal = aAnimalYear*SowGrowthNitrogen
           GrowthNitrogenInTotal = aAnimalYear*GrowthNitrogenIn
           GrowthNitrogenOutTotal = aAnimalYear*GrowthNitrogenOut

           Dim arrParameter
            redim arrParameter(4)

             arrParameter(0)= aFarmNumber
             arrParameter(1) = aScenarioID
             arrParameter(2) = aType
             arrParameter(3) = 407
             arrParameter(4) = CSng(GrowthNitrogenTotal)
             returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
        '   IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 407, CSng(GrowthNitrogenTotal)
             arrParameter(3) = 413
             arrParameter(4) = CSng(SowGrowthNitrogenTotal)
             returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
        '  IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 413, CSng(SowGrowthNitrogenTotal)
             arrParameter(3) = 414
             arrParameter(4) = CSng(GrowthNitrogenInTotal)
             returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
        '   IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 414, CSng(GrowthNitrogenInTotal)
             arrParameter(3) = 415
             arrParameter(4) = CSng(GrowthNitrogenOutTotal)
             returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)*/
        }
        void CalcManureNitrogenP(string aType, double aAnimalYear)
        {
            throw new System.ArgumentException("CalcManureNitrogenP is not translated since feedNitrogenAnimalYear is missing");
            /*manureNitrogenAnimalYear = feedNitrogenAnimalYear - GrowthNitrogenType
            manureNitrogenAnimalYearNorm = feedNitrogenAnimalYearNorm - GrowthNitrogenType
            manureNitrogenTotal = aAnimalYear*manureNitrogenAnimalYear
            manureNitrogenTotalNorm = aAnimalYear*manureNitrogenAnimalYearNorm
            Dim arrParameter
             redim arrParameter(4)

              arrParameter(0)= aFarmNumber
              arrParameter(1) = aScenarioID
              arrParameter(2) = aType
              arrParameter(3) = 408
              arrParameter(4) = CSng(manureNitrogenTotal)
              returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)*/
        }

        void CalcManureNitrogenFaecesUrineP(string aType, double aAnimalYear, string aNitrogenDigestability)
        {
            throw new System.ArgumentException("CalcManureNitrogenFaecesUrineP is not translated since feedNitrogenAnimalYear is missing");
            /*
           manureNitrogenFaecesAnimalYear = feedNitrogenAnimalYear*aNitrogenDigestability
           manureNitrogenFaecesTotal = aAnimalYear*manureNitrogenFaecesAnimalYear
           Dim arrParameter
            redim arrParameter(4)

             arrParameter(0)= aFarmNumber
             arrParameter(1) = aScenarioID
             arrParameter(2) = aType
             arrParameter(3) = 410
             arrParameter(4) = CSng(manureNitrogenFaecesTotal)
             returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
        '   IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 410, CSng(manureNitrogenFaecesTotal)

           manureNitrogenUrineAnimalYear = manureNitrogenAnimalYear - manureNitrogenFaecesAnimalYear
           manureNitrogenUrineTotal = aAnimalYear*manureNitrogenUrineAnimalYear
             arrParameter(3) = 411
             arrParameter(4) = CSng(manureNitrogenUrineTotal)
             returnValue = CallSPReturn ("IncrementPigHerdTotal",arrParameter)
        '   IncrementPigHerdTotal aFarmNumber, aScenarioID, aType, 411, CSng(manureNitrogenUrineTotal)*/
        }

        void CalcManureStableStorageP(string aType, double aAnimalYear, string aStable1ID, string aStable2ID)
        {
            string nStable, dictStable;
            string stableNitrogen, stableNitrogenLoss, stableNitrogenAfterLoss;
            string storageNitrogen, storageNitrogenLossAmmonia, storageNitrogenLossDenitrification, storageNitrogenAfterLoss;
            string stableNitrogenNormAfterLoss, storageNitrogenNormAfterLoss, stableNitrogenNorm;
            double stableBeddingNitrogen;
            if (aStable1ID == null || aStable2ID == null)
            {
            }
            else
            {

                dbInterface database = new dbInterface();
                List<dbInterface.data> arrParameter = new List<dbInterface.data>();
                dbInterface.data data = new dbInterface.data();
                data.name = "@aType";
                data.value = aType;
                arrParameter.Add(data);
                data = new dbInterface.data();
                data.name = "@aStable1ID";
                data.value = aStable1ID;
                arrParameter.Add(data);
                //nStable
                DataTable arrStable = database.GetValuesFromDB("GetPigStableList", arrParameter);

                for (int s = 0; s < arrStable.Rows.Count; s++)
                {
                    stableBeddingNitrogen = double.Parse(arrStable.Rows[s][3].ToString()) * double.Parse(arrStable.Rows[s][6].ToString()) / 100 * aAnimalYear * 0.85 * 0.005;

                    if (stableBeddingNitrogen == null)
                        stableBeddingNitrogen = 0;



                    arrParameter = new List<dbInterface.data>();
                    data = new dbInterface.data();
                    data.name = "@aFarmNumber";
                    data.value = HttpContext.Current.Session["farmNumber"].ToString();
                    arrParameter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@aScenarioID";
                    data.value = HttpContext.Current.Session["aScenarioID"].ToString();
                    arrParameter.Add(data);

                    data = new dbInterface.data();
                    data.name = "@aType";
                    data.value = aType;
                    arrParameter.Add(data);
                    data = new dbInterface.data();
                    data.name = "@stableBeddingNitrogen";
                    data.value = stableBeddingNitrogen.ToString();
                    arrParameter.Add(data);

                    database.GetValuesFromDB("IncrementPigHerdTotal", arrParameter);
                    throw new System.ArgumentException("CalcManureStableStorageP is not translated since manureNitrogenTotal is missing");
                    /*
                   stableNitrogen = manureNitrogenTotal*arrStable.Rows[s][3]/100*arrStable.Rows[s][7]/100 + stableBeddingNitrogen;
               '    Response.write "<br> stableNitrogen : " & stableNitrogen 
                   stableNitrogenNorm = manureNitrogenTotalNorm*arrStable(s,3)/100*arrStable(s,7)/100 + stableBeddingNitrogen
               '    Response.write "<br> stableNitrogenNorm : " & stableNitrogenNorm 
    
                   stableNitrogenLoss = stableNitrogen*arrStable(s,5)/100
                   stableNitrogenAfterLoss = stableNitrogen - stableNitrogenLoss
                   stableNitrogenNormAfterLoss = stableNitrogenNorm*(100-arrStable(s,5))/100
              '     Response.write "<br> stableNitrogenNormAfterLoss : " & stableNitrogenNormAfterLoss 

                    redim preserve arrParameter(7)
                  arrParameter(2) = 4
                  arrParameter(3) = arrStable(s,1)
                  arrParameter(4) = 0
                  arrParameter(5) = 0
                  arrParameter(6) = stableNitrogenLoss
                  arrParameter(7) = stableNitrogenAfterLoss
                  returnValue = CallSPReturn ("IncrementStable",arrParameter)
             '     IncrementStable  aFarmNumber,aScenarioID,4,arrStable(s,1),0,0,stableNitrogenLoss,stableNitrogenAfterLoss
      
                   storageNitrogen = stableNitrogenAfterLoss
                   storageNitrogenLossAmmonia = storageNitrogen*arrStable(s,8)/100
                   storageNitrogenLossDenitrification = storageNitrogen*arrStable(s,9)/100
                   storageNitrogenAfterLoss = storageNitrogen - storageNitrogenLossAmmonia - storageNitrogenLossDenitrification
                   storageNitrogenNormAfterLoss = stableNitrogenNormAfterLoss * (100- arrStable(s,8) - arrStable(s,9))/100
               '    Response.write "<br> storageNitrogenNormAfterLoss : " & storageNitrogenNormAfterLoss 
      
                   redim preserve arrParameter(14)
                  arrParameter(3) = arrStable(s,4)
                  arrParameter(4) = stableNitrogen
                  arrParameter(5) = stableNitrogenLoss
                  arrParameter(6) = stableNitrogenAfterLoss
                  arrParameter(7) = storageNitrogenLossAmmonia
                  arrParameter(8) = storageNitrogenLossDenitrification
                  arrParameter(9) = storageNitrogenAfterLoss
                  arrParameter(10) = 0
                  arrParameter(11) = 0
                  arrParameter(12) = 0
                  arrParameter(13) = 0
                  arrParameter(14) = 0  
                 returnValue = CallSPReturn ("IncrementStorage",arrParameter)
             '	  IncrementStorage aFarmNumber,aScenarioID,4,arrStable(s,4),stableNitrogen,stableNitrogenLoss,stableNitrogenAfterLoss,storageNitrogenLossAmmonia,storageNitrogenLossDenitrification,storageNitrogenAfterLoss,0,0,0,0,0  
                   redim preserve arrParameter(4)
                  arrParameter(3) = arrStable(s,4)
                  arrParameter(4) = storageNitrogenNormAfterLoss
                  returnValue = CallSPReturn ("IncrementStorageNorm",arrParameter)}*/

                }
            }
        }
    }
}
