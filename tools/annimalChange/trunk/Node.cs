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
        /**
         * Constructor
         */
        public Node()
        {
            SubNode = new List<Node>();
        }
        /**
        * add a child
        */
        public void addChild(string name, string value)
        {
            Node newNode = new Node();
            newNode.nodeName = name;
            newNode.setNodeValue(value);
            SubNode.Add(newNode);
        }
        /**
       * add a child
       */
        public void addChild(Node newNode)
        {
            SubNode.Add(newNode);
        }
        /**
       * set Node Name
       */
        public void setNodeName(string name)
        {
            nodeName = name;
        }
        /**
       * get Node Name
       */
        public string getNodeName()
        {
           return  nodeName;
        }
        /**
       * set Node Name
       */
        public void setNodeValue(string value)
        {
            nodeValue = value;
        }
        /**
       * get Node value
       */
        public string getNodeValue()
        {
            return nodeValue;
        }
    }
}
