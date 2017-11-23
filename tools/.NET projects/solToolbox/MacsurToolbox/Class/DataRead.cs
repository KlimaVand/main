using System.Configuration;
using System.IO;
using System.Web;

namespace MacsurToolbox.Class
{
   public class DataRead
   {
      private static string GetConnectionString(string name)
      {
         string returnValue=null; // Assume the name won't be found.

         ConnectionStringSettings settings=ConfigurationManager.ConnectionStrings[name];
         if(settings!=null)
         {
            returnValue=settings.ConnectionString;
         }
         return returnValue;
      }

      private static void WriteSQL2File(string aName,string aStrSQL)
      {
         if(HttpContext.Current.Server.MachineName=="JPM-LP-111020"||HttpContext.Current.Server.MachineName=="POLAS")
         {
            TextWriter tw=new StreamWriter("C:\\Documents\\SQL\\ProjectMgmt_"+aName+".sql");
            tw.Write(aStrSQL);
            tw.Close();
         }
      }
   }
}