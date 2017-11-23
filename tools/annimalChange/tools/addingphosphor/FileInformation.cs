using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;
using System.IO;
using System.Xml.XPath;
using AnimalChange;
public class FileInformation
{

    private string fileNameOrg = "test.xml";
    private string fileNameAlt = "test.xml";
    public List<int> Identity = new List<int>();
    public List<string> PathNames = new List<string>();
    private Node tree;

    private bool pauseBeforeExit=false;
    public void writeRecursive(ref XmlWriter writer, Node tree)
    {

        writer.WriteStartElement(tree.getNodeName());
        if (tree.SubNode.Count > 0)
        {
            for (int i = 0; i < tree.SubNode.Count; i++)
            {
                writeRecursive(ref writer, tree.SubNode[i]);
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
        writer.WriteStartElement("xml");
        for (int i = 0;i< tree.SubNode.Count();i++)
            writeRecursive(ref writer, tree.SubNode[i]);
        writer.WriteEndElement();
        writer.Close();
    }
    /**
* Constructor
*/
    private FileInformation()
    {
        
    }
    static List<string> FileNmae = new List<string>();
    static List<Node> AllNodes = new List<Node>();
    /**
* Reading file
*/
    private void ReadingFile(string nameOfFile)
    {
        bool inUse = false;
        for (int i = 0; i < FileNmae.Count; i++)
        {
            if (FileNmae.ElementAt(i).CompareTo(nameOfFile) == 0 && !nameOfFile.Contains("Alternative"))
            {
                inUse = true;
                tree = AllNodes.ElementAt(i);
            }

       

        }
        if (inUse == false)
        {
            
            tree = new Node();
            string itemName = "ino";
            try
            {
                XmlReader data = null;
                try
                {
                    data = XmlReader.Create(nameOfFile);
                    FileNmae.Add(nameOfFile);
                }
                catch
                {
                  
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
                AllNodes.Add(tree);

            }
            catch (Exception e)
            {
                if (nameOfFile!=null)
                if (!nameOfFile.Contains("Alternative.xml"))
                {
                   
                }
            }
        }
    }
    /**
* Constructor 
*/
    public FileInformation(string nameOfFiles)
    {




        ReadingFile(nameOfFiles);

   
  
    }
    public void adding()
    {
        addingrecursuve(ref tree.SubNode);
    }
    int FeedCode;
    public void addingrecursuve(ref List<Node> stuff)
    {
        for (int i = 0; i < stuff.Count; i++)
        {

            if (stuff[i].getNodeName().CompareTo("FeedCode") == 0)
            {
               
                FeedCode = Convert.ToInt32(stuff[i].getNodeValue());
            }

            if (stuff[i].getNodeName().CompareTo("Pconc") != 0)
                addingrecursuve(ref stuff[i].SubNode);
            else
            {
                Console.WriteLine(stuff[i].getNodeName());
                changeValue(ref stuff[i].SubNode);
            }
        }
    }
    public void changeValue(ref List<Node> stuff)
    {
        string[] lines = System.IO.File.ReadAllLines("FeedTable2004_modDC_NJH.txt");
        for (int i = 4; i < lines.Count();i++ )
        {
            string induvidualline = lines[i];
            string[] cells = induvidualline.Split('\t');
            if(FeedCode.ToString().CompareTo(cells[4])==0)
            {
      
                stuff[0].setNodeValue(cells[32]);
                
            }
        }
            
    }
    /**
* recursion Read
*/
    private void recursionRead(IEnumerable<XElement> node,  ref Node subNode)
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
    /**
* Set pauseBeforeExit
*/
    public void SetpauseBeforeExit(bool aVal) { pauseBeforeExit = aVal; }
    /**
* Check if ID exist
*/
    public bool doesIDExist(int ID)
    {


        return recursionDoesIDExist(tree.SubNode, ID, 0);
    

    }
    /**
* Check if ID exist recursive
*/
    private bool recursionDoesIDExist(List<Node> node, int id, int iteration)
    {
        if (Identity.Count() == iteration)
        {
            for (int i = 0; i < node.Count(); i++)
            {
                   //GlobalVars.Instance.log(node.ElementAt(i).Name.ToString());
                
                if (PathNames[iteration].CompareTo(node.ElementAt(i).getNodeName().ToString()) == 0)
                {
                    List<Node> ting = node.ElementAt(i).SubNode;
                  //  GlobalVars.Instance.log(ting.ElementAt(0).Value);
                    if (Convert.ToInt32(ting.ElementAt(0).getNodeValue()) == id)
                        return true;
                }
            }
        }
        else
        {
            for (int j = 0; j < node.Count(); j++)
            {
                if (node.ElementAt(j).getNodeName().ToString().CompareTo(PathNames[iteration]) == 0)
                {
                    List<Node> ting = node.ElementAt(j).SubNode;
                    if (ting.ElementAt(0).getNodeValue() == Identity[iteration].ToString() || Identity[iteration]==-1)
                        return recursionDoesIDExist(ting, id, iteration + 1);
                }
            }
        }
        return false;

    }
    /**
* set Path
*/
    public void setPath(List<int> aIdentity, List<string> aPathNames)
    {
        Identity = aIdentity;
        PathNames = aPathNames;
    }
    /**
* set Path
*/
    public void setPath(string name)
    {
        PathNames.Clear();
        Identity.Clear();
        string[] items=name.Split('.');
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
                 
                  
                }
            }
            PathNames.Add(tmp);
        }

    }
    /**
* set FileName
*/
    public void setFilename(string name)
    { 
        fileNameOrg = name;
        fileNameAlt = name.Split('.')[0]+"Alternative.xml";
    }
    /**
* set ID
*/
    public void setIdentity(List<int> nr)
    {
        Identity = nr; 
    }
    /**
* set PathNames
*/
    public void setPathNames(List<string> name)
    {
        PathNames = name;
    }
    /**
* get Item 
*/
    public int getItemInt(string itemName, bool stopOnError = true)
    {
        string output=getItemString(itemName, stopOnError);
        if (output.CompareTo("nothing") != 0)
            return Convert.ToInt32(getItemString(itemName, stopOnError));
        else return -1;
    }
    /**
* get Item 
*/
    public double getItemDouble(string itemName, bool stopOnError = true)
    {
        string stuff = getItemString(itemName, stopOnError);
        if (stuff.CompareTo("nothing") != 0)
        return Convert.ToDouble(getItemString(itemName, stopOnError));
        else return 0.0;
    }
    /**
* get Item 
*/
    public bool getItemBool(string itemName, bool stopOnError = true)
    {
        string stuff = getItemString(itemName, stopOnError);
        if (stuff.CompareTo("nothing") != 0)
            return Convert.ToBoolean(stuff);
        else return true;
    }

    /**
* get Item 
*/
    public int getItemInt(string itemName, string path, bool stopOnError = true)
    {
        setPath(path);
        return Convert.ToInt32(getItemString(itemName, stopOnError));
    }
    /**
* get Item 
*/
    public double getItemDouble(string itemName, string path, bool stopOnError = true)
    {
        setPath(path);
        return Convert.ToDouble(getItemString(itemName, stopOnError));
    }
    /**
* get Item 
*/
    public bool getItemBool(string itemName, string path, bool stopOnError=true)
    {
        setPath(path);
        string stuff = getItemString(itemName,stopOnError);
        return Convert.ToBoolean(stuff);
    }
    /**
* get Item 
*/
    public string getItemString(string itemName, string path, bool stopOnError=true)
    {
        setPath(path);
        return getItemString(itemName,stopOnError);
    }
    /**
* get Numbers Of Sections 
*/
    public int getNumbersOfSections()
    {
        int total=0;
        int min = 999999;
        int max = 0;
        getSectionNumber(ref min,ref max);
        for (int i = min; i < max; i++)
            if (doesIDExist(i) == true)
                total++;
        return total;

    }
    /**
* set Section
*/
    public void setSection(int sectionNumber)
    {
        int total = 0;
        int min = 999999;
        int max = 0;
        if (Identity.Count == PathNames.Count)
            Identity.RemoveAt(Identity.Count - 1);
        getSectionNumber(ref min, ref max);
        for (int i = min; i <= max; i++)
            if (doesIDExist(i) == true)
            {  
                if (total == sectionNumber)
                {
                    if (Identity.Count == PathNames.Count)
                        Identity[Identity.Count - 1] = i;
                    else
                        Identity.Add(i);
                    break;
                }
                total++;  
            }
    }
    /**
* get SectionNumber
*/
    public void getSectionNumber(ref int min, ref int max)
    {

        recursionForSectionNumber(tree.SubNode, ref min, ref max, 0);

    }
    /**
* get SectionNumber recursion
*/
    private bool recursionForSectionNumber(List<Node> node, ref int min, ref int max, int iteration)
    {
        if (Identity.Count() == iteration)
        {
            for (int i = 0; i < node.Count(); i++)
            {
                if (PathNames[iteration].CompareTo(node.ElementAt(i).getNodeName().ToString()) == 0)
                {
                    List<Node> ting = node.ElementAt(i).SubNode;
                    if (Convert.ToInt32(ting.ElementAt(0).getNodeValue()) > max)
                        max = Convert.ToInt32(ting.ElementAt(0).getNodeValue());
                    if (Convert.ToInt32(ting.ElementAt(0).getNodeValue()) < min)
                        min = Convert.ToInt32(ting.ElementAt(0).getNodeValue());

                }
            }
            return true;
        }
        else
        {
            for (int j = 0; j < node.Count(); j++)
            {
                if (node.ElementAt(j).getNodeName().ToString().CompareTo(PathNames[iteration]) == 0)
                {
                    List<Node> ting = node.ElementAt(j).SubNode;
                    if (ting.ElementAt(0).getNodeValue() == Identity[iteration].ToString() || Identity[iteration] == -1)
                             if (recursionForSectionNumber(ting, ref min, ref max, iteration + 1) == true)
                                 break;
                }
            }
        }
        return false;
    }
    /**
* get Item
*/
    public string getItemString(string itemName, bool stopOnError=true)
    {
        string info = "nothing";

        if(info.CompareTo("nothing")==0)
        {
        for (int i = 0; i < tree.SubNode.Count(); i++)
            if (PathNames[0].CompareTo(tree.SubNode[i].getNodeName()) == 0)
            {

                if (tree.SubNode[i].SubNode.ElementAt(0).getNodeValue() == Identity[0].ToString() || Identity[0] == -1)
                    info = recursionForItem(tree.SubNode[i].SubNode, itemName, 0, stopOnError);
            }
        }

        if (stopOnError == true && info.CompareTo("nothing")==0)
        {
          
        }
        return info;
      
        
    }
    /**
* get Item recursio
*/
    public string recursionForItem(List<Node> node, string itemName, int iteration, bool stopOnError    )
    {
     
        for (int i = 0; i < node.Count(); i++)
        {
            string tmp = node.ElementAt(i).getNodeName();
            if (node.ElementAt(0).getNodeName().ToString().CompareTo("Identity") == 0 || Identity[iteration] == -1)
                {
                    if (node.ElementAt(0).getNodeValue() == Identity[iteration].ToString() || Identity[iteration] == -1)
                    {
                        if (Identity.Count() == (iteration + 1))
                        {
                            for (int j = 0; j < node.Count(); j++)
                            {
                                if (node.ElementAt(j).getNodeName().ToString().CompareTo(itemName) == 0)
                                {
                                 if(node.ElementAt(j).getNodeValue().CompareTo("")==0)
                                     break;
                                 else
                                    return  node.ElementAt(j).getNodeValue();
                                }
                            }
                        }
                        else
                        {
                            for (int j = 0; j < node.Count(); j++)
                            {

                                if (node.ElementAt(j).getNodeName().ToString().CompareTo(PathNames[iteration + 1]) == 0)
                                {
                                    List<Node> ting=node.ElementAt(j).SubNode;
                                    if (ting.ElementAt(0).getNodeValue() == Identity[iteration + 1].ToString() || Identity[iteration + 1] == -1)
                                        return recursionForItem(ting, itemName, iteration + 1, stopOnError);
                                }

                            }
                        }

                    }
                }
            }
        if (stopOnError)
        {
        }
        return "nothing";
   }

}
