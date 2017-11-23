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
            IO extrting = new IO(args[2]);
            ting.setPath(args[1]);
            ting.getItemString(extrting.tree.SubNode[0]);
            ting.write();

            
        }
        
    }
}
