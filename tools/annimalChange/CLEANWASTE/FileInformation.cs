using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;
using System.IO;
using System.Xml.XPath;
public class FileInformation
{

    private string fileName = "test.xml";
    public List<int> Identity = new List<int>();
    public List<string> PathNames = new List<string>();
    private string errorFileName;
    private bool pauseBeforeExit=false;

    private FileInformation()
    {
        
    }

    public FileInformation(string nameOfFile)
    {
        fileName = nameOfFile;
        errorFileName = GlobalVars.Instance.GeterrorFileName();
    }
    
    public FileInformation(string nameOfFile, string errorName)
    {
        fileName = nameOfFile;
        errorFileName = errorName;
    }

    public void SetpauseBeforeExit(bool aVal) { pauseBeforeExit = aVal; }

    public bool doesIDExist(int ID)
    {
        try
        {
            XmlReader data = XmlReader.Create(fileName);
            //     data.MoveToContent();
            while (data.Read())
            {
                if (data.NodeType == XmlNodeType.Element)
                {
                    XElement el = XNode.ReadFrom(data) as XElement;
                    IEnumerable<XElement> node = el.Elements();
                    return recursionDoesIDExist(node, ID, 0);
                }
            }
            data.Close();
        }
        catch (Exception e)
        {
          System.IO.StreamWriter file = new System.IO.StreamWriter(errorFileName);
            file.WriteLine("problem with reading: " + fileName + " because of " + e.ToString());
            file.WriteLine("model terminated");
            file.WriteLine("the file name is " + fileName);
            file.Close();
            Console.WriteLine("problem with reading: " + fileName + " because of " + e.ToString());
            Console.WriteLine("model terminated");
            Console.WriteLine("the file name is " + fileName);
            if(pauseBeforeExit)
                Console.Read();

            Environment.Exit(0);
        }
        return false;

    }
    private bool recursionDoesIDExist(IEnumerable<XElement> node, int id, int iteration)
    {
        if (Identity.Count() == iteration)
        {
            for (int i = 0; i < node.Count(); i++)
            {
                   //Console.WriteLine(node.ElementAt(i).Name.ToString());
                
                if (PathNames[iteration].CompareTo(node.ElementAt(i).Name.ToString()) == 0)
                {
                    IEnumerable<XElement> ting = node.ElementAt(i).Elements();
                  //  Console.WriteLine(ting.ElementAt(0).Value);
                    if (Convert.ToInt32(ting.ElementAt(0).Value) == id)
                        return true;
                }
            }
        }
        else
        {
            for (int j = 0; j < node.Count(); j++)
            {
                if (node.ElementAt(j).Name.ToString().CompareTo(PathNames[iteration]) == 0)
                {
                    IEnumerable<XElement> ting = node.ElementAt(j).Elements();
                    if (ting.ElementAt(0).Value == Identity[iteration].ToString())
                        return recursionDoesIDExist(ting, id, iteration + 1);
                }
            }
        }
        return false;

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
                  System.IO.StreamWriter file = new System.IO.StreamWriter(errorFileName);
                    file.WriteLine(name + " is wrong");
                    file.WriteLine("model terminated");
                    file.WriteLine("the file name is " + fileName);
                    file.Close();
                    Console.WriteLine(name + " is wrong");
                    Console.WriteLine("model terminated");
                    Console.WriteLine("the file name is " + fileName);
                    if(pauseBeforeExit)
                         Console.Read();
                    Environment.Exit(0);
                }
            }
            PathNames.Add(tmp);
        }

    }
    public void setFilename(string name)
    { 
        fileName = name; 
    }
    public void setIdentity(List<int> nr)
    {
        Identity = nr; 
    }
    public void setPathNames(List<string> name)
    {
        PathNames = name;
    }
    public int getItemInt(string itemName)
    {
        return Convert.ToInt32(getItemString(itemName));
    }
    public double getItemDouble(string itemName)
    {
        string stuff = getItemString(itemName);
        return Convert.ToDouble(getItemString(itemName));
    }
    public bool getItemBool(string itemName)
    {
        string stuff = getItemString(itemName);
        return Convert.ToBoolean(stuff);
    }

    //finder item name in xml filen og retunere den som int. Dette kan være eks FarmType.
    public int getItemInt(string itemName, string path)
    {
        setPath(path);
        return Convert.ToInt32(getItemString(itemName));
    }
    public double getItemDouble(string itemName, string path)
    {
        setPath(path);
        return Convert.ToDouble(getItemString(itemName));
    }
    public bool getItemBool(string itemName, string path)
    {
        setPath(path);
        string stuff = getItemString(itemName);
        return Convert.ToBoolean(stuff);
    }
    public string getItemString(string itemName, string path)
    {
        setPath(path);
        return getItemString(itemName);
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
        try
        {
            XmlReader data = XmlReader.Create(fileName);
            while (data.Read())
            {
                if (data.NodeType == XmlNodeType.Element)
                {
                    XElement el = XNode.ReadFrom(data) as XElement;
                    IEnumerable<XElement> node = el.Elements();
                    recursionForSectionNumber(node, ref min, ref max, 0);
                }
            }
            data.Close();
        }
        catch (Exception e)
        {
          System.IO.StreamWriter file = new System.IO.StreamWriter(errorFileName);
            file.WriteLine("problem with reading: " + fileName + " because of " + e.ToString());
            file.WriteLine("model terminated");
            file.WriteLine("the file name is " + fileName);
            file.Close();
            Console.WriteLine("problem with reading: " + fileName + " becouse of " + e.ToString());
            Console.WriteLine("model terminated");
            Console.WriteLine("the file name is " + fileName);
            if (pauseBeforeExit)
                Console.Read();
            Environment.Exit(0);
        }

    }
    private bool recursionForSectionNumber(IEnumerable<XElement> node, ref int min, ref int max, int iteration)
    {
        if (Identity.Count() == iteration)
        {
            for (int i = 0; i < node.Count(); i++)
            {
                if (PathNames[iteration].CompareTo(node.ElementAt(i).Name.ToString()) == 0)
                {
                    IEnumerable<XElement> ting = node.ElementAt(i).Elements();
                    if (Convert.ToInt32(ting.ElementAt(0).Value) > max)
                        max = Convert.ToInt32(ting.ElementAt(0).Value);
                    if (Convert.ToInt32(ting.ElementAt(0).Value) < min)
                        min = Convert.ToInt32(ting.ElementAt(0).Value);

                }
            }
            return true;
        }
        else
        {
            for (int j = 0; j < node.Count(); j++)
            {
                if (node.ElementAt(j).Name.ToString().CompareTo(PathNames[iteration]) == 0)
                {
                    IEnumerable<XElement> ting = node.ElementAt(j).Elements();
                    if (ting.ElementAt(0).Value == Identity[iteration].ToString() || Identity[iteration] == -1)
                             if (recursionForSectionNumber(ting, ref min, ref max, iteration + 1) == true)
                                 break;
                }
            }
        }
        return false;
    }
    public string getItemString(string itemName)
    {
        try
        {
            XmlReader data = XmlReader.Create(fileName);
            while (data.Read())
            {
                if (data.NodeType == XmlNodeType.Element)
                {
                    XElement el = XNode.ReadFrom(data) as XElement;
                    IEnumerable<XElement> node = el.Elements();
                    for (int i = 0; i < node.Count(); i++)
                    {
                        if (PathNames[0].CompareTo(node.ElementAt(i).Name.ToString()) == 0)
                        {
                            IEnumerable<XElement> ting = node.ElementAt(i).Elements();
                            if (ting.ElementAt(0).Value == Identity[0].ToString() || Identity[0]==-1)
                                return recursionForItem(ting, itemName, 0);
                        }
                    }
                }
            }
               data.Close();
        }
        catch (Exception e)
        {
          System.IO.StreamWriter file = new System.IO.StreamWriter(errorFileName);
            file.WriteLine("problem with reading: " + fileName+" because of "+e.ToString());
            file.WriteLine("model terminated");
            file.Write("the path is: ");
            for (int i = 0; i < PathNames.Count; i++)
            {
                file.Write(PathNames[i] + "(" + Identity[i].ToString() + ")");
            }

            file.WriteLine("the file name is " + fileName);
            file.Close();
            Console.WriteLine("problem with reading: " + fileName + " because of " + e.ToString());
            Console.WriteLine("model terminated");
            Console.Write("the path is: ");
            for (int i = 0; i < PathNames.Count; i++)
            {
                Console.Write(PathNames[i] + "(" + Identity[i].ToString() + ")");
            }
            Console.WriteLine("the file name is " + fileName);
            if (pauseBeforeExit)
                Console.Read();
            Environment.Exit(0);
        }

        return "nothing";
        
    }
    public string recursionForItem(IEnumerable<XElement> node, string itemName, int iteration)
    {
     
        for (int i = 0; i < node.Count(); i++)
        {
            string tmp = node.ElementAt(i).Name.ToString();
            if (node.ElementAt(0).Name.ToString().CompareTo("Identity") == 0 || Identity[iteration]==-1)
                {
                    if (node.ElementAt(0).Value == Identity[iteration].ToString() || Identity[iteration] == -1)
                    {
                        if (Identity.Count() == (iteration + 1))
                        {
                            for (int j = 0; j < node.Count(); j++)
                            {
                                if (node.ElementAt(j).Name.ToString().CompareTo(itemName) == 0)
                                    return node.ElementAt(j).Value;
                            }
                        }
                        else
                        {
                            for (int j = 0; j < node.Count(); j++)
                            {
                         
                                if (node.ElementAt(j).Name.ToString().CompareTo(PathNames[iteration+1]) == 0)
                                {
                                    IEnumerable<XElement> ting = node.ElementAt(j).Elements();
                                    if (ting.ElementAt(0).Value == Identity[iteration + 1].ToString() || Identity[iteration + 1] == -1)
                                        return recursionForItem(ting, itemName, iteration + 1);
                                }

                            }
                        }

                    }
                }
            }
      System.IO.StreamWriter file = new System.IO.StreamWriter(errorFileName);
        file.WriteLine("could not find " + itemName);
        file.WriteLine("model terminated");
        file.Write("the path is: ");
        for (int i = 0; i < PathNames.Count; i++)
        {
            file.Write(PathNames[i] + "(" + Identity[i].ToString() + ")");
        }
        file.WriteLine("the file name is " + fileName);
        file.Close();
        Console.WriteLine("could not find " + itemName);
        Console.WriteLine("model terminated");
        Console.Write("the path is: ");
        for (int i = 0; i < PathNames.Count; i++)
        {
            Console.Write(PathNames[i] + "(" + Identity[i].ToString() + ")");
        }
        Console.WriteLine("the file name is " + fileName);
        if (pauseBeforeExit)
            Console.Read();
        Environment.Exit(0);
        return "nothing";
   }

}
