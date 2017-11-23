using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Linq;

namespace analsyse
{
    class Program
    {
        static void Main(string[] args)
        {
            string[] lines = System.IO.File.ReadAllLines(@"setup.txt");
            int filenr = 0;
            XmlWriterSettings ws = new XmlWriterSettings();
            ws.Indent = true;
            XmlWriter newSystemFile = XmlWriter.Create("NewSystem.xml", ws);
            newSystemFile.WriteStartDocument();
            newSystemFile.WriteStartElement("xml");
            int IdentityCount = 0;
            for(int i=0;i<lines.Count();i++)
            {
                string var = lines[i].Split('\t')[0];
                string procent = lines[i].Split('\t')[1];
                string FileName = lines[i].Split('\t')[2];
                XmlReader data = XmlReader.Create("System.xml");

                while (data.Read())
                {
                    if (data.NodeType == XmlNodeType.Element)
                    {
                        XElement el = XNode.ReadFrom(data) as XElement;
                        IEnumerable<XElement> node = el.Elements();

                        for (int j= 0; j < node.Count(); j++)
                        {
                            IEnumerable<XElement> ting = node.ElementAt(j).Elements();


                            for (int k = 0; k < ting.Count(); k++)
                            {

                                if (ting.ElementAt(k).Name == FileName)
                                {
                                    string fileName = ting.ElementAt(k).Value;
                                    fileName = fileName.Split('.')[0] + j + lines[i].Split('\t')[0];
                                   
                                    replaceData(ting.ElementAt(k).Value, var, procent, j,fileName);

                                }
                            }
                        }
                    }
                }


                data.Close();

                data = XmlReader.Create("System.xml");

                while (data.Read())
                {
                    if (data.NodeType == XmlNodeType.Element)
                    {
                        XElement el = XNode.ReadFrom(data) as XElement;
                        IEnumerable<XElement> node = el.Elements();
                       
                        for (int j = 0; j < node.Count(); j++)
                        {
                            IEnumerable<XElement> ting = node.ElementAt(j).Elements();
                            
                            for (int l = 0; l < 2; l++)
                            {
                                newSystemFile.WriteStartElement("settings");
                                for (int k = 0; k < ting.Count(); k++)
                                {
                                    newSystemFile.WriteStartElement(ting.ElementAt(k).Name.ToString());
                                    if (ting.ElementAt(k).Name == FileName)
                                    {

                                        string fileName = ting.ElementAt(k).Value;
                                        if(l==1)
                                            fileName = fileName.Split('.')[0] + j + lines[i].Split('\t')[0]+"m.xml";
                                        else
                                            fileName = fileName.Split('.')[0] + j +lines[i].Split('\t')[0]+ "p.xml";
                                        newSystemFile.WriteValue(fileName);
                                    }
                                    else
                                    {
                                        if (ting.ElementAt(k).Name.ToString() == "Identity")
                                        {
                                            newSystemFile.WriteValue(IdentityCount);
                                            IdentityCount++;
                                        }
                                        else

                                            newSystemFile.WriteValue(ting.ElementAt(k).Value);
                                    }
                                    newSystemFile.WriteEndElement();


                                    
                                }
                                newSystemFile.WriteEndElement();
                            }
                            


                        }
                        
                    }
                }


                data.Close();
     
                

            }
            newSystemFile.WriteEndElement();
            newSystemFile.WriteEndDocument();
            newSystemFile.Close();
        }
        static void replaceData(string fileName, string path,string procent, int outputNr,string PartFileName)
        {
             XmlWriterSettings ws = new XmlWriterSettings();
            ws.Indent = true;
              XmlWriter writerMinus = XmlWriter.Create(PartFileName + "m.xml", ws);

            XmlWriter writerPlus = XmlWriter.Create(PartFileName+ "p.xml", ws);

            writerMinus.WriteStartDocument();
            writerMinus.WriteStartElement("xml");

            writerPlus.WriteStartDocument();
            writerPlus.WriteStartElement("xml");

            string[] itemInpath=path.Split('.');
            XmlReader data = XmlReader.Create(fileName);
            while (data.Read())
            {
                if (data.NodeType == XmlNodeType.Element)
                {
                    
                    XElement el = XNode.ReadFrom(data) as XElement;
                    IEnumerable<XElement> node = el.Elements();

                    recursionRead(node, itemInpath, 0, double.Parse(procent), writerMinus, writerPlus,true);
                   

                }
            }
            writerMinus.WriteEndElement();
            writerMinus.WriteEndDocument();
            writerMinus.Close();
            writerPlus.WriteEndElement();
            writerPlus.WriteEndDocument();
            writerPlus.Close();
        }
        static private void recursionRead(IEnumerable<XElement> node, string[] itemInpath, int reNr, double procent, XmlWriter writerMinus, XmlWriter writerPlus, bool partOfChange)
        {
            if (itemInpath.Count() == reNr)
            {
            }
            for (int i = 0; i < node.Count(); i++)
            {
                
                writerMinus.WriteStartElement(node.ElementAt(i).Name.ToString());
                 writerPlus.WriteStartElement(node.ElementAt(i).Name.ToString());
                if (itemInpath.Count() ==( reNr+1) && node.ElementAt(i).Name.ToString().CompareTo(itemInpath[reNr]) == 0)
                {
                    if (partOfChange)
                    {
                        double baseValue = double.Parse(node.ElementAt(i).Value);

                        //  node.ElementAt(i).Value = (procent / 100 * double.Parse(node.ElementAt(i).Value)  + double.Parse(node.ElementAt(i).Value)).ToString();

                        string stuf1 = (baseValue+baseValue * procent / 100).ToString();
                        writerPlus.WriteValue(stuf1);
                        stuf1 = (baseValue - baseValue * procent / 100).ToString();
                        writerMinus.WriteValue(stuf1);
     
                    }
                    else
                    {
                        string baseValue = node.ElementAt(i).Value.ToString();

                        writerPlus.WriteValue(baseValue.ToString());
                        writerMinus.WriteValue(baseValue.ToString());
                    }

                }
                else if(itemInpath.Count() ==( reNr+1))
                {
                    string baseValue = node.ElementAt(i).Value.ToString();

                    writerPlus.WriteValue(baseValue.ToString());
                    writerMinus.WriteValue(baseValue.ToString());
                }

                if (node.ElementAt(i).Name.ToString().CompareTo( itemInpath[reNr])==0)
                {
                    
                    IEnumerable<XElement> ting = node.ElementAt(i).Elements();
                    recursionRead(ting, itemInpath, reNr + 1, procent, writerMinus,writerPlus,true);
                
                }
                else
                {
                    IEnumerable<XElement> ting = node.ElementAt(i).Elements();
                    recursionRead(ting, itemInpath, reNr + 1, procent, writerMinus,writerPlus, false);
                }


                writerMinus.WriteEndElement();
                writerPlus.WriteEndElement();




            }

        }
    }
}
