using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;

namespace puttingInformatinIntoIntoAxmlFile
{
    class Program
    {
        static void Main(string[] args)
        {
            IO ting = new IO(args[0]);
            ting.setPath(args[1]);
            ting.getItemString(args[2], args[3]);
            ting.write();
            
        }
        
    }
}
