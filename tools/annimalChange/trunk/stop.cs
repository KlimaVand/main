using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace AnimalChange
{
    class stop
    {
        int time;
        public stop(int aTime)
        {
           time =aTime;
        }
        public void counDown()
        {
            Thread.Sleep(time);
            Console.WriteLine("AnimalChange is halting since time is running out");
            Environment.Exit(0);
        }
    }
}
