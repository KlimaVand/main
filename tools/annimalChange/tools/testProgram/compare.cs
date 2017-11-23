using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;
using System.IO;

namespace testFarmAC
{
    class compare
    {
        System.IO.StreamWriter OutputFile; 
        public compare()
        {
            
        }
        ~compare()
        {
           
        }
        public void compare2Files()
        {  
            try
            {
                StreamReader sr = new StreamReader("input.txt");
                int i = 0;
                while (!sr.EndOfStream)
                {
                    i++;
                    string fileinput = sr.ReadLine();
                    string[] BothNames = fileinput.Split('\t');
                    OutputFile = new System.IO.StreamWriter("output"+i.ToString()+".txt");
                    XmlReader data1 = XmlReader.Create(BothNames[0]);
                    XmlReader data2 = XmlReader.Create(BothNames[1]);

                    while (data1.Read() && data2.Read())
                    {
                        if (data1.NodeType == XmlNodeType.Element)
                        {
                            XElement el1 = XNode.ReadFrom(data1) as XElement;
                            IEnumerable<XElement> node1 = el1.Elements();
                            XElement el2 = XNode.ReadFrom(data2) as XElement;
                            IEnumerable<XElement> node2 = el2.Elements();


                            recursionCompare(node1, node2);
                        }
                    }

                    data1.Close();
                    data2.Close();
                    OutputFile.Close();
                }
                sr.Close();
            }
            catch (Exception e)
            {
                OutputFile.WriteLine("A general error has happend "+e.ToString());
            }

        
        
        }
        private void recursionCompare(IEnumerable<XElement> node1, IEnumerable<XElement> node2)
        {

            if (node1.Count() == node2.Count())
            {

                for (int i = 0; i < node1.Count(); i++)
                {
                    
                    if (node1.ElementAt(i).Name.ToString().CompareTo(node2.ElementAt(i).Name.ToString())!=0)
                    {
                        OutputFile.WriteLine(node1.ElementAt(i).Name.ToString()+ " does not match " + node2.ElementAt(i).Name.ToString());
                   
                    }
                    if (node1.ElementAt(i).Value != node2.ElementAt(i).Value)
                    {
                        IEnumerable<XElement> subNode = node1.ElementAt(i).Elements();
                        if (subNode.Count() == 0)
                        {
                            OutputFile.Write(node1.ElementAt(i).Value + " does not match " + node2.ElementAt(i).Value);
                            OutputFile.WriteLine(" in "+node1.ElementAt(i).Name.ToString());
                        }

                    }

                    IEnumerable<XElement> subNode1 = node1.ElementAt(i).Elements();
                    IEnumerable<XElement> subNode2 = node2.ElementAt(i).Elements();
                    recursionCompare(subNode1, subNode2);
                }
            }
            else
            {
                OutputFile.WriteLine("number of subnodes do not match");
            }
            
        }
        

    }
}
