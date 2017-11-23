using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Linq;

namespace XMLdataToTabXLS
{
    class Program
    {
        static void Main(string[] args)
        {
            writingAndReading main =new writingAndReading();
            main.main(args[0], args[1]);
        }
        
    }
}
