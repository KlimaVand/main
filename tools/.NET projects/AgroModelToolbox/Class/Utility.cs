using System.Configuration;
using System;
using System.Globalization;

namespace AgroModelToolbox.Class
{
   public class Utility
   {
      private static string applicationID="88205b81-4a76-4a7d-b6a5-dca5dfff4383";

      public static string ApplicationID { get { return applicationID; } }

      public static string GetConnectionString(string name)
      {
         // Assume the name won't be found.
         string returnValue=null;

         ConnectionStringSettings settings=ConfigurationManager.ConnectionStrings[name];
         if(settings!=null)
         {
            returnValue=settings.ConnectionString;
         }
         return returnValue;
      }

      public static string DevelopmentStatus(int aStatusID)
      {
         string returnValue="";
         switch(aStatusID)
         {
            case 1:
               returnValue="Under construction";
               break;
            case 2:
               returnValue="Test version";
               break;
            case 3:
               returnValue="???";
               break;
            default:
               break;
         }
         return returnValue;
      }

      public static string LogUser(string conStr,string aLogName,string aIPAddress,string aBrowser,bool aShowMode)
      {
         string returnValue="";

         DateTime aDateTime = DateTime.Now;
         string aLogDateAndTime="";
         aLogDateAndTime+=aDateTime.Year;
         aLogDateAndTime+="-";
         aLogDateAndTime+=aDateTime.Month;
         aLogDateAndTime+="-";
         aLogDateAndTime+=aDateTime.Day;
         aLogDateAndTime+=" ";
         aLogDateAndTime+=aDateTime.Hour;
         aLogDateAndTime+=":";
         aLogDateAndTime+=aDateTime.Minute;
         aLogDateAndTime+=":00";
         int aLogYear=aDateTime.Year;
         int aLogMonth=aDateTime.Month;
         int aLogDay=aDateTime.Day;
         int aLogWeek=GetWeekNumber(aDateTime);
         int aLogWeekDay=Convert.ToInt32(aDateTime.DayOfWeek);
         int aLogDayNo=aDateTime.DayOfYear;
         string[] arrIPAddress=aIPAddress.Split('.');
         int aIPPart1=0;
         int aIPPart2=0;
         int aIPPart3=0;
         int aIPPart4=0;
         long aIPNumber=0;
         string aCountryID="";
         if(arrIPAddress.Length==4)
         {
            if(int.TryParse(arrIPAddress[0],out aIPPart1)) { } else aIPPart1=-9;
            if(int.TryParse(arrIPAddress[1],out aIPPart2)) { } else aIPPart2=-9;
            if(int.TryParse(arrIPAddress[2],out aIPPart3)) { } else aIPPart3=-9;
            if(int.TryParse(arrIPAddress[3],out aIPPart4)) { } else aIPPart4=-9;

            aIPNumber=16777216*aIPPart1+65536*aIPPart2+256*aIPPart3+aIPPart4;
            aCountryID="ZZ";// HostPathogenDataRead.GetCountryByIP(conStr,aIPNumber);
         }

         returnValue=DataExec.InsertLogUser(conStr,aLogDateAndTime,aLogYear,aLogMonth,aLogDay,aLogWeek,aLogWeekDay,aLogDayNo,aLogName,aIPAddress,aIPPart1,aIPPart2,aIPPart3,aIPPart4,aBrowser,aShowMode,aCountryID);

         return returnValue;
      }

      public static int GetWeekNumber(DateTime aDateTime)
      {
         CultureInfo ciCurr = CultureInfo.CurrentCulture;
         int weekNum = ciCurr.Calendar.GetWeekOfYear(aDateTime,CalendarWeekRule.FirstFourDayWeek,DayOfWeek.Monday);
         return weekNum;
      }
   }
}