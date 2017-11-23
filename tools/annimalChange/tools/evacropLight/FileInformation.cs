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
    private Node treeOrg;
    private Node treeAlt;
    private bool pauseBeforeExit=false;

    private FileInformation()
    {
        
    }
    static List<string> FileName = new List<string>();
    static List<Node> AllNodes = new List<Node>();
    //public string GetFileName() { return FileName; }

    private void ReadingFile(string nameOfFile)
    {
        string[] partFileName = nameOfFile.Split('\\');

        bool inUse = false;
        for (int i = 0; i < FileName.Count; i++)
        {
            if (FileName.ElementAt(i).CompareTo(partFileName[partFileName.Count() - 1]) == 0 && !nameOfFile.Contains("Alternative"))
            {
                inUse = true;
                treeOrg = AllNodes.ElementAt(i);
            }
            if (FileName.ElementAt(i).CompareTo(partFileName[partFileName.Count() - 1]) == 0 && nameOfFile.Contains("Alternative"))
            {
                inUse = true;
                treeAlt = AllNodes.ElementAt(i);
            }
       

        }
        if (inUse == false)
        {
            
            treeOrg = new Node();
            string itemName = "ino";
            try
            {
                XmlReader data = null;
                try
                {
                    while (!File.Exists(nameOfFile))
                    {

                        string[] path = nameOfFile.Split('\\');

                        if (path.Count() == 1 || (path.Count() == 2 && path[0][1] == ':'))
                            break;
                        nameOfFile = "";
                        for(int i=0;i<path.Count()-2;i++)
                        {
                            nameOfFile += path[i] + '\\';
                        }
                        nameOfFile += path[path.Count()-1];
                 
                    }
                    data = XmlReader.Create(nameOfFile);
                    string[] partFileNames = nameOfFile.Split('\\');
                
                    FileName.Add(partFileNames[partFileNames.Count() - 1]);
                }
                catch
                {
                    if (!nameOfFile.Contains("Alternative.xml"))
                    {
                        GlobalVars.Instance.Error(nameOfFile + " not found", "in FileInformation(string nameOfFile)", false);
                        throw new System.Exception("farm Fail");
                    }
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
                            treeOrg.addChild(newNode);


                        }
                    }
                }
                data.Close();
                AllNodes.Add(treeOrg);

            }
            catch (Exception e)
            {
                if (nameOfFile!=null)
                if (!nameOfFile.Contains("Alternative.xml"))
                {
                    GlobalVars.Instance.log(e.ToString(), 5);
                    if (e.Message.CompareTo("farm Fail") != 0)
                    {
                        string messageString = ("problem with reading: " + nameOfFile + " because of " + e.ToString()) + "\r\n";
                        messageString += ("model terminated") + "\r\n";
                        messageString += ("the path is: ");
                        for (int i = 0; i < PathNames.Count; i++)
                        {
                            messageString += (PathNames[i] + "(" + Identity[i].ToString() + ")");
                        }


                        GlobalVars.Instance.Error(messageString, e.StackTrace, true);
                    }
                    else
                    {
                        throw new System.Exception("farm Fail");
                    }
                }
            }
        }
    }
    public FileInformation(string[] nameOfFile)
    {

        fileNameOrg = nameOfFile[0];
        fileNameAlt = nameOfFile[1];//name.Split('.')[0] + "Alternative.xml";

        ReadingFile(fileNameAlt);
        ReadingFile(fileNameOrg);
        ReadingFile(fileNameAlt);
        ReadingFile(fileNameOrg);
   
  
    }

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

    public void SetpauseBeforeExit(bool aVal) { pauseBeforeExit = aVal; }

    public bool doesIDExist(int ID)
    {


        return recursionDoesIDExist(treeOrg.SubNode, ID, 0);
    

    }
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
                  //  Console.WriteLine(ting.ElementAt(0).getNodeValue());
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
    private string pathToString()
    {
        string returnValue="";
        for(int i=0;i<Identity.Count();i++)
        {
            returnValue+=PathNames[i]+"("+Identity[i]+")";
        }
        if (PathNames.Count > Identity.Count)
        {
            returnValue += PathNames[Identity.Count()];
        }
        return returnValue;

    }
    public void setPath(List<int> aIdentity, List<string> aPathNames)
    {
        Identity = aIdentity;
        PathNames = aPathNames;
    }
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
                 
                  string messageString=(name + " is wrong")+"\n";
                  messageString+=("model terminated") + "\n";
                  messageString += ("the file name is " + fileNameOrg) + "\n";
                  GlobalVars.Instance.Error(messageString);
                }
            }
            PathNames.Add(tmp);
        }

    }
    public void setFilename(string name)
    { 
        fileNameOrg = name;
        fileNameAlt = name.Split('.')[0]+"Alternative.xml";
    }
    public void setIdentity(List<int> nr)
    {
        Identity = nr; 
    }
    public void setPathNames(List<string> name)
    {
        PathNames = name;
    }
    public int getItemInt(string itemName, bool stopOnError = true)
    {
        string output=getItemString(itemName, stopOnError);
        if (output.CompareTo("nothing") != 0)
            return Convert.ToInt32(getItemString(itemName, stopOnError));
        else return -1;
    }
    public double getItemDouble(string itemName, bool stopOnError = true)
    {
        string stuff = getItemString(itemName, stopOnError);
        if (stuff.CompareTo("nothing") != 0)
            return Convert.ToDouble(stuff);
        else return 0.0;
    }
    public bool getItemBool(string itemName, bool stopOnError = true)
    {
        string stuff = getItemString(itemName, stopOnError);
        if (stuff.CompareTo("nothing") != 0)
            return Convert.ToBoolean(stuff);
        else return true;
    }

    //finder item name in xml filen og retunere den som int. Dette kan være eks FarmType.
    public int getItemInt(string itemName, string path, bool stopOnError = true)
    {
        setPath(path);
        return Convert.ToInt32(getItemString(itemName, stopOnError));
    }
    public double getItemDouble(string itemName, string path, bool stopOnError = true)
    {
        setPath(path);
        return Convert.ToDouble(getItemString(itemName, stopOnError));
    }
    public bool getItemBool(string itemName, string path, bool stopOnError=true)
    {
        setPath(path);
        string stuff = getItemString(itemName,stopOnError);
        return Convert.ToBoolean(stuff);
    }
    public string getItemString(string itemName, string path, bool stopOnError=true)
    {
        setPath(path);
        return getItemString(itemName,stopOnError);
    }
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
    public void getSectionNumber(ref int min, ref int max)
    {

        recursionForSectionNumber(treeOrg.SubNode, ref min, ref max, 0);

    }
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
    public string getItemString(string itemName, bool stopOnError=true)
    {
        string info = "nothing";
        if(treeAlt!=null)
        for (int i = 0; i < treeAlt.SubNode.Count(); i++)
            if (PathNames[0].CompareTo(treeAlt.SubNode[i].getNodeName()) == 0)
            {

                if (treeAlt.SubNode[i].SubNode.ElementAt(0).getNodeValue() == Identity[0].ToString() || Identity[0] == -1)
                    info = recursionForItem(treeAlt.SubNode[i].SubNode, itemName, 0, false);
            }

        bool found=false;
        if (info.CompareTo("nothing") != 0)
        {
            GlobalVars.Instance.log(pathToString() + itemName + " as " + info + "found in " + fileNameAlt, 6);
            found = true;
        }
        if(info.CompareTo("nothing")==0)
        {
        for (int i = 0; i < treeOrg.SubNode.Count(); i++)
            if (PathNames[0].CompareTo(treeOrg.SubNode[i].getNodeName()) == 0)
            {

                if (treeOrg.SubNode[i].SubNode.ElementAt(0).getNodeValue() == Identity[0].ToString() || Identity[0] == -1)
                    info = recursionForItem(treeOrg.SubNode[i].SubNode, itemName, 0, stopOnError);
            }
        }
        if (found==false)
            GlobalVars.Instance.log(pathToString() + itemName + " as " + info + " found in " + fileNameOrg, 6);
        if (stopOnError == true && info.CompareTo("nothing")==0)
        {
            string messageString = ("could not find " + itemName) + "\n";
            messageString += ("model terminated") + "\n";
            messageString += ("the path is: ");
            for (int i = 0; i < PathNames.Count; i++)
            {
                messageString += (PathNames[i] + "(" + Identity[i].ToString() + ")");
            }

            messageString += ("the file name is " + fileNameOrg);
            GlobalVars.Instance.Error(messageString);
        }
        return info;
      
        
    }
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
            string messageString = ("could not find " + itemName) + "\n";
            messageString += ("model terminated") + "\n";
            messageString += ("the path is: ");
            for (int i = 0; i < PathNames.Count; i++)
            {
                messageString += (PathNames[i] + "(" + Identity[i].ToString() + ")");
            }

            messageString += ("the file name is " + fileNameOrg);
            GlobalVars.Instance.Error(messageString);
        }
        return "nothing";
   }

}
