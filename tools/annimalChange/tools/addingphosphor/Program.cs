using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Include_phosphor
{
    class Program
    {
        static void Main(string[] args)
        {
            FileInformation feedstuff = new FileInformation(args[0]);
            feedstuff.adding();
            feedstuff.write();

        }
    }
}
