using System;
using System.Xml;
public class manure
{
    string path;
    int id;
    double DM;
    double nonDegDM;
    double degDM;
    double nonDegC;
    double orgDegC;
    double degC;
    double humicC;
    double organicN;
    double TAN;
    double humicN;
    int storageType;
    int speciesGroup;
    bool isSolid;

    public void SetdegC(double aValue) { degC = aValue; }
    public void SetnonDegC(double aValue) { nonDegC = aValue; }
    public void SethumicC(double aValue) { humicC = aValue; }
    public void SetTAN(double aValue) { TAN = aValue; }
    public void SetorganicN(double aValue) { organicN = aValue; }
    public void SetDM(double aVal) { DM = aVal; }
    public void SetspeciesGroup(int aValue) { speciesGroup = aValue; }
    public void SetstorageType(int aValue) { storageType = aValue; }
    public void SetisSolid(bool aValue) { isSolid = aValue; }
    public void SethumicN(double aVal) { humicN = aVal; }

    public double GetDM() { return DM; }
    public double GetnonDegDM() { return nonDegDM; }
    public double GetdegDM() { return degDM; }
    public double GetnonDegC() { return nonDegC; }
    public double GethumicC() { return humicC; }
    public double GetdegC() { return degC; }
    public double GetOrgDegC() { return orgDegC; }
    public double GetTAN() { return TAN; }
    public double GetorganicN() { return organicN; }
    public int GetstorageType() { return storageType; }
    public int GetspeciesGroup() { return speciesGroup; }
    public bool GetisSolid() { return isSolid; }
    public double GethumicN() { return humicN; }
    public double GetTotalN() { return TAN + organicN + humicN; }
    public bool isSame(manure aManure)
    {
        if ((storageType == aManure.storageType) && (speciesGroup == aManure.speciesGroup))
            return true;
        else
            return false;
    }
    public manure()
    {
        DM =0;
        nonDegDM =0;
        degDM = 0;
        nonDegC = 0;
        degC = 0;
        humicC = 0;
        organicN = 0;
        TAN = 0;
        humicN = 0;
        storageType = 0;
        speciesGroup = 0;
    }

    public manure(manure manureToCopy)
    {
        DM = manureToCopy.DM;
        nonDegDM = manureToCopy.nonDegDM;
        degDM = manureToCopy.degDM;
        nonDegC = manureToCopy.nonDegC;
        degC = manureToCopy.degC;
        humicC = manureToCopy.humicC;
        organicN = manureToCopy.organicN;
        TAN = manureToCopy.TAN;
        humicN = manureToCopy.humicN;
        storageType = manureToCopy.storageType;
        speciesGroup = manureToCopy.speciesGroup;
    }

    //create new instance of manure, with amount determined by N required
    public manure(string aPath,int aID, double amountN)
    {
        id=aID;
        path=aPath+'('+id.ToString()+')';

        FileInformation manureFile = new FileInformation(GlobalVars.Instance.getfertManFilePath());
        manureFile.setPath(path);
        storageType = manureFile.getItemInt("StorageType");
        speciesGroup = manureFile.getItemInt("SpeciesGroup");
        path += ".TANconcentration(-1)";
        manureFile.setPath(path);
        double tempTAN = manureFile.getItemDouble("Value");
        manureFile.PathNames[1] = "organicNconcentration";
        double temporganicN = manureFile.getItemDouble("Value");
        double proportionTAN = tempTAN / (tempTAN + temporganicN);
        TAN = proportionTAN * amountN;
        organicN = (1 - proportionTAN) * amountN;
        double amount = TAN / tempTAN;
        manureFile.PathNames[1] = "degCconcentration";
        degC = amount * manureFile.getItemDouble("Value");
        manureFile.PathNames[1] = "nonDegCconcentration";
        nonDegC = amount * manureFile.getItemDouble("Value");
        manureFile.PathNames[1] = "humicCconcentration";
        humicC = amount *manureFile.getItemDouble("Value");
    }
    public double GetTotalC()
    {
        return degC + nonDegC + humicC;
    }
    public void Initialise()
    {
        DM = 0;
        nonDegC = 0;
        nonDegDM = 0;
        degC = 0;
        humicC = 0;
    }
    public void AddManure(manure aManure)
    {
        DM += aManure.DM;
        nonDegDM += aManure.nonDegDM;
        degDM += aManure.degDM;
        nonDegC += aManure.nonDegC;
        degC += aManure.degC;
        humicC += aManure.humicC;
        organicN += aManure.organicN;
        TAN += aManure.TAN;
        humicN += aManure.humicN;
    }

    public void IncreaseManure(double factor)
    {
        DM*= factor;
        nonDegDM *= factor;
        degDM *= factor;
        nonDegC *= factor;
        degC *= factor;
        humicC *= factor;
        organicN *= factor;
        TAN *= factor;
        humicN *= factor;
    }
    public void DivideManure(double proportion)
    {
        DM *=proportion;
        nonDegDM *= proportion;
        degDM *= proportion;
        nonDegC *= proportion;
        degC *= proportion;
        humicC *= proportion;
        organicN *= proportion;
        TAN *= proportion;
        humicN *= proportion;
    }

    public void TakeManure(ref double amountN, ref manure aManure)
    {
        double totalN = GetTotalN();
        double proportion;
        if (amountN <= totalN)
            proportion = amountN / totalN;
        else
        {
            proportion=1.0;
            amountN = totalN;
        }
        aManure.DM=proportion*DM;
        DM -= aManure.DM;
            
        aManure.nonDegDM = proportion * nonDegDM;
        nonDegDM-=aManure.nonDegDM;
        aManure.degDM = proportion * degDM;
        degDM-=aManure.degDM;
        aManure.nonDegC = proportion * nonDegC;
        nonDegC-=aManure.nonDegC;
        aManure.degC = proportion * degC;
        degC-=aManure.degC;
        aManure.humicC = proportion * humicC;
        humicC-=aManure.humicC;
            
        aManure.organicN = proportion * organicN;
        organicN -= aManure.organicN;
        aManure.TAN = proportion * TAN;
        TAN -= aManure.TAN;
        aManure.humicN = proportion * humicN;
        humicN -= aManure.humicN;
    }
    public void Write(XmlWriter writer, System.IO.StreamWriter tabFile)
    {
        writer.WriteStartElement("manure");



        writer.WriteStartElement("nonDegDM");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("nonDegDM");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(nonDegDM);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("degDM");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("degDM");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(degDM);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("nonDegC");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("nonDegC");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(nonDegC);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("degC");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("degC");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(degC);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("humicC");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("humicC");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(humicC);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("storageType");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("storageType");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(storageType);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("speciesGroup");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("speciesGroup");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(speciesGroup);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("DM");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("DM");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(DM);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("TAN");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("TAN");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(TAN);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("organicN");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("organicN");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(organicN);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("degC");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("degC");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(degC);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("nonDegC");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("nonDegC");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(nonDegC);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("humicC");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("humicC");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(humicC);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteEndElement();



        tabFile.Write("manure"+"\n");
        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("nonDegDM" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(nonDegDM.ToString()+"\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("degDM" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(degDM.ToString()+"\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("nonDegC" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(nonDegC.ToString()+"\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("degC" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(degC.ToString()+"\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("humicC" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(humicC.ToString()+"\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("storageType" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(storageType.ToString()+"\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("speciesGroup" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(speciesGroup.ToString()+"\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("DM" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(DM.ToString()+"\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("TAN" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(TAN.ToString()+"\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("organicN" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(organicN.ToString()+"\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("degC" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(degC.ToString()+"\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("nonDegC" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(nonDegC.ToString()+"\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("humicC" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(humicC.ToString()+"\n");
        
        
    }
}
