using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Animal_housing
{
    class Program
    {
        static void Main(string[] args)
        {
            AnimalHousingClass aHouse1 = new AnimalHousingClass(6.0, 22.0, 1.5, 5.0, 0.8, 0.04, 0.8, 293.0, 2.0, 4.0, 10000.0);
            aHouse1.Ventilation(1);
            AnimalHousingClass aHouse2 = new AnimalHousingClass(0, 6.0, 22.0, 1.5, 5.0, 0.8, 0.04, 0.8, 0.1, 20.0, 5.0, 0.05);
            aHouse2.Ventilation(0);
        }
    }
}
