using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;

namespace puttingInformatinIntoIntoAxmlFile
{
    class IO
    {
        public void start(string farm, string colour)
        {
            XmlReader data = XmlReader.Create(farm);
            XmlReader ColourInXML = XmlReader.Create(colour);
            string[] outputfileName = farm.Split('.');
            XmlWriter writer = XmlWriter.Create(outputfileName[0]+"_Output.xml");
            writer.WriteStartDocument();
            List<string> path = new List<string>();
            while (data.Read() && ColourInXML.Read())
            {
                if (data.NodeType == XmlNodeType.Element)
                {
                    XElement el = XNode.ReadFrom(data) as XElement;
                    IEnumerable<XElement> node = el.Elements();
                    path.Add(node.ElementAt(0).Name.ToString());
                    Console.WriteLine(node.ElementAt(0).Name.ToString());
                    XElement elCoulurData = XNode.ReadFrom(ColourInXML) as XElement;
                    IEnumerable<XElement> nodeCoulurData = elCoulurData.Elements();
                    writer.WriteStartElement(el.Name.ToString());
                    for (int i = 0; i < node.Count(); i++)
                    {
                        writer.WriteStartElement(node.ElementAt(i).Name.ToString());
                       
                        IEnumerable<XElement> ting = node.ElementAt(i).Elements();

                        recursionForItem(ting, writer, nodeCoulurData, path);
                        writer.WriteEndElement();
                    }
                    writer.WriteEndElement();
                }
            }
            
            writer.Close();
            data.Close();
            ColourInXML.Close();
        }
        public XAttribute recursionForColour(IEnumerable<XElement> nodeColur, List<string> path, int number)
        {
            for (int i = 0; i < nodeColur.Count(); i++)
            {
                //Console.Write("Tag ");
                //Console.Write(path.ElementAt(number));
                //Console.Write(" col ");
                //Console.WriteLine(nodeColur.ElementAt(i).Name.ToString());
                if(path.ElementAt(number).CompareTo(nodeColur.ElementAt(i).Name.ToString())==0)
                {
                    if ((number + 1) == path.Count)
                    {
                            XAttribute t = nodeColur.ElementAt(i).FirstAttribute;
                            return t;                        
                    }
                    else
                    {
                        IEnumerable<XElement> ting = nodeColur.ElementAt(i).Elements();
                        return recursionForColour(ting, path, number + 1);
                    }
                }
            }
            return null;
        }
        public void recursionForItem(IEnumerable<XElement> node, XmlWriter writer, IEnumerable<XElement> nodeColur, List<string> path)
        {
            List<string> clone =new List<string>();
            for (int i = 0; i < path.Count; i++)
            {
                clone.Add(path.ElementAt(i));
            }
 
            for (int i = 0; i < node.Count(); i++)
            {
        
                IEnumerable<XElement> ting = node.ElementAt(i).Elements();

                if (node.ElementAt(i).HasElements)
                {
                    clone.Add(node.ElementAt(i).Name.ToString());
                    writer.WriteStartElement(node.ElementAt(i).Name.ToString());
                }
                else
                {
                   // Console.WriteLine(node.ElementAt(i).Name + " " + node.ElementAt(i).Value.ToString());
                    //clone.Clear();
                    clone.Add(node.ElementAt(i).Name.ToString());
                    XAttribute colour = recursionForColour(nodeColur, clone, 0);
                    
                    if (colour != null)
                    {
                                writer.WriteStartElement(node.ElementAt(i).Name.ToString());
                                writer.WriteAttributeString(colour.Name.ToString(), colour.Value.ToString());
                                writer.WriteValue(node.ElementAt(i).Value.ToString());
                                writer.WriteEndElement();
                                
                    }
                    else
                    {
                       // Console.WriteLine(node.ElementAt(i).Name.ToString()+" "+ node.ElementAt(i).Value.ToString());
                        writer.WriteElementString(node.ElementAt(i).Name.ToString(),node.ElementAt(i).Value.ToString());
                    }
                    clone.RemoveAt(clone.Count - 1);
                }
                recursionForItem(ting, writer, nodeColur, clone);
             //   Console.WriteLine(node.ElementAt(i).Name + " " + node.ElementAt(i).Value);
                if (node.ElementAt(i).HasElements)
                {
                    clone.RemoveAt(clone.Count - 1);
                    writer.WriteEndElement();
                }


            }

        }
    }
}
