using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;

namespace testFarmAC
{
    class Program
    {
        static void Main(string[] args)
        {
            compare test = new compare();
            test.compare2Files();
            Console.WriteLine("done");
        }
    }
    
}
