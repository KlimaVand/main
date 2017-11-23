using System;
using System.Data;
using System.Data.SqlClient;
using System.IO;
using System.Web;

namespace MacsurToolbox.Class
{
   public class DataExec
   {
      public static string InsertLogUser(string conStr,string aLogDateAndTime,int aLogYear,int aLogMonth,int aLogDay,int aLogWeek,int aLogWeekDay,int aLogDayNo,string aLogName,string aIPAddress,int aIPPart1,int aIPPart2,int aIPPart3,int aIPPart4,string aBrowser,bool aShowMode,string aCountryID)
      {
         string returnValue="";
         SqlConnection con = null;

         try
         {
            using(con=new SqlConnection(conStr))
            {
               SqlCommand cmdInsert = null;
               string strSQLInsert="INSERT INTO gen_LogUser";
               strSQLInsert+=" (LogDateAndTime";
               strSQLInsert+=",LogYear,LogMonth,LogDay";
               strSQLInsert+=",LogWeek,LogWeekDay,LogDayNo";
               strSQLInsert+=",LogName";
               strSQLInsert+=",IPNumber, IPPart1, IPPart2, IPPart3, IPPart4";
               strSQLInsert+=",Browser";
               strSQLInsert+=",ShowMode";
               strSQLInsert+=",CountryID)";
               strSQLInsert+=" VALUES ('"+aLogDateAndTime+"'";
               strSQLInsert+=","+aLogYear.ToString()+","+aLogMonth.ToString()+","+aLogDay.ToString()+"";
               strSQLInsert+=","+aLogWeek.ToString()+","+aLogWeekDay.ToString()+","+aLogDayNo.ToString()+"";
               strSQLInsert+=",'"+aLogName+"'";
               strSQLInsert+=",'"+aIPAddress+"',"+aIPPart1.ToString()+","+aIPPart2.ToString()+","+aIPPart3.ToString()+","+aIPPart4.ToString()+"";
               strSQLInsert+=",'"+aBrowser+"'";
               strSQLInsert+=",'"+aShowMode.ToString()+"'";
               strSQLInsert+=",'"+aCountryID.ToString()+"')";
               //WriteSQL2File("InsertLogUser",strSQLInsert);
               using(cmdInsert=new SqlCommand(strSQLInsert,con))
               {
                  cmdInsert.CommandType=CommandType.Text;
                  con.Open();
                  cmdInsert.ExecuteNonQuery();
                  con.Close();
               }
            }
         }
         catch(Exception ex)
         {
            returnValue="<br/><br/>Function: InsertLogUser, Message: "+ex.Message;
         }
         return returnValue;
      }

      public static string InsertUserAdditional(string conStr,string aUserID,string aUserInit,string aFirstName,string aLastName,string aInstitution,string aDepartment,string aCountryID)
      {
         string returnValue="";
         SqlConnection con=null;

         try
         {
            using(con=new SqlConnection(conStr))
            {
               SqlCommand cmd=null;
               string strSQL="INSERT INTO [UserAdditional]";
               strSQL+=" ([UserID], [UserInit], [FirstName], [LastName], [Institution], [Department], [CountryID])";
               strSQL+=" VALUES ('"+aUserID+"','"+aUserInit+"','"+aFirstName+"','"+aLastName+"','"+aInstitution+"','"+aDepartment+"','"+aCountryID+"')";
               WriteSQL2File("InsertCropProblemUser",strSQL);
               using(cmd=new SqlCommand(strSQL,con))
               {
                  cmd.CommandType=CommandType.Text;
                  con.Open();
                  cmd.ExecuteNonQuery();
                  con.Close();
               }
            }
         }
         catch(Exception ex)
         {
            returnValue="Function: InsertCropProblemUser, Message: "+ex.Message;
         }
         return returnValue;
      }

      private static void WriteSQL2File(string aName,string aStrSQL)
      {
         if(HttpContext.Current.Server.MachineName=="JPM-LP-111020"||HttpContext.Current.Server.MachineName=="POLAS")
         {
            TextWriter tw=new StreamWriter("C:\\Documents\\SQL\\Macsur_"+aName+".sql");
            tw.Write(aStrSQL);
            tw.Close();
         }
      }
   }
}