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
            IO ting = new IO();
            ting.start(args[0],args[1]);
        }
        
    }
}
