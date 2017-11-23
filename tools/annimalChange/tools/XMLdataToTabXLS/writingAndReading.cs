using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Linq;

//Reads FarmAC xml file and outputs text file for reading into Excel. Remember to set 'wrap text' in Excel before copying into clipboad and Word
namespace XMLdataToTabXLS
{
    class writingAndReading
    {
        public void main(string input,string output)
        {
            int max = 0;
            System.IO.StreamWriter file=null;
            try
            {
                file = new System.IO.StreamWriter(output);
            }
            catch
            {
                Console.WriteLine("cannot find file");
            }

            XmlReader data = null;
            try
            {
                data = XmlReader.Create(input);

            }
            catch
            {
                Console.WriteLine("cannot find file");
            }
            while (data.Read())
            {
                if (data.NodeType == XmlNodeType.Element)
                {
                    XElement el = XNode.ReadFrom(data) as XElement;
                    IEnumerable<XElement> node = el.Elements();
                    
                    counting(node,0, ref max);


                }
            }
            data.Close();

            data = null;
            try
            {
                data = XmlReader.Create(input);

            }
            catch
            {
                Console.WriteLine("cannot find file");
            }
            while (data.Read())
            {
                if (data.NodeType == XmlNodeType.Element)
                {
                    XElement el = XNode.ReadFrom(data) as XElement;
                    IEnumerable<XElement> node = el.Elements();
                    recursionRead(node, file, 0,max,false);

                    
                }
            }
            data.Close();
            file.Close();
        }
        private void counting(IEnumerable<XElement> node, int min,ref int max)
        {
            if (max < min)
                max = min;
            for (int i = 0; i < node.Count(); i++)
            {
                IEnumerable<XElement> ting = node.ElementAt(i).Elements();


                if (node.ElementAt(i).HasElements)
                {
                    // file.WriteLine();
                    counting(ting, min + 1, ref max);

                }
            }
        }
        private bool recursionRead(IEnumerable<XElement> node, System.IO.StreamWriter file, int min, int max, bool found2)
        {
            bool found = false;
            bool found1 = false;
            bool writeBefore = false;
            string Description="";
            string Units = "";
            string Value = "";
            for (int i = 0; i < node.Count(); i++)
            {
                IEnumerable<XElement> ting = node.ElementAt(i).Elements();

                if (!node.ElementAt(i).HasElements)
                {
                    if ("Description".CompareTo(node.ElementAt(i).Name.ToString()) == 0)
                    {   
                        
                        for (int t = 0; t < (max - min); t++)
                           Description+=('\t');
                        
                        Description += ('\t');
                        Description += (node.ElementAt(i).Value);
                        found = true;
                    }
                    if ("Units".CompareTo(node.ElementAt(i).Name.ToString()) == 0 || "Unit".CompareTo(node.ElementAt(i).Name.ToString()) == 0)
                    {


                        Units+=('\t');
                        Units += (node.ElementAt(i).Value);
                        found = true;
                    }
                    if ("Value".CompareTo(node.ElementAt(i).Name.ToString()) == 0)
                    {

                        Value+=('\t');
                         Value+=(node.ElementAt(i).Value);
                        found = true;
                    }
                    
                }
                else
                {
                    if (found == true && found2 == true && writeBefore==false)
                    {
                        file.Write(Description);
                        file.Write(Units);
                        file.Write(Value);
                        writeBefore = true;
                    }

                    if (found == true || found2)
                        file.WriteLine();
        
                    for (int t = 0; t < min; t++)
                    {
                        file.Write('\t');
                    }
                    Console.Write(node.ElementAt(i).Name.ToString());
                    file.Write(node.ElementAt(i).Name.ToString());
                    found1 = true;

                }
                
               
                if (node.ElementAt(i).HasElements)
                {
                   // file.WriteLine();
                    recursionRead(ting, file, min + 1, max, found1);
       
                   
                }


            }
            if (found == true && writeBefore==false)
            {

                file.Write(Description);
                file.Write(Units);
                file.Write(Value);

            }
            return found;


        }
    }
}
