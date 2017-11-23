using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace simplesoilModel
{
    class layerClass
        {
            //! Depth below the soil surface of the lower boundary of the layer (millimetres)
      double z_lower;
    //! Plant-available water content of the layer at field capacity
      double fieldCapacity;
    //! Thickness of the layer (millimetres)
      double thickness;  
      public double getz_lower()  { return z_lower;}
      public double getfieldCapacity()  { return fieldCapacity;}
      public double getthickness() { return thickness; }
      public void setz_lower(double az_lower) { z_lower = az_lower; }
      public void setfieldCapacity(double afieldCapacity) { fieldCapacity = afieldCapacity; }
      public void setthickness(double athickness) { thickness = athickness; }

      public layerClass(layerClass alayerClass)  
            {
             z_lower = alayerClass.z_lower;
             fieldCapacity = alayerClass.fieldCapacity;
             thickness = alayerClass.thickness;
            }
      public layerClass()
      {
      }
    //! Initialise an instance of this class
    //!Read the parameters associated with a single layer
    /*! 
    \param z_upper depth below the soil surface of the upper boundary of the layer
    */
    public void Initialise(int index, double z_upper)  
    { 
        z_lower = 0.0;
        fieldCapacity = 0.0;
        FileInformation fileInfo = new FileInformation(GlobalVars.Instance.getSimplesoilModelFileName());
        string temp = "layerClass(" + index.ToString() + ").z_lower(-1)";
        //fileInfo.setPath(temp);
        z_lower = fileInfo.getItemDouble("Value",temp);
        temp = "layerClass(" + index.ToString() + ").fieldCapacity(-1)";
        fieldCapacity = fileInfo.getItemDouble("Value",temp);
        //the field capacity is expressed as 100 * cubic metre/cubic metre
        fieldCapacity /= 100.0;
        thickness = z_lower - z_upper;
    }
    }
}
