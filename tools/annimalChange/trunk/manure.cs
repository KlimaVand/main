﻿using System;
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
    int manureType;
    int speciesGroup;
    string name;
    bool isSolid;
    string parens;
    double phosphorus;
    double water;
/**
     * get phosphorus
     */
    public double getphosphorus() { return phosphorus; }
/**
 * Set phosphorus
 */
    public void Setphosphorus(double newValue) { phosphorus = newValue; }
/**
 * get water
 */
    public double getwater() { return water; }
/**
 * set water
 */
    public void Setwater(double newValue) { water = newValue; }
/**
 * Set deg C
 */
    public void SetdegC(double aValue) { degC = aValue; }
/**
 * Set non Deg C
 */
    public void SetnonDegC(double aValue) { nonDegC = aValue; }
/**
 * Set humic C
 */
    public void SethumicC(double aValue) { humicC = aValue; }
   /**
     * Set TAN
     */
    public void SetTAN(double aValue) { TAN = aValue; }
/**
 * Set organic N
 */
    public void SetorganicN(double aValue) { organicN = aValue; }
/**
 * Set DM
 */
    public void SetDM(double aVal) { DM = aVal; }
/**
 * Set species Group
 */
    public void SetspeciesGroup(int aValue) { speciesGroup = aValue; }
/**
 * Set manure Type
 */
    public void SetmanureType(int aValue) { manureType = aValue; }
/**
 * Set isSolid
 */
    public void SetisSolid(bool aValue) { isSolid = aValue; }
/**
 * Set humicN
 */
    public void SethumicN(double aVal) { humicN = aVal; }
/**
 * Set name
 */
    public void Setname(string aname) { name = aname; }
/**
 * Get DM
 */
    public double GetDM() { return DM; }
/**
 * Get non DegDM
 */
    public double GetnonDegDM() { return nonDegDM; }
/**
 * Get degDM
 */
    public double GetdegDM() { return degDM; }
/**
 * Get nonDegC
 */
    public double GetnonDegC() { return nonDegC; }
/**
 * Get humicC
 */
    public double GethumicC() { return humicC; }
/**
 * Get degC
 */
    public double GetdegC() { return degC; }
/**
 * Get OrgDegC
 */
    public double GetOrgDegC() { return orgDegC; }
/**
 * Get TAN
 */
    public double GetTAN() { return TAN; }
/**
 * Get organic N
 */
    public double GetorganicN() { return organicN; }
/**
 * Get manure Type
 */
    public int GetmanureType() { return manureType; }
/**
 * Get species Group
 */
    public int GetspeciesGroup() { return speciesGroup; }
/**
 * Get isSolid
 */
    public bool GetisSolid() { return isSolid; }
/**
 * Get name
 */
    public string Getname() { return name; }
/**
 * Get humicN
 */
    public double GethumicN() { return humicN; }
/**
 * Get Total N
 */
    public double GetTotalN() { return TAN + organicN + humicN; }
    /**
     * isSame
     */
    public bool isSame(manure aManure)
    {
        if (manureType == aManure.manureType)
        {
            if ((speciesGroup == aManure.speciesGroup) || (aManure.manureType==12))
                return true;
            else
                return false;
        }
        else
            return false;
    }
    /**
     * Constructor
     */
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
        phosphorus = 0;
        manureType = 0;
        speciesGroup = 0;
        name = "";
    }
    /**
     * Copy Constructor
     */
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
        manureType = manureToCopy.manureType;
        speciesGroup = manureToCopy.speciesGroup;
        name = manureToCopy.name;
        phosphorus = manureToCopy.phosphorus;
        water = manureToCopy.water;
    }

    /**
     * create new instance of manure, with amount determined by N required
     */
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
        nonDegC = amount * manureFile.getItemDouble("Value");
        manureFile.PathNames[2] = "humicCconcentration";
        humicC = amount * manureFile.getItemDouble("Value");
        manureFile.PathNames[2] = "Pconcentration";
        phosphorus = amount * manureFile.getItemDouble("Value");
    }
/**
     * Get total C
     */
    public double GetTotalC()
    {
        return degC + nonDegC + humicC;
    }
/**
     * Initialise
     */
    public void Initialise()
    {
        DM = 0;
        nonDegC = 0;
        nonDegDM = 0;
        degC = 0;
        humicC = 0;
    }
/**
     * Add Manure
     */
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
        phosphorus += aManure.phosphorus;
    }
/**
     * Increase Manure
     */
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
        phosphorus *= factor;
    }
/**
     * Divide Manure
     */
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
        phosphorus *= proportion;
    }
/**
     * Take Manure
     */
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
        aManure.phosphorus = proportion * phosphorus;
        phosphorus -= aManure.phosphorus;
        aManure.Setname(Getname());
    }
/**
     * Write
     */
    public void Write(string addedInfo)
    {
        parens = "_" + addedInfo + "_" + name;
        GlobalVars.Instance.writeStartTab("manure");
        GlobalVars.Instance.writeInformationToFiles("name", "Name", "-", name, parens);
        GlobalVars.Instance.writeInformationToFiles("phosphorus", "phosphorus", "-", phosphorus, parens);
        GlobalVars.Instance.writeInformationToFiles("water", "water", "-", water, parens);
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