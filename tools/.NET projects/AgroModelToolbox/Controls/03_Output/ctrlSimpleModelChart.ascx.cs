using System;
using ChartDirector;
using simpleModel;

namespace AgroModelToolbox.Controls._03_Output
{
   public partial class ctrlSimpleModelChart : System.Web.UI.UserControl
   {
      //private int selectedOption;
      private static double[] dataX = new double[23];
      private double[] dataY, dataY2;
      private simpleModel.simpleModel simpleModel = new simpleModel.simpleModel();
      private double N_org, N_fert, CN3060, a1, a2, a3, C_30, CN30, C3060, Nr, Nc, C_10, Yp, Clay_10, NRE, NRR, NPCT, EP_season, P_season;
      private double CN3060_2, C_30_2, CN30_2, C3060_2, Nr_2, Nc_2, Yp_2, NRE_2, NRR_2, NPCT_2, EP_season_2, P_season_2;
      private double N_total, Norg_share, CN2550, C25, CN25, C2550, Clay_5, Temp, C_resid, D, fx, RTlogic, ypl;//additional and renamed parameters 19/1/15
      private double Norg_share_2, CN2550_2, C25_2, CN25_2, C2550_2, Clay_5_2, Temp_2, C_resid_2, D_2, fx_2, RTlogic_2, ypl_2;//additional and renamed parameters 19/1/15


      protected void Page_Load(object sender,EventArgs e)
      {
          //N_fert = 0;
          //a1 = 0.0037;
          //a2 = 0.19;
          //a3 = 0.35;
          //C_10 = 2;
          //Clay_10 = 15;
          //C3060 = Convert.ToDouble(Dropdown_C3060.SelectedValue.ToString());
          //C3060_2 = Convert.ToDouble(Dropdown_C3060_2.SelectedValue.ToString());
          //Nr = Convert.ToDouble(DropDown_Nr.SelectedValue.ToString());
          //Nr_2 = Convert.ToDouble(DropDown_Nr_2.SelectedValue.ToString());
          //Yp = Convert.ToDouble(DropDown_Yp.SelectedValue.ToString()); ;
          //Yp_2 = Convert.ToDouble(DropDown_Yp_2.SelectedValue.ToString());
          //C_30 = Convert.ToDouble(Dropdown_C30.SelectedValue.ToString());
          //C_30_2 = Convert.ToDouble(Dropdown_C30_2.SelectedValue.ToString());
          //Nc = Convert.ToDouble(DropDown_Nc.SelectedValue.ToString());
          //Nc_2 = Convert.ToDouble(DropDown_Nc_2.SelectedValue.ToString());
          //NRE = Convert.ToDouble(DropDown_NRE.SelectedValue.ToString());
          //NRE_2 = Convert.ToDouble(DropDown_NRE_2.SelectedValue.ToString());
          //NRR = Convert.ToDouble(DropDown_NRR.SelectedValue.ToString());
          //NRR_2 = Convert.ToDouble(DropDown_NRR.SelectedValue.ToString());
          //NRR_2 = Convert.ToDouble(DropDown_NRR_2.SelectedValue.ToString());
          //NPCT = Convert.ToDouble(DropDown_NPCT.SelectedValue.ToString());
          //NPCT_2 = Convert.ToDouble(DropDown_NPCT_2.SelectedValue.ToString());
          //CN3060 = Convert.ToDouble(DropDown_CN3060.SelectedValue.ToString());
          //CN3060_2 = Convert.ToDouble(DropDown_CN3060_2.SelectedValue.ToString());
          //EP_season = Convert.ToDouble(DropDown_EP_season.SelectedValue.ToString());
          //EP_season_2 = Convert.ToDouble(DropDown_EP_season_2.SelectedValue.ToString());
          //P_season = Convert.ToDouble(DropDown_P_season.SelectedValue.ToString());
          //P_season_2 = Convert.ToDouble(DropDown_P_season_2.SelectedValue.ToString());
          //CN30 = Convert.ToDouble(DropDown_CN30.SelectedValue.ToString());
          //CN30_2 = Convert.ToDouble(DropDown_CN30_2.SelectedValue.ToString());

          a1 = 0.0049;
          a2 = 0.12;
          a3 = 0.3;
          Clay_5 = 21;
          Clay_5_2 = 21;
          Temp = 9; //Fra lookup_Climate
          Temp_2 = 9; //Fra lookup_Climate
          C_resid = 4; //Kommer som delprodukt af de allometriske beregninger til C-Tool 
          C_resid_2 = 4; //Kommer som delprodukt af de allometriske beregninger til C-Tool 
          D = Convert.ToDouble(TextBox_D.Text.ToString()); //User-defined. Hvis reduceret jordbearbejdning: Jordbearbejdningsdybde i cm.
          D_2 = Convert.ToDouble(TextBox_D_2.Text.ToString()); //User-defined. Hvis reduceret jordbearbejdning: Jordbearbejdningsdybde i cm.
          ypl = 20;// Convert.ToDouble(DropDown_ypl.SelectedValue.ToString()); ;
          ypl_2 = 20;// Convert.ToDouble(DropDown_ypl_2.SelectedValue.ToString());
          fx = 2;
          fx_2 = 2;
          RTlogic = Convert.ToDouble(DropDown_RTlogic.SelectedValue.ToString()); //User-defined: Alm. jordbearbejdning = 0, Reduceret jordbearbejdning = 1, No-tillage = 2;
          RTlogic_2 = Convert.ToDouble(DropDown_RTlogic_2.SelectedValue.ToString()); //User-defined: Alm. jordbearbejdning = 0, Reduceret jordbearbejdning = 1, No-tillage = 2;


          C2550 = Convert.ToDouble(Dropdown_C3060.SelectedValue.ToString());
          C2550_2 = Convert.ToDouble(Dropdown_C3060_2.SelectedValue.ToString());
          Nr = Convert.ToDouble(DropDown_Nr.SelectedValue.ToString());
          Nr_2 = Convert.ToDouble(DropDown_Nr_2.SelectedValue.ToString());
          Yp = Convert.ToDouble(DropDown_Yp.SelectedValue.ToString()); ;
          Yp_2 = Convert.ToDouble(DropDown_Yp_2.SelectedValue.ToString());
          C25 = Convert.ToDouble(Dropdown_C30.SelectedValue.ToString());
          C25_2 = Convert.ToDouble(Dropdown_C30_2.SelectedValue.ToString());
          Nc = Convert.ToDouble(DropDown_Nc.SelectedValue.ToString());
          Nc_2 = Convert.ToDouble(DropDown_Nc_2.SelectedValue.ToString());
          NRE = Convert.ToDouble(DropDown_NRE.SelectedValue.ToString());
          NRE_2 = Convert.ToDouble(DropDown_NRE_2.SelectedValue.ToString());
          NRR = Convert.ToDouble(DropDown_NRR.SelectedValue.ToString());
          NRR_2 = Convert.ToDouble(DropDown_NRR.SelectedValue.ToString());
          NRR_2 = Convert.ToDouble(DropDown_NRR_2.SelectedValue.ToString());
          NPCT = Convert.ToDouble(DropDown_NPCT.SelectedValue.ToString());
          NPCT_2 = Convert.ToDouble(DropDown_NPCT_2.SelectedValue.ToString());
          CN2550 = Convert.ToDouble(DropDown_CN3060.SelectedValue.ToString());
          CN2550_2 = Convert.ToDouble(DropDown_CN3060_2.SelectedValue.ToString());
          EP_season = Convert.ToDouble(DropDown_EP_season.SelectedValue.ToString());
          EP_season_2 = Convert.ToDouble(DropDown_EP_season_2.SelectedValue.ToString());
          P_season = Convert.ToDouble(DropDown_P_season.SelectedValue.ToString());
          P_season_2 = Convert.ToDouble(DropDown_P_season_2.SelectedValue.ToString());
          CN25 = Convert.ToDouble(DropDown_CN30.SelectedValue.ToString());
          CN25_2 = Convert.ToDouble(DropDown_CN30_2.SelectedValue.ToString());
          Norg_share = Convert.ToDouble(DropDown_Norg_share.SelectedValue.ToString()); ;
          Norg_share_2 = Convert.ToDouble(DropDown_Norg_share_2.SelectedValue.ToString()); ;



          LabelMessage.Text="";

         CreateChart();
      }

      private void CreateChart()
      {
         Chart.setLicenseCode("DEVP-2A2N-7A2A-ZQFB-77D9-D646");

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
             for (int y = 0; y < dataX.Length; y++)
             {
                 //N_org = dataX[y] / 2;
                 //N_fert = dataX[y] / 2;
                 N_total = dataX[y];
                 //dataY[y] = simpleModel.calculate(N_org, N_fert, CN3060, a1, a2, a3, C_30, CN30, C3060, Nr, Nc, C_10, Yp, Clay_10, NRE, NRR, NPCT, EP_season, P_season);
                 //dataY2[y] = simpleModel.calculate(N_org, N_fert, CN3060_2, a1, a2, a3, C_30_2, CN30_2, C3060_2, Nr_2, Nc_2, C_10, Yp_2, Clay_10, NRE_2, NRR_2, NPCT_2, EP_season_2, P_season_2);
                 dataY[y] = simpleModel.calculate(N_total, Norg_share, CN2550, a1, a2, a3, C25, CN25, C2550, Nr, Nc, Yp, Clay_5, NRE, NRR, NPCT, EP_season, P_season, Temp, C_resid, ypl, D, fx, RTlogic);
                 dataY2[y] = simpleModel.calculate(N_total, Norg_share_2, CN2550_2, a1, a2, a3, C25_2, CN25_2, C2550_2, Nr_2, Nc_2, Yp_2, Clay_5_2, NRE_2, NRR_2, NPCT_2, EP_season_2, P_season_2, Temp_2, C_resid_2, ypl_2, D_2, fx_2, RTlogic_2);
             }



         XYChart c=new XYChart(545,390,0xFFFFCC,0x000000);
         c.setRoundedFrame();
         c.addTitle("<*br*>SmartSoil yield model","arialbd.ttf",12);
         c.setPlotArea(50,50,445,290,0xFFFFFF);

         c.xAxis().setTitle("N input [kg N/ha]");
         c.xAxis().setLinearScale(0,250,50);

         c.yAxis().setTitle("Crop yield [t/ha]");
         c.yAxis().setLinearScale(0,14,2);

         ScatterLayer aScatterLayer=c.addScatterLayer(dataX,dataY,"",Chart.CircleSymbol,1,0x0000ff,Chart.Transparent);
         aScatterLayer.setLineWidth(2);
         ScatterLayer aScatterLayer1 = c.addScatterLayer(dataX, dataY2, "", Chart.CircleSymbol, 1, 0xff0000, Chart.Transparent);
         aScatterLayer1.setLineWidth(2);

         WebChartViewerSimpleModelChart.Image=c.makeWebImage(Chart.PNG);
      }
   }
}