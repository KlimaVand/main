using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;
using AnimalChange;
namespace puttingInformatinIntoIntoAxmlFile
{
    class IO
    {

        private string fileName = "test.xml";
        public List<int> Identity = new List<int>();
        public List<string> PathNames = new List<string>();
        public Node tree;

        public IO(string nameOfFile)
        {
            fileName = nameOfFile;
            tree = new Node();
            try
            {
                XmlReader data = null;
                try
                {
                    data = XmlReader.Create(fileName);
                }
                catch
                {
                    Console.WriteLine(fileName + " not found");
                    Environment.Exit(0);
                }
                while (data.Read())
                {
                    if (data.NodeType == XmlNodeType.Element)
                    {
                        XElement el = XNode.ReadFrom(data) as XElement;
                        IEnumerable<XElement> node = el.Elements();

                        for (int i = 0; i < node.Count(); i++)
                        {
                            IEnumerable<XElement> ting = node.ElementAt(i).Elements();
                            Node newNode = new Node();

                            newNode.setNodeValue(node.ElementAt(i).Value);
                            newNode.setNodeName(node.ElementAt(i).Name.ToString());
                            recursionRead(ting, ref newNode);
                            tree.addChild(newNode);


                        }
                    }
                }
                data.Close();

            }
            catch (Exception e)
            {
                Console.WriteLine(e.ToString());
                if (e.Message.CompareTo("farm Fail") != 0)
                {
                    string messageString = ("problem with reading: " + fileName + " because of " + e.ToString()) + "\r\n";
                    messageString += ("model terminated") + "\r\n";
                    messageString += ("the path is: ");
                    for (int i = 0; i < PathNames.Count; i++)
                    {
                        messageString += (PathNames[i] + "(" + Identity[i].ToString() + ")");
                    }


                    Console.WriteLine(messageString);
                }
                else
                {
                    throw new System.Exception("farm Fail");
                }
            }
        }
        public void setPath(string name)
        {
            PathNames.Clear();
            Identity.Clear();
            string[] items = name.Split('.');
            for (int i = 0; i < items.Count(); i++)
            {
                int first = items[i].IndexOf('(');
                int last = items[i].IndexOf(')');
                string tmp = items[i];
                if (last != -1 && first != -1)
                {
                    try
                    {
                        tmp = items[i].Remove(last, 1);
                        tmp = tmp.Remove(0, first + 1);
                        Identity.Add(Convert.ToInt32(tmp));
                        tmp = items[i].Remove(first);
                    }
                    catch
                    {

                        string messageString = (name + " is wrong") + "\n";
                        messageString += ("model terminated") + "\n";
                        messageString += ("the file name is " + fileName) + "\n";
                        Console.WriteLine(messageString);

                    }
                }
                PathNames.Add(tmp);
            }
        }
        private void recursionRead(IEnumerable<XElement> node, ref Node subNode)
        {

            for (int i = 0; i < node.Count(); i++)
            {
                IEnumerable<XElement> ting = node.ElementAt(i).Elements();



                Node child = new Node();
                child.setNodeName(node.ElementAt(i).Name.ToString());
                child.setNodeValue(node.ElementAt(i).Value);

                recursionRead(ting, ref child);

                subNode.SubNode.Add(child);

            }

        }
        public void writeRecursive(ref XmlWriter writer, Node tree)
        {
           
            writer.WriteStartElement(tree.getNodeName());
            if (tree.SubNode.Count > 0)
            {
                for (int i = 0; i < tree.SubNode.Count; i++)
                {
                    writeRecursive(ref writer,tree.SubNode[i]);
                }
            }
            else
            {
                writer.WriteValue(tree.getNodeValue());
            }
            writer.WriteEndElement();
        }
        public void write()
        {
            XmlWriter writer = XmlWriter.Create("Output.xml");
            writer.WriteStartDocument();
            Console.WriteLine(tree.SubNode[0].getNodeName());

            writeRecursive(ref writer,tree.SubNode[0]);
            writer.Close();
        }
        public string getItemString(Node newInformation)
        {
            for (int i = 0; i < tree.SubNode.Count(); i++)
                if (PathNames[0].CompareTo(tree.SubNode[i].getNodeName()) == 0)
                {

                    if (tree.SubNode[i].SubNode.ElementAt(0).getNodeValue() == Identity[0].ToString() || Identity[0] == -1)
                        return recursionForItem(ref tree.SubNode[i].SubNode, 0, newInformation);
                }


            return "nothin";


        }
        public string recursionForItem(ref List<Node> node, int iteration, Node newInformation)
        {

           
                
                if (node.ElementAt(0).getNodeName().ToString().CompareTo("Identity") == 0 || Identity[iteration] == -1)
                {
                    if (node.ElementAt(0).getNodeValue() == Identity[iteration].ToString() || Identity[iteration] == -1)
                    {
                        if (Identity.Count()-1 == (iteration))
                        {

                            node.Add(newInformation);
                                       
                                        
                        }
                        else
                        {
                            for (int j = 0; j < node.Count(); j++)
                            {

                                if (node.ElementAt(j).getNodeName().ToString().CompareTo(PathNames[iteration + 1]) == 0)
                                {
                                    List<Node> ting = node.ElementAt(j).SubNode;
                                    if (ting.ElementAt(0).getNodeValue() == Identity[iteration + 1].ToString() || Identity[iteration + 1] == -1)
                                    {
                                        recursionForItem(ref ting, iteration + 1, newInformation);
                                        
                                    }
                                }

                            }
                        }

                    
                }
            }

    

            return "nothing";
        }
    }



    
}
