using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AnimalChange
{
    public class Node
    {
        private string nodeName;
        private string nodeValue;
        public List<Node> SubNode;

        public Node()
        {
            SubNode = new List<Node>();
        }
        public void addChild(string name, string value)
        {
            Node newNode = new Node();
            newNode.nodeName = name;
            newNode.setNodeValue(value);
            SubNode.Add(newNode);
        }
        public void addChild(Node newNode)
        {
            SubNode.Add(newNode);
        }
        public void setNodeName(string name)
        {
            nodeName = name;
        }
        public string getNodeName()
        {
           return  nodeName;
        }
        public void setNodeValue(string value)
        {
            nodeValue = value;
        }
        public string getNodeValue()
        {
            return nodeValue;
        }
    }
}
