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
    double Bo;
    int manureType;
    int speciesGroup;
    string name;
    bool isSolid;
    string parens;
    public void SetdegC(double aValue) { degC = aValue; }
    public void SetnonDegC(double aValue) { nonDegC = aValue; }
    public void SethumicC(double aValue) { humicC = aValue; }
    public void SetTAN(double aValue) { TAN = aValue; }
    public void SetorganicN(double aValue) { organicN = aValue; }
    public void SetDM(double aVal) { DM = aVal; }
    public void SetspeciesGroup(int aValue) { speciesGroup = aValue; }
    public void SetmanureType(int aValue) { manureType = aValue; }
    public void SetisSolid(bool aValue) { isSolid = aValue; }
    public void SethumicN(double aVal) { humicN = aVal; }
    public void SetBo(double aVal) { Bo = aVal; }
    public void Setname(string aname) { name = aname; }

    public double GetDM() { return DM; }
    public double GetnonDegDM() { return nonDegDM; }
    public double GetdegDM() { return degDM; }
    public double GetnonDegC() { return nonDegC; }
    public double GethumicC() { return humicC; }
    public double GetdegC() { return degC; }
    public double GetOrgDegC() { return orgDegC; }
    public double GetTAN() { return TAN; }
    public double GetorganicN() { return organicN; }
    public int GetmanureType() { return manureType; }
    public int GetspeciesGroup() { return speciesGroup; }
    public bool GetisSolid() { return isSolid; }
    public string Getname() { return name; }
    public double GethumicN() { return humicN; }
    public double GetTotalN() { return TAN + organicN + humicN; }
    public double GetBo() { return Bo; }
    public bool isSame(manure aManure)
    {
        if (manureType == aManure.manureType)
        {
            if ((speciesGroup == aManure.speciesGroup) || (aManure.speciesGroup == 0))
                return true;
            else
                return false;
        }
        else
        {
            if ((speciesGroup == aManure.speciesGroup) || (aManure.speciesGroup == 0))
            {
                if (manureType == 1 && aManure.manureType == 2)
                    return true;
                else if (manureType == 2 && aManure.manureType == 1)
                    return true;
                else if (manureType == 3 && aManure.manureType == 4)
                    return true;
                else if (manureType == 4 && aManure.manureType == 3)
                    return true;
                else if (manureType == 6 && aManure.manureType == 9)
                    return true;
                else if (manureType == 9 && aManure.manureType == 6)
                    return true;
                else if (manureType == 7 && aManure.manureType == 10)
                    return true;
                else if (manureType == 10 && aManure.manureType == 7)
                    return true;
                else if (manureType == 8 && aManure.manureType == 12)
                    return true;
                else if (manureType == 12 && aManure.manureType == 8)
                    return true;
                else if (manureType == 13 && aManure.manureType == 14)
                    return true;
                else if (manureType == 14 && aManure.manureType == 13)
                    return true;
                else
                    return false;
            }
            return false;
        }
           
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
        manureType = 0;
        speciesGroup = 0;
        Bo = 0;
        name = "";
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
        Bo = manureToCopy.Bo;
        manureType = manureToCopy.manureType;
        speciesGroup = manureToCopy.speciesGroup;
        name = manureToCopy.name;
    }

    //create new instance of manure, with amount determined by N required
    public manure(string aPath, int aID, double amountN, string aParens)
    {
        id=aID;
        path="AgroecologicalZone("+GlobalVars.Instance.GetZone().ToString()+")."+aPath+'('+id.ToString()+')';
        parens = aParens;
        FileInformation manureFile = new FileInformation(GlobalVars.Instance.getfertManFilePath());
        manureFile.setPath(path);
        name = manureFile.getItemString("Name");
        manureType = manureFile.getItemInt("ManureType");
        speciesGroup = manureFile.getItemInt("SpeciesGroup");
        path += ".TANconcentration(-1)";
        manureFile.setPath(path);
        double tempTAN = manureFile.getItemDouble("Value");
        manureFile.PathNames[2] = "organicNconcentration";
        double temporganicN = manureFile.getItemDouble("Value");
        double proportionTAN = tempTAN / (tempTAN + temporganicN);
        TAN = proportionTAN * amountN;
        organicN = (1 - proportionTAN) * amountN;
        double amount = TAN / tempTAN;
        manureFile.PathNames[2] = "degCconcentration";
        degC = amount * manureFile.getItemDouble("Value");
        manureFile.PathNames[2] = "nonDegCconcentration";
        double nonDegCconc = manureFile.getItemDouble("Value");
        nonDegC = amount * nonDegCconc;
        manureFile.PathNames[2] = "humicCconcentration";
        double humicCconc= manureFile.getItemDouble("Value");
        if (humicCconc>nonDegCconc)
        {
            string messageString = "Error; manure humic C is greater than total non-degradable C\n";
            messageString += "Manure name = " + name + "\n";
            GlobalVars.Instance.Error(messageString);
        }
        humicC = amount * humicCconc;
        if (humicC > 0)
        {
            humicN = humicC / GlobalVars.Instance.getCNhum();
            organicN -= humicN;
            if (organicN<0)
            {
                string messageString = "Error; manure humic N is greater than total organic N\n";
                messageString += "Manure name = " + name + "\n";
                GlobalVars.Instance.Error(messageString);
            }
        }
        else
            humicN = 0;
        manureFile.PathNames[2] = "Bo";
        Bo = manureFile.getItemDouble("Value",false);
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
        Bo = 0;
    }
    public void AddManure(manure aManure)
    {
        double totalC = nonDegC + degC;
        double oldBo = Bo * totalC;
        double donorC = aManure.degC + aManure.nonDegC;
        double addedBo = aManure.Bo * donorC;
        Bo = (oldBo + addedBo) / (totalC + donorC);
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
        aManure.Setname(Getname());
    }
    public void Write(string addedInfo)
    {
        parens = "_" + addedInfo + "_" + name;
        GlobalVars.Instance.writeStartTab("manure");
        GlobalVars.Instance.writeInformationToFiles("name", "Name", "-", name, parens);
        GlobalVars.Instance.writeInformationToFiles("speciesGroup", "Species number", "-", speciesGroup, parens);
        GlobalVars.Instance.writeInformationToFiles("typeStored", "Storage type", "-", manureType, parens);
        GlobalVars.Instance.writeInformationToFiles("DM", "Dry matter", "kg", DM, parens);
        GlobalVars.Instance.writeInformationToFiles("nonDegDM", "Non-degradable DM", "kg", nonDegDM, parens);
        GlobalVars.Instance.writeInformationToFiles("degDM", "Degradable DM", "kg", degDM, parens);
        GlobalVars.Instance.writeInformationToFiles("nonDegC", "Non-degradable C", "kg", nonDegC, parens);
        GlobalVars.Instance.writeInformationToFiles("degC", "Degradable C", "kg", degC, parens);
        GlobalVars.Instance.writeInformationToFiles("humicC", "Humic C", "kg", humicC, parens);
        GlobalVars.Instance.writeInformationToFiles("TAN", "TAN", "kg", TAN, parens);
        GlobalVars.Instance.writeInformationToFiles("organicN", "Organic N", "kg", organicN, parens);
        GlobalVars.Instance.writeEndTab();        
    }
}
