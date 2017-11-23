/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 Revision      16/5-97 J�rgen Berntsen added support for Methionin, Threonin, lysin ,..
 					13/6 support for plantname JB
               	july major revision of all product's.         (JB)
               	All operators calls the superclass
                  operator check's
                  	if similar types, storage etc  added/subtracted
                     if amount=0 the object gets the same unit, storage etc
                     	as the object that is added to it
\****************************************************************************/

#ifndef __PFEEDIT_H
  #define __PFEEDIT_H
#include <pplantit.h>

class feedItem  : public liveMatter
{
   /* Attributes */
   private:
   protected:
      double Ca_content;
      double Na_content;
      double Mg_content;
      double Cu_content;
      double S_content;
      double fat;
      double sugar;
      double starch;
      // double organic; in liveMatter now
      double chew;
      double aat;
      double pbv;
      double cell;   	// cell wall carbohydrate
      double fill;
      double fillU;
      double Lysin; // Lysin in feeditem
      double Threonin;
      double Methionin;
      double P_digest;
      double MaxPercentage;  // the maximum percentage in feed
      double OMD;	//added by NJH 20.06.00
      double pigFeedUnitsPerItemUnit; // Unit: fe
//      double fodderUnitsPerItemUnit_nonpregnant; // Unit: fe
      double          //added by NJH 18.04.01
         propRDP,						// proportion of crude protein N that is rumen degradable (RDP)
         propAAinUDP,				// proportion of amino acid in protein that is not degraded in rumen (UDP)
         propUDPdigestible,     		// proportion of amino acid in UDP that is digestible in small intestine
         microAAFactor,				// g amino acid per kg digestible carbohydrate
         CHO_content, 				// proportion digestible carbohydrate in fresh weight
         propDigestCHO,					// proportion of carbohydrate that is digestible
         ME;
      string plantName;      // NONE implies that the product is not produced

      double proteinN_digestibility; // proportion of protein N that is digestible
      double feedUnitsPerItemUnit; // Unit: fe
      int feedCategory;  //1 = concentrate, 2 = roughage, 3 = mineral supplement

      void InitVariables();

   public:
      // Default constructor
      feedItem();
      // Constructor with arguments
      feedItem(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      feedItem(const feedItem& afeedItem);
      // Destructor
      virtual ~feedItem();
      // Operator for output
      friend ostream& operator << (ostream& os, const feedItem& afeedItem);

      // Set functions
  //    void SetfodderUnitsPerItemUnit(double afodderUnitsPerItemUnit) {fodderUnitsPerItemUnit = afodderUnitsPerItemUnit;}
      void SetCa_content(double aCa_content) {Ca_content = aCa_content;}
      void SetNa_content(double aNa_content) {Na_content = aNa_content;}
      void SetMg_content(double aMg_content) {Mg_content = aMg_content;}
      void SetCu_content(double aCu_content) {Cu_content = aCu_content;}
      void SetS_content(double aS_content) {S_content = aS_content;}
      void Setfat(double afat) {fat = afat;}
      void Setsugar(double asugar) {sugar = asugar;}
      void Setstarch(double astarch) {starch = astarch;}
      void Setchew(double achew) {chew = achew;}
      void Setaat(double aaat) {aat = aaat;}
      void Setpbv(double apbv) {pbv = apbv;}
      void Setcell(double acell) {cell = acell;}
      void Setfill(double afill) {fill = afill;}
      void SetfillU(double afillU) {fillU = afillU;}
      void SetMethionin(double aMethionin) {Methionin = aMethionin;};
      void SetThreonin(double aThreonin) {Threonin = aThreonin;}
      void SetLysin(double alysin) {Lysin = alysin;}
      void SetproteinN_digestibility(double aproteinN_digestibility) {proteinN_digestibility = aproteinN_digestibility;}
//      void SetPlantName(string aplantName);
      void SetPlantName(char aplantName[MAX_TXT]);

      void SetP_digest(double aP_digest) {P_digest = aP_digest;}
      void SetMaxPercentage(double aMaxPercentage) {MaxPercentage = aMaxPercentage;};
      void SetpigFeedUnitsPerItemUnit_pregnant(double apigFeedUnitsPerItemUnit) {pigFeedUnitsPerItemUnit = apigFeedUnitsPerItemUnit;}
//      void SetfodderUnitsPerItemUnit_pregnant(double afodderUnitsPerItemUnit) {fodderUnitsPerItemUnit_pregnant = afodderUnitsPerItemUnit;}
  //    void SetfodderUnitsPerItemUnit_nonpregnant(double afodderUnitsPerItemUnit) {fodderUnitsPerItemUnit_nonpregnant = afodderUnitsPerItemUnit;}
   void SetOMD(double aOMD) {OMD = aOMD;}
   void SetpropRDP(double aprop) {propRDP=aprop;}
   void SetpropAAinUDP(double apropAAinUDP) {propAAinUDP=apropAAinUDP;}
   void SetpropUDPdigestible(double apropUDPdigestible) {propUDPdigestible=apropUDPdigestible;}
   void SetmicroAAFactor(double amicroAAFactor) {microAAFactor=amicroAAFactor;}
   void SetCHO_content(double aCHO_content) {CHO_content=aCHO_content;}
   void SetpropDigestCHO(double apropDigestCHO) {propDigestCHO=apropDigestCHO;}
   void SetME(double anME) {ME = anME;}
   void SetfeedCategory(int aValue) {feedCategory=aValue;}

      // Get functions
   double GetOMD(void) const {return OMD;}
      double GetproteinN_digestibility(void) const {return proteinN_digestibility;}
      double GetfeedUnitsPerItemUnit(void) const {return feedUnitsPerItemUnit;}
      double GetCa_content(void) const {return Ca_content;}
      double GetNa_content(void) const {return Na_content;}
      double GetMg_content(void) const {return Mg_content;}
      double GetCu_content(void) const {return Cu_content;}
      double GetS_content(void) const {return S_content;}
      double Getprotein_N(void) const {return orgN_content.n;}
      double Getfat(void) const {return fat;}
      double Getsugar(void) const {return sugar;}
      double Getstarch(void) const {return starch;}
      double Getchew(void) const {return chew;}
      double Getaat(void) const {return aat;}
      double Getpbv(void) const {return pbv;}
      double Getcell(void) const {return cell;}
      double Getfill(void) const {return fill;}
      double GetfillU(void) const {return fillU;}
      double GetMethionin(void) const {return Methionin;};
      double GetThreonin(void) const {return Threonin;}
      double GetLysin(void) const {return Lysin;}
      string GetPlantName(void) const {return plantName;}
      double GetP_digest(void) const {return P_digest;}
      double GetMaxPercentage(void) const {return MaxPercentage;};
//      double GetfodderUnitsPerItemUnit_pregnant(void) const {return fodderUnitsPerItemUnit_pregnant;}
  //    double GetfodderUnitsPerItemUnit_nonpregnant(void) const {return fodderUnitsPerItemUnit_nonpregnant;}
      double GetpigFeedUnitsPerItemUnit(void) const {return pigFeedUnitsPerItemUnit;}
      double GetPropRDP() const {return propRDP;};
      double GetmicroAAFactor() const {return microAAFactor;};
      double GetpropAAinUDP() const {return propAAinUDP;};
      double GetpropUDPdigestible() const {return propUDPdigestible;};
      double GetCHOContent() const {return CHO_content;};
      double GetpropDigestCHO() const {return propDigestCHO;};
      double GetME() const {return ME;};
      virtual double GetProteinContent();
      virtual double GetdigestibleEnergy();


		// Other functions
		virtual string DerivedFrom() const;	//returns name of product that can be transformed to this class
      virtual product& operator=(const product& someFeedItem);
      virtual product& operator+(const product& someFeedItem);
      virtual product& operator-(const product& someFeedItem);
      product* clone() const;
//      virtual void ReadParameters(fstream * file);
		bool ReadParams(fstream * file);
		virtual void CalcDigestibility();
	 	void CopyFromPlantItem(const plantItem& somePlantItem, int feedCat);
		virtual double CalcNFE();
		virtual double GetC();
		virtual double GetGrossEnergy();
		virtual void CalcFE();
		virtual void CalcME();
		virtual void CalcFill(int type);
		virtual void CalcAAT_PBV();
      virtual int GetfeedCategory() {return feedCategory;}
      virtual int GotItAll();
};

#endif

